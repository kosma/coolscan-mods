cmake_minimum_required(VERSION 3.21)
project(coolscan_updater C)

set(CMAKE_C_STANDARD 99)

add_executable(coolscan-updater src/coolscan-updater.c src/coolscan_buffer.c src/coolscan_command.c src/coolscan_updatefile.c)
add_executable(coolscan-memory-dump src/coolscan-memory-dump.c src/coolscan_buffer.c src/coolscan_command.c)

if(UNIX)
    target_sources(coolscan-updater PUBLIC src/coolscan_transport_libusb_linux.c)
    target_sources(coolscan-memory-dump PUBLIC src/coolscan_transport_libusb_linux.c)
    target_link_libraries(coolscan-updater PUBLIC usb-1.0)
    target_link_libraries(coolscan-memory-dump PUBLIC usb-1.0)
else()
    target_sources(coolscan-updater PUBLIC src/coolscan_transport_winapi_usb.c)
    target_sources(coolscan-memory-dump PUBLIC src/coolscan_transport_winapi_usb.c)
endif()