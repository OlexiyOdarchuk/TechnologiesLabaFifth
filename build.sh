#!/usr/bin/env bash

# --- Налаштування ---
EXECUTABLE_NAME="Validator"
DEFAULT_BUILD_TYPE="Debug"

# --- Кольори для виводу ---
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# --- Функції для виводу ---
info() { echo -e "${GREEN}[INFO]${NC} $1"; }
warn() { echo -e "${YELLOW}[WARN]${NC} $1"; }
error() { echo -e "${RED}[ERROR]${NC} $1"; exit 1; }

# --- Функція для виводу допомоги ---
usage() {
    echo "Usage: $0 [command] [build_type]"
    echo
    echo "Commands:"
    echo "  <none>      Збирає проєкт (за замовчуванням: ${DEFAULT_BUILD_TYPE})."
    echo "  run         Збирає (якщо потрібно) та запускає проєкт."
    echo "  clean       Видаляє всі артефакти збірки (директорії build та bin)."
    echo
    echo "Build Types:"
    echo "  debug       Збірка з відлагоджувальною інформацією (за замовчуванням)."
    echo "  release     Оптимізована збірка для продуктивності."
    exit 0
}

# --- Функція для перевірки та встановлення пакетів ---
install_package() {
    if ! command -v $1 &> /dev/null; then
        info "Залежність '$1' не знайдено. Спроба встановлення..."
        if [[ -f /etc/debian_version ]]; then
            sudo apt-get update && sudo apt-get install -y $2
        elif [[ -f /etc/arch-release ]]; then
            sudo pacman -Syu --noconfirm $2
        elif [[ -f /etc/redhat-release ]]; then
            sudo dnf install -y $2
        elif [[ "$OSTYPE" == "darwin"* ]]; then
            brew install $2
        else
            error "Не вдалося визначити менеджер пакетів. Будь ласка, встановіть '$2' вручну."
        fi
        if ! command -v $1 &> /dev/null; then
            error "Не вдалося встановити '$2'. Будь ласка, встановіть його вручну."
        fi
        info "'$1' успішно встановлено."
    fi
}

# --- Розбір аргументів ---
COMMAND="build"
BUILD_TYPE_INPUT=$DEFAULT_BUILD_TYPE

if [ "$1" == "help" ] || [ "$1" == "--help" ]; then usage; fi
if [ "$1" == "run" ] || [ "$1" == "clean" ]; then
    COMMAND=$1
    if [ ! -z "$2" ]; then BUILD_TYPE_INPUT=$2; fi
else
    if [ ! -z "$1" ]; then BUILD_TYPE_INPUT=$1; fi
fi

BUILD_TYPE=$(tr '[:lower:]' '[:upper:]' <<< ${BUILD_TYPE_INPUT:0:1})${BUILD_TYPE_INPUT:1}
BUILD_DIR="build/${BUILD_TYPE_INPUT,,}"

# --- Виконання команди clean ---
if [ "$COMMAND" == "clean" ]; then
    info "Очищення проєкту..."
    rm -rf build bin otherVariants/bin
    info "Директорії 'build', 'bin' та 'otherVariants/bin' видалено."
    exit 0
fi

# --- Перевірка та встановлення залежностей ---
install_package "cmake" "cmake"
install_package "ninja" "ninja-build"

# --- Логіка збірки ---
build_project() {
    info "Початок збірки (${BUILD_TYPE})..."
    
    if [ ! -d "$BUILD_DIR" ]; then
        info "Створення директорії '$BUILD_DIR'..."
        mkdir -p "$BUILD_DIR"
    fi
    
    cd "$BUILD_DIR"
    
    info "Генерація проєкту за допомогою CMake та Ninja..."
    cmake -G Ninja -DCMAKE_BUILD_TYPE=$BUILD_TYPE ../..
    if [ $? -ne 0 ]; then
        cd ../..
        error "Генерація CMake не вдалася."
    fi
    
    info "Збірка проєкту..."
    cmake --build .
    if [ $? -ne 0 ]; then
        cd ../..
        error "Збірка не вдалася."
    fi
    
    cd ../..
    info "✅ Збірка (${BUILD_TYPE}) завершена успішно."
}

# --- Перевірка, чи потрібна збірка ---
if [ ! -f "bin/${EXECUTABLE_NAME}" ]; then
    warn "Виконуваний файл не знайдено. Запускаю збірку..."
    build_project
fi

# --- Виконання команди run ---
if [ "$COMMAND" == "run" ]; then
    info "Запуск програми..."
    echo "----------------------------------------"
    ./bin/$EXECUTABLE_NAME
    echo "----------------------------------------"
    info "Програма завершила роботу."
fi
