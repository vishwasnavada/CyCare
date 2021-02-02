/******************************************************************************
I2Cdev library collection - MPU9250 I2C device class
Based on InvenSense MPU-6050 register map document rev. 2.0, 5/19/2011 
(RM-MPU-6000A-00)

10/3/2011 by Jeff Rowberg <jeff@rowberg.net>
Updates should (hopefully) always be available at 
https://github.com/jrowberg/i2cdevlib

Changelog:
26/May/2014: C version to be used on PSoC Creator (www.cypress.com) by 
Hernán Sánchez

NOTE: THIS IS ONLY A PARIAL RELEASE. THIS DEVICE CLASS IS CURRENTLY UNDERGOING
ACTIVE DEVELOPMENT AND IS STILL MISSING SOME IMPORTANT FEATURES. PLEASE KEEP 
THIS IN MIND IF YOU DECIDE TO USE THIS PARTICULAR CODE FOR ANYTHING.
===============================================================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2012 Jeff Rowberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
==============================================================================*/

#include <stdint.h>
#include <stdbool.h>

#ifndef _MPU_9250_H_
#define _MPU_9250_H_

#define MPU9250_ADDRESS_AD0_LOW     0x68 // address pin low (GND), default for InvenSense evaluation board
#define MPU9250_ADDRESS_AD0_HIGH    0x69 // address pin high (VCC)
#define MPU9250_DEFAULT_ADDRESS     MPU9250_ADDRESS_AD0_LOW

#define MPU9250_ADDRESS_COMPASS		0x0c

