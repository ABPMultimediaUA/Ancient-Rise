#ifndef ENEMIGO_H
#define ENEMIGO_H


#include "../headerfiles/Objeto.h"
#include "../headerfiles/GameObject.h"
#include "../headerfiles/Entorno.h"
#include "../headerfiles/Fachada.h"
#include "../headerfiles/NodoGrafo.h"


#define SCALE 30.0f

class BehaviorTree;
class Entorno;
class Proyectil;
class Blackboard;

class Enemigo : public GameObject
{
    public:
        Enemigo(IrrlichtDevice *dev, ISceneManager* smgr, vector<Posicion*> pos, float xlength, float pendValue, const Entorno* e, Blackboard *b);

        void update(Posicion* prota);
        void updateTiempo(const f32 Time);
        void actualizarHambre();
        void actualizarSed();
        virtual void comprobarEnergia()=0;
        
        /* Vision del Enemigo */
        bool checkInSight(Posicion* pos);
        bool see(GameObject* o);
        bool recordarProta();
        void actualizarVistos();
        void changeLastFaceDir();

        virtual void CreateBox(b2World& world, float X, float Y)=0;

        /* Getters y Setters */
        Posicion* getPosition() const override{ return EnemigoPosition; }
        f32 getVelocidad();
        f32 getSed();
        f32 getSalud();
        f32 getHambre();
        const f32 getVelNormal();
        int getTipo();
        int getClaseEnemigo();

        void* getNode();
        bool getAvistadoProta();
        vector <NodoGrafo*> getPosicion();
        float getXRange();
        float getYPend();
        bool getVisto();
        bool getLastFaceDir();
        b2Body* getBody();
        b2Vec2 getVelocidad2d();
        bool getUltDirecVisto();
        IVideoDriver* getDriver();
        int getOrden();
        bool getDisparo();
        Objeto* getAlarmaActivar();
        bool getSaltando();

        void setSed(f32 se);
        void setEnergia(f32 e);
        void setHambre(f32 h);
        void setSalud(f32 s);
        void setVelocidad(f32 v);
        void setPosition(Posicion* v);
        void setAvistadoProta(bool a);
        void setVelHambre(f32 v);
        void setVelSed(f32 v);
        void setXRange(float xRange);
        void setYPend(float yPend);
        void setLastFacedDir(bool dirx);
        void setUltDirecVisto(bool v);
        void setOrden(int o);
        void setDisparo(bool d);
        void setAlarmaActivar(Objeto *a);
        void setSaltando(bool s);

        /* COMBATE */   
        void setCombate(bool b);
        bool getCombate();
        void setPosCombate(int n);
        int getPosCombate();
        Proyectil* getProyectil();
      

        virtual ~Enemigo();

    protected:
        void * enemigo;
        IVideoDriver*       driver;
        Posicion* EnemigoPosition;            // VECTOR 3D CON LA POSICION DEL ENEMIGO 
        int contadorPatrulla;                       // PARA SABER LA POSICION EN LA QUE SE ENCUENTRA EN LA PATRULLA
        IGUIEnvironment *env;
        bool encontradoComida;                      // PARA SABER SI HA ENCONTRADO COMIDA
        f32 frameDeltaTime;

        BehaviorTree *comportamiento;               // Comportamiento del enemigo definido mediante un arbol de comportamiento (BEHAVIOR TREE)
        Blackboard *board;
        Objeto *alActivar;                          // Para saber que alarma tiene que activar el enemigo

        /* PARA LA VISION */
        bool lastFacedDir;                          // Para saber a que lado esta mirando el enemigo  (True -> Derecha / False -> Izquierda)
        float visionXmax;                           //
        float valorPendiente;                       // Valores para establecer el rango de vision de los enemigos
        bool visto;                                 // Si ha visto al prota o no
        bool direccVistoUlt;                        // Para saber por que lado vio por ultima vez al protagonista (True -> Derecha / False -> Izquierda)     
        vector<GameObject*> vistos;              

        /* ESTADISTICAS DEL ENEMIGO */
        f32 energia;
        f32 sed;
        f32 hambre;
        f32 salud;
        f32 VELOCIDAD_ENEMIGO;                      // VELOCIDAD DEL ENEMIGO
        f32 velHambre;                              // INDICA LA VELOCIDAD A LA QUE BAJA EL HAMBRE
        f32 velSed;                                 // INDICA LA VELOCIDAD A LA QUE BAJA LA SED
        int tipo;                                   // Indica el tipo de enemigo ( 1 = Melee 2 = Distancia) 
        int claseEnemigo;                           // Indica que clase enemigo es (1 = Basico, 2 = Avanzado, 3 = Elite )       
        const f32 VELOCIDAD_NORMAL = 15.f;          // Constante para saber cual es la velocidad normal de los enemigos que no consume energia   
        bool saltando;


         /* BOX2D */
        b2Body* Body;
        b2BodyDef BodyDef;
        b2PolygonShape Shape;
        b2Vec2 velocidad2d;
        short GROUP_ENEMIGOS = -2;                   // PARA EVITAR QUE COLISIONEN OBJETOS DEL MISMO GRUPO

        /* PATHFINDING */
        vector<NodoGrafo*> nodos;                    // CONTIENE TODOS LOS NODOS DEL GRAFO
        vector<NodoGrafo*> patrulla;                 // CONTIENE TODAS LAS POS DE LA PATRULLA DEL ENEMIGO
        Posicion* nodoPosition;

        /* MEMORIA */ 
        sf::Clock reloj;                            // Reloj para controlar el tiempo que tiene que estar huyendo
        int contador;                               // Para reiniciar el reloj cuando toca
        bool memoria;

        /* COMBATE */
        bool combate;                               // PARA SABER SI ESTA COMBATIENDO O NO
        int pos_combate;                            // INDICA LA POSICION DE COMBATE (1 = ARRIBA, 2 = CENTRO, 3 = ABAJO)
        bool ataca=false;                           // PROTA ATACANDO O NO
        bool disparo;                               // Para saber si tiene que disparar un proyectil o no
        Proyectil *proyectil;

         /* ENTORNO, GAMEOBJECTS */
        const Entorno *ent;
        int orden;                                  // Para saber si ha recibido una orden o no el enemigo basico (1--> Ataque, 2 --> Activar Alarma, 3->Explorar, 4->IrALarma)
        

        Fachada* fachada=fachada->getInstance();
};

#endif // ENEMIGO_H
