cmake_minimum_required (VERSION 3.10)

project (LabOS)

option(USE_CLANG "build application with clang" ON) # OFF is the default

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -target armv7-unknown-freebsd12.0-eabi  --sysroot /BUILDS/embedded/beaglebone/_.w/ -ggdb -O1 -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -target armv7-unknown-freebsd12.0-eabi  --sysroot /BUILDS/embedded/beaglebone/_.w/ -O1 -Wall")

file(GLOB LIBSPI_SRC ${PROJECT_SOURCE_DIR}/spi/GPIO.cpp
                     ${PROJECT_SOURCE_DIR}/spi/SPI.cpp
                     ${PROJECT_SOURCE_DIR}/spi/spiTest.cpp
)
file(GLOB LIBSPI_HDR ${PROJECT_SOURCE_DIR}/spi/GPIO.h
                     ${PROJECT_SOURCE_DIR}/spi/SPI.h
                     ${PROJECT_SOURCE_DIR}/spi/spiTest.h
)

file(GLOB LIBMCASP_SRC ${PROJECT_SOURCE_DIR}/mcasp/McASP.cpp
                       ${PROJECT_SOURCE_DIR}/mcasp/mcaspTest.cpp
)
file(GLOB LIBMCASP_HDR ${PROJECT_SOURCE_DIR}/mcasp/McASP.h
                       ${PROJECT_SOURCE_DIR}/mcasp/mcaspTest.h
)

add_library(spi ${LIBSPI_SRC} ${LIBSPI_HDR})
add_library(mcasp ${LIBMCASP_SRC} ${LIBMCASP_HDR})

add_executable(LabOS LabOS.cpp)

target_include_directories(LabOS PUBLIC
                          ${PROJECT_SOURCE_DIR} 
			  ${PROJECT_SOURCE_DIR}/spi
			  ${PROJECT_SOURCE_DIR}/mcasp
)
target_link_libraries(LabOS spi mcasp)
