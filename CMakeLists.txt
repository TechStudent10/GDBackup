cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(GDBackup)

add_library(${PROJECT_NAME} SHARED src/main.cpp)

include(cmake/CPM.cmake)

CPMAddPackage("gh:matcool/mat-dash#cdc9f70")
CPMAddPackage("gh:HJfod/cocos-headers#cbf79fc")
CPMAddPackage("gh:TsudaKageyu/minhook#4a45552")
CPMAddPackage("gh:kuba--/zip#a93ea8f")

target_compile_definitions(${PROJECT_NAME} PRIVATE CC_DLL=)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${minhook_SOURCE_DIR}/include
    ${zip_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME}
    mat-dash
    cocos2d
    minhook
    zip
)
