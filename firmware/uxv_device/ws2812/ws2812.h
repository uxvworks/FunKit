/**
 * @file    ws2812.h
 * @author  Austin Glaser <austin.glaser@gmail.com>
 * @brief   Interface to WS2812 LED driver
 *
 * Copyright (C) 2016 Austin Glaser
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 * @todo    Put in names and descriptions of variables which need to be defined to use this file
 */

#ifndef WS2812_H_
#define WS2812_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup WS2812 WS2812 Driver
 * @{
 *
 * @brief   DMA-based WS2812 LED driver
 *
 * A driver for WS2812 LEDs
 */

/* --- PUBLIC DEPENDENCIES -------------------------------------------------- */

// Standard
#include <stdint.h>

/* --- PUBLIC CONSTANTS ----------------------------------------------------- */

// TODO: Add these #define's to the headers of your project.
// Pin, timer and dma are all connected, check them all if you change one.
// Tested with STM32F4, working at 144 or 168 MHz.
#define WS2812_LED_N    7 // Number of LEDs
#define PORT_WS2812     GPIOB
#define PIN_WS2812      0
#define WS2812_TIM_N    3  // timer, 1-11
#define WS2812_TIM_CH   2  // timer channel, 0-3 (STM docs count ch1-4 so this needs CHx-1
#define WS2812_DMA_STREAM STM32_DMA1_STREAM2  // DMA stream for TIMx_UP (look up in reference manual under DMA Channel selection)
#define WS2812_DMA_CHANNEL 5                  // DMA channel for TIMx_UP

// The WS2812 expects 5V signal level (or at least 0.7 * VDD). Sometimes it works
// with a 3V signal level, otherwise the easiest way to get the signal level to 5V
// is to add an external pullup resistor from the DI pin to 5V (10k will do) and
// configure the pin as open drain.
// (An SMD resistor is easily solders on the connections of a light strip)
// Uncomment the next line if an external pullup resistor is used.
//#define WS2812_EXTERNAL_PULLUP

/**
 * @brief   Return codes from ws2812 interface functions
 */
typedef enum {
    WS2812_SUCCESS      = 0x00,     /**< Operation completeed successfully */
    WS2812_LED_INVALID,             /**< Attempted to index an invalid LED (@ref WS2812_N_LEDS) */
    MAX_WS2812_ERR,                 /**< Total number of possible error codes */
    WS2812_ERR_INVALID              /**< Invalid error value */
} ws2812_err_t;

/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

/**
 * @brief   Initialize the driver
 *
 * After this function is called, all necessary background tasks will be started.
 * The frame is initially dark.
 */
void ws2812_init(void);

/**
 * @brief   Write the value of a single LED in the chain
 *
 * The color value is written to a frame buffer, and will not
 * be updated until the next frame is written. Frames are written
 * at the maximum possible speed -- the longest latency between a
 * call to this function and the value being displayed is
 * 1.25uS*(24*@ref WS2812_LED_N + 50)
 *
 * @param[in] led_number:           The index of the LED to be written. Must be strictly less than
 *                                  @ref WS2812_N_LEDS
 * @param[in] r:                    The red level of the LED
 * @param[in] g:                    The green level of the LED
 * @param[in] b:                    The blue level of the LED
 *
 * @retval WS2812_SUCCESS:          The write was successful
 * @retval WS2812_LED_INVALID:      The write was to an invalid LED index
 */
ws2812_err_t ws2812_write_led(uint32_t led_number, uint8_t r, uint8_t g, uint8_t b);

/** @} defgroup WS2812 */

#ifdef __cplusplus
}
#endif

#endif //ifndef WS2812_H_
