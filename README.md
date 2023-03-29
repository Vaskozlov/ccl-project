# CCL-project

CCL - библиотека конструирования компилятора

## Поддерживаемые платформы
- Linux x64 (включая ALT linux)
- macOS x64 и arm64
- Windows x86 и x64

Проект при наличии нужных версий компилятора и библиотек может собраться под любую архитектуру и ос.

## Требования

Для сборки проекта подходят следующие версии компиляторов:
- Clang-16
- GCC-12
- MSVC-19.32

### Зависимости 
- [fmt](https://github.com/fmtlib/fmt)
- [cxxopts](https://github.com/jarro2783/cxxopts)

### Опциональные зависимости
- [catch2](https://github.com/catchorg/Catch2)
- [google-benchmark](https://github.com/google/benchmark)

! Все зависимости могут быть установлены при помощи vcpkg

## Сборка проекта

```cmake
mkdir build
cd build
```

```cmake
cmake .. -G Ninja
ninja
```

## Установка
```cmake
ninja install all
```

Также имеется возможность собрать rpm пакет при помощи CPack

```cmake
cpack -G RPM
```
