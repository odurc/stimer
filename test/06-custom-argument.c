#include <stdio.h>
#include <string.h>
#include "stimer.h"

typedef struct my_data_t {
    stimer_t *timer;
    char name[8];
} my_data_t;

void timer_cb(void *arg)
{
    my_data_t *my_data = (my_data_t *) arg;

    static int cb_counter;
    printf("%s callback %i\n", my_data->name, cb_counter++);

    if (cb_counter == 2)
        stimer_stop(my_data->timer);
}

int main(void)
{
    stimer_t *timer = stimer_create(STIMER_LOOP, timer_cb);

    my_data_t my_data;
    strcpy(my_data.name, "Timer1");
    my_data.timer = timer;

    stimer_argument(timer, &my_data);

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

        if (ticks >= 35)
            stimer_start(timer);

        if (ticks >= 50)
            break;
    }

    stimer_destroy(timer);

    return 0;
}
