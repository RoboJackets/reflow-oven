# Reflow Oven
Reflow soldering oven firmware using an Arduino Nano and touchscreen LCD

## Prerequesites

On Ubuntu, simply run the [`setup.sh`](./setup.sh) script to setup your computer up for compiling.
```
# set up all prerequesites
./setup.sh
```

The steps are also outlined here for those who run into trouble on a different Linux distro.

### Add udev Rules
You will need to add the provided [`99-ftdi.rules`](./99-ftdi.rules) file to your `/etc/udev/rules.d/` directory. Once it is moved there, you will need to restart the udev service. Here's a recap of how this can be done in a terminal.
```
# go to the repo's root location
cd reflow-oven

# copy over the udev rules
cp 99-ftdi.rules /etc/udev/rules.d/

# restart the udev service
sudo service udev restart
```

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
If you're building this on linux (tested on Ubuntu), there are a few setup steps required to compile the code. Don't use the Arduino IDE - go through the command line steps & experience the simplicity of no GUIs!

### Compiling On The Command Line
[You ain't cool, unless you type your terminal](https://www.youtube.com/watch?v=gGDW-IfVA2s)

#### Install Dependencies
```
# install some things for connecting with an Arduino board
sudo apt-get install -y libdevice-serialport-perl libyaml-perl

# easiest way to get all compiler related things
sudo apt-get install -y arduino-core arduino

# needed whenever using the serial port
sudo apt-get install -y python-serial
```

### Compiling With The Arduino IDE 

*** **Don't do anything in this section, you've been warned.** ***

#### Install Arduino IDE
To install the required items for compiling code for an Arduino, run the following command in a terminal.
```
# install everything required for compiling and uploading code
sudo apt-get install arduino arduino-core
```

#### Update `preferences.txt`
After the above step is complete, it's likely that you will have to update your Arduino IDE's `preferences.txt` file. You can find the location of this file by starting the Arduino IDE and opening `File` -> `Preferences`. Then, the location of this file can be seen after the line that says 'More preferences can be edited directly in the file'. The default location is `~/.arduino/preferences.txt`.

Once you know where to find your `preferences.txt` file, go ahead and close the Arduino IDE and open it in any text editor.

Find the line that starts with `serial.port=`, and change it to the following.
```
serial.port=/dev/arduino
```

#### Give Up & Use The Command Line
While writing this readme, I've been going through each step of setup with the Arduino IDE to make sure everything is accurate. Mainly because it wasn't too long ago when I was a beginner learning the ins and outs of a command line.

I thought it would be useful for beginners to see a taste of the command line by setting up the project with a command line, but keep the familiarity of the Arduino IDE. Doing just that is apparently way trickier than I initially thought...

So I'm giving up here because these Arduino IDE setup steps are only going to work for 1 out of 100 beginners using Ubuntu...I'm only leaving the previous steps here in the rare situation that someone decides to finish it one day (hint: there's only 1 more step to do;)).

Now, go do the command line setup. It's much easier & will actually work! :)

- Jon

