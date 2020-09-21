cmake_minimum_required(VERSION 3.17)
project(EpidemicCode__)

set(CMAKE_CXX_STANDARD 14)

add_executable(EvolveEpidemicProfile
        ../EpidemicCode++/main.cpp
        ../EpidemicCode++/ps.cpp
        ../EpidemicCode++/ps.h
        ../EpidemicCode++/setu.cpp
        ../EpidemicCode++/setu.h
        ../EpidemicCode++/stat.cpp
        ../EpidemicCode++/stat.h)