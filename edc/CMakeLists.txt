
include_directories(.)
include_directories(../uhconnect)

aux_source_directory(. DIR)

add_subdirectory(core)
add_subdirectory(driver)
add_subdirectory(worker)