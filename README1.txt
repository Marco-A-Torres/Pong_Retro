========================================================================
                  PONG RETRO C++ - PROYECTO PORTABLE
========================================================================

GRACIAS POR ADQUIRIR ESTE PROYECTO.
Este código ha sido diseñado para ser modular, portable y fácil de compilar.

IMPORTANTE:
Este proyecto ya incluye las librerías gráficas (SFML) configuradas dentro de la carpeta
'libs'. NO es necesario que instales SFML en tu sistema operativo.

------------------------------------------------------------------------
1. REQUISITOS DEL SISTEMA
------------------------------------------------------------------------
Para ejecutar y compilar este proyecto necesitas tener instalado:

1. Visual Studio Code.
2. Extensión "C/C++" (Microsoft) en VS Code.
3. Extensión "CMake Tools" (Microsoft) en VS Code.
4. Un compilador C++ (Recomendado: MinGW-w64 / GCC).
   * Nota: Si usas Windows, asegúrate de tener MinGW instalado y agregado al PATH.

------------------------------------------------------------------------
2. INSTRUCCIONES DE INICIO RÁPIDO
------------------------------------------------------------------------

PASO 1: ABRIR EL PROYECTO
   - Abre Visual Studio Code.
   - Ve a "Archivo" > "Abrir Carpeta..." (File > Open Folder).
   - Selecciona la carpeta principal "Pong_CPP" (NO abras solo el archivo main.cpp).

PASO 2: CONFIGURACIÓN AUTOMÁTICA (CMake)
   - Al abrir la carpeta, VS Code detectará el archivo CMakeLists.txt.
   - Si te pregunta "Would you like to configure this project?", dale a "Yes".
   - Si te pide seleccionar un "Kit", elige la opción que diga "GCC" o "MinGW" 
     (Ejemplo: GCC 13.1.0 x86_64-w64-mingw32).
   
   * Si no te pregunta nada: Presiona `Ctrl + Shift + P` y escribe "CMake: Configure".

PASO 3: COMPILAR (BUILD)
   - En la barra inferior de color azul/morado de VS Code, busca el botón que dice "Build" 
     (o un ícono de engranaje) y haz clic.
   - Espera a que termine. Debería decir "Build finished successfully".

PASO 4: EJECUTAR (RUN)
   - Al lado del botón de Build, verás un botón de "Play" (triángulo). Haz clic ahí.
   - ¡El juego debería abrirse inmediatamente!

------------------------------------------------------------------------
3. SOLUCIÓN DE PROBLEMAS COMUNES
------------------------------------------------------------------------

A) PROBLEMA: "El juego se abre y se cierra inmediatamente sin error"
   SOLUCIÓN: Windows no encuentra las librerías DLL junto al ejecutable.
   1. Ve a la carpeta "libs/SFML/bin".
   2. Copia todos los archivos .dll.
   3. Ve a la carpeta "build" (donde se creó el archivo .exe).
   4. Pégalos ahí.

B) PROBLEMA: "Error: g++ is not recognized..."
   SOLUCIÓN: No tienes el compilador MinGW instalado o configurado en tu PC.
   Necesitas instalar MSYS2/MinGW para poder compilar C++ en Windows.

C) PROBLEMA: "File not found" al compilar
   SOLUCIÓN: Asegúrate de haber abierto la CARPETA completa del proyecto en VS Code, 
   no solo los archivos individuales. CMake necesita ver toda la estructura.

------------------------------------------------------------------------
ESTRUCTURA DEL PROYECTO (PARA TU REFERENCIA)
------------------------------------------------------------------------
/src      -> Código fuente (.cpp) - Aquí está la lógica.
/include  -> Archivos de cabecera (.hpp) - Aquí están las definiciones.
/libs     -> Librerías SFML (No tocar, son para que el juego funcione).
/build    -> Aquí se genera el ejecutable final.

========================================================================