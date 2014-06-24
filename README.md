Embedded-Co-Design
==================

EMU Robot Arm TS-7300

Operating System
This project uses the Technologic Systems supplied Debian Sarge Linux distribution a precompiled version is available for the TS-7300 which runs on a 2.4.36 kernel.

bunzip2 sdimage.dd.bz2 dd if=sdimage.dd of=/dev/{SD-CARD-DEVICE}

After writing the image to the SD card increase the size of partition 3.

Prior to booting into Debian (third partition) the initial ramdisk will check for /vmlinux.bin on the third partition. If this file is found it will be loaded into memory(0x00218000) and used instead of the default kernel (TS7300 User Manual, 2014).

What you will need
1 x TS-7300 platform
1 x SD card
1x Robot Arm
1 x 5v 1A power supply
1 x Keypad + Ribbon cable + Inverter Board
1 x LCD + Ribbon cable + Contrast effector
17 x Jumper Cables

Setup instructions

Firstly insert the SD card into the board confirming it is installed in right hand side slot.
Next connect the Keypad to the inverter board, then the inverter to the main board checking the connections.
Next continue to wire up the LCD, starting from Pin1.
Match pin 1 from the LCD to pin one on the TS. Pin one as displayed here has as small white dot next to it to identify it.
Then to the contrast potentiometer.
Connect the robot arm’s interface wires
Connect the wire from the TS to the matching input on the EMU.
Next attach the power connections from power supply (5V 1A) to robot arm.
Connect ethernet if remote or computer access required.
Finally connect the 5 volt power supply to the TS-7300 Main board.
On start up load FPGA then run release.
