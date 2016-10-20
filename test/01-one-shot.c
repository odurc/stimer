#include <stdio.h>
#include "stimer.h"

int main(void)
{
    stimer_t *timer = stimer_create(STIMER_ONE_SHOT, STIMER_NO_CALLBACK);

    if (!timer)
    {
        printf("cannot create timer\n");
        return 1;
    }

    stimer_set_time(timer, 10);
    stimer_start(timer);

    printf("timer tick period: %ius\n", STIMER_TICK_PERIOD);
    printf("timer set to: 10ms\n");

    int ticks = 0;
    while (1)
    {
        stimer_tick();
        printf("tick %i\n", ++ticks);

        if (stimer_overflow(timer))
        {
            printf("timer overflow\n");
        }

        if (ticks >= 30)
            break;
    }

    stimer_destroy(timer);

    return 0;
}
