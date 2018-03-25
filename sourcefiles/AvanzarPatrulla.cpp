#include "../headerfiles/AvanzarPatrulla.h"



Status AvanzarPatrulla::run(Enemigo *e)
{
    protaPosition = board->getProta();

    pos = e->getPosicion();

    float enemigoX=e->getPosition()->getPosX();

    float posPatrullaX = pos[contadorPatrulla]->getPosX();

    int distanciaNodoX= posPatrullaX - enemigoX;     // DISTANCIA EN X AL NODO DE LA PATRULLA

    e->setCombate(false);

    if(distanciaNodoX==0) // SI ESTAMOS EN UNO DE LOS NODOS DE LA PATRULLA BUSCAMOS EL SIGUIENTE NODO
    {
        if(contadorPatrulla==pos.size()-1) // Si llegamos al final reiniciamos
        {
            contadorPatrulla = 0;
                    
        }
        else {
                contadorPatrulla++;
            }
    }
    else{  // AUN NO HEMOS LLEGADO A NINGUN NODO DE LA PATRULLA

            if (distanciaNodoX<0) // AVANZAMOS HACIA LA IZQUIERDA
            {
                e->getBody()->SetLinearVelocity(-(e->getVelocidad2d()));    // Velocidad Normal

                e->setLastFacedDir(false);                                    // INDICAMOS QUE EL ENEMIGO ESTA MIRANDO A LA IZQUIERDA                           

            }
            else{
                if(distanciaNodoX>0) // AVANZAMOS HACIA LA DERECHA
                {
                    e->getBody()->SetLinearVelocity(e->getVelocidad2d());
                    
                    e->setLastFacedDir(true);                                // INDICAMOS QUE EL ENEMIGO ESTA MIRANDO A LA DERECHA  
                }
            }
         }
         
   
   return BH_SUCCESS;
   

}

void AvanzarPatrulla::startClock()
{
    if(contadorReloj==0)
    {
        reloj.restart();
        contadorReloj = contadorReloj +1;
    }
}

void AvanzarPatrulla::onInitialize(Blackboard *b)
{
    contadorPatrulla=0;
    contadorReloj = 0;
    board = b;

    for(int cont=0; cont<pos.size();cont++)
    {
        pos[cont] = nullptr;
    }
}


AvanzarPatrulla::~AvanzarPatrulla()
{
    board  = nullptr;

    for(int cont=0; cont<pos.size();cont++)
    {
        pos[cont] = nullptr;
    }

    pos.clear();

}