#define MPU9250_RA_XG_OFFS_TC       0x00 //[7] PWR_MODE, [6:1] XG_OFFS_TC, [0] OTP_BNK_VLD
#define MPU9250_RA_YG_OFFS_TC       0x01 //[7] PWR_MODE, [6:1] YG_OFFS_TC, [0] OTP_BNK_VLD
#define MPU9250_RA_ZG_OFFS_TC       0x02 //[7] PWR_MODE, [6:1] ZG_OFFS_TC, [0] OTP_BNK_VLD
#define MPU9250_RA_X_FINE_GAIN      0x03 //[7:0] X_FINE_GAIN
#define MPU9250_RA_Y_FINE_GAIN      0x04 //[7:0] Y_FINE_GAIN
#define MPU9250_RA_Z_FINE_GAIN      0x05 //[7:0] Z_FINE_GAIN
#define MPU9250_RA_XA_OFFS_H        0x06 //[15:0] XA_OFFS
#define MPU9250_RA_XA_OFFS_L_TC     0x07
#define MPU9250_RA_YA_OFFS_H        0x08 //[15:0] YA_OFFS
#define MPU9250_RA_YA_OFFS_L_TC     0x09
#define MPU9250_RA_ZA_OFFS_H        0x0A //[15:0] ZA_OFFS
#define MPU9250_RA_ZA_OFFS_L_TC     0x0B
#define MPU9250_RA_XG_OFFS_USRH     0x13 //[15:0] XG_OFFS_USR
#define MPU9250_RA_XG_OFFS_USRL     0x14
#define MPU9250_RA_YG_OFFS_USRH     0x15 //[15:0] YG_OFFS_USR
#define MPU9250_RA_YG_OFFS_USRL     0x16
#define MPU9250_RA_ZG_OFFS_USRH     0x17 //[15:0] ZG_OFFS_USR
#define MPU9250_RA_ZG_OFFS_USRL     0x18
#define MPU9250_RA_SMPLRT_DIV       0x19
#define MPU9250_RA_CONFIG           0x1A
#define MPU9250_RA_GYRO_CONFIG      0x1B
#define MPU9250_RA_ACCEL_CONFIG     0x1C
#define MPU9250_RA_FF_THR           0x1D
#define MPU9250_RA_FF_DUR           0x1E
#define MPU9250_RA_MOT_THR          0x1F
#define MPU9250_RA_MOT_DUR          0x20
#define MPU9250_RA_ZRMOT_THR        0x21
#define MPU9250_RA_ZRMOT_DUR        0x22
#define MPU9250_RA_FIFO_EN          0x23
#define MPU9250_RA_I2C_MST_CTRL     0x24
#define MPU9250_RA_I2C_SLV0_ADDR    0x25
#define MPU9250_RA_I2C_SLV0_REG     0x26
#define MPU9250_RA_I2C_SLV0_CTRL    0x27
#define MPU9250_RA_I2C_SLV1_ADDR    0x28
#define MPU9250_RA_I2C_SLV1_REG     0x29
#define MPU9250_RA_I2C_SLV1_CTRL    0x2A
#define MPU9250_RA_I2C_SLV2_ADDR    0x2B
#define MPU9250_RA_I2C_SLV2_REG     0x2C
#define MPU9250_RA_I2C_SLV2_CTRL    0x2D
#define MPU9250_RA_I2C_SLV3_ADDR    0x2E
#define MPU9250_RA_I2C_SLV3_REG     0x2F
#define MPU9250_RA_I2C_SLV3_CTRL    0x30
#define MPU9250_RA_I2C_SLV4_ADDR    0x31
#define MPU9250_RA_I2C_SLV4_REG     0x32
#define MPU9250_RA_I2C_SLV4_DO      0x33
#define MPU9250_RA_I2C_SLV4_CTRL    0x34
#define MPU9250_RA_I2C_SLV4_DI      0x35
#define MPU9250_RA_I2C_MST_STATUS   0x36
#define MPU9250_RA_INT_PIN_CFG      0x37
#define MPU9250_RA_INT_ENABLE       0x38
#define MPU9250_RA_DMP_INT_STATUS   0x39
#define MPU9250_RA_INT_STATUS       0x3A
#define MPU9250_RA_ACCEL_XOUT_H     0x3B
#define MPU9250_RA_ACCEL_XOUT_L     0x3C
#define MPU9250_RA_ACCEL_YOUT_H     0x3D
#define MPU9250_RA_ACCEL_YOUT_L     0x3E
#define MPU9250_RA_ACCEL_ZOUT_H     0x3F
#define MPU9250_RA_ACCEL_ZOUT_L     0x40
#define MPU9250_RA_TEMP_OUT_H       0x41
#define MPU9250_RA_TEMP_OUT_L       0x42
#define MPU9250_RA_GYRO_XOUT_H      0x43
#define MPU9250_RA_GYRO_XOUT_L      0x44
#define MPU9250_RA_GYRO_YOUT_H      0x45
#define MPU9250_RA_GYRO_YOUT_L      0x46
#define MPU9250_RA_GYRO_ZOUT_H      0x47
#define MPU9250_RA_GYRO_ZOUT_L      0x48
#define MPU9250_RA_EXT_SENS_DATA_00 0x49
#define MPU9250_RA_EXT_SENS_DATA_01 0x4A
#define MPU9250_RA_EXT_SENS_DATA_02 0x4B
#define MPU9250_RA_EXT_SENS_DATA_03 0x4C
#define MPU9250_RA_EXT_SENS_DATA_04 0x4D
#define MPU9250_RA_EXT_SENS_DATA_05 0x4E
#define MPU9250_RA_EXT_SENS_DATA_06 0x4F
#define MPU9250_RA_EXT_SENS_DATA_07 0x50
#define MPU9250_RA_EXT_SENS_DATA_08 0x51
#define MPU9250_RA_EXT_SENS_DATA_09 0x52
#define MPU9250_RA_EXT_SENS_DATA_10 0x53
#define MPU9250_RA_EXT_SENS_DATA_11 0x54
#define MPU9250_RA_EXT_SENS_DATA_12 0x55
#define MPU9250_RA_EXT_SENS_DATA_13 0x56
#define MPU9250_RA_EXT_SENS_DATA_14 0x57
#define MPU9250_RA_EXT_SENS_DATA_15 0x58
#define MPU9250_RA_EXT_SENS_DATA_16 0x59
#define MPU9250_RA_EXT_SENS_DATA_17 0x5A
#define MPU9250_RA_EXT_SENS_DATA_18 0x5B
#define MPU9250_RA_EXT_SENS_DATA_19 0x5C
#define MPU9250_RA_EXT_SENS_DATA_20 0x5D
#define MPU9250_RA_EXT_SENS_DATA_21 0x5E
#define MPU9250_RA_EXT_SENS_DATA_22 0x5F
#define MPU9250_RA_EXT_SENS_DATA_23 0x60
#define MPU9250_RA_MOT_DETECT_STATUS    0x61
#define MPU9250_RA_I2C_SLV0_DO      0x63
#define MPU9250_RA_I2C_SLV1_DO      0x64
#define MPU9250_RA_I2C_SLV2_DO      0x65
#define MPU9250_RA_I2C_SLV3_DO      0x66
#define MPU9250_RA_I2C_MST_DELAY_CTRL   0x67
#define MPU9250_RA_SIGNAL_PATH_RESET    0x68
#define MPU9250_RA_MOT_DETECT_CTRL      0x69
#define MPU9250_RA_USER_CTRL        0x6A
#define MPU9250_RA_PWR_MGMT_1       0x6B
#define MPU9250_RA_PWR_MGMT_2       0x6C
#define MPU9250_RA_BANK_SEL         0x6D
#define MPU9250_RA_MEM_START_ADDR   0x6E
#define MPU9250_RA_MEM_R_W          0x6F
#define MPU9250_RA_DMP_CFG_1        0x70
#define MPU9250_RA_DMP_CFG_2        0x71
#define MPU9250_RA_FIFO_COUNTH      0x72
#define MPU9250_RA_FIFO_COUNTL      0x73
#define MPU9250_RA_FIFO_R_W         0x74
#define MPU9250_RA_WHO_AM_I         0x75

