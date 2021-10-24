#include <iostream>
#include <thread>
#include <semaphore>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include "Fila.h"
#include "Cliente.h"
#include "Vendedora.h"
#include "myLib.h"

void mostrarClientes(Fila<Cliente>* fila);
void llegadaClientes(Fila<Cliente>* fila);

int main()
{
	system("mode 650");
	system("cls");

	Fila<Cliente>* fila = new Fila<Cliente>();
	fila->Forma(Cliente(true));
	fila->Forma(Cliente(true));
	fila->Forma(Cliente(true));
	fila->Forma(Cliente(true));
	fila->Forma(Cliente(true));

	std::thread mostrar_clientes(mostrarClientes, fila);
	std::thread llegada_clientes(llegadaClientes, fila);

	Vendedora* A = new Vendedora("A", fila);
	Vendedora* B = new Vendedora("B", fila);

	std::thread caja_A(Vendedora::Vender, A);
	std::thread caja_B(Vendedora::Vender, B);

	llegada_clientes.join();
	mostrar_clientes.join();
	caja_A.join();
	caja_B.join();

	return 0;
}

static void mostrarClientes(Fila<Cliente>* fila)
{
	size_t largoAnterior = 0, largoActual = 0;
	while (1)
	{
		size_t largoActual = fila->Size();
		if (largoActual != largoAnterior)
		{
			std::thread posicionar, imprimir;

			posicionar = std::thread(gotoxy, 0, 0);
			posicionar.join();

			imprimir = std::thread(printString, "Tama\xa4o fila: ", NO_FINALIZADA);
			imprimir.join();
			imprimir = std::thread(printNumber, fila->Size(), NO_FINALIZADA);
			imprimir.join();
			imprimir = std::thread(printString, "   ", FINALIZADA);
			imprimir.join();

			posicionar = std::thread(gotoxy, 0, 3);
			posicionar.join();

			if (largoActual != 0)
			{
				imprimir = std::thread(printString, "Cliente primero en cola: ", NO_FINALIZADA);
				imprimir.join();
				imprimir = std::thread(printNumber, fila->PrimeroEnFila().getID(), FINALIZADA);
				imprimir.join();
			}
			else
			{
				imprimir = std::thread(printString, "No hay clientes en cola.    ", FINALIZADA);
				imprimir.join();
			}

			largoAnterior = largoActual;
		}
	}
}

void llegadaClientes(Fila<Cliente>* fila)
{
	using namespace std::this_thread;
	using namespace std::chrono;

	while (1)
	{
		sleep_for(milliseconds(randint(1000, 5000)));
		Cliente cliente(true);
		fila->Forma(cliente);
	}
}