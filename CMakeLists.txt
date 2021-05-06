cmake_minimum_required(VERSION 3.5)

set(USEAWS ON)
set(USEOPENCV ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#if (CMAKE_BUILD_TYPE STREQUAL Debug)
#set(${CMAKE_BINARY_DIR} install)
if (NOT DEFINED MS)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../build-app-Desktop_Qt_5_13_2_MSVC2017_64bit-${CMAKE_BUILD_TYPE}/plugin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/install/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../build-app-Desktop_Qt_5_13_2_MSVC2017_64bit-${CMAKE_BUILD_TYPE}/plugin)
else()
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${MS})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/install/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MS})
endif()

project(REA_TEST)

#add_definitions(-DEXPORTDLL)

find_package(Qt5 COMPONENTS Widgets Core Quick SerialPort SerialBus WebEngineWidgets WebEngine REQUIRED)
set(REA_DIR ${CMAKE_CURRENT_LIST_DIR}/../frm/cmake)
find_package(REA)

set(my_Sources
    "qml.qrc"
    "mainwindow.h"
    "mainwindow.cpp"
    "mainwindow.ui"
    "inspector.h"
    "inspector.cpp"
    "inspector.ui"
    "interface.cpp"
    "test_qml.cpp"
    "test_qsg.cpp"
    "test_storage.cpp"
    )

set(my_Libs
    Qt5::Core
    Qt5::Quick
    Qt5::Widgets
    Qt5::SerialPort
    Qt5::SerialBus
    Qt5::WebEngineWidgets
    Qt5::WebEngine
    ${REA_LIBS}
    )

if (${USEAWS} STREQUAL "ON")
    #set(AWSSDK_DIR "D:/awss3c++/aws-sdk-cpp/buildtest4/install/lib/cmake/AWSSDK")
    message("use aws")
    find_package(AWSSDK REQUIRED COMPONENTS core s3)
    add_definitions(-DUSEAWS)
    set(my_Sources
        ${my_Sources}
        "aws/aws_s3.h"
        "aws/aws_s3.cpp"
        "aws/awsStorage.h"
        "aws/awsStorage.cpp"
        "aws/test_storage.cpp")
    set(my_Libs
        ${my_Libs}
        ${AWSSDK_LINK_LIBRARIES})
    include_directories(${AWSSDK_INCLUDE_DIR})
    message(${AWSSDK_LINK_LIBRARIES})
endif()

if (${USEOPENCV} STREQUAL "ON")
    find_package(OpenCV REQUIRED)
    add_definitions(-DUSEOPENCV)
    set(my_Libs
        ${my_Libs}
        ${OpenCV_LIBS})
    set(my_Sources
        ${my_Sources})
    include_directories(${OpenCV_INCLUDE_DIRS})
endif()

# 根据库文件代码生成动态库
add_library(REA_TEST SHARED
    ${my_Sources}
    )
# 创建动态库别名

#add_library(hello::library ALIAS REA_TEST)

target_link_libraries(REA_TEST PRIVATE
    ${my_Libs})

include_directories(${REA_INCLUDE_DIRS})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/../frm/qml"
    "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/../qml"
    COMMENT "copying resource.")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/../frm/html"
    "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/../html"
    COMMENT "copying resource.")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_LIST_DIR}/test.qml"
    "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/.."
    COMMENT "copying resource.")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/html"
    "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/../html"
    COMMENT "copying resource.")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/test_vue/dist"
    "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/../test_vue"
    COMMENT "copying resource.")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/mnist-core/dist"
    "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/../mnist-core"
    COMMENT "copying resource.")
