
include_directories(./terasort)
include_directories(./uhconnect)

add_subdirectory(uhconnect)
add_subdirectory(uhshell) # lwdee/uhconnect/api/UhconnDco.cpp:8:10: fatal error: log.h: No such file or directory
add_subdirectory(terasort)

SET(SRC_FILES 
terasort/app.cpp
terasort/terasort.cpp
terasort/TerasortDCOFactory.cpp
)
add_executable(app ${SRC_FILES}) 

list(APPEND EXTRA_LIBS uhconnect)
list(APPEND EXTRA_LIBS core) 
list(APPEND EXTRA_LIBS matrix) 
list(APPEND EXTRA_LIBS lwdee) 
list(APPEND EXTRA_LIBS sample) 
list(APPEND EXTRA_LIBS map) 
list(APPEND EXTRA_LIBS reduce) 
list(APPEND EXTRA_LIBS driver) 
target_link_libraries(app PUBLIC ${EXTRA_LIBS})

target_link_options(app PUBLIC "-T${PROJECT_SOURCE_DIR}/app.lds" )

target_compile_options(app PUBLIC -Wall)
