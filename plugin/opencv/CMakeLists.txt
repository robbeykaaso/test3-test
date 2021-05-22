set(USEOPENCV OFF)

if (${USEOPENCV} STREQUAL "ON")
    find_package(OpenCV REQUIRED)
    add_definitions(-DUSEOPENCV)
    set(my_Libs
        ${my_Libs}
        ${OpenCV_LIBS})
#    set(my_Sources
#        ${my_Sources})
    include_directories(${OpenCV_INCLUDE_DIRS})
endif()