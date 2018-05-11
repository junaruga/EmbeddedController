#!/usr/bin/python
# Copyright 2016 The Chromium OS Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

from __future__ import print_function

import argparse
import errno
import os
import re
import subprocess
import time
import tempfile

import json
import fw_update
import ecusb.tiny_servo_common as c

FIRMWARE_PATH = "/usr/share/servo_updater/firmware/"
CONFIGS_PATH = "/usr/share/servo_updater/configs/"

class ServoUpdaterException(Exception):
  """Raised on exceptions generated by servo_updater."""

def flash(brdfile, serialno, binfile):
  """Call fw_update to upload to updater USB endpoint."""
  p = fw_update.Supdate()
  p.load_board(brdfile)
  p.connect_usb(serialname=serialno)
  p.load_file(binfile)

  # Start transfer and erase.
  p.start()
  # Upload the bin file
  print("Uploading %s" % binfile)
  p.write_file()

  # Finalize
  print("Done. Finalizing.")
  p.stop()

def flash2(vidpid, serialno, binfile):
  """Call fw update via usb_updater2 commandline."""
  cmd = "usb_updater2 -d %s" % vidpid
  if serialno:
    cmd += " -S %s" % serialno
  cmd += " -n"
  cmd += " %s" % binfile

  print(cmd)
  res = subprocess.call(cmd.split())

  if res in (0, 1, 2):
    return res
  else:
    raise ServoUpdaterException("usb_updater2 exit with res = %d" % res)

def connect(vidpid, iface, serialno, debuglog=False):
  """Connect to console.

  Args:
    vidpid: vidpid of desired device.
    iface: interface to connect.
    serialno: serial number, to differentiate multiple devices.
    debuglog: do chatty log.

  Returns:
    a connected pty object.
  """
  # Make sure device is up.
  c.wait_for_usb(vidpid, serialname=serialno)

  # make a console.
  pty = c.setup_tinyservod(vidpid, iface,
            serialname=serialno, debuglog=debuglog)

  return pty

def select(vidpid, iface, serialno, region, debuglog=False):
  """Ensure the servo is in the expected ro/rw partition."""

  if region not in ["rw", "ro"]:
    raise Exception("Region must be ro or rw")

  pty = connect(vidpid, iface, serialno)

  if region is "ro":
    cmd = "reboot"
  else:
    cmd = "sysjump %s" % region
  pty._issue_cmd(cmd)
  time.sleep(1)
  pty.close()

def do_version(vidpid, iface, serialno):
  """Check version via ec console 'pty'.

  Args:
    see connect()

  Returns:
    detected version number

  Commands are:
  # > version
  # ...
  # Build:   tigertail_v1.1.6749-74d1a312e
  """
  pty = connect(vidpid, iface, serialno)

  cmd = '\r\nversion\r\n'
  regex = 'Build:\s+(\S+)'

  results = pty._issue_cmd_get_results(cmd, [regex])[0]
  pty.close()

  return results[1].strip(' \t\r\n\0')

def do_updater_version(vidpid, iface, serialno):
  """Check whether this uses python updater or c++ updater

  Args:
    see connect()

  Returns:
    updater version number. 2 or 6.
  """
  vers = do_version(vidpid, iface, serialno)

  m = re.search('_v1.1.(\d\d\d\d)-', vers)
  if m:
    version_number = int(m.group(1))
    # Servo versions below 58 are from servo-9040.B.
    # Updater version is not directly queryable.
    if version_number < 5800:
      return 2
    else:
      return 6
  return 0

def findfiles(cname, fname):
  """Select config and firmware binary files.

  This checks default file names and paths.
  In: /usr/share/servo_updater/[firmware|configs]
  check for board.json, board.bin

  Args:
    cname: config name, or board name. eg. "servo_v4".
    fname: firmware binary name. Can be None to try default.
  Returns:
    cname, fname: validated filenames selected from the path.
  """
  if not os.path.isfile(cname):
    # If not an existing file, try checking on the default path.
    newname = CONFIGS_PATH + cname
    if os.path.isfile(newname):
      cname = newname
    elif os.path.isfile(newname + ".json"):
      # Try appending ".json" to convert board name to config file.
      cname = newname + ".json"
    else:
      raise ServoUpdaterException("Can't find file: %s." % cname)

  if not fname:
    # If None, try defaults.
    dev = None
    if "servo_v4" in cname:
      dev = "servo_v4"
    if "servo_micro" in cname:
      dev = "servo_micro"
    if os.path.isfile(FIRMWARE_PATH + dev + ".bin"):
      fname = FIRMWARE_PATH + dev + ".bin"
  elif not os.path.isfile(fname):
    # If a name is specified but not found, try the default path.
    if os.path.isfile(FIRMWARE_PATH + fname):
      fname = FIRMWARE_PATH + fname
    else:
      raise ServoUpdaterException("Can't find file: %s." % fname)

  return cname, fname

def find_available_version(boardname, binfile):
  """Find the version string from the binary file.

  Args:
    boardname: the name of the board, eg. "servo_micro"
    binfile: the binary to search

  Returns:
    the version string.
  """
  rawstrings = subprocess.check_output(
      ['cbfstool', binfile, 'read', '-r', 'RO_FRID', '-f', '/dev/stdout'])
  m = re.match(r'%s_v\S+' % boardname, rawstrings)
  if m:
    newvers = m.group(0).strip(' \t\r\n\0')
  else:
    raise ServoUpdaterException("Can't find version from file: %s." % binfile)

  return newvers

def main():
  parser = argparse.ArgumentParser(description="Image a servo micro device")
  parser.add_argument('-s', '--serialno', type=str,
      help="serial number to program", default=None)
  parser.add_argument('-b', '--board', type=str,
      help="Board configuration json file", default="servo_v4")
  parser.add_argument('-f', '--file', type=str,
      help="Complete ec.bin file", default=None)
  parser.add_argument('--force', action="store_true",
      help="Update even if version match", default=False)
  parser.add_argument('-v', '--verbose', action="store_true",
      help="Chatty output")

  args = parser.parse_args()

  brdfile, binfile = findfiles(args.board, args.file)

  serialno = args.serialno
  debuglog = (args.verbose is True)

  with open(brdfile) as data_file:
      data = json.load(data_file)

  vidpid = "%04x:%04x" % (int(data['vid'], 0), int(data['pid'], 0))
  iface = int(data['console'], 0)
  boardname = data['board']

  if not args.force:
    vers = do_version(vidpid, iface, serialno)
    print("Current %s version is   %s" % (boardname, vers))

    newvers = find_available_version(boardname, binfile)
    print("Available %s version is %s" % (boardname, newvers))

    if newvers == vers:
      print("No version update needed")
      return
    else:
      print("Updating to recommended version.")


  select(vidpid, iface, serialno, "ro", debuglog=debuglog)

  vers = do_updater_version(vidpid, iface, serialno)
  if vers == 2:
    flash(brdfile, serialno, binfile)
  elif vers == 6:
    flash2(vidpid, serialno, binfile)
  else:
    raise ServoUpdaterException("Can't detect updater version")

  select(vidpid, iface, serialno, "rw", debuglog=debuglog)

  vers = do_updater_version(vidpid, iface, serialno)
  if vers == 2:
    flash(brdfile, serialno, binfile)
  elif vers == 6:
    flash2(vidpid, serialno, binfile)
  else:
    raise ServoUpdaterException("Can't detect updater version")

  select(vidpid, iface, serialno, "ro", debuglog=debuglog)

if __name__ == "__main__":
  main()
