cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.2
    GIT_SHALLOW ON
)

# Make sure SFML builds for you
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "" FORCE)
set(SFML_USE_STATIC_STD_LIBS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SFML)

# Grab all .cpp files automatically
file(GLOB_RECURSE SRC_SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(main main.cpp ${SRC_SOURCES})

target_compile_features(main PRIVATE cxx_std_17)

# Now use official exported SFML targets (they exist now!)
target_link_libraries(main PRIVATE sfml-graphics sfml-window sfml-system)
