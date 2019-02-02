/*
 * stimer - Simple Timer Library
 * https://github.com/ricardocrudo/stimer
 *
 * Copyright (c) 2019 Ricardo Crudo <ricardo.crudo@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @file stimer.h
 * @author Ricardo Crudo
 * @brief Simple Timer Library
 */

#ifndef STIMER_H
#define STIMER_H

#ifdef __cplusplus
extern "C"
{
#endif


/*
****************************************************************************************************
*       INCLUDE FILES
****************************************************************************************************
*/

#include <stdint.h>


/*
****************************************************************************************************
*       MACROS
****************************************************************************************************
*/

// library version
#define STIMER_VERSION  "1.2.0"

// macro to set no callback mode
#define STIMER_NO_CALLBACK  0

// macros to convert time units
#define STIMER_SEC_TO_MS(t)     ((uint32_t)(t)*(uint32_t)1000)
#define STIMER_MIN_TO_MS(t)     ((uint32_t)(t)*(uint32_t)60000)
#define STIMER_HOUR_TO_MS(t)    ((uint32_t)(t)*(uint32_t)3600000)


/*
****************************************************************************************************
*       CONFIGURATION
****************************************************************************************************
*/

/** @def STIMER_TICK_PERIOD
 *  Define the tick period in us
 */
#define STIMER_TICK_PERIOD      1000

/** @def STIMER_MAX_INSTANCES
 *  Define the maximum of timers instances
 */
#define STIMER_MAX_INSTANCES    10


/*
****************************************************************************************************
*       DATA TYPES
****************************************************************************************************
*/

/**
 * @struct stimer_t
 * An opaque structure of a stimer object
 */
typedef struct stimer_t stimer_t;

/**
 * @enum stimer_mode_t
 * Timer operating modes
 */
typedef enum stimer_mode_t {
    STIMER_ONE_SHOT,        ///< one shot mode (stop after first overflow)
    STIMER_LOOP             ///< loop mode (keep running after overflow)
} stimer_mode_t;


/*
****************************************************************************************************
*       FUNCTION PROTOTYPES
****************************************************************************************************
*/

/**
 * @defgroup stimer_funcs Timer Functions
 * Set of functions to operate timers.
 * @{
 */

/**
 * Create stimer object
 *
 * When creating a timer the mode must be passed as the first argument and it must be one of the
 * values defined by stimer_mode_t enumeration. The second argument must be either a function
 * pointer or the macro STIMER_NO_CALLBACK. In case a callback function is provided, it will be
 * called when the timer overflow happens.
 *
 * By default the callback provides the timer object as argument. A custom argument can be defined
 * using the function stimer_argument().
 *
 * @param[in] mode must be either STIMER_ONE_SHOT or STIMER_LOOP
 * @param[in] callback a function callback pointer or the macro STIMER_NO_CALLBACK
 *
 * @return pointer to stimer object or NULL if no more timers are available
 */
stimer_t* stimer_create(stimer_mode_t mode, void (*callback)(void *arg));

/**
 * Destroy stimer object
 *
 * @param[in] timer stimer object pointer
 */
void stimer_destroy(stimer_t *timer);

/**
 * Set the timer time
 *
 * Use this function, after create stimer object, to set the time which the timer
 * must overflow. The time parameter is expected to be in milliseconds. The time
 * conversion macros can be used to convert from hour, minute or second.
 *
 * @param[in] timer stimer object pointer
 * @param[in] time_ms time in milliseconds
 */
void stimer_set_time(stimer_t *timer, uint32_t time_ms);

/**
 * Start the timer
 *
 * @param[in] timer stimer object pointer
 */
void stimer_start(stimer_t *timer);

/**
 * Stop the timer
 *
 * The timer counter is NOT reseted when stopped. The counting will continue from
 * the last value when the timer is started again.
 *
 * @param[in] timer stimer object pointer
 */
void stimer_stop(stimer_t *timer);

/**
 * Reset timer
 *
 * The timer is automatically stopped when reseted.
 *
 * @param[in] timer stimer object pointer
 */
void stimer_reset(stimer_t *timer);

/**
 * Check timer overflow
 *
 * Note that once this function is called the overflow counter is zeroed. This is,
 * a second subsequent call will return zero.
 *
 * @param[in] timer stimer object pointer
 *
 * @return number of times the timer overflow
 */
int stimer_overflow(stimer_t *timer);

/**
 * Set the timer argument
 *
 * This function is only useful when a callback is provided during the timer creation.
 * In case a custom argument is required when the callback is evoked, this function has
 * to be used to replace the original argument, which is the timer object itself.
 *
 * @param[in] timer stimer object pointer
 * @param[in] arg the custom argument of the callback function
 */
void stimer_argument(stimer_t *timer, void *arg);

/**
 * The tick function
 *
 * This function must be used to define the clock of the timers. It must be called
 * from an interrupt service routine (ISR). The period of the interruption must be set
 * using the STIMER_TICK_PERIOD macro.
 */
void stimer_tick(void);

/**
 * @}
 */

/*
****************************************************************************************************
*       CONFIGURATION ERRORS
****************************************************************************************************
*/

#if !defined(STIMER_MAX_INSTANCES) || !defined(STIMER_TICK_PERIOD)
#error "STIMER_MAX_INSTANCES and STIMER_TICK_PERIOD macros must be defined"
#endif

#if STIMER_TICK_PERIOD <= 0 || STIMER_TICK_PERIOD > 1000
#error "STIMER_TICK_PERIOD macro value must be set between 1 and 1000"
#endif

#ifdef __cplusplus
}
#endif

// STIMER_H
#endif
