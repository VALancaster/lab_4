add_library(p1_test_seidel_lib 
    main.cpp 
    solver.cpp
)

target_include_directories(p1_test_seidel_lib PRIVATE
    ${CMAKE_SOURCE_DIR}/shared
)

target_link_libraries(p1_test_seidel_lib PRIVATE 
    shared
    $<$<PLATFORM_ID:Linux,Darwin>:m>
)