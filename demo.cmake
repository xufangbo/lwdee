# configure a header file to pass some of the CMake settings
# to the source code
# 配置头文件以将某些CMake设置传递给源代码
configure_file(DemoConfig.h.in DemoConfig.h)

add_subdirectory(uhconnect)
add_subdirectory(uhshell)
add_subdirectory(kmeans)

# add the executable
# 添加可执行文件
add_executable(Demo demo.cpp demo_test.cpp) 


list(APPEND EXTRA_LIBS uhconnect)
list(APPEND EXTRA_LIBS uhshell)
list(APPEND EXTRA_LIBS kmeans)
target_link_libraries(Demo PUBLIC ${EXTRA_LIBS})

include_directories(${PROJECT_BINARY_DIR}/inc/libgo)
# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
# 将二进制目录添加到包含文件的搜索路径中，以便我们找到TutorialConfig.h
target_include_directories(Demo 
        PUBLIC ${PROJECT_BINARY_DIR}
        PUBLIC kmeans
        )

target_link_options(Demo
        PUBLIC "-T${PROJECT_SOURCE_DIR}/Demo.lds"
)
target_compile_options(Demo PUBLIC -Wall)