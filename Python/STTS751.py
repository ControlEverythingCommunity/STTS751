# Distributed with a free-will license.
# Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
# STTS751
# This code is designed to work with the STTS751_I2CS I2C Mini Module available from ControlEverything.com.
# https://www.controleverything.com/products

import smbus
import time

# Get I2C bus
bus = smbus.SMBus(1)

# STTS751 address, 0x39(57)
# Select configuration register, 0x03(03)
#		0x8C(140)	12-bit Resolution, Continuous conversion mode
bus.write_byte_data(0x39, 0x03, 0x8C)
# STTS751 address, 0x39(57)
# Select rate conversion register, 0x04(04)
#		0x04(04)	 1 conversion / second
bus.write_byte_data(0x39, 0x04, 0x04)

time.sleep(0.5)

# STTS751 address, 0x39(57)
# Read data back, 2 bytes
# temp MSB, temp LSB
data0 = bus.read_byte_data(0x39, 0x00)
data1 = bus.read_byte_data(0x39, 0x02)

# Convert the data to 12-bits
temp = ((data0 * 256) + (data1 & 0xF0)) / 16
if temp > 2047 :
	temp -= 4096
cTemp = temp * 0.0625;
fTemp = cTemp * 1.8 + 32

# Output data to screen
print "Temperature in Celsius : %.2f C" %cTemp
print "Temperature in Fahrenheit : %.2f F" %fTemp
