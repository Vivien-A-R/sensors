#include "scanner.h"
#include "stringutils.h"
#include "devices.h"
#include "serial/serial.h"
#include "spi/spi.h"
#include "i2c/i2c.h"
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <OneWire.h>

OneWire ds2401(48);
#define MetSenNum 0x09
#define LightSenNum 0x08

void InitSerialUSB() {
	SerialUSB.begin(9600);
	delay(10);
}

int ReadSerialUSB() {
	while (SerialUSB.available() == 0) {
		delay(1);
	}

	return SerialUSB.read();
}

char dataReading[256];
char buffer[256];
Scanner scanner(ReadSerialUSB);
CustomSerial customserial;
CustomSPI customspi;

void InitI2C() {
	Wire.begin();
	delay(10);
}

void commandID() {
	byte id[8];

	if (!ds2401.reset()) {
		SerialUSB.println("! device not ready");
		return;
	}

	ds2401.write(0x33);

	for (int i = 0; i < 8; i++) {
		id[i] = ds2401.read();
	}

	if (OneWire::crc8(id, 8) != 0) {
		SerialUSB.println("! id failed crc");
		return;
	}

	SerialUSB.print("id ");

	for (int i = 0; i < 8; i++) {
		byte h = (id[i] >> 4) & 0xf;
		byte l = (id[i] >> 0) & 0xf;
		SerialUSB.print(h, HEX);
		SerialUSB.print(l, HEX);
	}

	SerialUSB.println();
}

void commandVersion() {
	SerialUSB.println("ver 4.0.1");
}

// command add notes???
// serial read...
// should not do a byte print out and ascii format...

void commandListDevices() {
	ListDevices();
}

void commandWriteCore() {
	scanner.Scan();

	// If user wants to change Mac address of Met/Lightsense boards,
	scanner.Scan();
	strncpy(dataReading, scanner.TokenText(), sizeof(dataReading));

	// long metmac = strtol(dataReading, NULL, 10);
	// metsense.writeMac(metmac);
}

void commandReadCore() {
	SerialUSB.println("read");

	while (scanner.Scan() != '\n') {
		const char *name = scanner.TokenText();
		const Device *dev = FindDevice(name);

		if (dev == NULL) {
			SerialUSB.print("! no device ");
			SerialUSB.println(name);
			continue;
		}

		SerialUSB.print(name);
		SerialUSB.print(" ");
		dev->read();
		SerialUSB.println();
	}

	SerialUSB.println();
}

void printData(byte ID, int size, char *dataReading) {
	SerialUSB.print("data ");
	SerialUSB.print(ID, HEX);
	SerialUSB.print(' ');

	for (int i = 0; i < size; i++) {
		SerialUSB.print(dataReading[i], HEX);
		SerialUSB.print(' ');
	}

	SerialUSB.println();
}

void commandSPIconfig()
{
	int size = fillBuffer();

	int slavePin = (int)buffer[0];
	int bitOrder = (int)buffer[size-2];
	int dataMode = (int)buffer[size-1];

	long maxSpeed = (buffer[1] << 16) | (buffer[2] << 8) | (buffer[3]);

	customspi.configSPI(slavePin, maxSpeed, bitOrder, dataMode);
}

void commandSPIread()
{
	int size = fillBuffer();

	int bufferlength = size-2;
	int delaytime = (int)buffer[0];
	int iter = (int)buffer[1];
	int slavePin;

	for (int i = 0; i < bufferlength; i++)
		buffer[i] = buffer[i + 2];

	customspi.readSPI(buffer, bufferlength, delaytime, iter, &slavePin);
	printData(slavePin, bufferlength, buffer);
}

void commandSerialconfig()
{
	int size = fillBuffer();
	int port = (int)buffer[0];
	int powerPin = (int)buffer[size-1];
	long datarate = (buffer[1] << 16) | (buffer[2] << 8) | (buffer[3]);
	long timeout = (buffer[4] << 16) | (buffer[5] << 8) | (buffer[6]);

	customserial.configSerial(port, datarate, timeout, powerPin);
}

void commandSerialwrite()
{
	int size = fillBuffer();
	int port = (int)buffer[0];
	int bufferlength = size-1;

	for (int i = 0; i < bufferlength; i++)
		buffer[i] = buffer[i + 1];

	customserial.writeSerial(buffer, bufferlength, port);
}

