#ifndef __CLIENTE
#define __CLIENTE

class Cliente
{
private:
	int ID = 0;
public:
	Cliente() = delete;

	Cliente(bool generarID, int ID = 0)
	{
		if (generarID)
		{
			static int ID_tentativo = 0;

			this->ID = ++ID_tentativo;
		}
		else
			this->ID = ID;
	}
	Cliente(const Cliente& other)
	{
		this->ID = other.ID;
	}

	inline int getID() const { return ID; }
};

#endif