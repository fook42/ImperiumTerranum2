#
cmake_minimum_required(VERSION 3.13)

project(IT2_CPort C)

file(GLOB_RECURSE sources      src/*.c)
file(GLOB_RECURSE includes     include/*.h)

add_executable(IT2C ${sources} ${includes})

# just for example add some compiler flags
target_compile_options(IT2C PUBLIC -Wpointer-arith -noixemul -s -flto -g0 -O2 -m68020-60 -Wall -Wno-pointer-sign)
target_link_options(IT2C PRIVATE -flto -lm -noixemul -s -g0)

# this lets me include files relative to the root source directory with a <> pair
target_include_directories(IT2C PUBLIC include)

