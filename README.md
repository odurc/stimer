stimer
======

This is a simple software timer library implemented in C. It is designed to use only static memory
allocation which makes its use interesting for microcontrollers. The goal of the library is to offer
an easy way to create and manage multiple time events for reference usage, it's not designed to
provide accurate time counting.


Features
---

* oneshot and loop timer modes
* optional callback function for time overflow
* no external dependency
* static memory allocation
* easy to use and setup


How to install
---

There is no installation, simply copy the content of `src` directory to your work directory and
adjust your build file or IDE as necessary.


How to use
---

The function `stimer_tick()` must be called from an interrupt service routine (ISR). The ISR
provides a clock tick for the library, the minimum increment of time it will be able to count.
Ideally, your implementation should use a hardware timer ISR exclusively for this purpose to prevent
time overhead processing secondary code.

:warning: Be careful when using callback functions since `stimer_tick()` is called from an ISR the
callback will also called from the same ISR. Read more on the API docs.

To see more details on how to use the library, please check the online [API documentation][].

Configuration
---

The configuration of the library is done by setting 'define' macros in the header file,
under the configuration section.

There are two configuration macros that can be tweaked: *STIMER_TICK_PERIOD* and
*STIMER_MAX_INSTANCES*. The macro *STIMER_TICK_PERIOD* must be set to the same period of the ISR
used for `stimer_tick()`. On the other hand, *STIMER_MAX_INSTANCES* macro should be adjusted to the
amount of timers used in your implementation.


License
---

MIT

[API documentation]: https://odurc.gitlab.io/stimer
