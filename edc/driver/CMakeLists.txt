
# 将源代码添加到此项目的可执行文件。
add_executable (driver "driver.cpp"
)

target_link_libraries(driver core uhconnect)
