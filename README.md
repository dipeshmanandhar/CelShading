# CelShading
Project to Render Scenes in Cel (Toon-like) Shading using OpenGL

Warning: This is a Work in Progress.

## Building Notes
* Compile using mingw-w64
    * Install mingw-w64 to a folder path with no spaces ("C:\Program Files" is NOT acceptable)
* Assimp requires very specific CMake settings when using mingw-w64 as described [here](https://stackoverflow.com/questions/61692793/assimp-cmake-mingw-w64-vscode-win10)
    * `-DCMAKE_BUILD_TYPE:STRING=Release -DBUILD_SHARED_LIBS:BOOL=OFF -DASSIMP_BUILD_ASSIMP_TOOLS:BOOL=OFF -DASSIMP_HEADERCHECK:BOOL=OFF -DASSIMP_OPT_BUILD_PACKAGES:BOOL=OFF -DSYSTEM_IRRXML:BOOL=OFF -DASSIMP_BUILD_SAMPLES:BOOL=OFF -DBUILD_DOCS:BOOL=OFF -DASSIMP_BUILD_TESTS:BOOL=OFF -S. -Bbuild_win`