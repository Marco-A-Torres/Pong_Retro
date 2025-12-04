cmake_minimum_required(VERSION 3.17)
project(Pong_Retro VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 1. Definimos la ruta LOCAL de SFML (Portable)
set(SFML_DIR "${CMAKE_SOURCE_DIR}/libs/SFML/lib/cmake/SFML")

# Truco: Si no existe el archivo de config de CMake en libs, lo definimos manualmente
if(NOT EXISTS "${SFML_DIR}")
    set(SFML_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/libs/SFML/include")
    set(SFML_LIBRARY_DIR "${CMAKE_SOURCE_DIR}/libs/SFML/lib")
    
    include_directories(${SFML_INCLUDE_DIR})
    link_directories(${SFML_LIBRARY_DIR})
    
    # Definimos las librerías manualmente si no usamos find_package config mode
    set(SFML_LIBRARIES sfml-graphics sfml-window sfml-system sfml-audio sfml-main)
else()
    find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)
endif()

include_directories(include)

set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/PowerUp.cpp
)

add_executable(Pong_Retro ${SOURCES})

# Linkeamos
target_link_libraries(Pong_Retro sfml-graphics sfml-window sfml-system sfml-audio)

# Copia automática de DLLs desde TU carpeta local libs/SFML/bin al build
if(WIN32)
    add_custom_command(TARGET Pong_Retro POST_BUILD
        # 1. Copiar las DLLs de SFML (Binarios)
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/libs/SFML/bin
        $<TARGET_FILE_DIR:Pong_Retro>

        # 2. Copiar la carpeta de Assets (Fuentes, Imágenes)
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:Pong_Retro>/assets

        # Mensaje en consola para saber que está pasando
        COMMENT "Copiando DLLs y Assets al directorio de salida..."
    )
endif()