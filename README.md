# Remote Lights Microcontroller

Microcontroller code for the teensy3.6 remote lights project.

## Platform.io setup
[Platform.io systemd daemon setup reference](https://community.platformio.org/t/howto-remote-agent-systemd-service/1739/4)
### Edit /etc/systemd/system/pio-remote.service
`
[Unit]
Description=Run platformio remote agent
Requires=network-online.target
After=network-online.target

[Service]
Type=simple
Environment="PLATFORMIO_AUTH_TOKEN=<insert your token here>"
WorkingDirectory=/home/<your user name>/.platformio/penv
ExecStart=/home/<your user name>/.platformio/penv/bin/pio remote agent start
User=<your user name>
Restart=always
RestartSec=3

[Install]
WantedBy=multi-user.target
`
### Run commands:
`sudo systemctl daemon-reload && sudo systemctl enable pio-remote`
**then...**
`sudo systemctl start pio-remote`
**check status with...**
`sudo systemctl status pio-remote`

### Teensy udev

`
# UDEV Rules for Teensy boards, http://www.pjrc.com/teensy/
# MODIFIED for remotes-lights project, added symlink so teensies are easily identifiable.
#
# The latest version of this file may be found at:
#   http://www.pjrc.com/teensy/00-teensy.rules
#
# This file must be placed at:
#
# /etc/udev/rules.d/00-teensy.rules    (preferred location)
#   or
# /lib/udev/rules.d/00-teensy.rules    (req'd on some broken systems)
#
# To install, type this command in a terminal:
#   sudo cp 00-teensy.rules /etc/udev/rules.d/00-teensy.rules
#
# After this file is installed, physically unplug and reconnect Teensy.
#
ATTRS{idVendor}=="16c0", ATTRS{idProduct}=="04*", ENV{ID_MM_DEVICE_IGNORE}="1", ENV{ID_MM_PORT_IGNORE}="1"
ATTRS{idVendor}=="16c0", ATTRS{idProduct}=="04[789a]*", ENV{MTP_NO_PROBE}="1"
KERNEL=="ttyACM*", ATTRS{idVendor}=="16c0", ATTRS{idProduct}=="04*", MODE:="0666", RUN:="/bin/stty -F /dev/%k raw -echo" SYMLINK+="teensy%n"
KERNEL=="hidraw*", ATTRS{idVendor}=="16c0", ATTRS{idProduct}=="04*", MODE:="0666"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="16c0", ATTRS{idProduct}=="04*", MODE:="0666"

#
# If you share your linux system with other users, or just don't like the
# idea of write permission for everybody, you can replace MODE:="0666" with
# OWNER:="yourusername" to create the device owned by you, or with
# GROUP:="somegroupname" and mange access using standard unix groups.
#
# ModemManager tends to interfere with USB Serial devices like Teensy.
# Problems manifest as the Arduino Serial Monitor missing some incoming
# data, and "Unable to open /dev/ttyACM0 for reboot request" when
# uploading.  If you experience these problems, disable or remove
# ModemManager from your system.  If you must use a modem, perhaps
# try disabling the "MM_FILTER_RULE_TTY_ACM_INTERFACE" ModemManager
# rule.  Changing ModemManager's filter policy from "strict" to "default"
# may also help.  But if you don't use a modem, completely removing
# the troublesome ModemManager is the most effective solution.
`
