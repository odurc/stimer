find_package(Threads REQUIRED)
add_mocked_test(stimer LINK_LIBRARIES ${STIMER_LIBRARY_NAME} Threads::Threads)
