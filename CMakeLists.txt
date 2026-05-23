cmake_minimum_required(VERSION 3.16)
project(lab_4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    # Исправляем проблему с M_PI и предупреждениями
    add_definitions(-D_USE_MATH_DEFINES -D_CRT_SECURE_NO_WARNINGS)
    add_compile_options(/W4)
    # Разрешаем серверу линковать несколько библиотек с похожими структурами
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /FORCE:MULTIPLE")
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_subdirectory(shared)
add_subdirectory(core)

# СОБИРАЕМ СЕРВЕР
add_executable(web_server server_main.cpp)

target_include_directories(web_server PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/shared
)

# Линкуем библиотеки участников (добавляй по мере готовности)
target_link_libraries(web_server PRIVATE
    shared
    p1_test_seidel_lib
    p2_test_sor_lib
    p3_main_seidel_lib
    p4_main_sor_lib
    p5_main_sor_lib
)