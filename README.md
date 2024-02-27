[![](https://github.com/dviglo2d-learn/mini_games/actions/workflows/main.yml/badge.svg)](https://github.com/dviglo2d-learn/mini_games/actions)

# Мини-игры

Небольшие игры на [движке Dviglo2D](https://github.com/dviglo2d/dviglo2d).

![](15_puzzle/screenshot_15.png)
![](letalka/screenshot_letalka.png)

## Сборка в Windows

В пустой папке создайте и запустите батник:

```
:: Меняем кодировку консоли на UTF-8
chcp 65001

:: Указываем пути к необходимым утилитам
set "PATH=%SystemRoot%\system32;c:\programs\cmake\bin;c:\program files\git\bin"

:: Качаем репозиторий с играми в папку repo
git clone https://github.com/dviglo2d-learn/mini_games repo

:: Качаем репозиторий движка в папку engine
git clone https://github.com/dviglo2d/dviglo2d engine

:: Так как в движке могут произойти изменения, ломающие обратную совместимость,
:: то возвращаем состояние репозитория к определённой версии
git -C engine reset --hard 238eb1062346c724816186c8fb7e2d4e1daeb993

:: Создаём проекты для Visual Studio 2022 в папке build_vs, используя конфиг CMakeLists.txt из папки repo
cmake.exe repo -B build_vs -G "Visual Studio 17" -A x64

:: Компилируем проекты в папке build_vs
cmake --build build_vs --config Debug
::cmake --build build_vs --config Release

:: Ждём нажатие Enter перед закрытием консоли
pause
```

Возможно, потребуется изменить пути к утилитам.

Результат компиляции будет помещён в папку `build_vs\result`.

Подробнее о процессе сборки (в том числе и в Linux):
* https://github.com/dviglo2d-learn/minimal_app
* https://github.com/dviglo2d/dviglo2d/blob/main/docs/building.md
