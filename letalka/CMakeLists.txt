# Название таргета
set(target_name letalka)

# Создаём список файлов
file(GLOB_RECURSE source_files src/*.cpp src/*.hpp)

# Создаём приложение
add_executable(${target_name} ${source_files})

if(NOT DV_WIN32_CONSOLE)
    # Используем точку входа WinMain()
    set_property(TARGET ${target_name} PROPERTY WIN32_EXECUTABLE TRUE)
endif()

# Отладочная версия приложения будет иметь суффикс _d
set_property(TARGET ${target_name} PROPERTY DEBUG_POSTFIX _d)

# Подключаем библиотеки
target_link_libraries(${target_name} PRIVATE dviglo entt)

# Копируем динамические библиотеки в папку с приложением
dv_copy_shared_libs_to_bin_dir(${target_name} ${result_dir} copy_shared_libs_to_app_dir)

# Список папок с ресурсами движка для копирования в result
set(dir_names engine_data samples_data)

# Копируем папки с ресурсами движка в result, если нужно
foreach(dir_name ${dir_names})
    if(NOT EXISTS ${result_dir}/${dir_name})
        dv_create_dir_link(${engine_dir}/result/${dir_name} ${result_dir}/${dir_name})
    endif()
endforeach()

# Копируем папку letalka_data в result, если нужно
if(NOT EXISTS ${result_dir}/letalka_data)
    dv_create_dir_link(${CMAKE_CURRENT_SOURCE_DIR}/letalka_data ${result_dir}/letalka_data)
endif()

# Заставляем VS отображать дерево каталогов
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src FILES ${source_files})

# Добавляем приложение в список тестируемых
add_test(NAME ${target_name} COMMAND ${target_name} -duration 5)
