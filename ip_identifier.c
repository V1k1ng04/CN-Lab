#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void determineClass(char *ipAddress)
{
	int firstOctet;
	sscanf(ipAddress, "%d", &firstOctet);

	if (firstOctet >= 1 && firstOctet <= 126)
		printf("Class A\n");
	else if (firstOctet >= 128 && firstOctet <= 191)
		printf("Class B\n");
	else if (firstOctet >= 192 && firstOctet <= 223)
		printf("Class C\n");
	else if (firstOctet >= 224 && firstOctet <= 239)
		printf("Class D\n");
	else if (firstOctet >= 240 && firstOctet <= 254)
		printf("Class E\n");
	else
		printf("ERROR | Invalid IP Address\n");
}

void main()
{
	char ipAddress[16];

	printf("Enter an IP Address: ");
	scanf("%15s", ipAddress);

	determineClass(ipAddress);
}