USB Mouse to Serial
======
This Linux-program will make a computer act as an adapter for a USB mouse to serial port. It was developed for Raspberry Pi to be able to use a modern USB mouse on older computers that only has a serial port.

Requirements
------
* Raspberry Pi running Raspbian - tested on a RPi 2 Model B running 2017-11-29-raspbian-stretch-lite
* Serial port - a simple USB to serial port adapter works well
* Null modem cable
* libevdev

Mouse Protocols
------
* Microsoft 2-button mouse (microsoft) [right]
* Logitech 3-button mouse (logitech) [left and right]
* Microsoft 3-button wheel mouse (wheel) [middle]
* Mouse Systems (mousesystems) [left]

The protocol can be set when starting the program by using the protocol-parameter and the value in parentheses, like this: `usb_mouse_to_serial --protocol mousesystems`

The protocol can also be changed on-the-fly by pressing the mouse button(s) in square brackets during the DSR/CTS toggle when loading the mouse driver on the computer (will be reset to requested/default protocol after a power cycle).

Microsoft 2-button mouse is the default protocol.

Installation
------
Connect a Raspberry Pi to a computer over the serial ports using a null modem cable.
Connect a USB mouse to the Raspberry Pi.
For the best experience it is recommended to set the polling rate of the USB mouse to 1000hz. This can be done by adding "usbhid.mousepoll=1" (without quotes) at the end of /boot/cmdline.txt so it looks something like this:

```
...fsck.repair=yes rootwait usbhid.mousepoll=1
```
A reboot is required for the change to take effect.

Verify that it worked:

```
cat /sys/module/usbhid/parameters/mousepoll 
1
```
Then install dependencies:

```
sudo apt install libevdev-dev
```
And finally compile and install the program:

```
make
sudo make install
```
Running
------
The program should be run as root.

The following parameters are available:

```
-o, --output device
	Serial device to use as output.
	If not set it will use /dev/ttyUSB0.
-i, --input device
	USB mouse device to use as input.
	It is recommended to not set this and let the program find a mouse automatically. If a mouse can’t be found or if the computer has more than one mouse connected use this to manually select one. The device is one of /dev/input/event*.
-p, --protocol protocol
	The protocol to use. One of microsoft, logitech, wheel or mousesystems (see Mouse Protocols above). The default is microsoft.
-r, --rate rate
	The rate to write to the serial output device in milliseconds.
	This value can be used to tweak how often the program should send data to the computer. By default the rate is 7.5 milliseconds for all protocols except Mouse Systems which is 8.33 milliseconds (might change later).
-d, --daemon
	Run in background.
-s, --suspend
	Automatically suspend mouse when there is no power from the serial port.
	May not work on all devices.
-x multiplier / -y multiplier
	Multiply X / Y with this value.
-S, --swap
	Swap left and right mouse buttons.
-v, --verbose
	Increase verbosity, can be used multiple times.
-V, --version
	Show version.
-h, --help
	Show usage and help.
```

Example:

```
usb_mouse_to_serial -o /dev/ttyUSB0 -p wheel -s -d
```

Disclaimer
------
This project is provided as-is, comes with no warranty and is not liable for any loss or damage to your equipment while using this program.