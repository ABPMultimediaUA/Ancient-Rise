#include "../headerfiles/Mundo.h"

Mundo::Mundo():prota(nullptr),c(nullptr),f(nullptr),a(nullptr),t(nullptr),bebida(nullptr),b(nullptr),enem1(nullptr),enem2(nullptr),enemE1(nullptr),Plataforma(nullptr),
Plataforma2(nullptr), Plataforma3(nullptr), cam(nullptr)	//CONSTRUCTOR
{
Fachada* fachada=fachada->getInstance();


/*CREAMOS GESTOR DE SONIDO*/
	sonido = GestorSonido::getInstance();
	reverbCueva = sonido->create3DReverb();
	reverbCueva->setAtributos3D(0.0f,0.0f,0.0f, 10.0f, 2000.0f);
	reverbCueva->setTipo(sonido->REVERB_CUEVA);
	musicaBosque = sonido->createMusic(sonido->SOUND_MUSIC_BOSQUE);
/* CREAMOS PROTA */
	prota = prota->getInstance();
	addGameObject(prota);
    

	//creo el suelo, el bounding box del prota
    prota->CreateBox(world, -170.f, 0.f);
    

/* CREAMOS OBJETOS */

	Posicion* pC= new Posicion(-220.f, 0.34f, 30.f);
	c = new Comida(pC);
	comidas.push_back(c);
	addGameObject(c);

	Posicion* pC2= new Posicion(190.f, 0.34f, 30.f);
	c2 = new Comida(pC2);
	comidas.push_back(c2);
	addGameObject(c2);

	Posicion* pF= new Posicion(-190.f,0.34f,40.f);
	f = new Fuente( pF);
	fuentes.push_back(f);
	addGameObject(f);

	Posicion* pF2= new Posicion(320.f,0.34f,40.f);
	f2 = new Fuente( pF2);
	fuentes.push_back(f2);
	addGameObject(f2);

	Posicion* pA= new Posicion(120.f,0.34f,40.f);
	a = new Alarma( pA);
	alarmas.push_back(a);
	addGameObject(a);

	Posicion* pA2= new Posicion(-160.f,0.34f,40.f);
	a2 = new Alarma( pA2);
	alarmas.push_back(a2);
	addGameObject(a2);

	Posicion* posbebida= new Posicion(-300,0.34f,30.f);
 	bebida = new Bebida(posbebida);
 	addGameObject(bebida);

	Posicion* postrampa= new Posicion(520,0.34f,30.f);
 	t = new Trampa(postrampa);
 	addGameObject(t);


/* CREAMOS LA BLACKBOARD */

	b=new Blackboard();
    
	 b->setFuente(fuentes);
	 b->setComida(comidas);
	 b->setAlarma(alarmas);
	 b->setProtagonista(prota);
	 

/** ESTABLECEMOS LA CAMARA
 Aqui indicamos la posicion de la camara en el espacio 3d. En este caso,
 esta mirando desde la posicion (0, 30, -40) a la (0, 5, 0) donde es
 aproximadamente donde esta el objeto.
**/
    Posicion* camaraPos = new Posicion(prota->getPosition()->getPosX(),50,-140);
    
	cam = fachada->addCamara(camaraPos);
	//device->getCursorControl()->setVisible(true);

 /* AÑADIMOS UNA LUZ */   
    Posicion* luzPos=camaraPos;
    fachada->addLuz(luzPos);
    Posicion* dir = new Posicion(0,-1,1);
    fachada->addLuzDireccional(dir);
    Posicion* d = new Posicion(0,1,0);
    Posicion* origen = new Posicion(0,65,0);
    fachada->addLuzDirigida(origen,d);
    
/* CREAMOS EL TERRENO Y COLISIONES DE CAMARA */

	terrainBuilder();

/** TIME AND FRAMES
 Para poder hacer un movimiento independiente del framerate, tenemos que saber
 cuanto ha pasado desde el ultimo frame
**/
	lastFPS = -1;

	/* CREANDO GRAFO PARA EL MAPA PROTOTIPO */

 /* CREAMOS ENEMIGOS BASICOS */
	/*enem1 = new EnemigoBasico( pos, 140.0, 0.8, 2, this, b, world);
	enemB.push_back(enem1);
	addGameObject(enem1); 
	
	enem2 = new EnemigoBasico(pos2, 140.0, 0.8, 1, this, b, world);
	enemB.push_back(enem2);
	addGameObject(enem2); */

	/*for(int i=0;i<enemB.size();i++)   // Añadimos todos los enemigos basicos que existen a la blackboard
	{
		b->setEnemB(enemB[i]);
	}	*/

	/* CREAMOS ENEMIGOS ELITES */
	/*enemE1 = new EnemigoElite(pos3, 120.0, 0.8, 2, this, b, world);
	enemE.push_back(enemE1);
	addGameObject(enemE1);
*/

    
    Posicion* posmenu= new Posicion(.5f,-.5f,.5f);
 	Menu* menu = new Menu(posmenu);
 	addGameObject(menu);
    
    Posicion* pospausa= new Posicion(-20.5f,-.5f,.5f);
 	Pausa* pausa = new Pausa(pospausa);
 	addGameObject(pausa);

    cargarNivel();

    b->setNodosGrafo(nodos);            // Pasamos los nodos a la blackboard

   /* cout<<aristas.size()<<endl;
    for(int i=0; i<aristas.size();i++)
    {
        cout<<"Nodo Inicio "<<aristas[i]->getNodoInicio()->getNombre()<<" Nodo FInal "<<aristas[i]->getNodoFin()->getNombre()<<endl;
    }
*/

     for(int i=0;i<enemB.size();i++)   // Añadimos todos los enemigos basicos que existen a la blackboard
    {
        b->setEnemB(enemB[i]);
    }

}	

