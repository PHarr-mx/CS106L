cmake_minimum_required(VERSION 3.30)
project(HashMap)

# 设置 C++17 标准，并强制要求编译器支持
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加全局编译选项：-Wall -Werror
add_compile_options(-Wall -Werror)

# 定义可执行文件
add_executable(HashMap main.cpp)