set (PROJECT led-test2)
cmake_minimum_required(VERSION 3.18)
set (PICO_BOARD pico_w)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(${PROJECT} C CXX ASM)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)

set(PICO_CXX_ENABLE_RTTI 1)

pico_sdk_init()

add_executable(${PROJECT} 
		ws2812.cpp 
		Colour.cpp 
		LedStrip.cpp 
		ws2812Strip.cpp 
		LedSegment.cpp
		LedController.cpp
		Wiping.cpp
		AnimationHelper.cpp
		Blinking.cpp
		Chasing.cpp
		Cycling.cpp
		Animator.cpp
		Commander.cpp
		Twinkling.cpp
		Fading.cpp
		Scanning.cpp
		)

target_link_libraries(${PROJECT}
        pico_stdlib              # for core functionality
        hardware_pio
        )
        
pico_enable_stdio_usb(${PROJECT} 0)
pico_enable_stdio_uart(${PROJECT} 1)

pico_generate_pio_header(${PROJECT} ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio)

# create map/bin/hex file etc.
pico_add_extra_outputs(${PROJECT})