void Mundo::terrainBuilder(){	//CONSTRUCTOR DEL TERRENOS Y COLISIONES DE CAMARA

    fachada->drawTerreno();

}

void Mundo::update(){
    //Comprueba las entradas del teclado
	checkInput();
	//pasos de las fisicas en el mundo
	world.Step(1/60.f, 8, 3);
	//reinicio las fuerzas en el mundo
	world.ClearForces();

	/* Creamos el framedeltatime y el tiempo */

	float frameDeltaTime = fachada->getTime(); // Time in seconds

	Posicion* protaPosition = prota->getPosition();
	//core::vector3df camPosition = cam->getPosition();

	/* PROTA UPDATE */

	protaUpdate(frameDeltaTime);

	/* CAM UPDATE*/

    camUpdate(frameDeltaTime);

    b->setTime(frameDeltaTime);
    b->setProta(protaPosition->getPosX());

    /* ALARMA UPDATE*/
    a->update();
    //a2->update();

    /*for(int i=0; i<alarmas.size();i++)
    {
        alarmas[i]->update();
    }*/

    /* UPDATE DE LOS ENEMIGOS */

    for(int i=0; i<enemB.size();i++)   		// Enemigos Basicos
    {
    	if(enemB[i]->getNode()!=nullptr) 	// Solo si existen hacemos su update
    	{
	       	enemB[i]->updateTiempo(frameDeltaTime);
	     	enemB[i]->Update(prota->getPosition());
	    }
    }
/*
    for(int i2=0; i2<enemE.size();i2++) 	// Enemigos Elites
    {
    	if(enemE[i2]->getNode()!=nullptr)
    	{
	    	enemE[i2]->updateTiempo(frameDeltaTime);
	     	enemE[i2]->Update(prota->getPosition());
	    }
    }
*/
    /* DRAW SCENE */

    //draw();

    /* CONTROL DE FRAMES POR SEGUNDO */

    //fpsControl();

    /*UPDATE DE SONIDO*/
    sonido->playSound(musicaBosque);
    sonido->update();
	sonido->setListener(prota->getPosition()->getPosX(), prota->getPosition()->getPosY(), prota->getPosition()->getPosZ());

}

