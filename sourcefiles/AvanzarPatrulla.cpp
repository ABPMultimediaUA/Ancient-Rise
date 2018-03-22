#include "../headerfiles/AvanzarPatrulla.h"


Status AvanzarPatrulla::run(Enemigo *e)
{
    protaPosition = board->getProta();

    pos = e->getPosicion();

    Posicion* EnemigoPosition = e->getPosition(); // VOLVEMOS A OBTENER EL VECTOR DE POSICION DEL ENEMIGO POR SI HA CAMBIADO
    float enemigoX=EnemigoPosition->getPosX();
    float enemigoY = EnemigoPosition->getPosY();

    nodoPosition = pos[contadorPatrulla]->getPosition();
    float posPatrullaX = nodoPosition->getPosX();

    int distanciaNodoX= posPatrullaX - enemigoX;     // DISTANCIA EN X AL NODO DE LA PATRULLA

    e->setCombate(false);

    /* Calculamos el camino mas corto entre el nodo Inicial y el nodo Final */
    //cout<<pos[0]->getPosition()->getPosX()<<endl;
    //cout<<pos[1]->getPosition()->getPosX()<<endl;
    if(caminoCortoIda.size()==0)           // Para calcular el camino solo 1 vez y no siempre
    {   
        g = new Grafo();
        caminoCortoIda = g->pathfindDijkstra(pos[0], pos[1]);
    }

    /* Realizamos el recorrido a lo largo del camino corto calculado */
    if(ida == true)
    {
        if(contador<caminoCortoIda.size())
        {
            fin = caminoCortoIda[contador]->getNodoFin();

            if(caminoCortoIda[contador]->getComportamiento()==NORMAL)         // Movimiento normal del enemigo
            {   
                posNodoI = fin->getPosition();
                float distNodoF = posNodoI->getPosX() - enemigoX;

                if (distNodoF<-1.0f) // AVANZAMOS HACIA LA IZQUIERDA
                    {

                        e->getBody()->SetLinearVelocity(-(e->getVelocidad2d()));               // Velocidad Normal

                        e->setLastFacedDir(false);                                    
                    }
                else{
                        if(distNodoF>1.0f) // AVANZAMOS HACIA LA DERECHA
                        {

                            e->getBody()->SetLinearVelocity(e->getVelocidad2d());

                            e->setLastFacedDir(true);                                    
                        }
                        else // Si hemos llegado al nodo Fin
                        {
                            contador++;
                        }
                    }

            }
            else
            {
                if(caminoCortoIda[contador]->getComportamiento()==SALTO)         // SALTO
                {   
                    posNodoI = fin->getPosition();
                    float distNodoF = posNodoI->getPosX() - enemigoX;
                    float distNodoFY = posNodoI->getPosY() - enemigoY;
        
                    if(distNodoFY>1.0f)
                    {
                        e->setSaltando(true);
                        e->getBody()->ApplyForceToCenter(b2Vec2(0.f,3000.f),true);
                    }
                    else
                    {
                        e->setSaltando(false);
                    }

                    if(e->getSaltando()!=true)
                    {
                        if (distNodoF<-1.0f) // AVANZAMOS HACIA LA IZQUIERDA
                            {

                                e->getBody()->SetLinearVelocity(-(e->getVelocidad2d()));               // Velocidad Normal
                                e->setLastFacedDir(false);                                    
                            }
                        else{
                                if(distNodoF>1.0f) // AVANZAMOS HACIA LA DERECHA
                                {

                                    e->getBody()->SetLinearVelocity(e->getVelocidad2d());

                                    e->setLastFacedDir(true);                                    
                                }
                                else // Si hemos llegado al nodo Fin
                                {
                                    contador++;
                                }
                            }
                    }

                }
            }
        }

        if(contador==caminoCortoIda.size())
        {
            ida = false;
            contador = caminoCortoIda.size()-1;
        }
        
    }
    else
    {
        if(contador>=0)
        {
            if(caminoCortoVuelta.size()==0)        // Calculamos el camino mas corto para volver
            {
                g = new Grafo();
                caminoCortoVuelta = g->pathfindDijkstra(pos[1], pos[0]);
            }
    
            fin = caminoCortoVuelta[contador]->getNodoFin();
        
            if(caminoCortoVuelta[contador]->getComportamiento()==NORMAL)         // Movimiento normal del enemigo
            {   
                posNodoI = fin->getPosition();
                float distNodoF = posNodoI->getPosX() - enemigoX;

                if (distNodoF<-1.0f) // AVANZAMOS HACIA LA IZQUIERDA
                    {

                        e->getBody()->SetLinearVelocity(-(e->getVelocidad2d()));               // Velocidad Normal

                        e->setLastFacedDir(false);                                    
                    }
                else{
                        if(distNodoF>1.0f) // AVANZAMOS HACIA LA DERECHA
                        {

                            e->getBody()->SetLinearVelocity(e->getVelocidad2d());

                            e->setLastFacedDir(true);                                    
                        }
                        else // Si hemos llegado al nodo Fin
                        {
                            contador--;
                        }
                    }

            }
        }

        if(contador==0)
        {
            ida = true;
            contador = 0;
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
/* FUNCION PARA RECORRER LOS ARISTAS DEL NODO EN EL QUE SE ENCUENTRA EL ENEMIGO
Y OBTENER EL COMPORTAMIENTO DE LA ARISTA CORRESPONDIENTE */
void AvanzarPatrulla::recorrerAristas(int num)
{
    for(int i=0;i<aristSal.size();i++)
    {
        if(aristSal[i]->getNodoInicio()==pos[contadorPatrulla] && aristSal[i]->getNodoFin()==pos[num])  // Buscamos la arista correspondiente al camino que esta recorriendo el enemigo
        {
            tipoComp = aristSal[i]->getComportamiento();
            cout<<"entr"<<endl;
        }
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

    for(int cont=0; cont<aristSal.size();cont++)
    {
        aristSal[cont] = nullptr;
    }

    g = new Grafo();
    contador = 0;
    posNodoI = nullptr;
    fin = nullptr;
}


AvanzarPatrulla::~AvanzarPatrulla()
{
   board  = nullptr;

    for(int cont=0; cont<pos.size();cont++)
    {
        pos[cont] = nullptr;
    }
    pos.clear();

    for(int cont=0; cont<aristSal.size();cont++)
    {
        aristSal[cont] = nullptr;
    }
    aristSal.clear();

    delete g;

}
