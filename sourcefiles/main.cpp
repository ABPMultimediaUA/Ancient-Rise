#include "../headerfiles/Mundo.h"


#include <iostream>
#include <unistd.h>
#include <SFML/Graphics.hpp>


using namespace std;


/*
Estos son los 5 sub namespace del motor de Irrlicht

1º irr::core--> En este podemos encontrar las clases basicas como vectores, planos, arrays, listas y demas
2º irr::gui--> Contiene clases utiles para la facil creacion de una interfaz grafica de usuario
3º irr::io-->  Proporciona interfaces para la entrada/salida. Lectura y escritura de ficheros, acceso a ficheros zip, ficheros xml..
4º irr::scene--> Se encuentra toda la gestion de la escena
5º irr::video--> Contiene clases para acceder al driver del video. Todo el rendererizado 3d o 2d se realiza aqui
*/




int main()
{
	int screenWidth = 800;
	int screenHeight = 600;

	sf::RenderWindow* ventana = new sf::RenderWindow(sf::VideoMode(screenWidth, screenHeight), "Ancient Rise", sf::Style::Titlebar | sf::Style::Close);
    /*creo una vista*/
    ventana->setFramerateLimit(60);
	sf::View view(sf::FloatRect(0, 0, 1000, 600));
	
	view.setViewport(sf::FloatRect(0.75f, 0, 0.25f, 0.25f));
	ventana->setView(view);


	Mundo* mundo = new Mundo(ventana);


	// define a 120x50 rectangle
	sf::RectangleShape rectangle(sf::Vector2f(20, 20));

	// change the size to 100x100
	//rectangle.setSize(sf::Vector2f(10, 10));
	rectangle.setFillColor(sf::Color(100, 250, 50));

	/* BUCLE PRINCIPAL DEL JUEGO */

	while(ventana->isOpen())
	{

		char inputKey;
		bool keyPressed;
		ventana->clear(sf::Color::Black);
		//Evento
		sf::Event* evento = new sf::Event; 
		//Obtener eventos
		while (ventana->pollEvent(*evento))
		{
		    switch (evento-> type)
		    {
		        case sf::Event::Closed:
		       
		            ventana->close();
		        break;

		        case sf::Event::KeyPressed:
		        {    
		            inputKey = evento->key.code;
		            //std::cout<<evento->key.code<<"\n";
		            keyPressed = true;
		           
		        }   
		        break;
		    }
		    
		    
		}
        //	ventana.close();
		//

		 //ventana->draw(rectangle);
		ventana->display();

		/* ACTUALIZAMOS EL MUNDO */
		mundo->update();
		
		//ventana.display();
		
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

	//ventana.close();
}

