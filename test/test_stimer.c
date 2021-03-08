/*
****************************************************************************************************
*       INCLUDE FILES
****************************************************************************************************
*/

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <cmocka.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

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

typedef struct {
    stimer_t *timer;
    bool callback_flag;
    pthread_t tick_thread;
    bool running;
} test_state_t;

/*
****************************************************************************************************
*       INTERNAL GLOBAL VARIABLES
****************************************************************************************************
*/


/*
****************************************************************************************************
*       INTERNAL FUNCTIONS
****************************************************************************************************
*/

static void* timer_tick_thread(void *arg)
{
    test_state_t *test_state = arg;
    test_state->running = true;

    while (usleep(1000) == 0 && test_state->running)
    {
        stimer_tick();
    }

    return 0;
}

static void process_timer_event(void *arg)
{
    test_state_t *data = arg;

    assert_int_equal(stimer_overflow(data->timer), 1);
    stimer_reset(data->timer);

    assert_false(data->callback_flag);
    data->callback_flag = true;
}

// this run once before all group tests
static int group_setup(void **state)
{
    test_state_t *test_state = calloc(1, sizeof(test_state_t));
    *state = test_state;

    // create a thread to use as timer tick
    int ret = pthread_create(&test_state->tick_thread, NULL, timer_tick_thread, test_state);
    assert_int_equal(ret, 0);

    return 0;
}

// this run once after all group tests
static int group_teardown(void **state)
{
    test_state_t *test_state = *state;
    test_state->running = false;
    pthread_join(test_state->tick_thread, NULL);
    free(test_state);

    return 0;
}

static void test_timer_create(void **state)
{
    test_state_t *test_state = *state;
    stimer_t *timers[STIMER_MAX_INSTANCES];

    // create as many timers as possible
    for (int i = 0; i < STIMER_MAX_INSTANCES; i++)
    {
        timers[i] = stimer_create(STIMER_ONE_SHOT, STIMER_NO_CALLBACK);
        assert_non_null(timers[i]);
    }

    // should return null as all timers have been used
    assert_null(stimer_create(STIMER_ONE_SHOT, STIMER_NO_CALLBACK));

    // destroy all created timers
    for (int i = 0; i < STIMER_MAX_INSTANCES; i++)
    {
        stimer_destroy(timers[i]);
    }

    // create a new timer again
    test_state->timer = stimer_create(STIMER_LOOP, STIMER_NO_CALLBACK);
    assert_non_null(test_state->timer);
}

static void test_timer_100ms(void **state)
{
    test_state_t *test_state = *state;
    stimer_t *timer = test_state->timer;

    const int period_ms = 50;
    stimer_set_time(timer, period_ms);
    stimer_start(timer);

    // timer must not overflow as it was just created
    assert_int_equal(stimer_overflow(timer), 0);

    // wait time enough until timer overflows
    usleep(period_ms * 1000 * 1.15);

    // timer should overflow just once
    assert_int_equal(stimer_overflow(timer), 1);

    stimer_stop(timer);
}

static void test_timer_callback(void **state)
{
    test_state_t *test_state = *state;
    stimer_t *timer = test_state->timer;
    test_state->callback_flag = false;

    // destroy previous timer and create a new one with callback
    stimer_destroy(timer);
    timer = stimer_create(STIMER_ONE_SHOT, process_timer_event);
    assert_non_null(timer);

    // add custom argument to timer
    stimer_argument(timer, test_state);

    stimer_set_time(timer, 10);
    stimer_start(timer);
    usleep(50000);

    // callback flag shall be set to true in case it worked
    assert_true(test_state->callback_flag);
}


/*
****************************************************************************************************
*       MAIN FUNCTION
****************************************************************************************************
*/

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_timer_create),
        cmocka_unit_test(test_timer_100ms),
        cmocka_unit_test(test_timer_callback),
    };

    return cmocka_run_group_tests(tests, group_setup, group_teardown);
}
