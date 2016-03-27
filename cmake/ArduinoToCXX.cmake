# Transform a list of arduino source filenames to C++ filenames
# This macro copies all *.ino files into the binary directory with extension .cpp
# Other files are passed through unmodified
#
# Example:
#   set(SRCS foo.ino bar.cpp)
#   arduino_to_cxx(SRCS ${SRCS})
#   add_executable(foo ${SRCS})

macro(arduino_to_cxx output_var input)
    # Add the current list directory for include searching
    # This is required when a file is moved to the binary directory
    include_directories(${CMAKE_CURRENT_LIST_DIR})

    foreach(f ${input})
        string(REGEX MATCH "\.ino$" matches ${input})
        if(matches)
            # Get the new filename
            string(REGEX REPLACE "^(.*)\.ino$" "${CMAKE_CURRENT_BINARY_DIR}/\\1.cpp" newname ${f})

            # Replace the filename in the output list
            list(REMOVE_ITEM ${output_var} ${f})
            list(APPEND ${output_var} ${newname})

            # Move the file into the binary directory
            file(COPY ${f} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
            file(RENAME ${CMAKE_CURRENT_BINARY_DIR}/${f} ${newname})
        endif(matches)
    endforeach(f)
endmacro(arduino_to_cxx)
