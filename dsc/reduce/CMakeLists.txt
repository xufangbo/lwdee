
add_library (reduce 
ReduceDCO.cpp
)

target_link_libraries(reduce rdkafka++ core)