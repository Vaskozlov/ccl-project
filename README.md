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

```bash
git clone https://github.com/Vaskozlov/ccl-project.git
cd ccl-project
mkdir build
cd build
```

```bash
cmake .. -G Ninja
ninja
```

При использовании vcpkg:
```bash
cmake .. -G Ninja -DCMAKE_TOOLCHAIN_FILE=`путь до vcpkg + scripts/buildsystems/vcpkg.cmake`
ninja
```

## Установка
```bash
ninja install all
```

Также имеется возможность сборки rpm пакет при помощи CPack

```bash
cpack -G RPM
```
