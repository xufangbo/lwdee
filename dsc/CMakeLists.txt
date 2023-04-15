
include_directories(.)

aux_source_directory(. DIR)

add_subdirectory(core)
add_subdirectory(matrix)
add_subdirectory(driver)
add_subdirectory(kafka)
add_subdirectory(map)
add_subdirectory(reduce)
add_subdirectory(producer)