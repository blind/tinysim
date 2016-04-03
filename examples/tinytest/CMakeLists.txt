set(SRCS
    tinytest.ino
)
arduino_to_cxx(SRCS ${SRCS})

add_executable(tinytest ${SRCS})

target_link_libraries(tinytest tinysim ${SDL2_LIBRARY})
