
add_library (map 
MapDCO.cpp
Mapper.cpp
ToReduce.cpp
MapSchedule.cpp
)

target_link_libraries(map core leopard)