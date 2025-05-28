cmake_minimum_required(VERSION 3.13)
project(CMP1002_DarkestDungeon)

set(CMAKE_CXX_STANDARD 17)

add_executable(DarkestDungeon main.cpp unit.h skills.h common_types.h hero.h monster.h)
