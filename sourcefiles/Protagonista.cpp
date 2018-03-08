#include "../headerfiles/Protagonista.h"
#define SCALE 30.0f

static Protagonista* instance = NULL;


Protagonista::Protagonista()
{
    
    GameObject::setTipo(PROTA);

    /**
    Creamos un nodo que va ser movido con las teclas WSAD. Es una esfera que posicionamos
    en (0,0,30) y le asignamos una texura. Como no tenemos luces dinamicas en esta escena
    desabilitamos la luz en cada modelo (sino los modelos serian negros )
    **/ 

    rec=fachada->addSphere(0,0,30,false);
    energy=fachada->addCube(0,0,30,true);
    life=fachada->addCube(0,0,30,false);

    
    protaPosition=fachada->getPosicion(rec);
    
    energyScale=fachada->getScala(energy);
    energyScale->setPosZ(0.1f);
   

    
    lifeScale=fachada->getScala(life);
    lifeScale->setPosZ(0.1f);
   
    energyPosition=new Posicion(0.f,0.f,0.f);
    lifePosition=new Posicion(0.f,0.f,0.f);
    //trampaPosition=new Posicion(0.f,0.f,0.f);
    
    fachada->setScala(energy,energyScale);
    fachada->setScala(life,lifeScale);

    combate = false;
    pos_combate = 2; 

    sonido = GestorSonido::getInstance();

    nani = sonido->create2DSound(sonido->SOUND_BOSS3_NANI);
    omae = sonido->create2DSound(sonido->SOUND_BOSS3_OMAE);
    grito = sonido->create2DSound(sonido->SOUND_BOSS3_GRITO1);
    risa = sonido->create3DSound(sonido->SOUND_BOSS3_RISA);

}



//Con esto hacemos que sea singleton. Solo se puede crear el motorgrafico llamando a getInstance. Esta devuelve el motor si ya estaba creado, y sino lo crea
//Parametros: h-> Alto de la ventana, w-> Ancho de la ventana, fullscreen-> si será pantalla completa o no
Protagonista* Protagonista::getInstance() {
    if (instance == NULL) instance = new Protagonista();
    return (instance);
}

/**
FUNCION PARA crear el objeto dinamico
**/
void Protagonista::CreateBox(b2World& world, float X, float Y)
{

    BodyDef.position = b2Vec2(X/SCALE, Y/SCALE);
    BodyDef.type = b2_dynamicBody;
    Body = world.CreateBody(&BodyDef);
    Shape.SetAsBox((20.f/2)/SCALE, (20.f/2)/SCALE);
    b2FixtureDef FixtureDef;
    FixtureDef.density = 1.2f;
    FixtureDef.friction = 0.35f;
    FixtureDef.shape = &Shape;
    FixtureDef.isSensor = false;
    FixtureDef.filter.groupIndex = GROUP_PLAYER;
    Body->CreateFixture(&FixtureDef);
    Body->SetUserData( rec );
    //std::cout<<Body->GetMass()<<"\n";

  
}
/**
FUNCION PARA crear el objeto estatico
**/
void Protagonista::CreateGround(b2World& world, float X, float Y,int largo)
{
    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(X/SCALE, Y/SCALE);
    BodyDef.type = b2_staticBody;
    b2Body* Ground = world.CreateBody(&BodyDef);
    b2PolygonShape Shape;
    Shape.SetAsBox((largo/2)/SCALE, (300.f/2)/SCALE);
    b2FixtureDef FixtureDef;
    FixtureDef.density = 0.f;
    FixtureDef.friction = 0.65f;
    FixtureDef.shape = &Shape;
    Ground->CreateFixture(&FixtureDef);

}
/**
FUNCION PARA actualizar el cuerpo
**/
void Protagonista::updateBody(b2World& world)
{
    
    
    protaPosition->setPosX(Body->GetPosition().x);
    protaPosition->setPosY(Body->GetPosition().y);
    
    fachada->setPosicion(rec,protaPosition);


}
/**
FUNCION PARA DIBUJAR LA INTERFAZ
**/
void Protagonista::pintarInterfaz()
{
    
     //barra para mostrar la enegia
    float energyPositionX=protaPosition->getPosX()- 110;
    float energyPositionY=protaPosition->getPosY()+ 100;
    float energyPositionZ=protaPosition->getPosZ()- 30;
    energyPosition->setPosX(energyPositionX);
    energyPosition->setPosY(energyPositionY);
    energyPosition->setPosZ(energyPositionZ);
    fachada->setPosicion(energy,energyPosition);
    
    
    float lifePositionX=energyPositionX;
    float lifePositionY=energyPositionY+ 10;
    float lifePositionZ=energyPositionZ;
    
    lifePosition->setPosX(lifePositionX);
    lifePosition->setPosY(lifePositionY);
    lifePosition->setPosZ(lifePositionZ);

    fachada->setPosicion(life,lifePosition);

    energyScale->setPosX(energia/10);
    fachada->setScala(energy,energyScale);
    lifeScale->setPosX(vida/10);
    fachada->setScala(life,lifeScale);
}

