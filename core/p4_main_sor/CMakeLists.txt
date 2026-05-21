add_executable(p4_main_sor 
    main.cpp
    solver.cpp
)

target_include_directories(p4_main_sor PRIVATE
    ${CMAKE_SOURCE_DIR}/shared
)

target_link_libraries(p4_main_sor PRIVATE
    shared
    $<$<PLATFORM_ID:Linux,Darwin>:m>
)
