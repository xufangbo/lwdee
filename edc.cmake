include_directories(./edc)

SET(SRC_FILES 
app.cpp
edc.cpp
)
add_executable(app ${SRC_FILES}) 


list(APPEND EXTRA_LIBS uhconnect)
list(APPEND EXTRA_LIBS uhshell)
list(APPEND EXTRA_LIBS kmeans)
list(APPEND EXTRA_LIBS core lwdee driver worker) 
target_link_libraries(app PUBLIC ${EXTRA_LIBS})

include_directories(${PROJECT_BINARY_DIR}/inc/libgo)
# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
# 将二进制目录添加到包含文件的搜索路径中，以便我们找到TutorialConfig.h
target_include_directories(app 
        PUBLIC ${PROJECT_BINARY_DIR}
        PUBLIC kmeans
        )

target_link_options(app PUBLIC "-T${PROJECT_SOURCE_DIR}/app.lds" )

target_compile_options(app PUBLIC -Wall)
