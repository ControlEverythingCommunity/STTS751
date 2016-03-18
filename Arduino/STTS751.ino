// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// STTS751
// This code is designed to work with the STTS751_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/products

#include<Wire.h>

// STTS751 I2C address is 0x39(57)
#define Addr 0x39

void setup() 
{
  // Initialise I2C communication as MASTER 
  Wire.begin();
  // Initialise serial communication, set baud rate = 9600
  Serial.begin(9600);
  
  // Select configuration register
  Wire.write(0x03);
  // 12-bit Resolution, Continuous conversion mode
  Wire.write(0x8C);
  // Stop I2C Transmission
  Wire.endTransmission();
  
  // Select rate conversion register
  Wire.write(0x04);
  // 1 conversion / second
  Wire.write(0x04);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(300);
}

void loop()
{
  unsigned data[2];
  
  // Starts I2C communication
  Wire.beginTransmission(Addr);
  // Select data register
  Wire.write(0x00);
  // Stop I2C transmission
  Wire.endTransmission();
  
  // Request 1 byte of data
  Wire.requestFrom(Addr, 1);
  
  // Read 1 byte of data
  // temp MSB
  if(Wire.available() == 1)
  {
    data[0] = Wire.read();
  }
  
  // Starts I2C communication
  Wire.beginTransmission(Addr);
  // Select data register
  Wire.write(0x02);
  // Stop I2C transmission
  Wire.endTransmission();
  
  // Request 1 byte of data
  Wire.requestFrom(Addr, 1);
  
  // Read 1 byte of data
  // temp LSB
  if(Wire.available() == 1)
  {
    data[1] = Wire.read();
  }
  
  // Convert the data to 12-bits
  int temp = ((data[0]  * 256) + (data[1] & 0xFC)) / 16;
  if(temp > 2047)
  {
    temp -= 4096;
  }
  float cTemp = temp * 0.0625;
  float fTemp = cTemp * 1.8 + 32;
  
  // Output data to serial monitor
  Serial.print("Temperature in Celsius : ");
  Serial.print(cTemp);
  Serial.println(" C");
  Serial.print("Temperature in Farhenheit : ");
  Serial.print(fTemp);
  Serial.println(" F");
  delay(1000); 
}
