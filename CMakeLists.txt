# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    CMakeLists.txt                                     :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abelov <abelov@student.42london.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/28 18:10:53 by abelov            #+#    #+#              #
#    Updated: 2024/06/28 18:10:53 by abelov           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

cmake_minimum_required(VERSION 3.8)
#set(CMAKE_C_COMPILER clang)
set(CMAKE_C_COMPILER gcc)


project(minishell C)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_C_STANDARD 99)
include_directories(include)

set(FT_CFLAGS
        -Wall -Wextra -Werror
        -Wwrite-strings
        -Wimplicit
        -Wformat -Werror=format-security
        -Wstrict-prototypes
        -Wmissing-prototypes
        -fstack-protector-strong
#        -Wno-error=unused-parameter
#        -Wno-error=unused-variable
        -O0
        -masm=att
        -g3 -gdwarf-3
        -fasynchronous-unwind-tables
        -fno-omit-frame-pointer
        -fsanitize=address
#        -fsanitize=leak
        -fsanitize=undefined
        -fno-sanitize-recover=all
        -fsanitize=float-divide-by-zero
        -fsanitize=float-cast-overflow)

set(COMMON_LDFLAGS
        -fsanitize=address
        -fsanitize=undefined
#        -fsanitize=leak
        -fno-sanitize-recover=all
        -fsanitize=float-divide-by-zero
        -fsanitize=float-cast-overflow)

#==============================================================================#
# FT lib
#==============================================================================#

set(FT_LIB_NAME ft)
set(FT_LIB_FILENAME libft.a)
set(FT_LIB_DIR "${PROJECT_SOURCE_DIR}/lib/ft")

file(GLOB_RECURSE FT_LIB_HEADERS "${FT_LIB_DIR}/*.h")
file(GLOB_RECURSE FT_LIB_SOURCES "${FT_LIB_DIR}/ft_*.c")

add_custom_target(${FT_LIB_NAME}_target ALL
        COMMAND make -C ${FT_LIB_DIR} BUILD_WITH_ASAN=1
        BYPRODUCTS "${FT_LIB_DIR}/${FT_LIB_FILENAME}"
        SOURCES "${FT_LIB_HEADERS};${FT_LIB_SOURCES}"
        COMMENT "Building FT_LIB")

add_library(${FT_LIB_NAME} STATIC IMPORTED)

add_dependencies(${FT_LIB_NAME} "${FT_LIB_NAME}_target")
set_target_properties(${FT_LIB_NAME} PROPERTIES
        IMPORTED_LOCATION ${FT_LIB_DIR}/${FT_LIB_FILENAME}
        INTERFACE_COMPILE_DEFINITIONS ""
        INTERFACE_LINK_OPTIONS ""
        INTERFACE_SOURCES "${FT_LIB_HEADERS}"
        INTERFACE_INCLUDE_DIRECTORIES "${FT_LIB_DIR};${FT_LIB_DIR}/include;${FT_LIB_DIR}/include/ft")

#==============================================================================#

if(IS_DIRECTORY "${PROJECT_SOURCE_DIR}/scratch")
    if(EXISTS "${PROJECT_SOURCE_DIR}/scratch/CMakeLists.txt")
        add_subdirectory(scratch)
    endif()
endif()

file(GLOB_RECURSE TARGET_HEADERS "src/*.h")
list(FILTER TARGET_HEADERS EXCLUDE REGEX "sub/*" )
file(GLOB_RECURSE TARGET_SOURCES "src/*.c")
list(FILTER TARGET_SOURCES EXCLUDE REGEX "src/sub/*")

set(TARGET_NAME "${PROJECT_NAME}_exe")

find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(RL REQUIRED IMPORTED_TARGET readline)

set(TARGET_LIBRARIES
        ${FT_LIB_NAME}
        Threads::Threads
#         -ltinfo
#         /home/andrei_belov/staging/readline-8.0/build/libhistory.a
#         /home/andrei_belov/staging/readline-8.0/build/libreadline.a
       PkgConfig::RL
)

MESSAGE(STATUS "${RL_INCLUDE_DIRS}")

add_executable(${TARGET_NAME}
        ${TARGET_HEADERS}
        ${TARGET_SOURCES})

target_include_directories(${TARGET_NAME} PRIVATE
        ${PROJECT_SOURCE_DIR}
        /usr/include
        /usr/include/x86_64-linux-gnu
        )
target_link_libraries(${TARGET_NAME} "${TARGET_LIBRARIES}")
target_link_options(${TARGET_NAME} PRIVATE ${COMMON_LDFLAGS})
target_compile_options(${TARGET_NAME} PRIVATE "${FT_CFLAGS}")
set_target_properties(${TARGET_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}"
        OUTPUT_NAME "${PROJECT_NAME}")

#==============================================================================#
# Custom targets
#==============================================================================#
add_custom_target(fclean
        COMMAND make -C ${FT_LIB_DIR} fclean)

add_custom_target(re
        COMMAND make -C ${FT_LIB_DIR} re
        COMMAND make -C ${PROJECT_SOURCE_DIR} re)

add_custom_target(norm
        COMMAND make -C ${FT_LIB_DIR} norm
        COMMAND make -C ${PROJECT_SOURCE_DIR} norm)
