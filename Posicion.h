#ifndef POSICION_H
#define POSICION_H


class Posicion
{
    public:
        Posicion(float x, float y, float z);
        virtual ~Posicion();

        float getPosX();
        float getPosY();
        float getPosZ();

    private:

        float posX; // VALOR DE LA POSICION X
        float posY; // VALOR DE LA POSICION Y
        float posZ;
};

#endif // POSICION_H
