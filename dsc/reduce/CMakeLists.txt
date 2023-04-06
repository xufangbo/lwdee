
add_library (reduce 
ReduceDCO.cpp
Step2Task.cpp
)

target_link_libraries(reduce rdkafka++ core)