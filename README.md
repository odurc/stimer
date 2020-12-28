stimer
======

This is a simple timer library implemented in C. The library is designed to use only static
memory allocation, what make it interesting to employ in microcontrollers projects or other
devices used in embedded system.


Features
---

* no external dependency
* use only static memory allocation
* easy to use and setup


How to install
---

Simply copy the content of src directory to your work directory.


Configuration
---

The configuration of the library is done by setting 'define' macros in the header file,
under the configuration section.

There are two configuration macros which can be tweaked: *STIMER_TICK_PERIOD* and
*STIMER_MAX_INSTANCES*. The macro *STIMER_TICK_PERIOD* must be set to the period of
the interrupt service routine (ISR). In the other hand *STIMER_MAX_INSTANCES* macro
might be adjusted to the desired amount of timers.


How to use
---

Once the *STIMER_TICK_PERIOD* setting is done, the call of the function stimer_tick()
must be placed inside of the ISR function. Ideally the user should use a hardware timer
ISR exclusively for this purpose.

To see more details how to use the library, please check the online
[API documentation](http://ricardocrudo.github.io/stimer).


Build
---

    mkdir -p build && cd build
    cmake .. -DENABLE_TESTS=Yes -DENABLE_LINTER=Yes -DENABLE_COVERAGE=Yes -DCMAKE_BUILD_TYPE=Debug
    make -j`nproc`
    ctest -V
    ctest -V -T coverage
    ctest -V -T memcheck


License
---

MIT
