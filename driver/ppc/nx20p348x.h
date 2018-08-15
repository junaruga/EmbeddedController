/* Copyright 2018 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/* NX20P348x Type-C Power Path Controller */

#ifndef __CROS_EC_NX20P348X_H
#define __CROS_EC_NX20P348X_H

#define NX20P3483_ADDR0 0xE0
#define NX20P3483_ADDR1 0xE2
#define NX20P3483_ADDR2 0xE4
#define NX20P3483_ADDR3 0xE6

/*
 * This PPC hard-codes the over voltage protect of Vbus at 6.8V in dead-battery
 * mode. If we ever are every going to drop the PD rail, we need to first ensure
 * that Vbus is negotiated to below 6.8V otherwise we can lock out Vbus.
 */
#define NX20P348X_SAFE_RESET_VBUS_MV 5000

/* NX20P348x register addresses */
#define NX20P348X_DEVICE_ID_REG		0x00
#define NX20P348X_DEVICE_STATUS_REG		0x01
#define NX20P348X_SWITCH_CONTROL_REG		0x02
#define NX20P348X_SWITCH_STATUS_REG		0x03
#define NX20P348X_INTERRUPT1_REG		0x04
#define NX20P348X_INTERRUPT2_REG		0x05
#define NX20P348X_INTERRUPT1_MASK_REG		0x06
#define NX20P348X_INTERRUPT2_MASK_REG		0x07
#define NX20P348X_OVLO_THRESHOLD_REG		0x08
#define NX20P348X_HV_SRC_OCP_THRESHOLD_REG	0x09
#define NX20P348X_5V_SRC_OCP_THRESHOLD_REG	0x0A
#define NX20P348X_DEVICE_CONTROL_REG		0x0B

/* Device Control Register */
#define NX20P348X_CTRL_FRS_AT (1 << 3)
#define NX20P348X_CTRL_DB_EXIT (1 << 2)
#define NX20P348X_CTRL_VBUSDIS_EN (1 << 1)
#define NX20P348X_CTRL_LDO_SD (1 << 0)

/* Device Status Modes */
#define NX20P348X_DEVICE_MODE_MASK 0x7
#define NX20P348X_MODE_DEAD_BATTERY 0
#define NX20P348X_MODE_HV_SNK  1
#define NX20P348X_MODE_5V_SRC  2
#define NX20P348X_MODE_HV_SRC  3
#define NX20P348X_MODE_STANDBY 4

/* Switch Status Register */
#define NX20P348X_HVSNK_STS (1 << 0)
#define NX20P348X_HVSRC_STS (1 << 1)
#define NX20P348X_5VSRC_STS (1 << 2)

/* Internal 5V VBUS Switch Current Limit Settings (min) */
#define NX20P348X_ILIM_MASK 0xF
#define NX20P348X_ILIM_0_400  0
#define NX20P348X_ILIM_0_600  1
#define NX20P348X_ILIM_0_800  2
#define NX20P348X_ILIM_1_000  3
#define NX20P348X_ILIM_1_200  4
#define NX20P348X_ILIM_1_400  5
#define NX20P348X_ILIM_1_600  6
#define NX20P348X_ILIM_1_800  7
#define NX20P348X_ILIM_2_000  8
#define NX20P348X_ILIM_2_200  9
#define NX20P348X_ILIM_2_400 10
#define NX20P348X_ILIM_2_600 11
#define NX20P348X_ILIM_2_800 12
#define NX20P348X_ILIM_3_000 13
#define NX20P348X_ILIM_3_200 14
#define NX20P348X_ILIM_3_400 15

/* HV VBUS over voltage threshold settings V_mV*/
#define NX20P348X_OVLO_THRESHOLD_MASK 0x7
#define NX20P348X_OVLO_06_0 0
#define NX20P348X_OVLO_06_8 1
#define NX20P348X_OVLO_10_0 2
#define NX20P348X_OVLO_11_5 3
#define NX20P348X_OVLO_14_0 4
#define NX20P348X_OVLO_17_0 5
#define NX20P348X_OVLO_23_0 6

/* Interrupt 1 Register Bits */
#define NX20P348X_INT1_DBEXIT_ERR (1 << 7)
#define NX20P348X_INT1_OV_5VSRC   (1 << 4)
#define NX20P348X_INT1_RCP_5VSRC  (1 << 3)
#define NX20P348X_INT1_SC_5VSRC   (1 << 2)
#define NX20P348X_INT1_OC_5VSRC   (1 << 1)
#define NX20P348X_INT1_OTP        (1 << 0)

/* Interrupt 2 Register Bits */
#define NX20P348X_INT2_EN_ERR     (1 << 7)
#define NX20P348X_INT2_RCP_HVSNK  (1 << 6)
#define NX20P348X_INT2_SC_HVSNK   (1 << 5)
#define NX20P348X_INT2_OV_HVSNK   (1 << 4)
#define NX20P348X_INT2_RCP_HVSRC  (1 << 3)
#define NX20P348X_INT2_SC_HVSRC   (1 << 2)
#define NX20P348X_INT2_OC_HVSRC   (1 << 1)
#define NX20P348X_INT2_OV_HVSRC   (1 << 0)

struct ppc_drv;
extern const struct ppc_drv nx20p348x_drv;

/**
 * Interrupt Handler for the NX20P348x.
 *
 * @param port: The Type-C port which triggered the interrupt.
 */
void nx20p348x_interrupt(int port);

#endif /* defined(__CROS_EC_NX20P348X_H) */
