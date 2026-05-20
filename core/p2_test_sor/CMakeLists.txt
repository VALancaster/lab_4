add_executable(p2_test_sor 
    main.cpp
    solver.cpp
)

target_include_directories(p2_test_sor PRIVATE
    ${CMAKE_SOURCE_DIR}/shared
)

target_link_libraries(p2_test_sor PRIVATE
    shared
    $<$<PLATFORM_ID:Linux,Darwin>:m>
)