/**
FUNCION PARA CONTROLAR EL ATAQUE DEL PROTA
**/
void Protagonista::ataque(const f32 Time)
{

    b2Vec2 pos=Body->GetPosition();

    if(ataca == true && cont_ataque<20){
        energia-=0.5f;
        if(ataque_position!=0){
            Body->SetTransform(b2Vec2(pos.x,pos.y+(ataque_position+2)/3), 0.f);
        }else
            Body->SetTransform(b2Vec2(pos.x,pos.y-0.8f), 0.f);
        if(direccion==1)
        {
            Body->ApplyForceToCenter(b2Vec2(100.f,0.f),true);
            
        }else if(direccion==0){
            Body->ApplyForceToCenter(b2Vec2(-100.f,0.f),true);
            
        } 
        
        cont_ataque++;  
    }
    else if(cont_ataque>=20){
        cont_ataque=0;
        ataca=false;
    }
    
}

/**
FUNCION PARA CONTROLAR EL MOVIMIENTO DEL PROTA
**/

void Protagonista::movimiento(const f32 Time)
{
    b2Vec2 velo=Body->GetLinearVelocity();
    if(direccion==0) // MOVIMIENTO HACIA LA IZQUIERDA
    {

        if(sigilo==true)
        {
            velo.x=-10.f;
            //Body->ApplyForceToCenter(b2Vec2(-35.f,0.f),true);
            Body->SetLinearVelocity(velo);
            //protaPosition.X -= VELOCIDAD_MOVIMIENTO * Time*0.5;
        }else if(correr==true && energia>10.1)
        {
            velo.x=-90.f;
            Body->ApplyForceToCenter(b2Vec2(-150.f,0.f),true);
             //Body->SetLinearVelocity(velo);
            //protaPosition.X -= VELOCIDAD_MOVIMIENTO * Time*3;

            if(energia<10)
                correr=false;
        }else
        {
            velo.x=-40.f;
            //Body->ApplyForceToCenter(b2Vec2(-60.f,0.f),true);
            Body->SetLinearVelocity(velo);
            //protaPosition.X -= VELOCIDAD_MOVIMIENTO * Time*1.5;
        }

    }
    else        //MOVIMIENTO HACIA LA DERECHA
    {
         if(sigilo==true)
            {
                velo.x=10.f;
                //Body->ApplyForceToCenter(b2Vec2(35.f,0.f),true);
               Body->SetLinearVelocity(velo);
            }else if(correr==true && energia>10.1){
                velo.x=90.f;
                Body->ApplyForceToCenter(b2Vec2(150.f,0.f),true);
                //Body->SetLinearVelocity(velo);
                if(energia<10)
                    correr=false;
            }else{
                velo.x=40.f;
                //Body->ApplyForceToCenter(b2Vec2(60.f,0.f),true);
                Body->SetLinearVelocity(velo);
            }
                
    }  

}
/**
FUNCION PARA COMPROBAR LAS COLISIONES CON ENEMIGOS
**/
void Protagonista::comprobarColision(Enemigo *enemigo)
{
    enemigoPosition=enemigo->getPosition();
    float enemigoPosX=enemigoPosition->getPosX();
    float enemigoPosY=enemigoPosition->getPosY();
    
    float protaPosX=protaPosition->getPosX();
    float protaPosY=protaPosition->getPosY();

    if((enemigoPosX-(protaPosX+10))<0 
        && (enemigoPosX-(protaPosX+10))>-20
        && vida<=100 && vida>0 && protaPosY<10){

        if(enemigo->getPosCombate() != pos_combate)
        {
            vida-=5; 
        }
    }

}

