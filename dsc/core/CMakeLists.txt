
add_library (core 
log.cpp
Exception.cpp
Partition.cpp
ByteSpan.cpp
Stopwatch.cpp
cjson.cpp
DscConfig.cpp
)

target_link_libraries(core pthread)