void Mundo::protaUpdate(const glm::f32 frameDeltaTime)
{
	//scene::ISceneNode* pro = (scene::ISceneNode*)prota->getNode();
    //core::vector3df protaPosition = pro->getPosition();

	energiaAnterior = prota->getEnergia();

    //prota->ataque(frameDeltaTime);
    
    //prota->pintarInterfaz();
    
	prota->comprobarColision(c);
    prota->comprobarColision(bebida);
    prota->comprobarColision(t);

    prota->updateBody(world);
    
    if(!prota->checkVida())
		fachada->cerrar();

	//if(tiempo>0.2f) 	// HACEMOS QUE LO QUE HAYA DENTRO SE HAGA MENOS VECES POR SEGUNDO
    //{
        glm::f32 energia=prota->getEnergia();

        checkCombate(); 							// Comprobamos si hemos pulsado la tecla de combate (K)

         if(sf::Keyboard::isKeyPressed(sf::Keyboard::J))
        {   
            for(int i=0; i<enemB.size();i++)
            {
                enemB[i]->setInvisible();
            }
        }
        prota->update(b);
            
    //}

    if(prota->getCombate()==false)
    {
    	//prota->checkSigilo(prota);  						// Comprobamos si hemos pulsado la tecla de sigilo (C)
    }

    /* Velocidad Barra de Energia */
    energiaActual = prota->getEnergia();
    energiaDelta = energiaActual - energiaAnterior;

    if(energiaDelta < 0){
    	relojDescanso.restart();
    }

    tiempoTrans = relojDescanso.getElapsedTime().asSeconds();
    
    if(tiempoTrans > 0.8f)	
    	prota->setEnergia(25,frameDeltaTime);  //CAMBIO 5 a 15
    else
    	prota->setEnergia(2, frameDeltaTime);

}
void Mundo::checkInput(){
    
    
	if(sf::Joystick::isConnected(0)){
		JoyY=sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
		JoyX=sf::Joystick::getAxisPosition(0, sf::Joystick::X);
		////std::cout<<JoyX<<"\n";
	}
        /* movimiento hacia los lados y control de la velocidad en funcion de
        las variables de correr, sigilo y vitalidad */

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)||JoyX<=-50)//A
        {
            prota->setDireccion(0);
            prota->movimiento(0.1f);
    		if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)||sf::Joystick::isButtonPressed(0, 5))
    		{
                prota->setCorrer(true);
      		    prota->setEnergia(-1.0f,0.2f);
    		}else
		  prota->setCorrer(false);
        }

        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)||JoyX>=50){//D

            prota->setDireccion(1);
            prota->movimiento(0.1f);
    		if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)||sf::Joystick::isButtonPressed(0, 5))
    		{
		  prota->setCorrer(true);
      		  prota->setEnergia(-1.0f,0.2f);
    		}else
		  prota->setCorrer(false);
        }
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)||sf::Joystick::isButtonPressed(0, 0)){
        	prota->setSalto(true);
    	}else
		prota->setSalto(false);
	       
}

void Mundo::checkCombate()
{
	/* control de ataque*/
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::P)||sf::Joystick::isButtonPressed(0, 2))//p
    {  
        prota->setAtaque(true);
    }
        
    /* control de defensa*/
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::K)||sf::Joystick::isButtonPressed(0, 4))//k
    {
        prota->setCombate();
    }
}

void Mundo::camUpdate(const glm::f32 frameDeltaTime){
    //prueba zoom camara
    /*
    if(prota->getCaida()){
       while(CamZ>-200){
            CamZ-=0.00001f;
            ////std::cout<<"con zoom"<<endl;
        }
    }
    if(prota->getSalto()){
        while(CamZ<-100){
            CamZ+=0.00001f;
            ////std::cout<<"con zoom"<<endl;
        }
        
        zoom=false;
        //CamZ=-200;
    }
    ////std::cout<<"Camz"<<CamZ<<endl;
    */
    
	Posicion* protaPosition = prota->getPosition();
	//vec3 camPosition = cam->getPosicion();
    if(estado==2){
    cam->setPosicion(vec3(-protaPosition->getPosX(),-protaPosition->getPosY()-35,-130)); // cambio 5O A ProtaPosition.Y
    //camPosition=vec3(protaPosition->getPosX(),protaPosition->getPosY()+30,protaPosition->getPosZ());
    //camPosition.y=protaPosition->getPosY()+30;
    //Falta funcion para enfocar la camara
    //cam->setTarget(camPosition);
    }
    if(estado==1){
        cam->setPosicion(vec3(20,2,-20));
        //cam->Rotar(vec3(0,1,0), 3.0f);
    }
    if(estado==0){
        cam->setPosicion(vec3(0,2,-20));
        //cam->Rotar(vec3(0,1,0), 3.0f);
    }
}

