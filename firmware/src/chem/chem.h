#include <Arduino.h>

class Cchem
{
public:
	void ChemGet(int* NumVal, char* reading);
	void ChemGet(char* reading);

private:
	char input_byte;
	int len;
};