#define MPU9250_TC_PWR_MODE_BIT     7
#define MPU9250_TC_OFFSET_BIT       6
#define MPU9250_TC_OFFSET_LENGTH    6
#define MPU9250_TC_OTP_BNK_VLD_BIT  0

#define MPU9250_VDDIO_LEVEL_VLOGIC  0
#define MPU9250_VDDIO_LEVEL_VDD     1

#define MPU9250_CFG_EXT_SYNC_SET_BIT    5
#define MPU9250_CFG_EXT_SYNC_SET_LENGTH 3
#define MPU9250_CFG_DLPF_CFG_BIT    2
#define MPU9250_CFG_DLPF_CFG_LENGTH 3

#define MPU9250_EXT_SYNC_DISABLED       0x0
#define MPU9250_EXT_SYNC_TEMP_OUT_L     0x1
#define MPU9250_EXT_SYNC_GYRO_XOUT_L    0x2
#define MPU9250_EXT_SYNC_GYRO_YOUT_L    0x3
#define MPU9250_EXT_SYNC_GYRO_ZOUT_L    0x4
#define MPU9250_EXT_SYNC_ACCEL_XOUT_L   0x5
#define MPU9250_EXT_SYNC_ACCEL_YOUT_L   0x6
#define MPU9250_EXT_SYNC_ACCEL_ZOUT_L   0x7

#define MPU9250_DLPF_BW_256         0x00
#define MPU9250_DLPF_BW_188         0x01
#define MPU9250_DLPF_BW_98          0x02
#define MPU9250_DLPF_BW_42          0x03
#define MPU9250_DLPF_BW_20          0x04
#define MPU9250_DLPF_BW_10          0x05
#define MPU9250_DLPF_BW_5           0x06

#define MPU9250_GCONFIG_FS_SEL_BIT      4
#define MPU9250_GCONFIG_FS_SEL_LENGTH   2

#define MPU9250_GYRO_FS_250         0x00
#define MPU9250_GYRO_FS_500         0x01
#define MPU9250_GYRO_FS_1000        0x02
#define MPU9250_GYRO_FS_2000        0x03

#define MPU9250_ACONFIG_XA_ST_BIT           7
#define MPU9250_ACONFIG_YA_ST_BIT           6
#define MPU9250_ACONFIG_ZA_ST_BIT           5
#define MPU9250_ACONFIG_AFS_SEL_BIT         4
#define MPU9250_ACONFIG_AFS_SEL_LENGTH      2
#define MPU9250_ACONFIG_ACCEL_HPF_BIT       2
#define MPU9250_ACONFIG_ACCEL_HPF_LENGTH    3

#define MPU9250_ACCEL_FS_2          0x00
#define MPU9250_ACCEL_FS_4          0x01
#define MPU9250_ACCEL_FS_8          0x02
#define MPU9250_ACCEL_FS_16         0x03

#define MPU9250_DHPF_RESET          0x00
#define MPU9250_DHPF_5              0x01
#define MPU9250_DHPF_2P5            0x02
#define MPU9250_DHPF_1P25           0x03
#define MPU9250_DHPF_0P63           0x04
#define MPU9250_DHPF_HOLD           0x07

#define MPU9250_TEMP_FIFO_EN_BIT    7
#define MPU9250_XG_FIFO_EN_BIT      6
#define MPU9250_YG_FIFO_EN_BIT      5
#define MPU9250_ZG_FIFO_EN_BIT      4
#define MPU9250_ACCEL_FIFO_EN_BIT   3
#define MPU9250_SLV2_FIFO_EN_BIT    2
#define MPU9250_SLV1_FIFO_EN_BIT    1
#define MPU9250_SLV0_FIFO_EN_BIT    0

#define MPU9250_MULT_MST_EN_BIT     7
#define MPU9250_WAIT_FOR_ES_BIT     6
#define MPU9250_SLV_3_FIFO_EN_BIT   5
#define I2C_MPU9250_I2C_MST_P_NSR_BIT   4
#define I2C_MPU9250_I2C_MST_CLK_BIT     3
#define I2C_MPU9250_I2C_MST_CLK_LENGTH  4

