#!/bin/bash

# Función para ejecutar cpplint
run_cpplint() {
  local path=$1
  cpplint --filter=-legal/copyright,-build/header_guard,-whitespace/line_length,-build/include_subdir,-build/include_order,-build/c++11 --recursive "$path"
}

# Función para ejecutar cppcheck
run_cppcheck() {
  local path=$1
  cppcheck "$path" \
  --enable=warning,performance,portability,style \
  -isrc/common/lib \
  -ilevel_editor \
  --suppress=missingIncludeSystem \
  --suppress=missingInclude \
  --inline-suppr \
  --force
}

# Función para ejecutar clang-format
run_clang_format() {
  local path=$1
  ./clang-format-all "$path"
}

# Mostrar ayuda
show_help() {
  echo "Uso: $0 [hook] [path]"
  echo "Hooks disponibles:"
  echo "  cpplint       Ejecuta cpplint en el path especificado"
  echo "  cppcheck      Ejecuta cppcheck en el path especificado"
  echo "  clang-format  Ejecuta clang-format en el path especificado"
}

# Validar argumentos
if [ "$#" -lt 2 ]; then
  show_help
  exit 1
fi

hook=$1
path=$2

case $hook in
  cpplint)
    run_cpplint "$path"
    ;;
  cppcheck)
    run_cppcheck "$path"
    ;;
  clang-format)
    run_clang_format "$path"
    ;;
  *)
    echo "Hook desconocido: $hook"
    show_help
    exit 1
    ;;
esac