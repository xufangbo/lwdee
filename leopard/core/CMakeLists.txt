aux_source_directory(. DIR)

# ls */*.cpp
add_library(core SHARED 
Exception.cpp
log.cpp 
cjson.cpp
Stopwatch.cpp
)