/** Example 001 HelloWorld

This Tutorial shows how to set up the IDE for using the Irrlicht Engine and how
to write a simple HelloWorld program with it. The program will show how to use
the basics of the VideoDriver, the GUIEnvironment, and the SceneManager.
Microsoft Visual Studio is used as an IDE, but you will also be able to
understand everything if you are using a different one or even another
operating system than windows.

You have to include the header file <irrlicht.h> in order to use the engine. The
header file can be found in the Irrlicht Engine SDK directory \c include. To let
the compiler find this header file, the directory where it is located has to be
specified. This is different for every IDE and compiler you use. Let's explain
shortly how to do this in Microsoft Visual Studio:

- If you use Version 6.0, select the Menu Extras -> Options.
  Select the directories tab, and select the 'Include' Item in the combo box.
  Add the \c include directory of the irrlicht engine folder to the list of
  directories. Now the compiler will find the Irrlicht.h header file. We also
  need the irrlicht.lib to be found, so stay in that dialog, select 'Libraries'
  in the combo box and add the \c lib/VisualStudio directory.
  \image html "vc6optionsdir.jpg"
  \image latex "vc6optionsdir.jpg"
  \image html "vc6include.jpg"
  \image latex "vc6include.jpg"

- If your IDE is Visual Studio .NET, select Tools -> Options.
  Select the projects entry and then select VC++ directories. Select 'show
  directories for include files' in the combo box, and add the \c include
  directory of the irrlicht engine folder to the list of directories. Now the
  compiler will find the Irrlicht.h header file. We also need the irrlicht.lib
  to be found, so stay in that dialog, select 'show directories for Library
  files' and add the \c lib/VisualStudio directory.
  \image html "vcnetinclude.jpg"
  \image latex "vcnetinclude.jpg"

That's it. With your IDE set up like this, you will now be able to develop
applications with the Irrlicht Engine.

Lets start!

After we have set up the IDE, the compiler will know where to find the Irrlicht
Engine header files so we can include it now in our code.
*/

#include <irrlicht/irrlicht.h>
#include "Protagonista.h"
#include "EnemigoBasico.h"
#include "Posicion.h"
#include <iostream>


using namespace irr; // Para poder usar cualquier clase del motor Irrlicht se utiliza el namespace irr
using namespace std;

/*
Estos son los 5 sub namespace del motor de Irrlicht

1º irr::core--> En este podemos encontrar las clases basicas como vectores, planos, arrays, listas y demas
2º irr::gui--> Contiene clases utiles para la facil creacion de una interfaz grafica de usuario
3º irr::io-->  Proporciona interfaces para la entrada/salida. Lectura y escritura de ficheros, acceso a ficheros zip, ficheros xml..
4º irr::scene--> Se encuentra toda la gestion de la escena
5º irr::video--> Contiene clases para acceder al driver del video. Todo el rendererizado 3d o 2d se realiza aqui
*/
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

/*
To be able to use the Irrlicht.DLL file, we need to link with the Irrlicht.lib.
We could set this option in the project settings, but to make it easy, we use a
pragma comment lib for VisualStudio. On Windows platforms, we have to get rid
of the console window, which pops up when starting a program with main(). This
is done by the second pragma. We could also use the WinMain method, though
losing platform independence then.
*/
#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

/**
    Clase para poder recoger los eventos ( entrada por teclado )
**/
class MyEventReceiver : public IEventReceiver
{
public:

	virtual bool OnEvent(const SEvent& event)
	{
		// Remember whether each key is down or up
		if (event.EventType == irr::EET_KEY_INPUT_EVENT)
        {
           KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
           //keyIsPressed[event.KeyInput.Key] = event.KeyInput.
        }
		/*
		Always return false by default. If you return true you tell the engine
		that you handled this event completely and the Irrlicht should not
		process it any further. So for example if you return true for all
		EET_KEY_INPUT_EVENT events then Irrlicht would not pass on key-events
		to it's GUI system.
		*/
		return false;
	}

	/*
    Para saber que tecla esta pulsada
    */

	virtual bool IsKeyDown(EKEY_CODE keyCode) const
	{
		return KeyIsDown[keyCode];
	}

	MyEventReceiver()
	{
		for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
			KeyIsDown[i] = false;
	}

private:

	bool KeyIsDown[KEY_KEY_CODES_COUNT];
	bool keyIsPressed[KEY_KEY_CODES_COUNT];

};

/*
This is the main method. We can now use main() on every platform.
*/
int main()
{

    /**
        Velocidad de Movimiento NORMAL del prota en unidades por segundo
	*/

	// This is the movement speed in units per second.
	const f32 MOVEMENT_SPEED = 90.f;
    int saltando=0;
    int sigilo=0;
    int correr=0;
    int direccion=1;
    f32 vitalidad = 100.f;



	/**
	El irrlicht device es el objeto nucleo que necesitamos para interactuar con el motor de irrlicht
	Por eso la funcion createDevice es la mas importante puesto que es necesario crear el device para
	poder dibujar cualquier cosa en pantalla

	Tiene 7 parametros:

	-deviceType --> Tipo del device (Null-device, uno de los 2 software renders, OpenGL

    - WindowSize --> Tamaño de la ventana

    - Bits --> Cantidad de bits de colores por pixeles. Puede ser 16 o 32.

    - FUllScreen --> Especifica si queremos que el device se ejecute en pantalla completa o no

    - stencilbuffer --> Especifica si queremos usar el stencil buffer ( para dibujar sombras )

    - vsync --> Especificamos si queremos tener vsync activado, solo es util en pantalla completa

    - eventReceiver --> Un objeto para recibir eventos
	**/


    MyEventReceiver receiver;

	IrrlichtDevice *device =
		createDevice( video::EDT_OPENGL, dimension2d<u32>(700, 600),16, false, false, false, &receiver);

	if (!device)
		return 1;

    /**
	PUNTEROS AL VideoDriver, al SceneManager y al entorno de interfaz de usuario, para no tener que
	estar llamandolos siempre y solo los llamamos una vez
	*/

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

    // CREAMOS PROTA
	Protagonista *prota = new Protagonista(device, smgr);
	scene::ISceneNode  *rec = prota->getNode();

    Posicion *posiciones[5];

        posiciones[0]=new Posicion(40.f,0.f,30.f);
        posiciones[1]=new Posicion(20.f,0.f,30.f);
        posiciones[2]=new Posicion(0.f,0.f,30.f);
        posiciones[3]=new Posicion(-20.f,0.f,30.f);
        posiciones[4]=new Posicion(-40.f,0.f,30.f);


	//CREAMOS ENEMIGO BASICO
	EnemigoBasico *enem = new EnemigoBasico(device, smgr, posiciones);


	/**

	Para poner texto en el subtitulo de la ventana. Necesita de una 'L' delante del string
	debido a que lo necesita el motor de irrlicht

	**/

	device->setWindowCaption(L"Mecanicas Basicas: Movimiento");


	/**

	Etiqueta de texto utilizando el entorno de GUI. Indicamos en que posicion queremos
	colocarla (10,10) y (260,20) es la esquina derecha mas baja

	guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
		rect<s32>(10,10,260,22), true);
	**/


    /**

	Aqui indicamos la posicion de la camara en el espacio 3d. En este caso,
	esta mirando desde la posicion (0, 30, -40) a la (0, 5, 0) donde es
	aproximadamente donde esta el objeto.

	**/

	scene::ICameraSceneNode* cam =smgr->addCameraSceneNode(0, vector3df(rec->getPosition().X,50,-120), vector3df(0,5,0));
	device->getCursorControl()->setVisible(true);


    /**
	Vamos a dibujar la escena y escribir los fps

	*/

	int lastFPS = -1;

	/**
	  Para poder hacer un movimiento independiente del framerate, tenemos que saber
	  cuanto ha pasado desde el ultimo frame
	*/
	u32 then = device->getTimer()->getTime();
	u32 time_input = device->getTimer()->getTime();

	/*
	Ok, now we have set up the scene, lets draw everything: We run the
	device in a while() loop, until the device does not want to run any
	more. This would be when the user closes the window or presses ALT+F4
	(or whatever keycode closes a window).
	*/

	while(device->run())
	{


		// recojo el frame delta time y el tiempo.
		const u32 now = device->getTimer()->getTime();
		const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
		then = now;
		f32 tiempo=(f32)(now - time_input)/1000.f;


		core::vector3df protaPosition = prota->getPosition();
		core::vector3df camPosition = cam->getPosition();


        prota->salto(frameDeltaTime);

        /* lanza el salto al pulsat w */

		if(receiver.IsKeyDown(irr::KEY_SPACE) && protaPosition.Y<1){
            prota->setSalto(true);
            prota->setEnergia(-100.f, frameDeltaTime);

		}


        /* ajusta la vitalidad a los valores max y min y la muestra por consola */
        if(prota->getEnergia()>100){
            prota->setEnergia(100.f, frameDeltaTime);
        }else if(prota->getEnergia()<0)
            prota->setEnergia(100.f, frameDeltaTime);
        std::cout<<prota->getEnergia()<<"\n";

        /* 5 veces por segundo registra si pulsamos s para controlar el modo sigilo */

        if(tiempo>0.2f)
        {
            f32 energia=prota->getEnergia();

            time_input=now;

                if(receiver.IsKeyDown(irr::KEY_KEY_C)) // AGACHARSE
                {
                        prota->setSigilo();
                        prota->setCorrer(false);
                }

                if(energia<99.9)
                {
                    prota->setEnergia(100.f, frameDeltaTime);
                }

        }

        /* control de correr*/

        if(receiver.IsKeyDown(irr::KEY_LSHIFT)){

            prota->setCorrer(true);

            /* baja la vitalidad en funcion del tiempo*/

             if(tiempo>0.19f)
            {
                prota->setEnergia(-100.f, frameDeltaTime);
            }

        }else
            prota->setCorrer(false);



        /* movimiento hacia los lados y control de la velocidad en funcion de
        las variables de correr, sigilo y vitalidad */

		if(receiver.IsKeyDown(irr::KEY_KEY_A))
        {
            camPosition.X -= MOVEMENT_SPEED * frameDeltaTime;
            prota->setDireccion(0);

             prota->movimiento(frameDeltaTime);
		}

		else if(receiver.IsKeyDown(irr::KEY_KEY_D)){

            camPosition.X += MOVEMENT_SPEED * frameDeltaTime;
             prota->setDireccion(1);

             prota->movimiento(frameDeltaTime);
		}

		/* ajuste de la posicion de la camara y su foco en funcion de la posicion de
		nuestro protagonita */

		rec->setPosition(protaPosition);
        cam->setPosition(vector3df(protaPosition.X,50,-140));
        protaPosition.Y=50;
        cam->setTarget(protaPosition);


        /**
        ENEMIGO
        */
        /*
         if(receiver.IsKeyDown(irr::KEY_KEY_K))
        {
           enem->setPatrulla(false);
        }
        else{

            enem->setPatrulla(true);
        }
        */
        enem->Patrulla(frameDeltaTime, posiciones, protaPosition.X);  //INICIAMOS LA PATRULLA DEL ENEMIGO
        enem->comprobarComportamiento();
		/*
		Anything can be drawn between a beginScene() and an endScene()
		call. The beginScene() call clears the screen with a color and
		the depth buffer, if desired. Then we let the Scene Manager and
		the GUI Environment draw their content. With the endScene()
		call everything is presented on the screen.
		*/

		driver->beginScene(true, true, SColor(255,100,101,140));

		smgr->drawAll(); // draw the 3d scene
		device->getGUIEnvironment()->drawAll(); // draw the gui environment (the logo)

		driver->endScene();

		int fps = driver->getFPS();

		if (lastFPS != fps)
		{
			core::stringw tmp(L"Movement Example - Irrlicht Engine [");
			tmp += driver->getName();
			tmp += L"] fps: ";
			tmp += fps;

			device->setWindowCaption(tmp.c_str());
			lastFPS = fps;
		}

	}

	/*
	After we are done with the render loop, we have to delete the Irrlicht
	Device created before with createDevice(). In the Irrlicht Engine, you
	have to delete all objects you created with a method or function which
	starts with 'create'. The object is simply deleted by calling ->drop().
	See the documentation at irr::IReferenceCounted::drop() for more
	information.
	*/

	/**

	Hay que eliminar el objete device que creamos anteriormente antes de terminar con el bucle de render
	En el motor de irrlicht todos los objetos que se han creado mediante una funcion que empieza por
	'create' deben ser eliminados y esto se hace simplemente llamanado a '->drop()'

	**/
	device->drop();
	delete prota;
	delete enem;
    //delete [] posiciones;

	return 0;
}

