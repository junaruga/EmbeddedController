/* Copyright 2020 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
/* Asurada board configuration */

#ifndef __CROS_EC_BOARD_H
#define __CROS_EC_BOARD_H

/* Chipset config */
#define CONFIG_BRINGUP
#define CONFIG_CHIPSET_MT8192
#define CONFIG_CMD_POWERINDEBUG
#define CONFIG_POWER_COMMON

/* Optional features */
#define CONFIG_BOARD_VERSION_CUSTOM
#define CONFIG_EXTPOWER_GPIO
#define CONFIG_LOW_POWER_IDLE
#define CONFIG_LOW_POWER_S0
#define CONFIG_POWER_BUTTON
#define CONFIG_PWM
#define CONFIG_VBOOT_HASH
#define CONFIG_VOLUME_BUTTONS
#define CONFIG_WP_ACTIVE_HIGH

/* Battery */
#define CONFIG_BATTERY_CUT_OFF
#define CONFIG_BATTERY_FUEL_GAUGE
#define CONFIG_BATTERY_PRESENT_GPIO GPIO_EC_BATT_PRES_ODL
#define CONFIG_BATTERY_SMART

/* Charger */
#define CONFIG_CHARGER
#define CONFIG_CHARGER_INPUT_CURRENT 512
#define CONFIG_CHARGER_ISL9238
#define CONFIG_CHARGER_MAINTAIN_VBAT
#define CONFIG_CHARGER_SENSE_RESISTOR_AC 20 /* BOARD_RS1 */
#define CONFIG_CHARGER_SENSE_RESISTOR 10 /* BOARD_RS2 */
#define CONFIG_CHARGER_OTG
#define CONFIG_CHARGE_MANAGER

/* Chipset */

/* Keyboard */
#define CONFIG_CMD_KEYBOARD
#define CONFIG_KEYBOARD_BOARD_CONFIG
#define CONFIG_KEYBOARD_COL2_INVERTED
#define CONFIG_KEYBOARD_PROTOCOL_MKBP
#define CONFIG_MKBP_USE_GPIO

/* I2C */
#define CONFIG_I2C
#define CONFIG_I2C_MASTER
#define I2C_PORT_CHARGER IT83XX_I2C_CH_A
#define I2C_PORT_BATTERY IT83XX_I2C_CH_A
#define I2C_PORT_ACCEL IT83XX_I2C_CH_B
#define I2C_PORT_PPC0  IT83XX_I2C_CH_C
#define I2C_PORT_PPC1  IT83XX_I2C_CH_E

/* PD / USB-C / PPC */
#define CONFIG_CMD_PPC_DUMP
#define CONFIG_USBC_PPC
#define CONFIG_USBC_PPC_DEDICATED_INT
#define CONFIG_USBC_PPC_POLARITY
#define CONFIG_USBC_PPC_SYV682X
#define CONFIG_USBC_PPC_VCONN
#define CONFIG_USBC_SS_MUX
#define CONFIG_USBC_VCONN
#define CONFIG_USBC_VCONN_SWAP
#define CONFIG_USB_PD_ALT_MODE
#define CONFIG_USB_PD_ALT_MODE_DFP
#define CONFIG_USB_PD_DEBUG_LEVEL 3
#define CONFIG_USB_PD_DECODE_SOP
#define CONFIG_USB_PD_DISCHARGE
#define CONFIG_USB_PD_DISCHARGE_PPC
#define CONFIG_USB_PD_DUAL_ROLE
#define CONFIG_USB_PD_ITE_ACTIVE_PORT_COUNT 2
#define CONFIG_USB_PD_MAX_SINGLE_SOURCE_CURRENT TYPEC_RP_3A0
#define CONFIG_USB_PD_PORT_MAX_COUNT 2
#define CONFIG_USB_PD_TCPMV1
#define CONFIG_USB_PD_TCPM_ITE_ON_CHIP
#define CONFIG_USB_PD_TCPM_TCPCI
#define CONFIG_USB_PD_TRY_SRC
#define CONFIG_USB_PD_VBUS_DETECT_PPC
#define CONFIG_USB_PD_VBUS_MEASURE_CHARGER
#define CONFIG_USB_PID 0x5566  /* TODO: update PID */
#define CONFIG_USB_POWER_DELIVERY

