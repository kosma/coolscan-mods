cmake_minimum_required(VERSION 3.21)
project(coolscan_updater C)

set(CMAKE_C_STANDARD 99)

add_executable(coolscan-updater src/coolscan-updater.c src/coolscan_transport_winapi_usb.c src/coolscan_buffer.c src/coolscan_command.c src/coolscan_updatefile.c)
add_executable(coolscan-memory-dump src/coolscan-memory-dump.c src/coolscan_transport_winapi_usb.c src/coolscan_buffer.c src/coolscan_command.c)