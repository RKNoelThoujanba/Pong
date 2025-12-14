cmake_minimum_required(VERSION 3.30)

project(Pong)

file(GLOB HGL_SRC_FILES "HGL/*.cpp")
file(GLOB HGL_HEADER_FILES "HGL/include/*.hpp")
file(GLOB PONG_HEADER_FILES "Pong/*.hpp")
file(GLOB PONG_SRC_FILES "Pong/*.cpp")

add_executable(Pong 
    vendor/glad/src/gl.c vendor/glad/include/glad/gl.h
    ${HGL_HEADER_FILES}
    ${HGL_SRC_FILES}
    ${PONG_HEADER_FILES}
    ${PONG_SRC_FILES}
    main.cpp
)

set(GLFW_BUILD_X11 0)

add_subdirectory(vendor/glfw)
target_link_libraries(Pong glfw)

add_subdirectory(vendor/glad)
target_link_libraries(Pong glad)

target_include_directories(Pong PRIVATE vendor/glm)
target_include_directories(Pong PRIVATE vendor/glad/include)
target_include_directories(Pong PRIVATE vendor/stb)
target_include_directories(Pong PRIVATE HGL/include)

set_property(TARGET Pong PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/")
add_compile_definitions("$<$<NOT:$<CONFIG:Debug>>:NDEBUG>")


if(MSVC)
    string(REGEX REPLACE "/W[1-3]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    string(REGEX REPLACE "/W[1-3]" "/W4" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    target_compile_options(Pong  PRIVATE /WX /permissive-)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /std:c11")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++23preview")

    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

    if(ASAN)
        target_compile_options(Pong  PRIVATE /fsanitize=address)
        target_link_options(Pong PRIVATE /INCREMENTAL:NO /INFERASANLIBS) 
    endif()


else()

    target_compile_options(Pong  PRIVATE -Wall -Wextra -pedantic -Werror -Wno-language-extension-token)


    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23")
    if(ASAN)
        target_compile_options(Pong  PRIVATE -fsanitize=address)
        target_link_options(Pong  PRIVATE -fsanitize=address)
    endif()
endif()
