#include "myLib.h"
#include <iostream>
#include <time.h>
#include <Windows.h>
#include <random>

int randint(int lower, int upper)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(lower, upper);

	return distr(gen);
}

void gotoxy(short x, short y)
{
	positionLock.acquire();
	printLock.acquire();

	COORD coords;
	
	coords.X = x;
	coords.Y = y;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coords);
}

void printString(const char* string, bool finalize)
{
	std::cout << string;

	if (finalize)
	{
		positionLock.release();
		printLock.release();
	}
}

void printNumber(size_t number, bool finalize)
{
	std::cout << number;

	if (finalize)
	{
		positionLock.release();
		printLock.release();
	}
}