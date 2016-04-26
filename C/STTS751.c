// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// STTS751
// This code is designed to work with the STTS751_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/products

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

void main() 
{
	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-1";
	if((file = open(bus, O_RDWR)) < 0) 
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device, STTS751 I2C address is 0x39(57)
	ioctl(file, I2C_SLAVE, 0x39);

	// Select configuration register(0x03)
	// Continuous conversion, 12-bit resolution(0x8C)
	char config[2] = {0};
	config[0] = 0x03;
	config[1] = 0x8C;
	write(file, config, 2);
	// Select rate conversion register(0x04)
	// 1 conversion / sec(0x04)
	config[0] = 0x04;
	config[1] = 0x04;
	write(file, config, 2);
	sleep(1);

	// Read 2 bytes of data
	// temp lsb, temp msb
	// Read msb
	char reg[1] = {0x02};
	write(file, reg, 1);
	char data[1] = {0};
	if(read(file, data, 1) != 1)
	{
		printf("Erorr : Input/output Erorr \n");
		exit(1);
	}
	int data_0 = data[0];
	// Read lsb
	reg[0] = 0x00;
	write(file, reg, 1);
	read(file, data, 1);
	int data_1 = data[0];

	// Convert the data to 12-bits
	int temp = (data_1 * 256 + (data_0 & 0xF0)) / 16;
	if(temp > 2047)
	{
		temp -= 4096;
	}
	double cTemp = temp * 0.0625;
	double fTemp = cTemp * 1.8 + 32;

	// Output to screen
	printf("Temperature in Celsius : %.2f C \n", cTemp);
	printf("Temperature in Fahrenheit : %.2f F \n", fTemp);
}
