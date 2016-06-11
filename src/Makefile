### PROJECT_DIR
### This is the path to where you have created/cloned your project
PROJECT_DIR       = $(realpath $(dir $(CURDIR)))

### ARDMK_DIR
### Path to the Arduino-Makefile directory.
# ARDMK_DIR         = $(PROJECT_DIR)/external/Arduino-Makefile
ARDMK_DIR = /usr/share/arduino

### ARDUINO_DIR
### Path to the Arduino application and resources directory.
# ARDUINO_DIR       = $(PROJECT_DIR)/external/Arduino
ARDUINO_DIR       = /usr/share/arduino
# Switch with line below for OS X:
#ARDUINO_DIR       = /Applications/Arduino.app/Contents/Java

### USER_LIB_PATH
### Path to where the your project's libraries are stored.
USER_LIB_PATH    :=  $(PROJECT_DIR)/lib

### ARDUINO_LIBS
### The arduino libraries to include for use
ARDUINO_LIBS     += SPI Wire
ARDUINO_LIBS     += MAX6675-library Adafruit_ILI9341 Adafruit_FT6206_Library Adafruit-GFX-Library
# SPI Wire

### BOARD_TAG
### It must be set to the board you are currently using. (i.e uno, mega2560, etc.)
BOARD_TAG         = nano328
# BOARD_SUB         = atmega328p
# ARDUINO_VERSION   = 10609
# FORCE_MONITOR_PORT = true

### MONITOR_BAUDRATE
### It must be set to Serial baudrate value you are using.
MONITOR_BAUDRATE  = 57600

### AVR_TOOLS_DIR
### Path to the AVR tools directory such as avr-gcc, avr-g++, etc.
AVR_TOOLS_DIR     = /usr
# Switch with line below for OS X:
#AVR_TOOLS_DIR     = /usr/local

### AVRDUDE
### Path to avrdude directory.
AVRDUDE           = $(shell which avrdude)
AVRDUDE_CONF      = /etc/avrdude.conf

### CFLAGS_STD
### Set the C standard to be used during compilation - https://github.com/sudar/Arduino-Makefile/blob/std-flags/arduino-mk-vars.md#cflags_std
CFLAGS_STD        = -std=gnu11

### CXXFLAGS_STD
### Set the C++ standard to be used during compilation. https://github.com/sudar/Arduino-Makefile/blob/std-flags/arduino-mk-vars.md#cxxflags_std
CXXFLAGS_STD      = -std=gnu++11

### CXXFLAGS
### Flags you might want to set for debugging purpose. Comment to stop.
# CXXFLAGS         += -pedantic -Wall -Wextra
# CXXFLAGS         += -g -Os -w
CPPFLAGS        += -fno-threadsafe-statics
# -x c++ -E -CC
# /usr/bin/avr-g++ -MMD -c
# -mmcu=atmega328p
# -DF_CPU=16000000L
# -DARDUINO=105
# -D__PROG_TYPES_COMPAT__
# -I/usr/share/arduino/hardware/arduino//cores/arduino
# -I/usr/share/arduino/hardware/arduino//variants/eightanaloginputs
# -I/usr/share/arduino/libraries/SPI
# -I/usr/share/arduino/libraries/Wire -I/usr/share/arduino/libraries/Wire/utility
# -I/home/jonathan/Documents/reflow-oven/lib/Adafruit-GFX-Library
# -I/home/jonathan/Documents/reflow-oven/lib/Adafruit_FT6206_Library
# -I/home/jonathan/Documents/reflow-oven/lib/Adafruit_ILI9341
# -I/home/jonathan/Documents/reflow-oven/lib/MAX6675-library
# -Wall -ffunction-sections -fdata-sections -Os -fno-exceptions -std=c++11 button.cpp -o /home/jonathan/Documents/reflow-oven/build/nano328/oven/button.cpp.o

### MONITOR_PORT
### The port your board is connected to. Using an '*' tries all the ports and finds the right one.
MONITOR_PORT      = /dev/ttyUSB*

### CURRENT_DIR
### Do not touch - used for binaries path
CURRENT_DIR       = $(shell basename $(CURDIR))

### OBJDIR
### This is where you put the binaries you just compile using 'make'
OBJDIR            = $(PROJECT_DIR)/build/$(BOARD_TAG)/$(CURRENT_DIR)

### Do not touch - the path to Arduino.mk, inside the ARDMK_DIR
include $(ARDMK_DIR)/Arduino.mk

# print-%  : ; @echo $* = $($*)
# allvars: $(foreach v, $(.VARIABLES), $(info $(v) = $($(v))))
