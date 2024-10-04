# Путь к исходникам движка
set(engine_dir "${CMAKE_SOURCE_DIR}/dviglo2d")

# Указываем минимальную версию CMake
include(${engine_dir}/cmake/cmake_min_version.cmake)
cmake_minimum_required(VERSION ${dv_cmake_min_version})

# Название проекта
project("mini games")

# Подключаем директорию с движком
include(${engine_dir}/cmake/common.cmake)
add_subdirectory(${engine_dir} dviglo2d)

# Подключаем директорию с EnTT
add_subdirectory(entt)

# Папка для скомпилированных игр
set(result_dir "${CMAKE_BINARY_DIR}/result")

# Создаём папку result
file(MAKE_DIRECTORY ${result_dir})

# Указываем папку для следующих скомпилированных экзешников
dv_set_bin_dir(${result_dir})

add_subdirectory(15_puzzle)
add_subdirectory(clicker)
add_subdirectory(letalka)
