# Требования
C++20, для сборки по пресетам - clang++-16 или g++-12, Ninja.
# Сборка
CMake
```
cmake --preset=*имя пресета*
cmake --build /build/*имя билда*
```
Или (без пресета)
```
mkdir build
cd build
cmake ..
cmake --build .
```

# Использование
```
r2v *путь к .dat файлу с бинарными данными*
```
