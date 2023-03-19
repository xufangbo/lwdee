SET(SRC_FILES 
edc2.cpp
)
add_executable(edc2 ${SRC_FILES}) 


list(APPEND EXTRA_LIBS uhconnect)
list(APPEND EXTRA_LIBS uhshell)
list(APPEND EXTRA_LIBS kmeans)
list(APPEND EXTRA_LIBS core lwdee driver worker) #-
target_link_libraries(edc2 PUBLIC ${EXTRA_LIBS})

include_directories(${PROJECT_BINARY_DIR}/inc/libgo)
# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
# 将二进制目录添加到包含文件的搜索路径中，以便我们找到TutorialConfig.h
target_include_directories(edc2 
        PUBLIC ${PROJECT_BINARY_DIR}
        PUBLIC kmeans
        )

target_link_options(edc2
        PUBLIC "-T${PROJECT_SOURCE_DIR}/edc2.lds"
)
target_compile_options(edc2 PUBLIC -Wall)
