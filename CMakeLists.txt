cmake_minimum_required(VERSION 3.25)

project(actual-difficulty-mod VERSION 1.0.0 LANGUAGES CXX)

find_package(Geode REQUIRED)

add_geode_mod(
    derlandix.actual_difficulty_mod
    RESOURCES
        resources/pill_na.png
        resources/impossible_demon.png
        resources/ultimate_obsidian.png
    SOURCES
        src/main.cpp
)
