# Require the CMake version matching what is included in the latest LTS version
# of Ubuntu, which at the time of writing is 22.04.
cmake_minimum_required(VERSION 3.22)

project(SimulatedRTOS
  VERSION 1.0
  LANGUAGES C
)

# Most embedded systems conform to C99 and might not have more modern functionality
set(CMAKE_C_STANDARD 99)

# Tell CMake we are cross‑compiling for ARM microcontroller
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/arm-gcc-toolchain.cmake)

# Include the source files. src subdirectory defines the executable "sim_rtos.elf"
add_subdirectory(src)
target_compile_definitions(sim_rtos.elf PRIVATE STM32F4)

# Add the STM Discovery chip startup file as a source for sim_rtos.elf
target_sources(sim_rtos.elf PRIVATE startup/startup_stm32f407xx.s)

# Include extra CMake configs
add_subdirectory(cmake)


# ---------------------------------------------------------------------------
# Paths to headers & libraries
# ---------------------------------------------------------------------------
set(LIBOPENCM3_DIR ${CMAKE_SOURCE_DIR}/lib/libopencm3)

include(ExternalProject)

ExternalProject_Add(libopencm3
    PREFIX ${CMAKE_BINARY_DIR}/libopencm3
    SOURCE_DIR ${LIBOPENCM3_DIR}
    CONFIGURE_COMMAND ""
    BUILD_COMMAND make CC=arm-none-eabi-gcc
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND ""
)

# Add libopencm3 as a dependency for sim_rtos.elf
target_link_libraries(sim_rtos.elf PRIVATE ${LIBOPENCM3_DIR}/lib/libopencm3_stm32f4.a)
add_dependencies(sim_rtos.elf libopencm3)

target_include_directories(sim_rtos.elf PRIVATE
    ${LIBOPENCM3_DIR}/include
    ${CMAKE_SOURCE_DIR}/startup)

link_directories(${LIBOPENCM3_DIR}/lib)

# ---------------------------------------------------------------------------
# Flags
# ---------------------------------------------------------------------------

# Compiler warnings
set(CMAKE_C_FLAGS "-Wall -Wextra")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wdouble-promotion")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wduplicated-branches")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wduplicated-cond")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wmisleading-indentation")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wunused")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

# Flags specific to cross compilation for ARM M4 Cortex
set(CPU_FLAGS "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")
set(COMMON_FLAGS "-ffunction-sections -fdata-sections ${CPU_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_FLAGS}")

set(CMAKE_EXE_LINKER_FLAGS
    "${CPU_FLAGS} \
     -nostartfiles \
     -Wl,-T,${CMAKE_SOURCE_DIR}/linker/stm32f407vg_flash.ld,--gc-sections")
