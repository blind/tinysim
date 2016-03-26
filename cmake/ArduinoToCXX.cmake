# Transform a list of arduino source filenames to C++ filenames
# This macro copies all *.ino files into the binary directory with extension .cpp
# Other files are passed through unmodified
#
# Example:
#   set(SRCS foo.ino bar.cpp)
#   arduino_to_cxx(SRCS ${SRCS})
#   add_executable(foo ${SRCS})

macro(arduino_to_cxx output_var input)
    foreach(f ${input})
        string(REGEX MATCH "\.ino$" matches ${input})
        if(matches)
            string(REGEX REPLACE "^(.*)\.ino$" "${CMAKE_CURRENT_BINARY_DIR}/\\1.cpp" newname ${f})
            string(REGEX REPLACE ${f} ${newname} ${output_var} ${${output_var}})

            file(COPY ${f} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
            file(RENAME ${CMAKE_CURRENT_BINARY_DIR}/${f} ${newname})
        endif(matches)
    endforeach(f)
endmacro(arduino_to_cxx)
