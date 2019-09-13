#include <stdio.h>
#include "stimer.h"

int main(void)
{
    stimer_t *timer1 = stimer_create(STIMER_LOOP, STIMER_NO_CALLBACK);
    stimer_t *timer2 = stimer_create(STIMER_ONE_SHOT, STIMER_NO_CALLBACK);
    stimer_t *timer3 = stimer_create(STIMER_ONE_SHOT, STIMER_NO_CALLBACK);

    if (!timer1 || !timer2 || !timer3)
    {
        printf("cannot create one or more timers\n");
        return 1;
    }

    stimer_set_time(timer1, 10);
    stimer_set_time(timer2, 20);
    stimer_set_time(timer3, 30);

    stimer_start(timer1);
    stimer_start(timer2);
    stimer_start(timer3);

    printf("timer tick period: %ius\n", STIMER_TICK_PERIOD);
    printf("timer1 set to: 10ms\n");
    printf("timer2 set to: 20ms\n");
    printf("timer3 set to: 30ms\n");

    int ticks = 0;
    while (1)
    {
        stimer_tick();
        printf("tick %i\n", ++ticks);

        if (stimer_overflow(timer1))
        {
            printf("timer1 overflow\n");
        }

        if (stimer_overflow(timer2))
        {
            printf("timer2 overflow\n");
            stimer_destroy(timer1);
        }

        if (stimer_overflow(timer3))
        {
            printf("timer3 overflow\n");
            break;
        }
    }

    stimer_destroy(timer2);
    stimer_destroy(timer3);

    return 0;
}