void readSerial3() {
	char buffer[256];
	Serial3.setTimeout(5000);
	int len = Serial3.readBytesUntil('\n', buffer, 255);
	buffer[len] = 0;

	// clean up whitespace
	for (int i = 0; i < len; i++) {
		int c = buffer[i];
		if (isspace(c)) {
			buffer[i] = ' ';
		}
	}

	SerialUSB.println(buffer);
}

void commandSerialRead() {
	if (scanner.Scan() == '\n') {
		SerialUSB.println("! missing port number");
	}

	void (*readFunc)();

	if (strcmp(scanner.TokenText(), "3") == 0) {
		readFunc = readSerial3;
	} else {
		readFunc = NULL;
	}

	if (readFunc == NULL) {
		SerialUSB.println("! invalid port");
		return;
	}

	SerialUSB.println("readser");
	readFunc();
	SerialUSB.println();
}

void commandI2CWrite()
{
	int size = fillBuffer();
	char address = buffer[0];
	int bufferlength = size-1;

	for (int i = 0; i < bufferlength; i++) {
		buffer[i] = buffer[i+1];
	}

	WriteI2C(address, bufferlength, buffer);
}

void commandI2Cread()
{
	int size = fillBuffer();
	char address = buffer[0];
	int length = buffer[1];

	ReadI2C(address, length, buffer);
	printData(address, length, buffer);
}

void commandAnalogread()
{
	int size = fillBuffer();
	int analogPin = (int)buffer[0];
	char analogid = 0xa0|analogPin;

	SerialUSB.print("data ");
	SerialUSB.println(buffer[0], HEX);

	int analogreading = analogRead(analogPin);
	// Print data,
	printReading(analogid, analogreading);
}

void commandDigitalread()
{
	int size = fillBuffer();
	int digitalPin = (int)buffer[0];
	char digitalid = digitalPin;

	SerialUSB.print("data ");
	SerialUSB.println(buffer[0], HEX);

	int digitalreading = digitalRead(digitalPin);
	// Print data,
	printReading(digitalid, digitalreading);
}

void commandDigitalwrite()
{
	int size = fillBuffer();
	int digitalPin = (int)buffer[0];
	int power = (int)buffer[1];

	if (power == 0) {
		digitalWrite(digitalPin, LOW); // power on
	} else if (power == 1) {
		digitalWrite(digitalPin, HIGH); // power off
	}
}

void printReading(char id, int value) {
	SerialUSB.print("data ");
	SerialUSB.print(id, HEX);
	SerialUSB.print(' ');
	SerialUSB.println(value);
}

int fillBuffer() {
	memset(buffer, 0, sizeof(buffer));
	memset(dataReading, 0, sizeof(dataReading));

	int size = 0;

	while (scanner.Scan() != '\n') {
		strncpy(dataReading, scanner.TokenText(), sizeof(dataReading));
		buffer[size++] = strtol(dataReading, NULL, 16);
	}

	return size;
}

struct Command {
	const char *name;
	void (*func)();
};

const Command commands[] = {
	{"read", commandReadCore},
	{"id", commandID},
	{"ver", commandVersion},
	{"devices", commandListDevices},
	{"readser", commandSerialRead},

	{"Corewrite", commandWriteCore},
	{"Coreread", commandReadCore},
	{"SPIconfig", commandSPIconfig},
	{"SPIread", commandSPIread},
	{"Serialconfig", commandSerialconfig},
	{"Serialwrite", commandSerialwrite},
	{"Serialread", commandSerialRead},
	{"I2Cwrite", commandI2CWrite},
	{"I2Cread", commandI2Cread},
	{"analogRead", commandAnalogread},
	{"digitalRead", commandDigitalread},
	{"digitalWrite", commandDigitalwrite},
};

const int numcommands = sizeof(commands) / sizeof(commands[0]);

const Command *FindCommand(const char *name) {
	for (int i = 0; i < numcommands; i++) {
		const Command *cmd = commands+i;

		if (strcmp(cmd->name, name) == 0) {
			return cmd;
		}
	}

	return NULL;
}

void ExecCommand() {
	// consume leading newline tokens
	while (scanner.Scan() == '\n') {
	}

	const Command *cmd = FindCommand(scanner.TokenText());

	if (cmd == NULL) {
		SerialUSB.print("! no command ");
		SerialUSB.println(scanner.TokenText());
		return;
	}

	cmd->func();
}

void setup() {
	InitSerialUSB();
	InitI2C();
	InitDevices();
}

void loop() {
	ExecCommand();
}
