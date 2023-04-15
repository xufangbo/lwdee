aux_source_directory(. DIR)

# ls */*.cpp
add_library(net SHARED
LeopardConfig.cpp 
Socket.cpp 
Epoll.cpp
LeopardProtocal.cpp
)

target_link_libraries(net core)