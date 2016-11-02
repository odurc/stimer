/*
 * stimer - Simple Timer Library
 * https://github.com/ricardocrudo/stimer
 *
 * Copyright (c) 2016 Ricardo Crudo <ricardo.crudo@gmail.com>
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

/*
****************************************************************************************************
*       INCLUDE FILES
****************************************************************************************************
*/

#include "stimer.h"


/*
****************************************************************************************************
*       INTERNAL MACROS
****************************************************************************************************
*/


/*
****************************************************************************************************
*       INTERNAL CONSTANTS
****************************************************************************************************
*/


/*
****************************************************************************************************
*       INTERNAL DATA TYPES
****************************************************************************************************
*/

struct stimer_t {
    uint32_t time, counter;
    int enabled, overflow;
    stimer_mode_t mode;
    void (*callback)(void *arg);
};


/*
****************************************************************************************************
*       INTERNAL GLOBAL VARIABLES
****************************************************************************************************
*/

stimer_t g_timers[STIMER_MAX_INSTANCES];


/*
****************************************************************************************************
*       INTERNAL FUNCTIONS
****************************************************************************************************
*/

static inline void *stimer_take(void)
{
    static unsigned int stimer_counter;

    // first time timers are requested
    if (stimer_counter < STIMER_MAX_INSTANCES)
    {
        stimer_t *timer  = &g_timers[stimer_counter++];
        return timer;
    }

    // iterate all array searching for a free spot
    // a timer is considered free when enabled is negative
    for (int i = 0; i < STIMER_MAX_INSTANCES; i++)
    {
        stimer_t *timer  = &g_timers[i];
        if (timer->enabled < 0)
            return timer;
    }

    return 0;
}

static inline void stimer_give(void *timer)
{
    if (timer)
    {
        stimer_t *self = timer;
        self->enabled = -1;
    }
}


/*
****************************************************************************************************
*       GLOBAL FUNCTIONS
****************************************************************************************************
*/

stimer_t *stimer_create(stimer_mode_t mode, void (*callback)(void *arg))
{
    stimer_t *timer = stimer_take();

    if (timer)
    {
        timer->enabled = 0;
        timer->overflow = 0;
        timer->mode = mode;
        timer->callback = callback;
    }

    return timer;
}

void stimer_destroy(stimer_t *timer)
{
    stimer_give(timer);
}

void stimer_set_time(stimer_t *timer, uint32_t time_ms)
{
    timer->time = time_ms;
    timer->counter = (time_ms * 1000) / STIMER_TICK_PERIOD;
}

void stimer_start(stimer_t *timer)
{
    timer->enabled = 1;
}

void stimer_stop(stimer_t *timer)
{
    timer->enabled = 0;
}

void stimer_reset(stimer_t *timer)
{
    timer->enabled = 0;
    timer->overflow = 0;
    stimer_set_time(timer, timer->time);
}

int stimer_overflow(stimer_t *timer)
{
    int overflow = timer->overflow;
    timer->overflow = 0;

    // disable and reload timer
    if (timer->mode == STIMER_ONE_SHOT && overflow)
    {
        timer->enabled = 0;
        stimer_set_time(timer, timer->time);
    }

    return overflow;
}

void stimer_tick(void)
{
    for (int i = 0; i < STIMER_MAX_INSTANCES; i++)
    {
        stimer_t *timer = &g_timers[i];

        if (!timer->enabled)
            continue;

        if (timer->counter > 0)
            timer->counter--;

        if (timer->counter == 0)
        {
            timer->overflow++;

            if (timer->callback)
            {
                if (timer->mode == STIMER_ONE_SHOT)
                    timer->enabled = 0;

                timer->callback(timer);
                timer->overflow = 0;
            }

            // always reload the timer
            stimer_set_time(timer, timer->time);
        }
    }
}
