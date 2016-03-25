find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)

set(SRCS
    simtinyscreen.cpp
    simwire.cpp
    spisim.cpp
    tinysim.cpp
    TinyScreen.cpp
)

set(HEADERS
    simtinyscreen.h
    tinysim.h
)

add_library(tinysim STATIC ${SRCS} ${HEADERS})
set_property(TARGET tinysim PROPERTY CXX_STANDARD 11)
set_property(TARGET tinysim PROPERTY CXX_STANDARD_REQUIRED true)
