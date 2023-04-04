
add_library (driver 
Driver.cpp
TerasortConfig.cpp
)

target_link_libraries(driver core lwdee uhconnect)