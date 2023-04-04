
include_directories(./dsc)
include_directories(./uhconnect)
# include_directories(.)
# include_directories(../uhconnect)
# include_directories(../inc/libgo)
# include_directories(.../inc/boost)
# include_directories(../inc )

add_subdirectory(uhconnect)
add_subdirectory(uhshell) # lwdee/uhconnect/api/UhconnDco.cpp:8:10: fatal error: log.h: No such file or directory
add_subdirectory(dsc)

SET(SRC_FILES 
dsc/app.cpp
dsc/DscDcoFactory.cpp
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
