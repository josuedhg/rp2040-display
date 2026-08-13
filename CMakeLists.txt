cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(st7735_lvgl_app)

set(IMAGE_IN ${CMAKE_CURRENT_SOURCE_DIR}/images/room.jpg)
set(IMAGE_OUT_C ${CMAKE_CURRENT_BINARY_DIR}/my_bg.c)

if(EXISTS ${IMAGE_IN})
    message(STATUS "FILE ${IMAGE_IN} found")
else()
    message(FATAL_ERROR "FILE ${IMAGE_IN} not found, add image to continue")
endif()

add_custom_command(
    OUTPUT ${IMAGE_OUT_C}
    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/scripts/img_converter.py 
            ${IMAGE_IN} 
            ${IMAGE_OUT_C} 
            my_bg
    DEPENDS ${IMAGE_IN} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/img_converter.py
)
target_sources(app PRIVATE src/main.c ${IMAGE_OUT_C})
