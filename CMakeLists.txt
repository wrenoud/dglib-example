cmake_minimum_required(VERSION 4.0)
project(dggrid_experiment)

set(CMAKE_CXX_STANDARD 17)

# Fetch DGGRID
include(FetchContent)
set(WITH_GDAL off)
FetchContent_Declare(dggrid
    GIT_REPOSITORY https://github.com/sahrk/DGGRID.git
    GIT_TAG v8.42)
FetchContent_MakeAvailable(dggrid)


add_executable(dggrid_experiment main.cpp)

target_link_libraries(dggrid_experiment PRIVATE dglib)
