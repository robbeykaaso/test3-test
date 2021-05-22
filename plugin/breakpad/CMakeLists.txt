set(USEBREAKPAD ON)

if (${USEBREAKPAD} STREQUAL "ON")
    set(my_Libs
        ${my_Libs}
        "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.14393.0/um/x64/WinInet.Lib"
        "D:/mywork3/breakpad/breakpad-main/src/client/windows/${CMAKE_BUILD_TYPE}/lib/common.lib"
        "D:/mywork3/breakpad/breakpad-main/src/client/windows/${CMAKE_BUILD_TYPE}/lib/exception_handler.lib"
        "D:/mywork3/breakpad/breakpad-main/src/client/windows/${CMAKE_BUILD_TYPE}/lib/crash_report_sender.lib"
        "D:/mywork3/breakpad/breakpad-main/src/client/windows/${CMAKE_BUILD_TYPE}/lib/crash_generation_client.lib"
        "D:/mywork3/breakpad/breakpad-main/src/client/windows/${CMAKE_BUILD_TYPE}/lib/crash_generation_server.lib"
        )
    set(my_Sources
        ${my_Sources}
        "plugin/breakpad/test_breakpad.cpp")
    include_directories(
        "D:/mywork3/breakpad/breakpad-main/src"
        )
endif()