
add_library(driver 
app.cpp 
Driver.cpp
)

target_link_libraries(driver core lwdee uhconnect worker)