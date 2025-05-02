# Simulated RTOS

This is a repo containing
These instructions are for running our code on linux
Clone this repo: “git clone git@github.com:olincollege/simulated-rtos.git”
To compile the code, run “make”
You must have python installed on your computer
This will produce a .elf file

Install renode by following the instructions in the README here: https://github.com/renode/renode/#installation
Open renode by running “renode renode-config.resc”. This should pop up two external windows.
In the external window labeled Renode, enter “start”. This should produce output on the other screen

We also included a ipynp for anyone who is interested in generating a visualzation to see what is going on. which is in the
folder titled "plot"
