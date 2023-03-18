file(GLOB_RECURSE SHELL_SRC ./letter-shell/src/*.c ./letter-shell/extensions/*.c)
add_library(uhshell 
        shell_cpp.cpp
        shell_port.c
        uhshell.cpp
        ${SHELL_SRC} 
        )

target_link_libraries(uhshell
        PUBLIC pthread
        )

target_include_directories(uhshell 
    PUBLIC ${PROJECT_BINARY_DIR}
    PUBLIC .
    PUBLIC letter-shell/src
    PUBLIC letter-shell/extensions/fs_support
    PUBLIC letter-shell/extensions/cpp_support
    PUBLIC letter-shell/extensions/log
    PUBLIC letter-shell/extensions/shell_enhance
    PUBLIC letter-shell/extensions/telnet
    )

target_compile_options(uhshell PRIVATE -w)
