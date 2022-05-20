# Turbod
WE-BT16 v4 (turbo release) daemon

## Requirements
- Linux
- macOS 10.15 or higher
- A valid WE-BT16 installtion installed to `/usr/bin`

## Install
```bash 
$ echo "deb https://apt.cssudii.tk main main" | sudo tee -a /etc/apt/sources.list
$ sudo apt update
$ sudo apt-get install turbod
```

## Build
```bash
$ mkdir build && cd build
$ cmake ..
$ make

# if you want to install
$ make install
```