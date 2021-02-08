# Khadas VIM3 board running SPI device MAX6675
For the newest commit of `fenix` these steps works for me to have /dev/spidev1.0 based on https://forum.khadas.com/t/vim3-ubuntu-spi/9372/2?u=chiahungmou and suggestions above.

1.Modify `/fenix/linux/arch/arm64/boot/dts/amlogic/kvim3_linux.dts b/arch/arm64/boot/dts/amlogic/kvim3_linux.dts` as below:
    	 
     &pwm_ef {
			status = "okay";
			pinctrl-names = "default";
			pinctrl-0 = <&pwm_f_pins1>;
	 };

     ...	 

	 &spicc1 {
			status = "enable";
			pinctrl-names = "default";
			pinctrl-0 = <&spicc1_pins>;
			cs-gpios = <&gpio GPIOH_6 0>;
     ...

2. Run `make kernel && make debs && make`

3.  Replace` /boot/zImage` and `/boot/dtb/kvim3_linux.dtb` with `Image` and `kvim3_linux.dtb` which are built at step 2.

        $scp ./fenix/linux/arch/arm64/boot/Image khadas@192.168.XXX.XXX:/tmp
        $scp ./fenix/linux/arch/arm64/boot/dts/amlogic/kvim3_linux.dtb  khadas@192.168.XXX.XXX:/tmp
        $ssh khadas@192.168.XXX.XXX
        $sudo mv /tmp/Image /boot/zImage
        $sudo mv /tmp/kvim3_linux.dtb /boot/dtb

4. Update device tree overlay.

   `vim /boot/env.txt` 
    to change this line: overlays=uart3 i2c3 os08a10 spi1

5.WiringPi check gpio.

    `	khadas@Khadas:~$ gpio readall
	 +------+-----+----------+------+---+----+---- Model  Khadas VIM3 --+----+---+------+----------+-----+------+
	 | GPIO | wPi |   Name   | Mode | V | DS | PU/PD | Physical | PU/PD | DS | V | Mode |   Name   | wPi | GPIO |
	 +------+-----+----------+------+---+----+-------+----++----+-------+----+---+------+----------+-----+------+
	 |      |     |       5V |      |   |    |       |  1 || 21 |       |    |   |      | GND      |     |      |
	 |      |     |       5V |      |   |    |       |  2 || 22 | P/U   |    | 1 | ALT1 | PIN.A15  | 6   |  475 |
	 |      |     |   USB_DM |      |   |    |       |  3 || 23 | P/U   |    | 1 | ALT1 | PIN.A14  | 7   |  474 |
	 |      |     |   USB_DP |      |   |    |       |  4 || 24 |       |    |   |      | GND      |     |      |
	 |      |     |      GND |      |   |    |       |  5 || 25 | P/U   |    | 1 | ALT0 | PIN.AO2  | 8   |  498 |
	 |      |     |   MCU3V3 |      |   |    |       |  6 || 26 | P/U   |    | 1 | ALT0 | PIN.AO3  | 9   |  499 |
	 |      |     |  MCUNRST |      |   |    |       |  7 || 27 |       |    |   |      | 3V3      |     |      |
	 |      |     |  MCUSWIM |      |   |    |       |  8 || 28 |       |    |   |      | GND      |     |      |
	 |      |     |      GND |      |   |    |       |  9 || 29 | P/D   |    | 0 | ALT0 | PIN.A1   | 10  |  461 |
	 |      |  18 |     ADC0 |      |   |    |       | 10 || 30 | P/D   |    | 1 | ALT0 | PIN.A0   | 11  |  460 |
	 |      |     |      1V8 |      |   |    |       | 11 || 31 | P/D   |    | 0 | ALT0 | PIN.A3   | 12  |  463 |
	 |      |  19 |     ADC1 |      |   |    |       | 12 || 32 | P/D   |    | 0 | ALT0 | PIN.A2   | 13  |  462 |
	 |  506 |   1 | PIN.AO10 | ALT3 | 0 |    |   P/U | 13 || 33 | P/D   |    | 0 | ALT1 | PIN.A4   | 14  |  464 |
	 |      |     |     GND3 |      |   |    |       | 14 || 34 |       |    |   |      | GND      |     |      |
	 |  433 |   2 |   PIN.H6 |  OUT | 1 |    |   P/D | 15 || 35 | P/D   |    | 0 | ALT2 | PWM-F    | 15  |  432 |
	 |  434 |   3 |   PIN.H7 | ALT2 | 0 |    |   P/D | 16 || 36 |       |    |   |      | RTC      |     |      |
	 |      |     |      GND |      |   |    |       | 17 || 37 | P/D   |    | 0 | ALT2 | PIN.H4   | 16  |  431 |
	 |  497 |   4 |  PIN.AO1 | ALT0 | 1 |    |   P/U | 18 || 38 |       |    |   |      | MCU-FA1  |     |      |
	 |  496 |   5 |  PIN.AO0 | ALT0 | 1 |    |   P/U | 19 || 39 | P/D   |    | 0 | IN   | PIN.Z15  | 17  |  426 |
	 |      |     |      3V3 |      |   |    |       | 20 || 40 |       |    |   |      | GND      |     |      |
	 +------+-----+----------+------+---+----+-------+----++----+-------+----+---+------+----------+-----+------+

     `

5. Check pins related to SPI.

       VIM3 SPI:
       PIN37: GPIOH_4/SPIB_MOSI  -> type ALT2
       PIN35: PWM_F/SPIB_MISO    -> type ALT2
       PIN15: UARTC_RX/SPIB_SS   -> type OUT
       PIN16: UARTC_TX/SPIB_SCLK -> type ALT2

6. Build tester and run.
	   cd /WiringPi_khadas/examples
	   make spi_test_vim3
	   ./spi_test_vim3

MAX6675 reading temperature

![image](https://github.com/MouChiaHung/Khadas/blob/master/max6675.JPG)

