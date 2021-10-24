#ifndef __FILA
#define __FILA
#include <string.h>
#include <utility>

namespace {
	class FilaVacia
	{
	private:
		char* mensaje;

	public:
		FilaVacia() : mensaje((char*)"Error") {}

		FilaVacia(const char* mensaje)
		{
			this->mensaje = (char*)mensaje;
		}

		char* getMessage() { return this->mensaje; }
	};
}

template<typename T>
class Fila
{
private:
	T* m_Data = nullptr;

	size_t m_Size = 0;

	/*
		Cu�nta memoria hemos reservado realmente.
		No necesariamente memoria reservada realmente en el
		momento, sino cu�nto podr�amos almacenar sin tener
		que reasignar memoria.
		�Por qu� tener esta variable? Resulta que haciendo
		un control de asignaciones de memoria, podremos ser
		capaces de no tener que reasignar memoria siempre cada
		vez que entre un nuevo elemento.
	*/
	size_t m_Capacity = 0;

	/*
		Hay tres cosas, b�sicamente, que debe hacer esta funci�n:
		1. Asignar un nuevo bloque de memoria.
		2. Necesitamos mover todos los elementos viejos al nuevo bloque de memoria.
		3. Borrar el bloque viejo de memoria.
	*/
	void ReAlloc(size_t newCapacity)
	{
		T* newBlock = (T*)::operator new(newCapacity * sizeof(T));

		/*
			Si estamos disminuyendo el tama�o de nuestro vector, entonces
			reasignamos el tama�o m_Size al de newCapacity.
		*/
		if (newCapacity < m_Size)
			m_Size = newCapacity;

		for (size_t i = 0; i < m_Size; i++)
			new (&newBlock[i]) T(std::move(m_Data[i]));

		for (size_t i = 0; i < m_Size; i++)
			m_Data[i].~T();

		::operator delete(m_Data, m_Capacity * sizeof(T));
		m_Data = newBlock;
		m_Capacity = newCapacity;
	}

	void ReAllocDecrement()
	{
		T* newBlock = (T*)::operator new((m_Size - 1) * sizeof(T));

		for (size_t i = 0; i < m_Size - 1; i++)
			new (&newBlock[i]) T(std::move(m_Data[i+1]));

		for (size_t i = 0; i < m_Size; i++)
			m_Data[i].~T();
		
		::operator delete(m_Data, m_Capacity * sizeof(T));
		m_Data = newBlock;
		m_Capacity = --m_Size;
	}

public:
	Fila()
	{
		// Esto lo que har� es reasignar la memoria
		ReAlloc(2);
	}

	Fila(const Fila<T>& other)
	{
		this->m_Size = other.m_Size;
		this->m_Capacity = other.m_Capacity;

		T* newBlock = (T*)::operator new(m_Size * sizeof(T));

		for (size_t i = 0; i < m_Size; i++)
			new (&newBlock[i]) T(std::move(other.m_Data[i]));

		m_Data = newBlock;
	}

	~Fila()
	{
		Clear();
		::operator delete(m_Data, m_Capacity * sizeof(T));
	}

	void Clear()
	{
		for (size_t i = 0; i < m_Size; i++)
			m_Data[i].~T();

		m_Size = 0;
	}

	void Forma(const T& element)
	{
		/*
			El problema aqu� es que, si no hay espacio para
			la asignaci�n del nuevo valor, deber�amos
			reasignar memoria.
		*/
		if (m_Size >= m_Capacity)
			ReAlloc(m_Capacity + m_Capacity / 2);

		m_Data[m_Size++] = element;
	}

	void Forma(T&& element)
	{
		if (m_Size >= m_Capacity)
			ReAlloc(m_Capacity + m_Capacity / 2);

		m_Data[m_Size++] = std::move(element);
	}

	T Sale()
	{
		T primerElementoEnFila(*m_Data);

		if (m_Size > 0)
		{
			ReAllocDecrement();
		}
		else
			throw FilaVacia();

		return primerElementoEnFila;
	}

	T& PrimeroEnFila()
	{
		return *m_Data;
	}

	size_t Size() const { return m_Size; }
};

#endif