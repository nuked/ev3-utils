ev3-utils
=========

Command-line utilities for the LEGO EV3 / Linux.

 * pwmcmd: command-line utility for prodding the EV3 PWM (motor driver) device.  Expects /dev/ev3dev\_pwm.
   e.g.: "pwmcmd -v 0x03 S 0xa4 0x01 0x00 S 0xa6 0x01 S 0xa4 0x01 0x20 W1000 0xa4 0x01 0x00 S 0x02".

