# # Set target CPU architecture flags
# set(CPU_FLAGS "-mcpu=cortex-m4 -mthumb")

# set(CMAKE_C_FLAGS "${CPU_FLAGS} -Wall -g")
# set(CMAKE_EXE_LINKER_FLAGS "${CPU_FLAGS} -Wl,--gc-sections")

# add_executable(arm-cortex-m4.elf
#     main
#     startup/startup_stm32f407xx.s
# )
