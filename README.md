# delta-bob-cat
Dumping grounds of Bob's delta 3D-printer electronics and test firmwares.

2015-06-13 catmaker
Tested flashing of LPC1769 LCPXpresso mcu with blink-LED exmple, using lpc21isp command.
  # apt-get install arm-none-eabi-gcc
  # apt-get install lpc21isp
  # cd lpc1769_blink_led-1.2.2-120407
  # make
  # lpc21isp lpc1769_blink_led.hex /dev/ttyUSB1 115200 12000
