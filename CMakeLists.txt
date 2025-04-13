cmake_minimum_required(VERSION 3.10)
project(H264SendReceiver)

# 设置编译器
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 设置为debug可调试
set(CMAKE_BUILD_TYPE Debug)

# 可执行程序输出路径
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# 参与编译的文件
aux_source_directory(${CMAKE_SOURCE_DIR}/src SRC_FILES)

# 添加可执行文件
add_executable(${PROJECT_NAME} ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

# 链接库
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/thirdPartLib/jrtplib-3.11.2/Install/include)
target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/thirdPartLib/jrtplib-3.11.2/build/src)
target_link_libraries(${PROJECT_NAME} PRIVATE jrtp)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/thirdPartLib/jthread-1.3.3/Install/include)
target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/thirdPartLib/jthread-1.3.3/Install/lib)
target_link_libraries(${PROJECT_NAME} PRIVATE jthread)

