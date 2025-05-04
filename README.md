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

### 3. Run Renode

To run our script in Renode, run the shell script start.sh. The script assumes your download of Renode is stored in the same folder as the cloned repo; you may have to update the path in this script if your download of Renode is stored somewhere else.

```
./start.sh
```

Running start.sh will open two Renode terminals (one of them takes a couple seconds to appear). To run our RTOS, type start in the first window that pops up. It should print out a whole bunch of log messages indicating what the scheduler is doing as it runs. You can also pause and quit (watch out, quitting will close both windows and return to terminal).

# Visualization and Testing

We provide a Jupyter notebook to help visualize task scheduling behavior. This can be found in the plot/ directory. In the notebook you will find a fuction that we used for testing. But if you want to take your own data you can copy the print log output from Renode and paste it into a new .txt in the same folder, then use the premade plotting function in the Jupyter notebook to visualize it.

### Testing

We tested our RTOS in two different ways. The first is unit tests using Criterion, which we used to validate our queue implementation. The rest of our code is heavily hardware-dependent and not easily tested with regular unit tests, so we opted for a more behavioral testing approach using our visualization code.

Criterion unit tests:

- A note on this: while our general scheduler code is compiled with Make, our Criterion test code is separately compiled with CMake. We did this because we are familiar with how to set up Criterion with CMake and have heard it can be finicky to set up with Make. However, this has presented one issue, which is that our CMake setup isn't set up to work with libopencm3, meaning that we can't compile any of our function libraries that need to include libopencm3. We spent some time trying to either get our Make setup to include Criterion or get our CMake setup to include libopencm3 (which would let us test the C logic around the hardwarde functions, but still not the hardware functions themselves), but ultimately we decided this was not worth our time to set up. Our code outside of the queue implementation (ie the scheduler, the interrupts, and the tasks) is mostly made up of hardware functions we can't test with unit tests anyway, and the logic in there that is unit testable is simple enough that we felt our time was better spent on behavioral testing (described below).
- Our criterion unit tests for the queue library are in the tests/ folder (test_queue).

Behavioral testing with plots:

- Since much of our scheduler behavior is governed by interrupts that are not unit-testable, we felt a different testing approach was necessary. We ran our scheduler many times, each time isolating a single component or small set of components of our system, plotting the resulting behavior, and comparing that to our expectations for how it should act. Although this approach doesn't give us as fine-grained insight into our system as unit tests, we believe these tests are sufficient to validate that our scheduler works as intended.
- The resulting data files are in the plot/ folder and the resulting plots can be found in the jupyter notebook.