#define MPU9250_CLOCK_DIV_348       0x0
#define MPU9250_CLOCK_DIV_333       0x1
#define MPU9250_CLOCK_DIV_320       0x2
#define MPU9250_CLOCK_DIV_308       0x3
#define MPU9250_CLOCK_DIV_296       0x4
#define MPU9250_CLOCK_DIV_286       0x5
#define MPU9250_CLOCK_DIV_276       0x6
#define MPU9250_CLOCK_DIV_267       0x7
#define MPU9250_CLOCK_DIV_258       0x8
#define MPU9250_CLOCK_DIV_500       0x9
#define MPU9250_CLOCK_DIV_471       0xA
#define MPU9250_CLOCK_DIV_444       0xB
#define MPU9250_CLOCK_DIV_421       0xC
#define MPU9250_CLOCK_DIV_400       0xD
#define MPU9250_CLOCK_DIV_381       0xE
#define MPU9250_CLOCK_DIV_364       0xF

#define I2C_MPU9250_I2C_SLV_RW_BIT      7
#define I2C_MPU9250_I2C_SLV_ADDR_BIT    6
#define I2C_MPU9250_I2C_SLV_ADDR_LENGTH 7
#define I2C_MPU9250_I2C_SLV_EN_BIT      7
#define I2C_MPU9250_I2C_SLV_BYTE_SW_BIT 6
#define I2C_MPU9250_I2C_SLV_REG_DIS_BIT 5
#define I2C_MPU9250_I2C_SLV_GRP_BIT     4
#define I2C_MPU9250_I2C_SLV_LEN_BIT     3
#define I2C_MPU9250_I2C_SLV_LEN_LENGTH  4

#define I2C_MPU9250_I2C_SLV4_RW_BIT         7
#define I2C_MPU9250_I2C_SLV4_ADDR_BIT       6
#define I2C_MPU9250_I2C_SLV4_ADDR_LENGTH    7
#define I2C_MPU9250_I2C_SLV4_EN_BIT         7
#define I2C_MPU9250_I2C_SLV4_INT_EN_BIT     6
#define I2C_MPU9250_I2C_SLV4_REG_DIS_BIT    5
#define I2C_MPU9250_I2C_SLV4_MST_DLY_BIT    4
#define I2C_MPU9250_I2C_SLV4_MST_DLY_LENGTH 5

#define MPU9250_MST_PASS_THROUGH_BIT    7
#define MPU9250_MST_I2C_SLV4_DONE_BIT   6
#define MPU9250_MST_I2C_LOST_ARB_BIT    5
#define MPU9250_MST_I2C_SLV4_NACK_BIT   4
#define MPU9250_MST_I2C_SLV3_NACK_BIT   3
#define MPU9250_MST_I2C_SLV2_NACK_BIT   2
#define MPU9250_MST_I2C_SLV1_NACK_BIT   1
#define MPU9250_MST_I2C_SLV0_NACK_BIT   0

#define MPU9250_INTCFG_INT_LEVEL_BIT        7
#define MPU9250_INTCFG_INT_OPEN_BIT         6
#define MPU9250_INTCFG_LATCH_INT_EN_BIT     5
#define MPU9250_INTCFG_INT_RD_CLEAR_BIT     4
#define MPU9250_INTCFG_FSYNC_INT_LEVEL_BIT  3
#define MPU9250_INTCFG_FSYNC_INT_EN_BIT     2
#define MPU9250_INTCFG_I2C_BYPASS_EN_BIT    1
#define MPU9250_INTCFG_CLKOUT_EN_BIT        0

#define MPU9250_INTMODE_ACTIVEHIGH  0x00
#define MPU9250_INTMODE_ACTIVELOW   0x01

#define MPU9250_INTDRV_PUSHPULL     0x00
#define MPU9250_INTDRV_OPENDRAIN    0x01

#define MPU9250_INTLATCH_50USPULSE  0x00
#define MPU9250_INTLATCH_WAITCLEAR  0x01

#define MPU9250_INTCLEAR_STATUSREAD 0x00
#define MPU9250_INTCLEAR_ANYREAD    0x01

