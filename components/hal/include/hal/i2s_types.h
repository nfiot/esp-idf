/*
 * SPDX-FileCopyrightText: 2020-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "esp_bit_defs.h"
#include "soc/soc_caps.h"
#include "soc/clk_tree_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief I2S channel slot mode
 */
typedef enum {
    I2S_SLOT_MODE_MONO = 1,          /*!< I2S channel slot format mono, transmit same data in all slots for tx mode, only receive the data in the first slots for rx mode. */
    I2S_SLOT_MODE_STEREO = 2,        /*!< I2S channel slot format stereo, transmit different data in different slots for tx mode, receive the data in all slots for rx mode. */
} i2s_slot_mode_t;

/**
 * @brief I2S channel direction
 */
typedef enum {
    I2S_DIR_RX = BIT(0),            /*!< I2S channel direction RX */
    I2S_DIR_TX = BIT(1),            /*!< I2S channel direction TX */
} i2s_dir_t;

/**
 * @brief I2S controller role
 */
typedef enum {
    I2S_ROLE_MASTER,                /*!< I2S controller master role, bclk and ws signal will be set to output */
    I2S_ROLE_SLAVE                  /*!< I2S controller slave role, bclk and ws signal will be set to input */
} i2s_role_t;

/**
 * @brief Available data bit width in one slot
 */
typedef enum {
    I2S_DATA_BIT_WIDTH_8BIT    = 8,            /*!< I2S channel data bit-width: 8 */
    I2S_DATA_BIT_WIDTH_16BIT   = 16,           /*!< I2S channel data bit-width: 16 */
    I2S_DATA_BIT_WIDTH_24BIT   = 24,           /*!< I2S channel data bit-width: 24 */
    I2S_DATA_BIT_WIDTH_32BIT   = 32,           /*!< I2S channel data bit-width: 32 */
} i2s_data_bit_width_t;

/**
 * @brief Total slot bit width in one slot
 *
 */
typedef enum {
    I2S_SLOT_BIT_WIDTH_AUTO           = (0),      /*!< I2S channel slot bit-width equals to data bit-width */
    I2S_SLOT_BIT_WIDTH_8BIT           = (8),      /*!< I2S channel slot bit-width: 8 */
    I2S_SLOT_BIT_WIDTH_16BIT          = (16),     /*!< I2S channel slot bit-width: 16 */
    I2S_SLOT_BIT_WIDTH_24BIT          = (24),     /*!< I2S channel slot bit-width: 24 */
    I2S_SLOT_BIT_WIDTH_32BIT          = (32),     /*!< I2S channel slot bit-width: 32 */
} i2s_slot_bit_width_t;

#if SOC_I2S_SUPPORTED
typedef soc_periph_i2s_clk_src_t    i2s_clock_src_t; /*!< I2S clock source */
#else
typedef int                         i2s_clock_src_t; /*!< Define a default type to avoid compiling warnings */
#endif


#if SOC_I2S_SUPPORTS_PCM
/**
 * @brief A/U-law decompress or compress configuration.
 *
 */
typedef enum {
    I2S_PCM_DISABLE = 0,      /*!< Disable A/U law decompress or compress*/
    I2S_PCM_A_DECOMPRESS,     /*!< A-law decompress*/
    I2S_PCM_A_COMPRESS,       /*!< A-law compress*/
    I2S_PCM_U_DECOMPRESS,     /*!< U-law decompress*/
    I2S_PCM_U_COMPRESS,       /*!< U-law compress*/
} i2s_pcm_compress_t;
#endif // SOC_I2S_SUPPORTS_PCM

/**
 * @brief I2S PDM data format
 *
 */
typedef enum {
    I2S_PDM_DATA_FMT_PCM = 0,       /*!< PDM RX:
                                     *   Enable the hardware PDM to PCM filter to convert the inputted PDM data on the line into PCM format in software,
                                     *   so that the read data in software is PCM format data already, no need additional software filter.
                                     *   PCM data format is only available when PCM2PDM filter is supported in hardware.
                                     *   PDM TX:
                                     *   Enable the hardware PCM to PDM filter to convert the written PCM data in software into PDM format on the line,
                                     *   so that we only need to write the PCM data in software, no need to prepare raw PDM data in software.
                                     *   PCM data format is only available when PDM2PCM filter is supported in hardware.
                                     */
    I2S_PDM_DATA_FMT_RAW = 1,       /*!< PDM RX:
                                     *   Read the raw PDM data directly in software, without the hardware PDM to PCM filter.
                                     *   You may need a software PDM to PCM filter to convert the raw PDM data that read into PCM format.
                                     *   PDM TX:
                                     *   Write the raw PDM data directly in software, without the hardware PCM to PDM filter.
                                     *   You may need to prepare the raw PDM data in software to output the PDM format data on the line.
                                     */
} i2s_pdm_data_fmt_t;

