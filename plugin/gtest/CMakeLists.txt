set(USEGTEST ON)

if (${USEGTEST} STREQUAL "ON")
    #set(GTEST_ROOT "D:/mywork3/gtest/googletest-master/install/lib/cmake/GTest")
    if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        set(GTEST_LIBRARY "D:/mywork3/gtest/googletest-master/install/${CMAKE_BUILD_TYPE}/lib/gtestd.lib")
        set(GTEST_MAIN_LIBRARY "D:/mywork3/gtest/googletest-master/install/${CMAKE_BUILD_TYPE}/lib/gtest_maind.lib")
    else()
        set(GTEST_LIBRARY "D:/mywork3/gtest/googletest-master/install/${CMAKE_BUILD_TYPE}/lib/gtest.lib")
        set(GTEST_MAIN_LIBRARY "D:/mywork3/gtest/googletest-master/install/${CMAKE_BUILD_TYPE}/lib/gtest_main.lib")
    endif()
    set(GTEST_INCLUDE_DIR "D:/mywork3/gtest/googletest-master/install/${CMAKE_BUILD_TYPE}/include")
    find_package(GTest REQUIRED)
    set(my_Libs
        ${my_Libs}
        ${GTEST_LIBRARIES})
    set(my_Sources
        ${my_Sources}
        "plugin/gtest/test_gtest.cpp")
    include_directories(${GTEST_INCLUDE_DIRS})
endif()