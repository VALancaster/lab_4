add_library(shared INTERFACE)

target_include_directories(shared INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(shared INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/grid.h
    ${CMAKE_CURRENT_SOURCE_DIR}/poisson_base.h
    ${CMAKE_CURRENT_SOURCE_DIR}/utils.h
)