#if SOC_I2S_SUPPORTS_PDM_RX
/**
 * @brief I2S PDM RX down-sampling mode
 */
typedef enum {
    I2S_PDM_DSR_8S = 0,  /*!< downsampling number is 8 for PDM RX mode*/
    I2S_PDM_DSR_16S,     /*!< downsampling number is 16 for PDM RX mode*/
    I2S_PDM_DSR_MAX,
} i2s_pdm_dsr_t;
#endif // SOC_I2S_SUPPORTS_PDM_RX

#if SOC_I2S_SUPPORTS_PDM_TX
/**
 * @brief pdm tx signal scaling mode
 */
typedef enum {
    I2S_PDM_SIG_SCALING_DIV_2 = 0,   /*!< I2S TX PDM signal scaling: /2 */
    I2S_PDM_SIG_SCALING_MUL_1 = 1,   /*!< I2S TX PDM signal scaling: x1 */
    I2S_PDM_SIG_SCALING_MUL_2 = 2,   /*!< I2S TX PDM signal scaling: x2 */
    I2S_PDM_SIG_SCALING_MUL_4 = 3,   /*!< I2S TX PDM signal scaling: x4 */
} i2s_pdm_sig_scale_t;

#if SOC_I2S_HW_VERSION_2
/**
 * @brief PDM TX line mode
 * @note  For the standard codec mode, PDM pins are connect to a codec which requires both clock signal and data signal
 *        For the DAC output mode, PDM data signal can be connected to a power amplifier directly with a low-pass filter,
 *        normally, DAC output mode doesn't need the clock signal.
 */
typedef enum {
    I2S_PDM_TX_ONE_LINE_CODEC,         /*!< Standard PDM format output, left and right slot data on a single line */
    I2S_PDM_TX_ONE_LINE_DAC,           /*!< PDM DAC format output, left or right slot data on a single line */
    I2S_PDM_TX_TWO_LINE_DAC,           /*!< PDM DAC format output, left and right slot data on separated lines */
} i2s_pdm_tx_line_mode_t;
#endif // SOC_I2S_HW_VERSION_2
#endif // SOC_I2S_SUPPORTS_PDM_TX

/**
 * @brief I2S slot select in standard mode
 * @note  It has different meanings in tx/rx/mono/stereo mode, and it may have different behaviors on different targets
 *        For the details, please refer to the I2S API reference
 */
typedef enum {
    I2S_STD_SLOT_LEFT   = BIT(0),               /*!< I2S transmits or receives left slot */
    I2S_STD_SLOT_RIGHT  = BIT(1),               /*!< I2S transmits or receives right slot */
    I2S_STD_SLOT_BOTH   = BIT(0) | BIT(1),      /*!< I2S transmits or receives both left and right slot */
} i2s_std_slot_mask_t;

/**
 * @brief I2S slot select in PDM mode
 *
 */
typedef enum {
    I2S_PDM_SLOT_RIGHT      = BIT(0),           /*!< I2S PDM only transmits or receives the PDM device whose 'select' pin is pulled up */
    I2S_PDM_SLOT_LEFT       = BIT(1),           /*!< I2S PDM only transmits or receives the PDM device whose 'select' pin is pulled down */
    I2S_PDM_SLOT_BOTH       = BIT(0) | BIT(1),  /*!< I2S PDM transmits or receives both two slots */
#if SOC_I2S_PDM_MAX_RX_LINES > 1
    /* The following enumerators are only used in multi-line PDM RX mode */
    I2S_PDM_RX_LINE0_SLOT_RIGHT  = I2S_PDM_SLOT_RIGHT,  /*!< I2S PDM receives the right slot on line 0 */
    I2S_PDM_RX_LINE0_SLOT_LEFT   = I2S_PDM_SLOT_LEFT,   /*!< I2S PDM receives the left slot on line 0 */
    I2S_PDM_RX_LINE1_SLOT_RIGHT  = BIT(2),              /*!< I2S PDM receives the right slot on line 1 */
    I2S_PDM_RX_LINE1_SLOT_LEFT   = BIT(3),              /*!< I2S PDM receives the left slot on line 1 */
    I2S_PDM_RX_LINE2_SLOT_RIGHT  = BIT(4),              /*!< I2S PDM receives the right slot on line 2 */
    I2S_PDM_RX_LINE2_SLOT_LEFT   = BIT(5),              /*!< I2S PDM receives the left slot on line 2 */
    I2S_PDM_RX_LINE3_SLOT_RIGHT  = BIT(6),              /*!< I2S PDM receives the right slot on line 3 */
    I2S_PDM_RX_LINE3_SLOT_LEFT   = BIT(7),              /*!< I2S PDM receives the left slot on line 3 */
    I2S_PDM_LINE_SLOT_ALL        = 0x00ff,              /*!< I2S PDM receives all slots */
#endif
} i2s_pdm_slot_mask_t;

