cmake_minimum_required(VERSION 3.28)
project(Adventure-of-the-Slime LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set policy version to allow older CMake versions in dependencies (FreeType)
if(POLICY CMP0150)
    cmake_policy(SET CMP0150 NEW)
endif()

include(FetchContent)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.1.0
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM
)
FetchContent_MakeAvailable(SFML)

add_executable(AOFTS
        src/main.cpp
        src/AnimationManager.cpp
        src/SoundPlayer.cpp
        src/slime.cpp
        src/utils.cpp
        assets/icon/icon.rc
)

target_compile_features(AOFTS PRIVATE cxx_std_17)

target_link_libraries(AOFTS PRIVATE sfml-graphics sfml-audio sfml-system)

add_custom_command(TARGET AOFTS POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:AOFTS>/assets"
)