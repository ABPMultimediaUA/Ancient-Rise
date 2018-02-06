#include "../headerfiles/BuscarComida.h"



Status BuscarComida::run(Enemigo *e)
{
    // DATOS DEL ENEMIGO
	enemigoNode = e->getNode();
    core::vector3df EnemigoPosition = enemigoNode->getPosition(); 
    float enemigoX=EnemigoPosition.X;

    // DATOS DE LA COMIDA
    comidaNode = c->getObjeto();
    core::vector3df comidaPosition = comidaNode->getPosition(); 
    float comidaX=comidaPosition.X;

    int distanciaComida = comidaX - enemigoX;  // Calculamos la distancia hasta la fuente

    frameDeltaTime = board->getTime();

    e->setVelocidad(25.f);

    if (distanciaComida<0) // AVANZAMOS HACIA LA IZQUIERDA
     {
        EnemigoPosition.X-= e->getVelocidad() * frameDeltaTime*3;

        e->setPosition(EnemigoPosition); // CAMBIAMOS LA POSICION

        e->setLastFacedDir(false); 
     }
     else{
            if(distanciaComida>0) // AVANZAMOS HACIA LA DERECHA
            {
                EnemigoPosition.X+= e->getVelocidad() * frameDeltaTime*3;

                e->setPosition(EnemigoPosition);

                e->setLastFacedDir(true);    
            }
            else // Si hemos llegado
            {
                 /* RELOJ COMER */
                 this->startClock();                             // INICIAMOS EL RELOJ (O RESEATEAMOS)

                 int time = reloj.getElapsedTime().asSeconds();  // OBTENEMOS SU DURACION EN SEGUNDOS

                 if(time>2)     // COMIENDO
                 {
                     e->setHambre(100.f);       // RECUPERAMOS HAMBRE
                     contador  = 0;
                 }
            }
        }

    return BH_SUCCESS;

}

void BuscarComida::startClock()
{
    if(contador==0)
    {
        reloj.restart();
        contador = contador +1;
    }
}

void BuscarComida::onInitialize(Blackboard *b)
{
   c = b->getComida();
   board = b;
}


BuscarComida::~BuscarComida()
{
    board = nullptr;
    c = nullptr;
    enemigoNode = nullptr;
    comidaNode = nullptr;

    //delete board;
    //delete c;
}