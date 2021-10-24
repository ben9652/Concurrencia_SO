#include <iostream>
#include <semaphore>

#ifndef __MYLIB
#define __MYLIB

#define FINALIZADA true
#define NO_FINALIZADA false

static bool esta_A_ocupada = false;

static std::counting_semaphore hayClientes(2);
static std::binary_semaphore caja_A_ocupada(0);
static std::binary_semaphore positionLock(1);
static std::binary_semaphore printLock(1);

int randint(int lower, int upper);

void gotoxy(short x, short y);
void printString(const char* string, bool finalize);
void printNumber(size_t number, bool finalize);

#endif