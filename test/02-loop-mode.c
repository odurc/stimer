#include <stdio.h>
#include "stimer.h"

int main(void)
{
    stimer_t *timer = stimer_create(STIMER_LOOP, STIMER_NO_CALLBACK);

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
        printf("tick %i\n", ++ticks);
        stimer_tick();

        if (stimer_overflow(timer))
        {
            printf("timer overflow\n");
        }

        if (ticks == 26)
            stimer_stop(timer);

        if (ticks == 40)
            stimer_start(timer);

        if (ticks >= 50)
            break;
    }

    stimer_destroy(timer);

    return 0;
}
