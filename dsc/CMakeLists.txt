
include_directories(.)
include_directories(../uhconnect)
include_directories(../inc/libgo)
include_directories(.../inc/boost)
include_directories(../inc )

aux_source_directory(. DIR)

add_subdirectory(core)
add_subdirectory(matrix)
add_subdirectory(lwdee)
add_subdirectory(driver)
add_subdirectory(kafka)
add_subdirectory(map)
add_subdirectory(reduce)
add_subdirectory(producer)