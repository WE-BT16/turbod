# Turbod
WE-BT16 v4 (turbo release) daemon

## Support
- Linux
- macOS 10.15 or higher

## Install
```bash 
$ echo "deb https://url main main" | sudo tee -a /etc/apt/sources.list
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