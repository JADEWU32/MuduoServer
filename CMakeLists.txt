cmake_minimum_required(VERSION 3.5)
project(MuduoServer)

# MuduoServer最终编译成so动态库，设置动态库的路径，放在根目录的lib文件夹下面
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
# 设置调制信息 以及启动C++17语言标准
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++17")

# 定义参与编译的源代码文件
aux_source_directory(. SRC_LIST)
# 编译生成动态库MuduoServer
add_library(MuduoServer SHARED ${SRC_LIST})