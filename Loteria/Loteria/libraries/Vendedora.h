#include <iostream>
#include <thread>
#include <chrono>
#include <semaphore>
#include "Cliente.h"
#include "Fila.h"
#include "myLib.h"

#ifndef __VENDEDORA
#define __VENDEDORA

static int columna_vendedora = 35;

class Vendedora
{
private:
	char* nombre;
	bool estaAtendiendo;
	size_t tiempo_atendiendo_ms;
	Cliente* cliente_en_atencion;
	Fila<Cliente>* fila_clientes;
	Fila<Cliente>* clientes_atendidos;

	// Para que se vayan imprimiendo los mensajes en una columna determinada
	int columna_a_presentar;

	// Para que se vayan imprimiendo los mensajes en la fila que corresponda
	int fila_a_presentar;

	Cliente* Se_dirige_a_caja()
	{
		Cliente* cliente_que_sale = new Cliente(false, 0);

		*cliente_que_sale = fila_clientes->Sale();

		std::thread posicionar(gotoxy, columna_a_presentar, fila_a_presentar);
		posicionar.join();
		std::thread imprimir;
		imprimir = std::thread(printString, "Cliente ", NO_FINALIZADA);
		imprimir.join();
		imprimir = std::thread(printNumber, cliente_que_sale->getID(), NO_FINALIZADA);
		imprimir.join();
		imprimir = std::thread(printString, " se dirige a caja...", FINALIZADA);
		imprimir.join();

		std::this_thread::sleep_for(std::chrono::milliseconds(randint(1000, 2000)));

		hayClientes.acquire();

		return cliente_que_sale;
	}

public:
	Vendedora(const char* nombre, Fila<Cliente>* clientes);
	~Vendedora();

	void Atender();
	void asignarTiempoAtendido();
	static void Vender(Vendedora* vendedora);
};

#endif