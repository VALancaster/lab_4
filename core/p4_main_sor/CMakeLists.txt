add_library(p4_main_sor_lib main.cpp solver.cpp)
target_link_libraries(p4_main_sor_lib PRIVATE shared $<$<PLATFORM_ID:Linux,Darwin>:m>)