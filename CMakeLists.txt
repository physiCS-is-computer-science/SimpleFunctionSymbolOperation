#   此处 f7 编译和 f5 编译调试不是一个东西！前者属于 CMake 控制，后者属于 launch.json 控制，
#前后者的 exe 都可以调试，后者寻找 exe 去调试的路径我手动设置为前者输出的 exe 路径，以防冲突混乱；
#   以下是该 CMake 的文件配置(for C/C++)，使用时必须手动创建如下文件夹/文件，防止出错：
#   1  .vscode
#         settings.json
#         launch.json # 非必选
#   2  bin
#   3  include
#   4  src
#   5  CMakeLists.txt

cmake_minimum_required(VERSION 3.20.0)
set(PROJECT_NAME "symbol_operation") # storage the name "xxx" in variable PROJECT_NAME
project(${PROJECT_NAME}) # Project name

# set(CMAKE_EXE_LINKER_FLAGS "-static") # 链接静态库

set(CMAKE_C_STANDARD 11) # Specify the C language standard
# set(CMAKE_CXX_STANDARD 17) # Specify the C++ language standard

# 可以不写代码，右边 cmake 插件可以手动改
# set(CMAKE_C_FLAGS_DEBUG "-O0 -g") # Debug mode (不优化，包含调试符号)
# set(CMAKE_C_FLAGS_RELEASE "-O3") # Release mode (最高优化)

# Store the ${CMAKE_SOURCE_DIR}/bin path to the EXE_OUTPUT_DIR variable
set(EXE_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${EXE_OUTPUT_DIR}) # exe's output path: EXE_OUTPUT_DIR

file(GLOB_RECURSE SOURCE "src/*.c") # Recursively search all .c files in path src/*.c and store them in SOURCE

add_executable(${PROJECT_NAME} ${SOURCE}) # Compile all .c files of ${SOURCE}

target_include_directories(${PROJECT_NAME} PRIVATE include) # fine header files in path include/xxx.h

# 启用编译器警告（放在 add_executable 之后！）
if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()