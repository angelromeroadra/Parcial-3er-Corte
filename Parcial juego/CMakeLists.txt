cmake_minimum_required(VERSION 3.10)
project(DungeonCrawler)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

set(FUENTES
    main.cpp
    src/mundo.cpp
    src/render.cpp
    src/jugador.cpp
    src/enemigos.cpp
)

add_executable(dungeon ${FUENTES})
