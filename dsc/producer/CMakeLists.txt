
include_directories(.)
include_directories(/home/kevin/git/lwdee/inc)

# link_directories(/home/kevin/git/lwdee/lib/librdkafka)
link_directories(/home/kevin/tmp/librdkafka/lib)

add_executable(dscproducer app.cpp) 
target_link_libraries(dscproducer rdkafka++ pthread core)
