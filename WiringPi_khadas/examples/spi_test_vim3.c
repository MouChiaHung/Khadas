#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#include <string.h>

//#define MAX6675_CLOCK_SPEED 4000000
#define MAX6675_CLOCK_SPEED 4000000


int main()
{
	//unsigned char data1 = 'a';
	//unsigned char data2;
	
	//unsigned char data1[8]= {0x12, 0x34, 0x56, 0x78};
	unsigned char data1[8]= {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
	unsigned char data2[8]= {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

	int reg;

	if(-1 == wiringPiSetup()){
		printf("set up error");
		exit(1);
	}
	printf("set up ok\n");


	//if(-1 == wiringPiSPISetup(0, MAX6675_CLOCK_SPEED)){
	if(-1 == wiringPiSPISetupMode(0, MAX6675_CLOCK_SPEED, 0)){
		printf("set up spi error");
		exit(-1);
	}
	printf("set up spi ok\n");
	
	int t = 30;
	while (t--) {
	memset(data1, 0, 8);
	memset(data2, 0, 8);
	reg = wiringPiSPIDataRW_khadas(0, data1, data2, 2);
	if(reg < 0){
		printf("RW error");
		exit(-1);
	}
	printf("RW ok\n");
	printf("data1 : %x \n", *((unsigned int*)data1));
	printf("data2 : %x \n", *((unsigned int*)data2));
	short s = (data2[0]<<8) + data2[1];
	float temp = (s>>3)*0.25;
	printf("temperature: %f celsius\n", temp);
	delay(300);
	}

	exit(0);
}

