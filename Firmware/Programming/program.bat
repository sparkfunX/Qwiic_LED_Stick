@echo Programming the Qwiic LED Stick.
@pause
:loop

@echo -
@echo Flashing bootloader...
rem High fuse is 0xD5 to set BOD at 2.7V. LFuse is 0xE2 for internal 8MHz operation
@avrdude -C avrdude.conf -pattiny85 -cusbtiny -e -Uefuse:w:0xFF:m -Uhfuse:w:0xD5:m -Ulfuse:w:0xE2:m

rem @timeout 1

@echo -
@echo Flashing firmware...

rem The -B1 option reduces the bitclock period (1us = 1MHz SPI), decreasing programming time
rem May increase verification errors

@avrdude -C avrdude.conf -pattiny85 -cusbtiny -e -Uflash:w:Qwiic_LED_Stick.ino.hex:i -B1

@echo Done programming! Ready for next board.
@pause

goto loop