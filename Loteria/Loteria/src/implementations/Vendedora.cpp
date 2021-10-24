#include "Vendedora.h"
#include "Cliente.h"
#include "myLib.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <chrono>
#include <thread>

Vendedora::Vendedora(const char* nombre, Fila<Cliente>* clientes)
{
	this->nombre = (char*)nombre;
	this->fila_clientes = clientes;
	this->clientes_atendidos = new Fila<Cliente>();
	this->estaAtendiendo = false;

	this->cliente_en_atencion = nullptr;
	this->tiempo_atendiendo_ms = 0;

	fila_a_presentar = 2;
	columna_a_presentar = columna_vendedora;
	columna_vendedora += 35;

	std::thread posicionar(gotoxy, columna_a_presentar, 0);
	posicionar.join();
	std::thread imprimir;
	imprimir = std::thread(printString, "Vendedora ", false);
	imprimir.join();
	imprimir = std::thread(printString, nombre, true);
	imprimir.join();
}

Vendedora::~Vendedora()
{
	columna_vendedora -= 35;

	delete fila_clientes;
	delete clientes_atendidos;

	delete nombre;
}

void Vendedora::Atender()
{
	using namespace std::this_thread;
	using namespace std::chrono;

	std::thread posicionar(gotoxy, columna_a_presentar, fila_a_presentar);
	posicionar.join();
	std::thread imprimir;
	imprimir = std::thread(printString, nombre, NO_FINALIZADA);
	imprimir.join();
	imprimir = std::thread(printString, ": \"atendiendo al cliente ", NO_FINALIZADA);
	imprimir.join();
	imprimir = std::thread(printNumber, this->cliente_en_atencion->getID(), NO_FINALIZADA);
	imprimir.join();
	imprimir = std::thread(printString, "\" ", FINALIZADA);
	imprimir.join();

	this->asignarTiempoAtendido();

	sleep_for(microseconds(this->tiempo_atendiendo_ms));

	posicionar = std::thread(gotoxy, columna_a_presentar, fila_a_presentar);
	posicionar.join();
	imprimir = std::thread(printString, nombre, NO_FINALIZADA);
	imprimir.join();
	imprimir = std::thread(printString, ": \"cliente ", NO_FINALIZADA);
	imprimir.join();
	imprimir = std::thread(printNumber, this->cliente_en_atencion->getID(), NO_FINALIZADA);
	imprimir.join();
	imprimir = std::thread(printString, " atendido\"       ", FINALIZADA);
	imprimir.join();

	fila_a_presentar++;

	this->clientes_atendidos->Forma(*cliente_en_atencion);

	hayClientes.release();
}

void Vendedora::asignarTiempoAtendido()
{
	this->tiempo_atendiendo_ms = randint(2000000, 7000000);
}

void Vendedora::Vender(Vendedora* vendedora)
{
	while (1)
	{
		if (vendedora->fila_clientes != NULL)
		{
			if (vendedora->fila_clientes->Size() != 0)
			{
				if (strcmp(vendedora->nombre, "A") == 0)
					esta_A_ocupada = true;

				if (strcmp(vendedora->nombre, "B") == 0 && esta_A_ocupada)
					caja_A_ocupada.acquire();

				std::this_thread::sleep_for(std::chrono::milliseconds(randint(1000, 1500)));

				std::thread posicionar(gotoxy, vendedora->columna_a_presentar, vendedora->fila_a_presentar);
				posicionar.join();
				std::thread imprimir(printString, vendedora->nombre, FINALIZADA);
				imprimir.join();

				Cliente* cliente_tentativo = vendedora->Se_dirige_a_caja();
				vendedora->cliente_en_atencion = cliente_tentativo;
				vendedora->estaAtendiendo = true;

				if (strcmp(vendedora->nombre, "A") == 0)
				{
					caja_A_ocupada.release();
					esta_A_ocupada = false;
				}

				vendedora->Atender();

				vendedora->estaAtendiendo = false;
			}
		}
	}
}