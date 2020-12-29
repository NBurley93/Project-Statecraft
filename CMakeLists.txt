cmake_minimum_required(VERSION 3.15)
project(project-statecraft)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(RITUAL_BUILD_PLATFORM "RITUAL_PLATFORM_UNSUPPORTED")
if(WIN32)
    set(RITUAL_BUILD_PLATFORM "RITUAL_PLATFORM_WIN32")
endif()

add_library(RitualCommonLib)
add_executable(game_runtime)
add_executable(game_editor)
add_executable(game_toolbox)
add_subdirectory(GameRuntime)
add_subdirectory(CommonLib)
add_subdirectory(Editor)
add_subdirectory(Toolbox)


set_target_properties(game_runtime PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    RUNTIME_OUTPUT_NAME_DEBUG project_statecraft_debug
    RUNTIME_OUTPUT_NAME_RELEASE project_statecraft_release
)


set_target_properties(game_editor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    RUNTIME_OUTPUT_NAME philosopher
)


set_target_properties(game_toolbox PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    RUNTIME_OUTPUT_NAME toolbox
)


target_compile_definitions(game_runtime PRIVATE
    _CRT_SECURE_NO_WARNINGS
    RITUAL_BUILD_$<$<CONFIG:Debug>:DEBUG>$<$<CONFIG:Release>:RELEASE>
    ${RITUAL_BUILD_PLATFORM}
    $<$<CONFIG:Debug>:RITUAL_ENABLE_ASSERTS>
)


if (MSVC)
    target_compile_options(game_runtime PRIVATE
        /MD$<$<CONFIG:Debug>:d>
        $<$<CONFIG:Debug>:/Od>$<$<CONFIG:Release>:/O2>
        /W$<$<CONFIG:Debug>:3>$<$<CONFIG:Release>:4>
    )
    target_link_options(game_runtime PRIVATE
        /NOLOGO
    )
    target_compile_options(game_editor PRIVATE
        /MD$<$<CONFIG:Debug>:d>
        $<$<CONFIG:Debug>:/Od>$<$<CONFIG:Release>:/O2>
        /W$<$<CONFIG:Debug>:3>$<$<CONFIG:Release>:4>
    )
    target_link_options(game_editor PRIVATE
        /NOLOGO
    )
    target_compile_options(game_toolbox PRIVATE
        /MD$<$<CONFIG:Debug>:d>
        $<$<CONFIG:Debug>:/Od>$<$<CONFIG:Release>:/O2>
        /W$<$<CONFIG:Debug>:3>$<$<CONFIG:Release>:4>
    )
    target_link_options(game_toolbox PRIVATE
        /NOLOGO
    )
endif()


# Vendor libs

# SDL2
set(SDL_TEST OFF CACHE BOOL "" FORCE)
set(SDL_STATIC OFF CACHE BOOL "" FORCE)
set(SDL_SHARED ON CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/SDL)
set_target_properties(SDL2 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

# tinyxml2
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/tinyxml2)


# spdlog
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/spdlog)


# ZLib
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/zlib)

set(COMMON_INCLUDES
    ${CMAKE_SOURCE_DIR}/CommonLib/source
    ${CMAKE_SOURCE_DIR}/vendor/spdlog/include
    ${CMAKE_SOURCE_DIR}/vendor/SDL/include
    ${CMAKE_SOURCE_DIR}/vendor/tinyxml2
    ${CMAKE_SOURCE_DIR}/vendor/zlib
    ${CMAKE_SOURCE_DIR}/vendor/zstr/src
    ${CMAKE_BINARY_DIR}/vendor/zlib
)

target_include_directories(RitualCommonLib PRIVATE
    ${COMMON_INCLUDES}
)

target_include_directories(game_runtime PRIVATE
    ${CMAKE_SOURCE_DIR}/GameRuntime/source
    ${COMMON_INCLUDES}
)


target_include_directories(game_editor PRIVATE
    ${CMAKE_SOURCE_DIR}/Editor/source
    ${COMMON_INCLUDES}
)


target_include_directories(game_toolbox PRIVATE
    ${CMAKE_SOURCE_DIR}/Toolbox/source
    ${COMMON_INCLUDES}
)


target_link_libraries(RitualCommonLib PUBLIC
    SDL2main
    SDL2
    tinyxml2
    spdlog::spdlog
    zlibstatic
)


target_link_libraries(game_runtime PRIVATE
    RitualCommonLib
)