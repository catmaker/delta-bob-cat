# delta-bob-cat
Dumping grounds of Bob's delta 3D-printer electronics and test firmwares.

2015-06-14 catmaker
Imported Smoothieware into project. Test-built but flashing is untested.
arm-none-eabi-gcc needs adjustment described in 
http://www.lpcware.com/content/forum/newlib-nano-libraries-missing-lpcxpresso-76
  # cd /usr/lib/arm-none-eabi/newlib
  # sudo ln -s libstdc++_nano.a libstdc++_s.a
  # sudo ln -s libsupc++_nano.a libsupc++_s.a
  # sudo ln -s libc_nano.a libc_s.a

2015-06-13 catmaker
Tested flashing of LPC1769 LCPXpresso mcu with blink-LED exmple,
using lpc21isp command.
  # apt-get install arm-none-eabi-gcc
  # apt-get install lpc21isp
  # cd lpc1769_blink_led-1.2.2-120407
  # make
  # lpc21isp lpc1769_blink_led.hex /dev/ttyUSB1 115200 12000
