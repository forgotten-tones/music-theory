cmake_minimum_required(VERSION 3.10)
project(mahler)

add_library(
    mahler
    src/err/err.c
    src/inter/inter.c
    src/chord/chord.c
    src/scale/scale.c
    src/key/key.c
    src/misc/misc.c
    src/shared/shared.c
    src/note/note.c
    src/rhythm/rhythm.c
    src/nontertian/quartal.c
    src/nontertian/quintal.c
)

target_include_directories(
    mahler PRIVATE
    "src"
)

# Examples
add_executable(chart ex/chart.c)
target_link_libraries(chart mahler)
target_include_directories(chart PRIVATE "inc" "src")

add_executable(fifth ex/fifth.c)
target_link_libraries(fifth mahler)
target_include_directories(fifth PRIVATE "inc" "src")

add_executable(quartal ex/quartal.c)
target_link_libraries(quartal mahler)
target_include_directories(quartal PRIVATE "inc" "src")

add_executable(quintal ex/quintal.c)
target_link_libraries(quintal mahler)
target_include_directories(quintal PRIVATE "inc" "src")

add_executable(trans ex/trans.c)
target_link_libraries(trans mahler)
target_include_directories(trans PRIVATE "inc" "src")

# Custom target to run all examples
add_custom_target(run-examples
    COMMAND chart
    COMMAND fifth
    COMMAND quartal
    COMMAND quintal
    COMMAND trans
    DEPENDS chart fifth quartal quintal trans
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running all examples"
)