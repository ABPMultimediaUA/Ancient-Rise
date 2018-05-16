#include "../headerfiles/Palanca.h"



Palanca::Palanca(Posicion* pos):Objeto(pos)
{
	GameObject::setTipo(PALANCA);
	fachada=fachada->getInstance();

	objeto = fachada->addMalla(pos->getPosX(),pos->getPosY(),pos->getPosZ(), "resources/Palanca/Palanca1.obj");
	if (objeto)
	{
        Posicion escala(0.3f,0.3f,0.3f);
		fachada->setScala(objeto,&escala);

	}

}

void Palanca::update()
{
	

}


void* Palanca::getNode()
{
    return objeto;
}

Palanca::~Palanca()
{
	fachada->destruirObjeto(objeto);
}