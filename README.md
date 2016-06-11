# Reflow Oven
Reflow soldering oven firmware using an Arduino Nano and touchscreen LCD

## Prerequesites

[PlatformIO](http://docs.platformio.org/en/latest/installation.html)

On Ubuntu, simply run the [`setup.sh`](./setup.sh) script to setup your computer up for compiling.
```
# set up all prerequesites
./setup.sh
```

The steps are also outlined here for those who run into trouble on a different Linux distro.

### Get The Code
Make sure you have git installed.
```
sudo apt-get install -y git
```

Now, let's grab the code and external libraries.
```
# get source code with git
git clone https://github.com/RoboJackets/reflow-oven

# download the external libraries needed
cd reflow-oven
git submodule update --init
```

## Building

### Linux Users (Ubuntu)
If you're building this on linux (tested on Ubuntu), here's how to compile the code.

### Compiling On The Command Line

Use the `make` command to build and optionally upload the compiled program to an Arduino
```
# only compile
make oven

# upload if Arduino is connected
make oven-prog
```
