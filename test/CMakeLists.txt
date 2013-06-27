
IF(JEMALLOC_LIBRARY)
  ADD_EXECUTABLE(message_queue_test
    message_queue_test.cc
    )
  TARGET_LINK_LIBRARIES(message_queue_test green_turtle jemalloc)
ELSE(JEMALLOC_LIBRARY)
  ADD_EXECUTABLE(message_queue_test
    message_queue_test.cc
    )
  TARGET_LINK_LIBRARIES(message_queue_test green_turtle)
ENDIF(JEMALLOC_LIBRARY)


ADD_EXECUTABLE(timer_test
  timer_test.cc
  )
TARGET_LINK_LIBRARIES(timer_test green_turtle)

ADD_SUBDIRECTORY(net)