#define MPU9250_INTERRUPT_FF_BIT            7
#define MPU9250_INTERRUPT_MOT_BIT           6
#define MPU9250_INTERRUPT_ZMOT_BIT          5
#define MPU9250_INTERRUPT_FIFO_OFLOW_BIT    4
#define MPU9250_INTERRUPT_I2C_MST_INT_BIT   3
#define MPU9250_INTERRUPT_PLL_RDY_INT_BIT   2
#define MPU9250_INTERRUPT_DMP_INT_BIT       1
#define MPU9250_INTERRUPT_DATA_RDY_BIT      0

// TODO: figure out what these actually do
// UMPL source code is not very obivous
#define MPU9250_DMPINT_5_BIT            5
#define MPU9250_DMPINT_4_BIT            4
#define MPU9250_DMPINT_3_BIT            3
#define MPU9250_DMPINT_2_BIT            2
#define MPU9250_DMPINT_1_BIT            1
#define MPU9250_DMPINT_0_BIT            0

#define MPU9250_MOTION_MOT_XNEG_BIT     7
#define MPU9250_MOTION_MOT_XPOS_BIT     6
#define MPU9250_MOTION_MOT_YNEG_BIT     5
#define MPU9250_MOTION_MOT_YPOS_BIT     4
#define MPU9250_MOTION_MOT_ZNEG_BIT     3
#define MPU9250_MOTION_MOT_ZPOS_BIT     2
#define MPU9250_MOTION_MOT_ZRMOT_BIT    0

#define MPU9250_DELAYCTRL_DELAY_ES_SHADOW_BIT   7
#define MPU9250_DELAYCTRL_I2C_SLV4_DLY_EN_BIT   4
#define MPU9250_DELAYCTRL_I2C_SLV3_DLY_EN_BIT   3
#define MPU9250_DELAYCTRL_I2C_SLV2_DLY_EN_BIT   2
#define MPU9250_DELAYCTRL_I2C_SLV1_DLY_EN_BIT   1
#define MPU9250_DELAYCTRL_I2C_SLV0_DLY_EN_BIT   0

#define MPU9250_PATHRESET_GYRO_RESET_BIT    2
#define MPU9250_PATHRESET_ACCEL_RESET_BIT   1
#define MPU9250_PATHRESET_TEMP_RESET_BIT    0

#define MPU9250_DETECT_ACCEL_ON_DELAY_BIT       5
#define MPU9250_DETECT_ACCEL_ON_DELAY_LENGTH    2
#define MPU9250_DETECT_FF_COUNT_BIT             3
#define MPU9250_DETECT_FF_COUNT_LENGTH          2
#define MPU9250_DETECT_MOT_COUNT_BIT            1
#define MPU9250_DETECT_MOT_COUNT_LENGTH         2

#define MPU9250_DETECT_DECREMENT_RESET  0x0
#define MPU9250_DETECT_DECREMENT_1      0x1
#define MPU9250_DETECT_DECREMENT_2      0x2
#define MPU9250_DETECT_DECREMENT_4      0x3

#define MPU9250_USERCTRL_DMP_EN_BIT             7
#define MPU9250_USERCTRL_FIFO_EN_BIT            6
#define MPU9250_USERCTRL_I2C_MST_EN_BIT         5
#define MPU9250_USERCTRL_I2C_IF_DIS_BIT         4
#define MPU9250_USERCTRL_DMP_RESET_BIT          3
#define MPU9250_USERCTRL_FIFO_RESET_BIT         2
#define MPU9250_USERCTRL_I2C_MST_RESET_BIT      1
#define MPU9250_USERCTRL_SIG_COND_RESET_BIT     0

#define MPU9250_PWR1_DEVICE_RESET_BIT   7
#define MPU9250_PWR1_SLEEP_BIT          6
#define MPU9250_PWR1_CYCLE_BIT          5
#define MPU9250_PWR1_TEMP_DIS_BIT       3
#define MPU9250_PWR1_CLKSEL_BIT         2
#define MPU9250_PWR1_CLKSEL_LENGTH      3

#define MPU9250_CLOCK_INTERNAL          0x00
#define MPU9250_CLOCK_PLL_XGYRO         0x01
#define MPU9250_CLOCK_PLL_YGYRO         0x02
#define MPU9250_CLOCK_PLL_ZGYRO         0x03
#define MPU9250_CLOCK_PLL_EXT32K        0x04
#define MPU9250_CLOCK_PLL_EXT19M        0x05
#define MPU9250_CLOCK_KEEP_RESET        0x07

#define MPU9250_PWR2_LP_WAKE_CTRL_BIT       7
#define MPU9250_PWR2_LP_WAKE_CTRL_LENGTH    2
#define MPU9250_PWR2_STBY_XA_BIT            5
#define MPU9250_PWR2_STBY_YA_BIT            4
#define MPU9250_PWR2_STBY_ZA_BIT            3
#define MPU9250_PWR2_STBY_XG_BIT            2
#define MPU9250_PWR2_STBY_YG_BIT            1
#define MPU9250_PWR2_STBY_ZG_BIT            0

