project(rtspi)
cmake_minimum_required(VERSION 2.8)

# Check for Xenomai:
set(XENOMAI_SEARCH_PATH /usr/local/xenomai /usr/xenomai /usr)
set(PROJECT_NAME ${PROJECT_NAME})
find_path(XENOMAI_DIR bin/xeno-config ${XENOMAI_SEARCH_PATH})
if (XENOMAI_DIR)
  set (XENOMAI_CONFIG ${XENOMAI_DIR}/bin/xeno-config)
  message ("Xenomai found in ${XENOMAI_DIR}")
  execute_process(COMMAND ${XENOMAI_CONFIG} --native --rtdm --cflags OUTPUT_VARIABLE XENOMAI_CFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
  execute_process(COMMAND ${XENOMAI_CONFIG} --native --rtdm --ldflags OUTPUT_VARIABLE XENOMAI_LDFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
  set (ROSRT_PLATFORM_CFLAGS ${XENOMAI_CFLAGS})
  set (ROSRT_PLATFORM_LDFLAGS ${XENOMAI_LDFLAGS})
endif (XENOMAI_DIR)

# find_path(Name.h ${PATH})
# find_library(lib ${PATH})
# include_directories(${PATH})

# build
add_executable(${PROJECT_NAME} main.c spi.c)

# XENOMAI_CFLAGS
add_definitions(${ROSRT_PLATFORM_CFLAGS})

# XENOMAI_LDFLAGS
target_link_libraries(${PROJECT_NAME} ${ROSRT_PLATFORM_LDFLAGS})
