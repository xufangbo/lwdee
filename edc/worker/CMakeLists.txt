
# 将源代码添加到此项目的可执行文件。
add_library(worker 
worker.cpp
StageTask.cpp
ResultTask.cpp
)

target_link_libraries(worker core lwdee uhconnect)
