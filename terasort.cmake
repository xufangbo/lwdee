
include_directories(./terasort)
# include_directories(.)
# include_directories(../uhconnect)
# include_directories(../inc/libgo)
# include_directories(.../inc/boost)
# include_directories(../inc )

# add_subdirectory(uhconnect)
add_subdirectory(terasort)

SET(SRC_FILES 
app.cpp
terasort/terasort.cpp
terasort/TerasortDCOFactory.cpp
)
add_executable(app ${SRC_FILES}) 

list(APPEND EXTRA_LIBS uhconnect)
list(APPEND EXTRA_LIBS core) 
list(APPEND EXTRA_LIBS lwdee) 
list(APPEND EXTRA_LIBS map) 
list(APPEND EXTRA_LIBS reduce) 
target_link_libraries(app PUBLIC ${EXTRA_LIBS})

target_link_options(app PUBLIC "-T${PROJECT_SOURCE_DIR}/app.lds" )

target_compile_options(app PUBLIC -Wall)
