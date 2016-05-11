# Reflow Oven
Reflow soldering oven firmware using an Arduino Nano and touchscreen LCD

## Building
To build the reflow oven controller, first download the external libraries with the following command.
```
git submodule update --init
```

### Linux Users (Ubuntu)
If you're building this on linux (tested on Ubuntu), there are a few setup steps required if you don't have the Arudino IDE on your computer.

#### Install Arduino IDE
To install the required items for compiling code for an Arduino, run the following command in a terminal.
```
# install everything required for compiling and uploading code
sudo apt-get install arduino arduino-core
```

#### Add udev rules
You will need to add the provided [`99-ftdi.rules`](./99-ftdi.rules) file to your `/etc/udev/rules.d/` directory. Once it is moved there, you will need to restart the udev service. Here's a recap of how this can be done in a terminal.
```
# go to the repo's root location
cd reflow-oven

# copy over the udev rules
cp 99-ftdi.rules /etc/udev/rules.d/

# restart the udev service
sudo service udev restart
```

#### Update `preferences.txt`
After the above step is complete, it's likely that you will have to update your Arduino IDE's `preferences.txt` file. You can find the location of this file by starting the Arduino IDE and opening `File` -> `Preferences`. Then, the location of this file can be seen after the line that says 'More preferences can be edited directly in the file'. The default location is `~/.arduino/preferences.txt`.

Once you know where to find your `preferences.txt` file, go ahead and close the Arduino IDE and open it in any text editor.

Find the line that starts with `serial.port=`, and change it to the following.
```
serial.port=/dev/arduino
```

