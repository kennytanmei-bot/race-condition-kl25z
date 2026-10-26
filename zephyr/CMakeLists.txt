# Arquivo CMake exigido pelo Zephyr no PlatformIO.
# Se o seu projeto atual ja possui um zephyr/CMakeLists.txt funcionando,
# mantenha o seu e apenas confira se a lista de fontes abaixo esta incluida.

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(monitor_acesso_race_condition)

# Os codigos-fonte ficam em src/ (pasta acima de zephyr/)
target_sources(app PRIVATE
    ../src/main.c
    ../src/sensores.c
    ../src/registro_eventos.c
)

# Permite que os .c encontrem configuracao.h e os demais cabecalhos em src/
target_include_directories(app PRIVATE ../src)
