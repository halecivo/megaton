cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(PROJECT_NAME megaton)
project(${PROJECT_NAME})

set(${CMAKE_PROJECT_NAME}_SKETCH megaton.ino)

#### Uncomment below additional settings as needed.
set(${CMAKE_PROJECT_NAME}_BOARD mega)
set(${CMAKE_PROJECT_NAME}_PORT COM6)
set(mega.build.mcu atmega2560)
set(mega.upload.protocol wiring)
set(mega.upload.speed 115200)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../libraries)
set(U8glib_RECURSE True)

generate_arduino_firmware(${CMAKE_PROJECT_NAME})