#define PD_MAX_CURRENT_MA 3000
#define PD_MAX_VOLTAGE_MV 20000
#define PD_OPERATING_POWER_MW 15000
#define PD_MAX_POWER_MW 45000
#define PD_POWER_SUPPLY_TURN_ON_DELAY  30000  /* us */
#define PD_POWER_SUPPLY_TURN_OFF_DELAY 250000 /* us */
#define PD_VCONN_SWAP_DELAY 5000 /* us */

/* Optional console commands */
#define CONFIG_CMD_FLASH
#define CONFIG_CMD_SCRATCHPAD
#define CONFIG_CMD_STACKOVERFLOW

/* Sensor */
#define CONFIG_GMR_TABLET_MODE
#define CONFIG_TABLET_MODE
#define GMR_TABLET_MODE_GPIO_L GPIO_TABLET_MODE_L

#define CONFIG_ACCEL_FIFO
#define CONFIG_ACCEL_FIFO_SIZE 256
#define CONFIG_ACCEL_FIFO_THRES (CONFIG_ACCEL_FIFO_SIZE / 3)
#define CONFIG_ACCEL_INTERRUPTS
#define CONFIG_ACCELGYRO_BMI160 /* Base accel */
#define CONFIG_ACCELGYRO_BMI160_INT_EVENT \
	TASK_EVENT_MOTION_SENSOR_INTERRUPT(BASE_ACCEL)
#define CONFIG_ACCELGYRO_SEC_ADDR_FLAGS BMM150_ADDR0_FLAGS

#define CONFIG_ACCEL_LIS2DWL
#define CONFIG_ACCEL_LIS2DW_AS_BASE
#define CONFIG_ACCEL_LIS2DW12_INT_EVENT \
	TASK_EVENT_MOTION_SENSOR_INTERRUPT(LID_ACCEL)

#define CONFIG_LID_ANGLE
#define CONFIG_LID_ANGLE_SENSOR_BASE BASE_ACCEL
#define CONFIG_LID_ANGLE_SENSOR_LID LID_ACCEL
#define CONFIG_LID_ANGLE_UPDATE

#define CONFIG_CMD_ACCEL_INFO
#define CONFIG_CMD_ACCELS

#define CONFIG_MAG_BMI_BMM150
#define CONFIG_MAG_CALIBRATE

/* UART */
#undef CONFIG_UART_TX_BUF_SIZE
#define CONFIG_UART_TX_BUF_SIZE 4096

/* GPIO name remapping */
#define GPIO_EN_HDMI_PWR        GPIO_EC_X_GPIO1
#define GPIO_USB_C1_FRS_EN      GPIO_EC_X_GPIO1
#define GPIO_USB_C1_PPC_INT_ODL GPIO_X_EC_GPIO2
#define GPIO_PS185_EC_DP_HPD    GPIO_X_EC_GPIO2
#define GPIO_USB_C1_DP_IN_HPD   GPIO_EC_X_GPIO3
#define GPIO_PS185_PWRDN_ODL    GPIO_EC_X_GPIO3

#ifndef __ASSEMBLER__

#include "gpio_signal.h"
#include "registers.h"

enum battery_type {
	BATTERY_C235,
	BATTERY_TYPE_COUNT,
};

enum pwm_channel {
	PWM_CH_PWRLED,
	PWM_CH_COUNT,
};

enum adc_channel {
	ADC_TEMP_SENSOR_SUBPMIC, /* ADC 0 */
	ADC_BOARD_ID_0,          /* ADC 1 */
	ADC_BOARD_ID_1,          /* ADC 2 */
	ADC_TEMP_SENSOR_AMB,     /* ADC 3 */
	ADC_TEMP_SENSOR_CHARGER, /* ADC 5 */
	ADC_CHARGER_PMON,        /* ADC 6 */
	ADC_TEMP_SENSOR_AP,      /* ADC 7 */

	/* Number of ADC channels */
	ADC_CH_COUNT,
};

enum power_signal {
	PMIC_PWR_GOOD,
	AP_IN_S3_L,
	AP_WDT_ASSERTED,
	POWER_SIGNAL_COUNT,
};

enum board_sub_board {
	SUB_BOARD_NONE = -1,
	SUB_BOARD_TYPEC,
	SUB_BOARD_HDMI,

	SUB_BOARD_COUNT,
};

enum sensor_id {
	BASE_ACCEL = 0,
	BASE_GYRO,
	BASE_MAG,
	LID_ACCEL,
	SENSOR_COUNT,
};

void board_reset_pd_mcu(void);
int board_get_version(void);

#endif /* !__ASSEMBLER__ */
#endif /* __CROS_EC_BOARD_H */