#if SOC_I2S_SUPPORTS_TDM
/**
 * @brief tdm slot number
 * @note  Multiple slots in TDM mode.
 *        For TX module, only the active slot send the audio data, the inactive slot send a constant or will be skipped if 'skip_msk' is set.
 *        For RX module, only receive the audio data in active slots, the data in inactive slots will be ignored.
 *        the bit map of active slot can not exceed (0x1<<total_slot_num).
 *        e.g: slot_mask = (I2S_TDM_SLOT0 | I2S_TDM_SLOT3), here the active slot number is 2 and total_slot is not supposed to be smaller than 4.
 */
typedef enum {
    I2S_TDM_SLOT0  = BIT(0),               /*!< I2S slot 0 enabled */
    I2S_TDM_SLOT1  = BIT(1),               /*!< I2S slot 1 enabled */
    I2S_TDM_SLOT2  = BIT(2),               /*!< I2S slot 2 enabled */
    I2S_TDM_SLOT3  = BIT(3),               /*!< I2S slot 3 enabled */
    I2S_TDM_SLOT4  = BIT(4),               /*!< I2S slot 4 enabled */
    I2S_TDM_SLOT5  = BIT(5),               /*!< I2S slot 5 enabled */
    I2S_TDM_SLOT6  = BIT(6),               /*!< I2S slot 6 enabled */
    I2S_TDM_SLOT7  = BIT(7),               /*!< I2S slot 7 enabled */
    I2S_TDM_SLOT8  = BIT(8),               /*!< I2S slot 8 enabled */
    I2S_TDM_SLOT9  = BIT(9),               /*!< I2S slot 9 enabled */
    I2S_TDM_SLOT10 = BIT(10),              /*!< I2S slot 10 enabled */
    I2S_TDM_SLOT11 = BIT(11),              /*!< I2S slot 11 enabled */
    I2S_TDM_SLOT12 = BIT(12),              /*!< I2S slot 12 enabled */
    I2S_TDM_SLOT13 = BIT(13),              /*!< I2S slot 13 enabled */
    I2S_TDM_SLOT14 = BIT(14),              /*!< I2S slot 14 enabled */
    I2S_TDM_SLOT15 = BIT(15),              /*!< I2S slot 15 enabled */
} i2s_tdm_slot_mask_t;
#endif // SOC_I2S_SUPPORTS_TDM

/**
 * @brief I2S channel events that supported by the ETM module
 */
typedef enum {
    /** Trigger condition:
     *  TX: no data to send in the TX FIFO, i.e., DMA need to stop (next desc is NULL)
     *  RX: 1. If rx_stop_mode = 0, this event will trigger when DMA is stopped (next desc is NULL)
     *      2. If rx_stop_mode = 1, this event will trigger when DMA in_suc_eof.
     *      3. If rx_stop_mode = 2, this event will trigger when RX FIFO is full.
     */
    I2S_ETM_EVENT_DONE, /*!< Event that I2S TX or RX stopped */
    /** Trigger condition:
     *  TX: the sent words(in 32-bit) number reach the threshold that configured in `etm_tx_send_word_num`
     *  RX: the received words(in 32-bit) number reach the threshold that configured in `etm_rx_receive_word_num`
     *      and `etm_rx_receive_word_num` should be smaller than the size of the DMA buffer in one `in_suc_eof` event.
     */
    I2S_ETM_EVENT_REACH_THRESH, /*!< Event that the I2S sent or received data reached the threshold */
    I2S_ETM_EVENT_MAX, /*!< Maximum number of events */
} i2s_etm_event_type_t;

/**
 * @brief I2S channel tasks that supported by the ETM module
 */
typedef enum {
    I2S_ETM_TASK_START,     /*!< Start the I2S channel */
    I2S_ETM_TASK_STOP,      /*!< Stop the I2S channel */
#if SOC_I2S_SUPPORTS_TX_FIFO_SYNC
    I2S_ETM_TASK_SYNC_FIFO, /*!< Check the I2S TX channel sync status */
#endif
    I2S_ETM_TASK_MAX,       /*!< Maximum number of tasks */
} i2s_etm_task_type_t;

#ifdef __cplusplus
}
#endif
