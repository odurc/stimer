#ifndef STIMER_H
#define STIMER_H

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

#define STIMER_NO_CALLBACK      0

#define STIMER_SEC_TO_MS(t)     ((uint32_t)(t)*(uint32_t)1000)
#define STIMER_MIN_TO_MS(t)     ((uint32_t)(t)*(uint32_t)60000)
#define STIMER_HOUR_TO_MS(t)    ((uint32_t)(t)*(uint32_t)3600000)


/*
****************************************************************************************************
*       CONFIGURATION
****************************************************************************************************
*/

// tick period in us
#define STIMER_TICK_PERIOD      1000

// maximum of timers instances
#define STIMER_MAX_INSTANCES    10


/*
****************************************************************************************************
*       DATA TYPES
****************************************************************************************************
*/

typedef struct stimer_t stimer_t;

typedef enum stimer_mode_t {STIMER_ONE_SHOT, STIMER_LOOP} stimer_mode_t;


/*
****************************************************************************************************
*       FUNCTION PROTOTYPES
****************************************************************************************************
*/

stimer_t *stimer_create(stimer_mode_t mode, void (*callback)(void *arg));
void stimer_destroy(stimer_t *timer);

void stimer_set_time(stimer_t *timer, uint32_t time_ms);
void stimer_start(stimer_t *timer);
void stimer_stop(stimer_t *timer);
void stimer_reset(stimer_t *timer);
int stimer_overflow(stimer_t *timer);

void stimer_tick(void);


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

#endif
