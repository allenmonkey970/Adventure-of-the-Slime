cmake_minimum_required(VERSION 3.28)
project(Adventure-of-the-Slime LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.0
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM
)
FetchContent_MakeAvailable(SFML)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(main
        src/main.cpp
        src/AnimationManager.cpp
        src/icon.h
        src/tileMap.h
        src/AnimationManager.h
        src/SoundPlayer.cpp
        src/SoundPlayer.h
        src/enemy.h
        assets/icon/icon.rc
        src/slime.cpp
        src/utils.cpp
        src/utils.h
)

target_compile_features(main PRIVATE cxx_std_17)

# Link SFML libraries
target_link_libraries(main PRIVATE sfml-graphics sfml-audio sfml-system)

target_include_directories(main PRIVATE ${SFML_SOURCE_DIR}/include)