# toolchain-windows.cmake

# Tell CMake we're cross-compiling for Windows
set(CMAKE_SYSTEM_NAME Windows)

# Specify MinGW compiler paths
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)

# Optional: Enable static linking for portability (no DLL dependencies)
set(CMAKE_EXE_LINKER_FLAGS "-static")

# Optional: Set Windows-style file extensions
set(CMAKE_EXECUTABLE_SUFFIX ".exe")
