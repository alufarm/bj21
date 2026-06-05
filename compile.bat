@echo off
rem Включаем поддержку UTF-8 в консоли Windows
chcp 65001 > nul

set "COMPILER=C:/msys64/ucrt64/bin/g++.exe"

if not exist "%COMPILER%" (
    echo [ОШИБКА] Компилятор НЕ найден по пути: %COMPILER%
    echo Пожалуйста, проверьте Шаг 2 под этим сообщением.
    goto end
)

if not exist "Debug" mkdir "Debug"

echo Начинаем компиляцию...
"%COMPILER%" -fdiagnostics-color=always -g "%CD%\src\*.cpp" -o "%CD%\Debug\main.exe" -I"%CD%\include" -I"%CD%\ThirdParty" -L"%CD%\lib" -lsfml-graphics -lsfml-window -lsfml-system

if %errorlevel% equ 0 (
    echo [УСПЕХ] Сборка завершена успешно!
    echo Запуск программы из корня проекта...
    echo --------------------------------------------------
    
    rem Запуск файла без смены рабочей директории консоли
    .\Debug\main.exe
    
    echo --------------------------------------------------
    echo Программа завершила работу.
) else (
    echo [ОШИБКА] Во время компиляции произошла ошибка. Сборка остановлена.
)

:end
pause