#define MPU9250_WAKE_FREQ_1P25      0x0
#define MPU9250_WAKE_FREQ_2P5       0x1
#define MPU9250_WAKE_FREQ_5         0x2
#define MPU9250_WAKE_FREQ_10        0x3

#define MPU9250_BANKSEL_PRFTCH_EN_BIT       6
#define MPU9250_BANKSEL_CFG_USER_BANK_BIT   5
#define MPU9250_BANKSEL_MEM_SEL_BIT         4
#define MPU9250_BANKSEL_MEM_SEL_LENGTH      5

#define MPU9250_WHO_AM_I_BIT        6
#define MPU9250_WHO_AM_I_LENGTH     6

#define MPU9250_DMP_MEMORY_BANKS        8
#define MPU9250_DMP_MEMORY_BANK_SIZE    256
#define MPU9250_DMP_MEMORY_CHUNK_SIZE   16


uint8_t devAddr;
uint8_t buffer[22];

extern void I2CReadBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *value);
extern void I2CReadBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *value);
extern void I2CReadByte(uint8_t devAddr, uint8_t regAddr, uint8_t *value);
extern void I2CReadBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *value);
extern void I2CWriteBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t value);
extern void I2CWriteBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t value);
extern void I2CWriteByte(uint8_t devAddr, uint8_t regAddr, uint8_t value);
extern void I2CWriteBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *value);
extern void I2CWriteWord(uint8_t devAddr, uint8_t regAddr, uint16_t value);
extern void I2CWriteWords(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *value);

void MPU9250_init();
void I2C_MPU9250_I2CAddress(uint8_t address);

void MPU9250_initialize();
bool MPU9250_testConnection();

// AUX_VDDIO register
uint8_t MPU9250_getAuxVDDIOLevel();
void MPU9250_setAuxVDDIOLevel(uint8_t level);

// SMPLRT_DIV register
uint8_t MPU9250_getRate();
void MPU9250_setRate(uint8_t rate);

// CONFIG register
uint8_t MPU9250_getExternalFrameSync();
void MPU9250_setExternalFrameSync(uint8_t sync);
uint8_t MPU9250_getDLPFMode();
void MPU9250_setDLPFMode(uint8_t bandwidth);
void MPU9250_set_A_DLPFMode(uint8_t bandwidth);  //JOBI

// GYRO_CONFIG register
uint8_t MPU9250_getFullScaleGyroRange();
void MPU9250_setFullScaleGyroRange(uint8_t range);

// ACCEL_CONFIG register
bool MPU9250_getAccelXSelfTest();
void MPU9250_setAccelXSelfTest(bool enabled);
bool MPU9250_getAccelYSelfTest();
void MPU9250_setAccelYSelfTest(bool enabled);
bool MPU9250_getAccelZSelfTest();
void MPU9250_setAccelZSelfTest(bool enabled);
uint8_t MPU9250_getFullScaleAccelRange();
void MPU9250_setFullScaleAccelRange(uint8_t range);
uint8_t MPU9250_getDHPFMode();
void MPU9250_setDHPFMode(uint8_t mode);

// FF_THR register
uint8_t MPU9250_getFreefallDetectionThreshold();
void MPU9250_setFreefallDetectionThreshold(uint8_t threshold);

// FF_DUR register
uint8_t MPU9250_getFreefallDetectionDuration();
void MPU9250_setFreefallDetectionDuration(uint8_t duration);

// MOT_THR register
uint8_t MPU9250_getMotionDetectionThreshold();
void MPU9250_setMotionDetectionThreshold(uint8_t threshold);

// MOT_DUR register
uint8_t MPU9250_getMotionDetectionDuration();
void MPU9250_setMotionDetectionDuration(uint8_t duration);

// ZRMOT_THR register
uint8_t MPU9250_getZeroMotionDetectionThreshold();
void MPU9250_setZeroMotionDetectionThreshold(uint8_t threshold);

// ZRMOT_DUR register
uint8_t MPU9250_getZeroMotionDetectionDuration();
void MPU9250_setZeroMotionDetectionDuration(uint8_t duration);

