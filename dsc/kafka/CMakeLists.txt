
add_library (kafka 
KafkaDCO.cpp
KafkaTask.cpp
KafkaCallback.cpp
KafkaJobConsumer.cpp
JobManager.cpp
)

target_link_libraries(kafka rdkafka++ core)