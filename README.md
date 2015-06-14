# delta-bob-cat
Dumping grounds of Bob's delta 3D-printer electronics and test firmwares.

2015-06-14 catmaker
It has been discovered that the earlier test builds on a Raspberry Pi 2
running Ubuntu Mate Desktop (Trusty 14.04 LTS) worked, the same sources
did not work on an x86 Ubuntu Desktop (also Trusty 14.04 LTS).
The issue is described at:
https://bugs.archlinux.org/task/43629
The temporary solution is to use gcc-arm-none-eabi_4.9 available at:
https://launchpad.net/~terry.guo/+archive/ubuntu/gcc-arm-embedded
	# sudo add-apt-repository ppa:terry.guo/gcc-arm-embedded
	# sudo apt-get update
	# sudo apt-get install gcc-arm-none-eabi
This gcc 4.9 package also contains binutils 2.24 for ARM, so no need for
additional install of binutils-arm-none-eabi.
Also, the symlinks of *_s.a files to *_nano.a files described below are
still needed, but the location of these files are in:
      /usr/arm-none-eabi/lib

2015-06-14 catmaker
Imported Smoothieware into project. Test-built but flashing is untested.
arm-none-eabi-gcc needs adjustment described in 
http://www.lpcware.com/content/forum/newlib-nano-libraries-missing-lpcxpresso-76
  # cd /usr/lib/arm-none-eabi/newlib
  # sudo ln -s libstdc++_nano.a libstdc++_s.a
  # sudo ln -s libsupc++_nano.a libsupc++_s.a
  # sudo ln -s libc_nano.a libc_s.a

2015-06-13 catmaker
Tested flashing of LPC1769 LCPXpresso mcu with blink-LED example,
using lpc21isp command.
  # apt-get install arm-none-eabi-gcc
  # apt-get install lpc21isp
  # cd lpc1769_blink_led-1.2.2-120407
  # make
  # lpc21isp lpc1769_blink_led.hex /dev/ttyUSB1 115200 12000