// FIFO_EN register
bool MPU9250_getTempFIFOEnabled();
void MPU9250_setTempFIFOEnabled(bool enabled);
bool MPU9250_getXGyroFIFOEnabled();
void MPU9250_setXGyroFIFOEnabled(bool enabled);
bool MPU9250_getYGyroFIFOEnabled();
void MPU9250_setYGyroFIFOEnabled(bool enabled);
bool MPU9250_getZGyroFIFOEnabled();
void MPU9250_setZGyroFIFOEnabled(bool enabled);
bool MPU9250_getAccelFIFOEnabled();
void MPU9250_setAccelFIFOEnabled(bool enabled);
bool MPU9250_getSlave2FIFOEnabled();
void MPU9250_setSlave2FIFOEnabled(bool enabled);
bool MPU9250_getSlave1FIFOEnabled();
void MPU9250_setSlave1FIFOEnabled(bool enabled);
bool MPU9250_getSlave0FIFOEnabled();
void MPU9250_setSlave0FIFOEnabled(bool enabled);

// I2C_MST_CTRL register
bool MPU9250_getMultiMasterEnabled();
void MPU9250_setMultiMasterEnabled(bool enabled);
bool MPU9250_getWaitForExternalSensorEnabled();
void MPU9250_setWaitForExternalSensorEnabled(bool enabled);
bool MPU9250_getSlave3FIFOEnabled();
void MPU9250_setSlave3FIFOEnabled(bool enabled);
bool MPU9250_getSlaveReadWriteTransitionEnabled();
void MPU9250_setSlaveReadWriteTransitionEnabled(bool enabled);
uint8_t MPU9250_getMasterClockSpeed();
void MPU9250_setMasterClockSpeed(uint8_t speed);

// I2C_SLV* registers (Slave 0-3)
uint8_t MPU9250_getSlaveAddress(uint8_t num);
void MPU9250_setSlaveAddress(uint8_t num, uint8_t address);
uint8_t MPU9250_getSlaveRegister(uint8_t num);
void MPU9250_setSlaveRegister(uint8_t num, uint8_t reg);
bool MPU9250_getSlaveEnabled(uint8_t num);
void MPU9250_setSlaveEnabled(uint8_t num, bool enabled);
bool MPU9250_getSlaveWordByteSwap(uint8_t num);
void MPU9250_setSlaveWordByteSwap(uint8_t num, bool enabled);
bool MPU9250_getSlaveWriteMode(uint8_t num);
void MPU9250_setSlaveWriteMode(uint8_t num, bool mode);
bool MPU9250_getSlaveWordGroupOffset(uint8_t num);
void MPU9250_setSlaveWordGroupOffset(uint8_t num, bool enabled);
uint8_t MPU9250_getSlaveDataLength(uint8_t num);
void MPU9250_setSlaveDataLength(uint8_t num, uint8_t length);

// I2C_SLV* registers (Slave 4)
uint8_t MPU9250_getSlave4Address();
void MPU9250_setSlave4Address(uint8_t address);
uint8_t MPU9250_getSlave4Register();
void MPU9250_setSlave4Register(uint8_t reg);
void MPU9250_setSlave4OutputByte(uint8_t data);
bool MPU9250_getSlave4Enabled();
void MPU9250_setSlave4Enabled(bool enabled);
bool MPU9250_getSlave4InterruptEnabled();
void MPU9250_setSlave4InterruptEnabled(bool enabled);
bool MPU9250_getSlave4WriteMode();
void MPU9250_setSlave4WriteMode(bool mode);
uint8_t MPU9250_getSlave4MasterDelay();
void MPU9250_setSlave4MasterDelay(uint8_t delay);
uint8_t MPU9250_getSlate4InputByte();

// I2C_MST_STATUS register
bool MPU9250_getPassthroughStatus();
bool MPU9250_getSlave4IsDone();
bool MPU9250_getLostArbitration();
bool MPU9250_getSlave4Nack();
bool MPU9250_getSlave3Nack();
bool MPU9250_getSlave2Nack();
bool MPU9250_getSlave1Nack();
bool MPU9250_getSlave0Nack();

// INT_PIN_CFG register
bool MPU9250_getInterruptMode();
void MPU9250_setInterruptMode(bool mode);
bool MPU9250_getInterruptDrive();
void MPU9250_setInterruptDrive(bool drive);
bool MPU9250_getInterruptLatch();
void MPU9250_setInterruptLatch(bool latch);
bool MPU9250_getInterruptLatchClear();
void MPU9250_setInterruptLatchClear(bool clear);
bool MPU9250_getFSyncInterruptLevel();
void MPU9250_setFSyncInterruptLevel(bool level);
bool MPU9250_getFSyncInterruptEnabled();
void MPU9250_setFSyncInterruptEnabled(bool enabled);
bool MPU9250_getI2CBypassEnabled();
void MPU9250_setI2CBypassEnabled(bool enabled);
bool MPU9250_getClockOutputEnabled();
void MPU9250_setClockOutputEnabled(bool enabled);

