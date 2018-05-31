// ML8511 Light sensor -- UV

void InitSensor10()
{
	if (flagMCP1 == false)
	{
		mcp3428_1.init(MCP342X::L, MCP342X::L);
		MCP3428_1_ADDRESS = mcp3428_1.returnAddress();
		flagMCP1 = true;
	}
}

void ConfigSensor10()
{
	return;
}

void EnableSensor10()
{
	EnableSensor(0x10);
}

void DisableSensor10()
{
	DisableSensor(0x10);
}

void ReadSensor10(byte *sensorReading, int *readingLength)
{
	// byte address = mcp3428_1.returnAddress();
	byte writebyte[1] = {mcp3428_1.returnRegister(MCP342X::CHANNEL_2)};
	byte readarray[3];
	WriteReadI2C(MCP3428_1_ADDRESS, 1, writebyte, 3, readarray, 100);

	for (int i = 0; i < 2; i++)
    {
		sensorReading[i] = readarray[i];
	    *readingLength += 1;
    }

	// the last read is not used

	// mcp3428_1.selectChannel(MCP342X::CHANNEL_2, MCP342X::GAIN_1);
	// int ML8511 = mcp3428_1.readADC();
}

void WriteSensor10(byte *packet)
{
	return;
}