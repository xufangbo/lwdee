
add_library (kafka 
KafkaDCO.cpp
KafkaCallback.cpp
KafkaJobConsumer.cpp
JobManager.cpp
ToMap.cpp
)

target_link_libraries(kafka rdkafka++ core crypto ssl)