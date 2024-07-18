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

project(minishell_sub C)

file(GLOB TEST_TARGET_HEADERS "${PROJECT_SOURCE_DIR}/*.h")
file(GLOB TEST_TARGET_SOURCES "${PROJECT_SOURCE_DIR}/*.c")
file(GLOB BTINS_TARGET_SOURCES "${PROJECT_SOURCE_DIR}/builtins/*.c")
file(GLOB UTILS_TARGET_SOURCES "${PROJECT_SOURCE_DIR}/utils/*.c")
list (APPEND TEST_TARGET_SOURCES "${UTILS_TARGET_SOURCES};${BTINS_TARGET_SOURCES}")

set(TEST_TARGET_NAME "${PROJECT_NAME}_test")

find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(readline REQUIRED IMPORTED_TARGET readline)

add_executable(${TEST_TARGET_NAME}
        ${TEST_TARGET_HEADERS}
        ${TEST_TARGET_SOURCES})
target_include_directories(${TEST_TARGET_NAME} PRIVATE "${PROJECT_SOURCE_DIR}")
target_link_libraries(${TEST_TARGET_NAME} asan ${FT_LIB_NAME} Threads::Threads PkgConfig::readline)
set_target_properties(${TEST_TARGET_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}"
        OUTPUT_NAME "${PROJECT_NAME}")