/**
FUNCION PARA COMPROBAR LAS COLISIONES CON COMIDA
**/

void Protagonista::comprobarColision(Comida *comida)
{
    float protaPosX=protaPosition->getPosX();
    float protaPosY=protaPosition->getPosY();
    
    comidaPosition=comida->getPosition();
    float comidaPosX=comidaPosition->getPosX();
    float comidaPosY=comidaPosition->getPosY();
    
    
    //std::cout<<protaPosX<<endl;
    if((comidaPosX-(protaPosX+10))<-5 
        && (comidaPosX-(protaPosX+10))>-15){
        if(/*comida->getNode()->isVisible()&&*/ protaPosY<10)
        {
            //std::cout<<comidaPosX<<endl;
           vida+=10;
            if(vida>100)
                vida=100;
            
            comidaPosX+=500;
        if(comidaPosX>2500)
            comidaPosX=-1900;
            //comida->getNode()->setPosition(comidaPosition);

        }
       
    }
    //else
        //comida->getNode()->setVisible(true);
    
}

void Protagonista::comprobarColision(Bebida *bebida)
{
    float protaPosX=protaPosition->getPosX();
    float protaPosY=protaPosition->getPosY();
    
    bebidaPosition=bebida->getPosition();
    float bebidaPosX=bebidaPosition->getPosX();
    float bebidaPosY=bebidaPosition->getPosY();
    
    if((bebidaPosX-(protaPosX+10))<=-5 
        && (bebidaPosX-(protaPosX+10))>-15){
        if(/*bebida->getNode()->isVisible()&&*/ protaPosY<10)
        {
            std::cout<<bebidaPosX<<endl;
           energia+=10;
            if(energia>100)
                energia=100;

            //bebida->getNode()->setVisible(false);
            
            bebidaPosX+=400;
        if(bebidaPosX>2200)
            bebidaPosX=-1800;
            //bebida->getNode()->setPosition(bebidaPosition);

        }
       
    }
    //else
        //bebida->getNode()->setVisible(true);
    
}

void Protagonista::comprobarColision(Trampa *trampa)
{
    float protaPosX=protaPosition->getPosX();
    float protaPosY=protaPosition->getPosY();
    
    trampaPosition=trampa->getPosition();
    float tramPosX=trampaPosition->getPosX();
    float tramPosY=trampaPosition->getPosY();
    
    if((tramPosX-(protaPosX+10))<8 
        && (tramPosX-(protaPosX+10))>-28
        && protaPosY<10){
        
           vida-=0.4f;
           //protaPosition.X-=15; //+=15 animacion, rebote de la trampa 
       
    }
    
    
}
/*
FUNCION PARA COMPROBAR LA POSICION DE COMBATE DEL PROTA Y CAMBIAR LA POS EN Y 
DEL PROTA 
*/
void Protagonista::checkPosCombate()
{
    
    if(pos_combate == 1)    // ARRIBA
    {
        protaPosition->setPosY(10.f); 
        fachada->setPosicion(rec,protaPosition);
    }
    else
    {
        if(pos_combate == 3) // ABAJO
        {
            protaPosition->setPosY(0.f); 
            fachada->setPosicion(rec,protaPosition);
        }
        else        // CENTRO
        {
            protaPosition->setPosY(5.f); 
            fachada->setPosicion(rec,protaPosition);
        }
    }
  
}

/**
FUNCION PARA COMPROBAR LA VIDA DEL PROTA
**/
bool Protagonista::checkVida()
{
    if(vida<=0)
    {
        return false;
    }else
    {
        return true;
    }
}

