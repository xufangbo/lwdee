
add_library (map 
MapDCO.cpp
Step1Task.cpp
KafkaCallback.cpp
KafkaJobConsumer.cpp
)

target_link_libraries(map rdkafka++ core)