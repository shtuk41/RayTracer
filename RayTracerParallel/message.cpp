#include "stdafx.h"
#include <iostream>
#include "openclfunctions.h"

void message(char *message, char *fileName, int lineNumber)
{
	std::cout << message << std::endl;
}

void message_pause(char *message, char *fileName, int lineNumber)
{
	std::cout << message << std::endl;
	system("pause");
}

void message(char *message)
{
	std::cout << message << std::endl;
}	

void message_pause(char *message)
{
	std::cout << message << std::endl;
	system("pause");
}