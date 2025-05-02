# Simulated RTOS

# Overview

This repository contains a simulated real-time operating system (RTOS) that runs on a virtual ARM Cortex-M microcontroller using Renode, an open-source emulator. The RTOS handles basic task scheduling and interrupt-driven execution, and it is designed to run on an STM32F4-discovery board. It uses the libopencm3 library.

We created this project for educational purposes, offering insight into RTOS concepts like preemptive task switching, periodic interrupts, and queue-based task scheduling—all within a simulated embedded environment.

# Features

- Semi-preemptive task scheduling
- Timer-based interrupts using the stm32f4's TIM2 and TIM3 timers
- Optional Python visualization included via Jupyter notebook

# Prerequisites

Make sure the following are installed on your Linux system:

For compiling:
- Make
- GCC
- Python3 (needed for libopencm3)

Running the code:
- Renode emulator
    - Installation instructions here: https://github.com/renode/renode/#installation (we recommend using the Linux portable release)

Visualization:
- Python3

# How to use

### 1. Clone the Repository

```
git clone git@github.com:olincollege/simulated-rtos.git
cd simulated-rtos
```

### 2. Compile the RTOS

Compile the code by running make in the terminal. It will take a minute or so for libopencm3 to compile.
```
make
```
This will generate an ELF binary file that can be loaded into Renode.

### 3. Install and Run Renode

Install renode by following the instructions in the README here: https://github.com/renode/renode/#installation
We wrote a bash script start file to start the renode file.
Open renode by running “./start.sh”.
This will open two windows—one for the main Renode console and another for UART output.
In the external window labeled Renode, enter “start”. This should produce output on the other screen

# Visualization (Optional)

We provide a Python Jupyter notebook to help visualize task scheduling behavior. This can be found in the plot/ directory.

# Testing

Testing done with cmake for queue library. Everything else can't be tested well with unit tests, so we did behavioral testing using plotting. Found in tests/ directory.
