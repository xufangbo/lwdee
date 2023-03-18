
# 将源代码添加到此项目的可执行文件。
add_executable (driver "driver.cpp"
)

target_link_libraries(driver core uhconnect worker)

# include_directories(${PROJECT_BINARY_DIR}/inc/libgo)
# # add the binary tree to the search path for include files
# # so that we will find TutorialConfig.h
# # 将二进制目录添加到包含文件的搜索路径中，以便我们找到TutorialConfig.h
# target_include_directories(driver 
#         PUBLIC ${PROJECT_BINARY_DIR}
#         )

# target_link_options(driver
#         PUBLIC "-T${PROJECT_SOURCE_DIR}/Demo.lds"
# )
# target_compile_options(driver PUBLIC -Wall)

