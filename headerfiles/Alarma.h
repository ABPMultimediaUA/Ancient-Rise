#ifndef ALARMA_H
#define ALARMA_H


#include "../headerfiles/Objeto.h"


class Alarma : public Objeto
{

	public:
		Alarma(Posicion* pos);
		void update();
		void checkActivada();
		void startClock();
		virtual ~Alarma();



	private:

		sf::Clock reloj; 		  			// Reloj para controlar el tiempo que tiene que estar activada la alarma
	    int contador; 			  			// Para reiniciar el reloj cuando toca
};

#endif
