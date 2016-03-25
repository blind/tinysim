find_package(SDL2 REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SRCS
    tinytest.ino
)

foreach(f ${SRCS})
    string(REGEX REPLACE "^(.*)\.ino$" "${CMAKE_CURRENT_BINARY_DIR}/\\1.cpp" newname ${f})
    string(REGEX REPLACE ${f} ${newname} SRCS ${SRCS})

    file(COPY ${f} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(RENAME ${CMAKE_CURRENT_BINARY_DIR}/${f} ${newname})
endforeach(f)

add_executable(tinytest ${SRCS})

target_link_libraries(tinytest tinysim ${SDL2_LIBRARY})
