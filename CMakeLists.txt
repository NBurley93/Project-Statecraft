cmake_minimum_required(VERSION 3.15)
project(project-statecraft)


set(CXX_STANDARD 17)

set(RITUAL_BUILD_PLATFORM "RITUAL_PLATFORM_UNSUPPORTED")
if(WIN32)
    set(RITUAL_BUILD_PLATFORM "RITUAL_PLATFORM_WIN32")
endif()

add_executable(game_runtime)
add_subdirectory(source)


set_target_properties(game_runtime PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    RUNTIME_OUTPUT_NAME_DEBUG project_statecraft_debug
    RUNTIME_OUTPUT_NAME_RELEASE project_statecraft_release
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


target_include_directories(game_runtime PRIVATE
    ${CMAKE_SOURCE_DIR}/source
    ${CMAKE_SOURCE_DIR}/vendor/spdlog/include
    ${CMAKE_SOURCE_DIR}/vendor/SDL/include
    ${CMAKE_SOURCE_DIR}/vendor/tinyxml2
    ${CMAKE_SOURCE_DIR}/vendor/zlib
    ${CMAKE_SOURCE_DIR}/vendor/zstr/src
)


target_link_libraries(game_runtime PRIVATE
    SDL2main
    SDL2
    tinyxml2
    spdlog::spdlog
    zlibstatic
)