void Mundo::fpsControl(){
	
	int fps = 60;

	if (lastFPS != fps)
	{
		//core::stringw tmp(L"Movement Example - Irrlicht Engine [");
		//tmp += driver->getName();
		//tmp += L"] fps: ";
		//tmp += fps;

		//device->setWindowCaption(tmp.c_str());
		lastFPS = fps;
	}

	this->timeWait();
}

void Mundo::timeWait(){
	static long t=clock();
	const float fps = 60.f;

	long toWait = t + CLOCKS_PER_SEC / fps - clock();
	
	if(toWait > 0)
		usleep(toWait);

	t = clock();
}

void Mundo::draw(){
	//estado=1;
    //camUpdate(5.f);
    //fachada->draw();
    //estado=0;
    //camUpdate(5.f);
    fachada->draw();
}

void Mundo::addGameObject (GameObject* o){
	gos.push_back(o);
}

GameObject* Mundo::getGameObject(uint8_t pos) const{
	GameObject* o = nullptr;

	if(pos < gos.size() && gos[pos] != nullptr){
		o = gos[pos];
	}

	return o;
}
 void Mundo::CambioEstado(int n){
    estado=n;
}
int Mundo::getEstado(){
    return estado;
}


/* Funcion para hacer la lectura de la logica de los niveles a partir de un xml */ 
void Mundo::cargarNivel()
{
    TiXmlDocument doc;

    if(doc.TiXmlDocument::LoadFile("resources/nivel2.xml",TIXML_ENCODING_UTF8 )) //TIXML_ENCODING_UTF8
    { 
      //std::cout <<"Leyendo bien"<<endl;    
    }

    //OBTENER ELEMENTO MAPA
    TiXmlElement* map = doc.FirstChildElement("map");

    if(map)
    { 
        //MOSTRAR ATRIBUTOS MAPA
        TiXmlAttribute* atri = map->FirstAttribute();
          
        int i=0;
        int ival;
        double dval;
         
          
        while (atri)
        {
          if (atri->QueryIntValue(&ival)==TIXML_SUCCESS)    //printf( " int=%d", ival);//muestra su valor entero
          if (atri->QueryDoubleValue(&dval)==TIXML_SUCCESS) //printf( " d=%1.1f", dval);//muestra su valor doble
          //printf( "\n" );

          i++;
          atri=atri->Next();

        }

        //OBTENER ELEMENTO TILESET
        TiXmlElement* tileset = map->FirstChildElement("tileset");

        if(tileset)
        { 
            TiXmlAttribute* atri2 = tileset->FirstAttribute();
              
            int i2=0;
            int ival2;
            double dval2;
             
            while (atri2)
            {
              if (atri2->QueryIntValue(&ival2)==TIXML_SUCCESS)    //printf( " int=%d", ival2);//muestra su valor entero
              if (atri2->QueryDoubleValue(&dval2)==TIXML_SUCCESS) //printf( " d=%1.1f", dval2);//muestra su valor doble

                i2++;
                atri2=atri2->Next();
                   
            }
          } 

        //OBTENER ELEMENTO LAYER
        TiXmlElement* layer = map->FirstChildElement("layer");//a la misma altura que tileset(hijos de mapa)
        if(layer)
        { 
            //std::cout <<"Layer ok"<<endl;
        }

        //OBTENER ELEMENTO DATA
        TiXmlElement* data = layer->FirstChildElement("data");//data es hijo de layer
        if(data)
        { 
            //std::cout <<"data ok"<<endl;
        }

        //OBTENER ELEMENTO OBJECTGROUP
        TiXmlElement* grupo = map->FirstChildElement("objectgroup");//objectgroup es hijo de map
        if(grupo)
        {                     
            //OBTENER ELEMENTO SUELO
            TiXmlElement* obje = grupo->FirstChildElement("object");//object es hjo de objectgroup

            int id;
            int i3=1;
            double altura;
            double ancho;
            double x;
            double y;

            while(obje)
            {
                TiXmlAttribute* atri3 = obje->FirstAttribute();
                          
                while (atri3)
                {
                    if(strcmp(atri3->Name(),"id")==0)
                    {
                        atri3->QueryIntValue(&id);
                    }

                    //OBTENER DIMENSIONES DE LAS CAJAS QUE FORMAN EL SUELO
                    if(strcmp(atri3->Name(),"x")==0)
                    {
                        atri3->QueryDoubleValue(&x);
                    }//CAJAS MAS ALTAS QUE ANCHAS NO TRATARLAS

                    if(strcmp(atri3->Name(),"y")==0)
                    {
                        atri3->QueryDoubleValue(&y);
                    }

                    if(strcmp(atri3->Name(),"width")==0)
                    {
                        atri3->QueryDoubleValue(&ancho);
                    }

                    if(strcmp(atri3->Name(),"height")==0)
                    {
                        atri3->QueryDoubleValue(&altura);

                    }  
                                
                    atri3=atri3->Next();   

                }//atributos
                            
                i3++;
                fachada->CreateGround(world, (int)x, (int)y,(int)ancho, (int)altura);
                obje=obje->NextSiblingElement("object");//pasamos a la siguiente caja

                             
            }//objeto/elemento    


            //TRATAMOS LAS PLATAFORMAS DE UNA FORMA LIGERAMENTE DISTINTA, LOSDEMAS OBJETOS COMPARTEN NUMERO Y TIPODE ATRIBUTOS
            TiXmlElement* grupo2=grupo->NextSiblingElement("objectgroup");
                  
            while(grupo2)
            { 
                double dval9;
                TiXmlAttribute* atriE = grupo2->FirstAttribute();

                while(atriE)
                {
                    atriE->QueryDoubleValue(&dval9)==TIXML_SUCCESS;
                    atriE=atriE->Next();
                }//atributosGrupoEnemigos
                   
                TiXmlElement* ene = grupo2->FirstChildElement("object");//object es hjo de objectgroup

                int idE;        // Identificador de los objects del xml
                int iE;         // No se
                int tipo;       
                double xEn;     // Pos en X del object
                double yEn;     // Pos en Y del object
                int name;
                    
                while(ene)
                {
                    TiXmlAttribute* atriEn = ene->FirstAttribute();
                    iE++;
                    while(atriEn)
                    {
                        if(strcmp(atriEn->Name(),"id")==0)
                        {
                            atriEn->QueryIntValue(&idE);
                        }

                        if(strcmp(atriEn->Name(),"type")==0)
                        {
                            atriEn->QueryIntValue(&tipo);                                                                    
                        }

                        if(strcmp(atriEn->Name(),"x")==0)
                        {
                            atriEn->QueryDoubleValue(&xEn);
                        }

                        if(strcmp(atriEn->Name(),"y")==0)
                        {
                            atriEn->QueryDoubleValue(&yEn);
                        }

                        if(strcmp(atriEn->Name(),"name")==0)
                        {
                            atriEn->QueryIntValue(&name);
                        }

                        atriEn=atriEn->Next();

                    }//atributosEnemigo


                    if(strcmp(grupo2->FirstAttribute()->Value(),"enemigos")==0)
                    {   
                        int nodoI = name/100;
                        int nodoF = name%100;

                        NodoGrafo *nI, *nF;                        
                        for(int i=0; i<nodos.size();i++)
                        {
                            if(nodos[i]->getNombre()==nodoI)
                            {
                                nI = nodos[i];
                            }

                            if(nodos[i]->getNombre()==nodoF)
                            {
                                nF = nodos[i];
                            }
                        }


                        Posicion *p0 = new Posicion(nI->getPosition()->getPosX(),(nI->getPosition()->getPosY()),30.f);
                        pos.push_back(p0);

                        Posicion *p1 = new Posicion(nF->getPosition()->getPosX(),(nF->getPosition()->getPosY()),30.f);
                        pos.push_back(p1);

                        int t = tipo/10;    // Tipo de enemigo
                        int a = tipo%10;    // TIpo de ataque

                        switch (t)
                        {   
                            case 1: // Enemigos Basicos
                            {
                                enem1 = new EnemigoBasico( pos, 140.0, 0.8, a, this, b, world);
                                enemB.push_back(enem1);
                                addGameObject(enem1);
                                break;
                            }

                            case 2: // Enemigos Avanzados
                            {
                                break;
                            }

                            case 3: // Enemigos Elites
                            {
                                enemE1 = new EnemigoElite( pos, 120.0, 0.8, a, this, b, world);
                                enemE.push_back(enemE1);
                                addGameObject(enemE1);
                                break;
                            }

                        }
                        pos.clear();    // Vaciamos el vector para que no de problemas para el siguiente
                        
                    }   

                    if(strcmp(grupo2->FirstAttribute()->Value(),"recolectables")==0)
                    {
                    
                    } 

                    if(strcmp(grupo2->FirstAttribute()->Value(),"armas")==0)
                    {
                
                    }  

                    if(strcmp(grupo2->FirstAttribute()->Value(),"alarmas")==0)
                    {
                        Posicion* posA= new Posicion(xEn-190,-yEn+60,0.f);

                        Alarma* alarm = new Alarma( posA);
                        alarmas.push_back(alarm);
                        addGameObject(alarm);

                        delete posA;
                    }  

                    if(strcmp(grupo2->FirstAttribute()->Value(),"fuentes")==0)
                    {
                        Posicion* posF= new Posicion(xEn-190,-yEn+60,0.f);

                        Fuente* fuente = new Fuente( posF);
                        fuentes.push_back(fuente);
                        addGameObject(fuente);

                        delete posF;
                    }  

                    if(strcmp(grupo2->FirstAttribute()->Value(),"nodos")==0)
                    {
                        NodoGrafo *nA = new NodoGrafo(idE,xEn-190, -yEn+60);           
                        nodos.push_back(nA);
                        addGameObject(nA);
                        //cout<<idE<<endl;
                    } 

                    if(strcmp(grupo2->FirstAttribute()->Value(),"aristas")==0)
                    {
                        int p = tipo/10;                    // Peso de la arista
                        int t = tipo%10;                    // Tipo de comportamiento

                        Arista *a1 = new Arista(p,t);       // Indicamos peso y tipo de la arista
                        aristas.push_back(a1);

                        int nodoI = name/100;               // Nodo Inicio
                        int nodoF = name%100;               // Nodo FInal
                        
                        NodoGrafo *nI, *nF;                        
                        for(int i=0; i<nodos.size();i++)
                        {
                            if(nodos[i]->getNombre()==nodoI)
                            {
                                nI = nodos[i];
                            }

                            if(nodos[i]->getNombre()==nodoF)
                            {
                                nF = nodos[i];
                            }
                        }

                        nI->insertAristaS(a1,nF);

                    } 

                    ene=ene->NextSiblingElement("object");//pasamos a la siguiente caja
                }//enemigo
                      

                grupo2=grupo2->NextSiblingElement("objectgroup");//pasamos a la siguiente caja
            }//grupo2

        }//objectgroup
                  
    }//mapa

}    


