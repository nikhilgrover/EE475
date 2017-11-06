#include <stdio.h>
#include <stdint.h>

struct packet
{
	float Temperature;
	float CO2;
	float Salinity;
	float FlowRate;
};

float ConvertToUInt32(char bytes[], char index);
struct packet ParsePacket(char bytes[]);

//example of what a packet will look like from the local system. 
//values are Temperature=0, CO2=100, Salinity=100, FlowRate=300
//char buffer[16] = { 0, 0, 0, 0, 0x64, 0, 0, 0, 0x64, 0, 0, 0, 0x2c, 0x01, 0, 0 };

/*
int main()
{

	struct packet d;
	d = ParsePacket(buffer);

	printf("d.Temperature = %f\n", d.Temperature);
	printf("d.CO2 = %f\n", d.CO2);
	printf("d.Salinity = %f\n", d.Salinity);
	printf("d.FlowRate = %f\n", d.FlowRate);

	getchar();
	return 0;
}
*/
float ConvertToUInt32(char bytes[], char index)
{
	float value = (float)(bytes[index] | (bytes[index + 1] << 8) | (bytes[index + 2] << 16) | (bytes[index + 3] << 24));
	return value / 100;
}

struct packet ParsePacket(char bytes[])
{
	struct packet p;
	p.Temperature = (float)ConvertToUInt32(bytes, 0);
	p.CO2 = (float)ConvertToUInt32(bytes, 4);
	p.Salinity = (float)ConvertToUInt32(bytes, 8);
	p.FlowRate = (float)ConvertToUInt32(bytes, 12);

	return p;
}


struct packet ParsePacket(char bytes[])
{
	struct packet p;
	p.Temperature = (float)ConvertToUInt32(bytes, 0);
	p.CO2 = (float)ConvertToUInt32(bytes, 4);
	p.Salinity = (float)ConvertToUInt32(bytes, 8);
	p.FlowRate = (float)ConvertToUInt32(bytes, 12);

	return p;
}