/*
FUNCION PARA CAMBIAR LA POS DE COMBATE DEL PROTA
*/
void Protagonista::setPosCombate(int n)
{
    pos_combate = n;
    //cout<<pos_combate<<endl;
}
/**
FUNCION PARA RECUPERAR LA VIDA DEL PROTA
**/
void Protagonista::setVida(f32 cantidad,const f32 Time)
{
    if(vida<100)
        vida+=cantidad* Time;
    if(vida<0){
        vida=0;
    }else if(vida>100){
        vida=100;
    }

}
/**
METODO PARA GESTIONAR LA ENERGIA
**/
void Protagonista::setEnergia(f32 cantidad,const f32 Time)
{
    if(energia>0 || energia<100)
        energia+=cantidad* Time;
    if(energia<0){
        energia=0;
        //setVida(-5,Time);
    }else if(energia>100)
        energia=100;

}
/**
METODO PARA GESTIONAR EL SALTO
**/
void Protagonista::setSalto(bool s)
{
    bool flag;
    //sonido->playSound(risa);
    b2Vec2 velocidad=Body->GetLinearVelocity();
    //std::cout<<velocidad.y<<"\n";
    if(velocidad.y>=-5 && velocidad.y<5 && s && !saltando && !sigilo){
        if(correr && energia>10)
        {   
            flag = sonido->playSound(omae);
            if(flag){
                DSP* dsp = sonido->createDSP("echo");
                omae->getCanal()->addDSP(dsp);
                omae->getCanal()->setGrupoCanales(sonido->getGrupoVoces());
            }
            Body->ApplyForceToCenter(b2Vec2(0.f,10000.f),true);
        }else if(energia<10)
        {
            sonido->playSound(grito);
            grito->getCanal()->setGrupoCanales(sonido->getGrupoVoces());
            Body->ApplyForceToCenter(b2Vec2(0.f,2500.f),true);
        }
        else{
            flag = sonido->playSound(nani);
            if(flag){
                nani->getCanal()->setGrupoCanales(sonido->getGrupoVoces());
                DSP* dsp = sonido->createDSP("echo");
                nani->getCanal()->addDSP(dsp);
            }
            Body->ApplyForceToCenter(b2Vec2(0.f,6000.f),true);    
        }
        //cont_salto=1;
        //saltando=s;
        setEnergia(1.f,-15);
    }
    saltando=s;
}

/**
ACTUALIZA LA POSICION DEL PROTA
**/
void Protagonista::setPosition(Posicion* v)
{
   protaPosition=v;
}

/*
FUNCION PARA ACTIVAR O DESACTIVAR EL SIGILO
*/
void Protagonista::setSigilo()
{
    if(sigilo==false)
    {
        sigilo=true;
        //rec->setMaterialFlag(video::EMF_LIGHTING, true);
    }
    else{
        //rec->setMaterialFlag(video::EMF_LIGHTING, false);
        sigilo=false;
    }
}

void Protagonista::setCorrer(bool s)
{
    correr=s;
}

void Protagonista::setDireccion(int d)
{
    direccion=d;
}

void Protagonista::setAtaque(bool d)
{

    ataca = d;
    if(ataca == true)
    {
        if(cont_ataque==0 && energia>10)        // CONTADOR PARA LA ANIMACION DE ATAQUE
        {
            cont_ataque=1;
        }
    }

}

void Protagonista::setCombate()
{
    if(combate == true)
    {
        combate = false;        // DESACTIVAMOS MODO COMBATE
        fachada->setMaterialFlag(rec,false);
        
    }
    else
    {
        combate = true;         // MODO COMBATE ACTIVADO
        fachada->setMaterialFlag(rec,true);
    }
}

/**
DEVUELVE EL NODO QUE HEMOS CREADO
**/
void* Protagonista::getNode()
{
    return rec;
}

bool Protagonista::getCombate()
{
    return combate;
}

Posicion* Protagonista::getPosition()
{
   return protaPosition;
}

f32 Protagonista::getEnergia()
{
    return energia;
}

bool Protagonista::getSigilo()
{
    return sigilo;
}
bool Protagonista::getCorrer()
{
    return correr;
}

int Protagonista::getPosCombate()
{
    return pos_combate;
}


Protagonista::~Protagonista()
{
    //dtor
    rec = nullptr;
    energy = nullptr;
    life = nullptr;  
}
