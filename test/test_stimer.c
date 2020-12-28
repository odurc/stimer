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
    stimer_t *timer = 0;

    // create as many timers as possible
    for (int i = 0; i < STIMER_MAX_INSTANCES; i++)
    {
        timer = stimer_create(STIMER_ONE_SHOT, 0);
        assert_non_null(timer);
    }

    // should return null as all timers have been used
    assert_null(stimer_create(STIMER_ONE_SHOT, 0));

    // store last timer created
    test_state->timer = timer;
    assert_non_null(test_state->timer);
}

static void test_timer_100ms(void **state)
{
    test_state_t *test_state = *state;
    stimer_t *timer = test_state->timer;

    stimer_set_time(timer, 100);
    stimer_start(timer);

    assert_int_equal(stimer_overflow(timer), 0);
    usleep(120000);
    assert_int_equal(stimer_overflow(timer), 1);
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
    };

    return cmocka_run_group_tests(tests, group_setup, group_teardown);
}
