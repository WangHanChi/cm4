# cm4  

The shell interface with ARM Cortex-M4 processor  
- Device: [NUCLEO-F429ZI](https://www.st.com/en/evaluation-tools/nucleo-f429zi.html)  

## Dependence

- ARM Cross Compiler
```
$ sudo apt-get install gcc-arm-none-eabi
```
- lsb-core
```
$ sudo apt-get install lsb-core
```
- OpenOCD
```
$ git clone git://git.code.sf.net/p/openocd/code openocd
$ sudo apt-get install build-essential pkg-config autoconf automake libtool libusb-dev libusb-1.0-0-dev libhidapi-dev
$ sudo apt-get install libtool libsysfs-dev
$ cd openocd && ./bootstrap
$ ./configure
$ make
$ sudo make install
$ openocd -v
```
- Screen
```
$ sudo apt-get install screen
```
- ST-link
```
$ sudo apt-get install git cmake libusb-1.0-0-dev
$ git clone https://github.com/stlink-org/stlink && cd stlink
$ cmake .
$ make
$ cd bin && sudo cp st-* /usr/local/bin
$ cd ../lib && sudo cp *.so* /lib32
$ cd ../config/udev/rules.d/ && sudo cp 49-stlinkv* /etc/udev/rules.d/
```

## Build

- Compile
```
$ make os
```
- Upload to board
```
$ make upload
```
- Connect with screen
```
$ make connect
```
- Help
```
$ make help
```
