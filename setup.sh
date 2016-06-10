#!/bin/bash

# Sets a computer up for compiling the code

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# install deps
sudo apt-get install -y git arduino arduino-core libdevice-serialport-perl libyaml-perl python-serial

# cd into the repo
cd "$SCRIPT_DIR"

# update library sources
git submodule update --init
cd -

