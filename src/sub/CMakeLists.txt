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

set(TEST_TARGET_NAME "${PROJECT_NAME}_test")

add_executable(${TEST_TARGET_NAME}
        ${TEST_TARGET_HEADERS}
        ${TEST_TARGET_SOURCES})

target_link_libraries(${TEST_TARGET_NAME} asan lsan ${FT_LIB_NAME})