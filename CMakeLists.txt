cmake_minimum_required(VERSION 3.24)
project(SM2024-Projekt)

set(CMAKE_CXX_STANDARD 17)
find_package(SDL2 REQUIRED COMPONENTS SDL2)
add_executable(SM2024-Projekt
        Projekt.cpp
        Paleta.cpp
        Pliki.cpp
        Zmienne.cpp
        Funkcje.cpp
        Modele.cpp
        MedianCut.cpp
        Kompresja.cpp Kompresja.h)

target_link_libraries(SM2024-Projekt PRIVATE SDL2::SDL2)