// INT_ENABLE register
uint8_t MPU9250_getIntEnabled();
void MPU9250_setIntEnabled(uint8_t enabled);
bool MPU9250_getIntFreefallEnabled();
void MPU9250_setIntFreefallEnabled(bool enabled);
bool MPU9250_getIntMotionEnabled();
void MPU9250_setIntMotionEnabled(bool enabled);
bool MPU9250_getIntZeroMotionEnabled();
void MPU9250_setIntZeroMotionEnabled(bool enabled);
bool MPU9250_getIntFIFOBufferOverflowEnabled();
void MPU9250_setIntFIFOBufferOverflowEnabled(bool enabled);
bool MPU9250_getIntI2CMasterEnabled();
void MPU9250_setIntI2CMasterEnabled(bool enabled);
bool MPU9250_getIntDataReadyEnabled();
void MPU9250_setIntDataReadyEnabled(bool enabled);

// INT_STATUS register
uint8_t MPU9250_getIntStatus();
bool MPU9250_getIntFreefallStatus();
bool MPU9250_getIntMotionStatus();
bool MPU9250_getIntZeroMotionStatus();
bool MPU9250_getIntFIFOBufferOverflowStatus();
bool MPU9250_getIntI2CMasterStatus();
bool MPU9250_getIntDataReadyStatus();

// ACCEL_*OUT_* registers
void MPU9250_getMotion6(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz);



// I2C_SLV*_DO register
void MPU9250_setSlaveOutputByte(uint8_t num, uint8_t data);


// MOT_DETECT_CTRL register
uint8_t MPU9250_getAccelerometerPowerOnDelay();
void MPU9250_setAccelerometerPowerOnDelay(uint8_t delay);
uint8_t MPU9250_getFreefallDetectionCounterDecrement();
void MPU9250_setFreefallDetectionCounterDecrement(uint8_t decrement);
uint8_t MPU9250_getMotionDetectionCounterDecrement();
void MPU9250_setMotionDetectionCounterDecrement(uint8_t decrement);

// USER_CTRL register
bool MPU9250_getFIFOEnabled();
void MPU9250_setFIFOEnabled(bool enabled);
bool MPU9250_getI2CMasterModeEnabled();
void MPU9250_setI2CMasterModeEnabled(bool enabled);
void MPU9250_switchSPIEnabled(bool enabled);
void MPU9250_resetFIFO();
void MPU9250_resetI2CMaster();
void MPU9250_resetSensors();

// PWR_MGMT_1 register
void MPU9250_reset();
bool MPU9250_getSleepEnabled();
void MPU9250_setSleepEnabled(bool enabled);
bool MPU9250_getWakeCycleEnabled();
void MPU9250_setWakeCycleEnabled(bool enabled);
bool MPU9250_getTempSensorEnabled();
void MPU9250_setTempSensorEnabled(bool enabled);
uint8_t MPU9250_getClockSource();
void MPU9250_setClockSource(uint8_t source);

// PWR_MGMT_2 register
uint8_t MPU9250_getWakeFrequency();
void MPU9250_setWakeFrequency(uint8_t frequency);
bool MPU9250_getStandbyXAccelEnabled();
void MPU9250_setStandbyXAccelEnabled(bool enabled);
bool MPU9250_getStandbyYAccelEnabled();
void MPU9250_setStandbyYAccelEnabled(bool enabled);
bool MPU9250_getStandbyZAccelEnabled();
void MPU9250_setStandbyZAccelEnabled(bool enabled);
bool MPU9250_getStandbyXGyroEnabled();
void MPU9250_setStandbyXGyroEnabled(bool enabled);
bool MPU9250_getStandbyYGyroEnabled();
void MPU9250_setStandbyYGyroEnabled(bool enabled);
bool MPU9250_getStandbyZGyroEnabled();
void MPU9250_setStandbyZGyroEnabled(bool enabled);

// FIFO_COUNT_* registers
uint16_t MPU9250_getFIFOCount();

// FIFO_R_W register
uint8_t MPU9250_getFIFOByte();
void MPU9250_setFIFOByte(uint8_t data);
void MPU9250_getFIFOBytes(uint8_t *data, uint8_t length);

// WHO_AM_I register
uint8_t MPU9250_getDeviceID();
void MPU9250_setDeviceID(uint8_t id);
void MPU9250_EnableAccFilter(void);
#endif /* _MPU9250_H_ */

/* [] END OF FILE */
