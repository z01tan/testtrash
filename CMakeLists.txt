cmake_minimum_required(VERSION 2.8)

project(test)

set(SOURCE test.cpp InitAltera.cpp CreateImage.cpp HardProc.cpp)
set(HEADER InitAltera.h CreateImage.h HardProc.h)

set(SOURCE_EXE main.cpp)
set(SOURCE_LIB CreateImage.cpp Detect.cpp HardProc.cpp InitAltera.cpp UnitKoibScan.cpp UnitServiceMenu.cpp MyX.cpp Ruler.cpp cr_BMP.cpp printer.cpp usb.cpp supp.cpp)

add_library(Libs STATIC ${SOURCE_LIB})

add_executable(out ${SOURCE_EXE})

target_link_libraries(out Libs pthread)