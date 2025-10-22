@echo off
setlocal enabledelayedexpansion

:: --- Налаштування ---
set "EXECUTABLE_NAME=StudentManager.exe"
set "DEFAULT_BUILD_TYPE=Debug"

:: --- Кольори (можуть не працювати на старих версіях Windows) ---
for /f "tokens=1,2 delims=#" %%a in ('"prompt #$H#$E# & echo on & for %%b in (1) do rem"') do set "ESC=%%b"
set "GREEN=!ESC![0;32m"
set "YELLOW=!ESC![1;33m"
set "RED=!ESC![0;31m"
set "NC=!ESC![0m"

:: --- Функції для виводу ---
:info
echo %GREEN%[INFO]%NC% %~1
goto :eof
:warn
echo %YELLOW%[WARN]%NC% %~1
goto :eof
:error
echo %RED%[ERROR]%NC% %~1
goto :eof

:: --- Функція для виводу допомоги ---
:usage
echo Usage: %0 [command] [build_type]
echo.
echo Commands:
echo   (none)      Збирає проєкт (за замовчуванням: %DEFAULT_BUILD_TYPE%).
echo   run         Збирає (якщо потрібно) та запускає проєкт.
echo   clean       Видаляє всі артефакти збірки (директорії build та bin).
echo.
echo Build Types:
echo   debug       Збірка з відлагоджувальною інформацією (за замовчуванням).
echo   release     Оптимізована збірка для продуктивності.
exit /b 0

:: --- Функція для перевірки прав адміністратора ---
:check_admin
openfiles >nul 2>&1
if %errorlevel% neq 0 (
    call :error "Для встановлення залежностей потрібні права адміністратора."
    call :error "Будь ласка, запустіть цей скрипт від імені адміністратора."
    pause
    exit /b 1
)
goto :eof

:: --- Функція для перевірки та встановлення пакетів ---
:install_package
where %~1 >nul 2>&1
if %errorlevel% equ 0 (
    rem call :info "Залежність '%~1' вже встановлено."
    goto :eof
)
call :check_admin
call :info "Залежність '%~1' не знайдено. Спроба встановлення..."
where winget >nul 2>&1
if %errorlevel% equ 0 (
    winget install --id %~2 -e --source winget
) else (
    where choco >nul 2>&1
    if %errorlevel% equ 0 (
        choco install %~3 -y
    ) else (
        call :error "Не знайдено ani winget, ani Chocolatey. Будь ласка, встановіть один з них."
        pause
        exit /b 1
    )
)
where %~1 >nul 2>&1
if %errorlevel% neq 0 (
    call :error "Не вдалося встановити '%~1'. Будь ласка, встановіть його вручну."
    pause
    exit /b 1
)
call :info "'%~1' успішно встановлено."
goto :eof

:: --- Розбір аргументів ---
set "COMMAND=build"
set "BUILD_TYPE=%DEFAULT_BUILD_TYPE%"
if /i "%~1"=="help" (call :usage)
if /i "%~1"=="run" (set "COMMAND=run")
if /i "%~1"=="clean" (set "COMMAND=clean")
if defined %2 (set "BUILD_TYPE=%~2")
if /i not "%~1"=="run" if /i not "%~1"=="clean" if not "%~1"=="" (set "BUILD_TYPE=%~1")

set "BUILD_TYPE_LOWER=%build_type:A=a%"
set "BUILD_TYPE_LOWER=%build_type_lower:B=b%"
:: ... (продовжити для всього алфавіту, але для Debug/Release достатньо)
if /i "%BUILD_TYPE%"=="Debug" set "BUILD_TYPE_LOWER=debug"
if /i "%BUILD_TYPE%"=="Release" set "BUILD_TYPE_LOWER=release"
set "BUILD_DIR=build\%BUILD_TYPE_LOWER%"

:: --- Виконання команди clean ---
if /i "%COMMAND%"=="clean" (
    call :info "Очищення проєкту..."
    if exist build (
        rmdir /s /q build
        call :info "Директорію 'build' видалено."
    )
    if exist bin (
        rmdir /s /q bin
        call :info "Директорію 'bin' видалено."
    )
    if exist otherVariants\bin (
        rmdir /s /q otherVariants\bin
        call :info "Директорію 'otherVariants\bin' видалено."
    )
    exit /b 0
)

:: --- Перевірка та встановлення залежностей ---
call :install_package "cmake" "Kitware.CMake" "cmake"
call :install_package "ninja" "Ninja-build.Ninja" "ninja"
call :install_package "cl" "Microsoft.VisualStudio.BuildTools" "visualstudio2022buildtools"

:: --- Логіка збірки ---
:build_project
call :info "Початок збірки (%BUILD_TYPE%)..."
if not exist "%BUILD_DIR%" (
    call :info "Створення директорії '%BUILD_DIR%'..."
    mkdir "%BUILD_DIR%"
)
cd "%BUILD_DIR%"
call :info "Генерація проєкту за допомогою CMake та Ninja..."
cmake -G Ninja -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ../..
if %errorlevel% neq 0 (
    cd ..\..
    call :error "Генерація CMake не вдалася."
    pause
    exit /b 1
)
call :info "Збірка проєкту..."
cmake --build .
if %errorlevel% neq 0 (
    cd ..\..
    call :error "Збірка не вдалася."
    pause
    exit /b 1
)
cd ..\..
call :info "✅ Збірка (%BUILD_TYPE%) завершена успішно."
goto :eof

:: --- Перевірка, чи потрібна збірка ---
if not exist "bin\%EXECUTABLE_NAME%" (
    call :warn "Виконуваний файл не знайдено. Запускаю збірку..."
    call :build_project
)

:: --- Виконання команди run ---
if /i "%COMMAND%"=="run" (
    call :info "Запуск програми..."
    echo ----------------------------------------
    .\bin\%EXECUTABLE_NAME%
    echo ----------------------------------------
    call :info "Програма завершила роботу."
)

endlocal
