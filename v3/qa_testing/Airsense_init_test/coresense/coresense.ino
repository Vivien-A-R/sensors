#include <Wire.h>
extern TwoWire Wire1;
#include <OneWire.h>
#include "config.cpp"



byte MAC_ID[LENGTH_FORMAT3 + 2] = {ID_MAC, 134,0,0,0,0,0,0}; // MAC address
OneWire ds2401(PIN_DS2401);  //DS2401 PIN
byte Temp_byte[8];


void initializecoresense(void)
{
    byte i;

    pinMode(PIN_SPV_AMP,INPUT);
    pinMode(PIN_SVP_SPL,INPUT);
    pinMode(PIN_RAW_MIC,INPUT);
    pinMode(PIN_HIH4030,INPUT);
    pinMode(PIN_CHEMSENSE_POW, OUTPUT);

    if (ds2401.reset() == TRUE)
    {
        ds2401.write(0x33);
        for (i = 0; i < 8; i++)
        {
            Temp_byte[i] = ds2401.read();
        }

        if (OneWire::crc8(Temp_byte, 8) == 0)
        {
            for (i=1; i<7; i++)
            {
                MAC_ID[i + 1] = Temp_byte[i];
            }

        }

        else
        {

            MAC_ID[3] = 0xff;

        }
    }
    else //Nothing is connected in the bus
    {
        MAC_ID[3] = 0xaa;
    }
}

void writeEEPROM (unsigned int memory_address, byte data_byte )
{
    Wire.beginTransmission(EEPROM_ADDRESS);
    Wire.write((int)(memory_address >> 8));   // MSB
    Wire.write((int)(memory_address & 0xFF)); // LSB
    Wire.write(data_byte);
    Wire.endTransmission();
    delay(5);
}

byte readEEPROM (unsigned int memory_address )
{
    byte recv_data = 0xff;

    Wire.beginTransmission(EEPROM_ADDRESS);
    Wire.write((int)(memory_address >> 8));   // MSB
    Wire.write((int)(memory_address & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(EEPROM_ADDRESS,1);

    if (Wire.available())
    {
        recv_data = Wire.read();
    }
    return recv_data;
}


void setup()
{
    delay(2000);
    Wire.begin();
    delay(2000);
    SerialUSB.begin(115200);
    initializecoresense();
}


void loop()
{
    SerialUSB.println(">>>>>> Core Sense Testing - Airsense Init Test <<<<<<");
    SerialUSB.println(" ");
    SerialUSB.print("1. Unique Board ID - ");
    for (byte i=2; i<8; i++)
    {
        SerialUSB.print(MAC_ID[i],HEX);

        if (i < 7)
        {
            SerialUSB.print(":");
        }
        else
        {
            SerialUSB.print("\n");
        }
    }
    SerialUSB.println(" ");
    delay(2000);
    SerialUSB.println("2. Internal I2C Communication Test : ");
    for (byte i=0; i<LENGTH_FORMAT3; i++)
    {
        writeEEPROM (i,MAC_ID[i+2]);
    }
    SerialUSB.println(" ");
    delay(100);
    SerialUSB.print("Printing UID from EEPROM - ");
    for (byte i=0; i<LENGTH_FORMAT3; i++)
    {
        SerialUSB.print(readEEPROM (i), HEX);
        if (i < 5)
        {
            SerialUSB.print(":");
        }
        else
        {
            SerialUSB.print("\n");
        }
    }
    SerialUSB.println(" ");
    SerialUSB.println(">>>>>> Test Finished <<<<<<");

    while (1)
    {
      delay(100);
    }

}





