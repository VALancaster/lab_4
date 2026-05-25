add_library(p2_test_sor_lib 
    main.cpp
    solver.cpp
)

target_include_directories(p2_test_sor_lib PRIVATE
    ${CMAKE_SOURCE_DIR}/shared
)

target_link_libraries(p2_test_sor_lib PRIVATE
    shared
    $<$<PLATFORM_ID:Linux,Darwin>:m>
)