Mundo::~Mundo()	//DESTRUCTOR
{
	/* DELETE PROTAGONISTA */
	delete prota;

	/* DELETE ENEMIGOS */
	for(int cont=0; cont<enemB.size();cont++)
	{
		delete enemB[cont];
	}
	enemB.clear();

	for(int cont2=0; cont2<enemE.size();cont2++)
	{
		delete enemE[cont2];
	}
	enemE.clear();

    for(int cont3=0; cont3<pos.size();cont3++)
    {
        delete pos[cont3];
    }
	pos.clear();

    for(int cont4=0; cont4<pos2.size();cont4++)
    {
        delete pos2[cont4];
    }
    pos2.clear();

    for(int cont5=0; cont5<pos3.size();cont5++)
    {
        delete pos3[cont5];
    }
    pos3.clear();

    for(int cont6=0; cont6<pos4.size();cont6++)
    {
        delete pos4[cont6];
    }
    pos4.clear();

    delete c;
    delete c2;
    delete f;
    delete f2;
    delete a;
    delete a2;
    delete bebida;
    delete t;
    delete sonido;

   // delete pC, pC2, pF, pF2, pA, pA2, posbebida, postrampa;

    /* DELETE DEL GRAFO PROVISIONAL */
   	for(int cont3=0; cont3<nodos.size();cont3++)
   	{
   		delete nodos[cont3];
   	}
   	nodos.clear();

   	for(int cont4=0; cont4<aristas.size();cont4++)
   	{
   		delete aristas[cont4];
   	}
   	aristas.clear();
    
    
}
