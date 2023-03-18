
# 将源代码添加到此项目的可执行文件。
add_executable (worker 
"worker.cpp" "StageTask.h" "ResultTask.h" "Task.h")

target_link_libraries(worker core uhconnect)
