
add_library (reduce 
ReduceDCO.cpp
Reducer.cpp
)

target_link_libraries(reduce rdkafka++ core leopard)