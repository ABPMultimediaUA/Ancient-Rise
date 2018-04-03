#include "../headerfiles/Trampa.h"


Trampa::Trampa(IrrlichtDevice *dev, ISceneManager* smgr, Posicion* pos):Objeto(pos)
{
	GameObject::setTipo(TRAMPA);
    Fachada* fachada=fachada->getInstance();
    FObjeto* obObjeto = fachada->addCube(pos->getPosX(),pos->getPosY(),pos->getPosZ(),false);
	//FObjeto* obObjeto = fachada->addMalla(pos->getPosX(),pos->getPosY(),pos->getPosZ(),"resources/pinchos.obj");
	objeto = obObjeto;
    //posicion = &pos;
    std::cout<<"Pinchos: " << this->getPosition()->getPosX()<<endl;
	if (objeto)
	{
        
		//driver = fachada->getDriver();
		//objeto ->setPosition(core::vector3df(pos.getPosX(),pos.getPosY(),pos.getPosZ()));
        Posicion escala(1.f,1.f,1.f);
		fachada->setScala(objeto,&escala);
		//objeto ->setMaterialFlag(video::EMF_LIGHTING, false);
		//objeto ->setMaterialTexture(0,driver->getTexture("resources/pinchos.jpeg"));
        //fachada->setMaterial(objeto,"resources/pinchos.jpeg");

	}
}



void* Trampa::getNode()
{
    return objeto;
}

