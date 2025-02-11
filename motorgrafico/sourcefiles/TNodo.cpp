#include "../headerfiles/TNodo.h"

//Clase para la gestion de nodos y estructura de arbol. Original.

TNodo::TNodo():entidad(nullptr),padre(nullptr)
{
	encontrado = false;
}

TNodo::~TNodo(){
	remHijos();												//Borramos primero los hijos del nodo

	if(padre != nullptr)
		padre->remHijo(this);								//Quitamos el nodo del vector hijos de su padre

	if(entidad != nullptr)
		delete entidad;										//Borramos la entidad
	
	entidad = nullptr;										//Puntero entidad apunta a null
	padre = nullptr;										//Puntero a padre apunta a null
}
/* Anyade un nodo al final del vector de hijos del nodo */
int TNodo::addHijoBack(TNodo* n){

	n->setPadre(this); 											// Al nodo hijo que añadimos le indicamos que este nodo es su nodo padre
	hijos.push_back(n);

	return hijos.size();
}
/* Funcion para anyadir un nodo en el vector de hijos en la pos que le indicamos por parametro */
int TNodo::addHijo(TNodo* n, int pos)
{
	n->setPadre(this); 											// Al nodo hijo que añadimos le indicamos que este nodo es su nodo padre
	hijos.insert(hijos.begin()+pos, n); 						// Metemos el nodo en la posicion indicada

	return hijos.size();
}

/* Funcion para borrar un hijo del nodo. En el caso que sea un nodo hoja solo lo borra del vector. 
	Si se trata de un nodo rama, lo que hace es borrarlo y a sus hijos se le cambia el puntero del padre al padre del nodo borrado */
TNodo* TNodo::remHijo(TNodo* n)
{	
	encontrado = false;
	if(hijos.size()!=0) 										// Comprobamos si tiene algun hijo que borrar
	{
		for(int i=0;i<hijos.size() && !encontrado;i++) 			// Recorremos todos los hijos de este nodo (this)
		{
			if(hijos[i]==n)
			{
				hijosPadre = hijos[i]->getHijos(); 				// Obtenemos los hijos del nodo que queremos borrar
				hijos.erase(hijos.begin()+i); 					// Borramos el nodo del vector
				encontrado = true;

				if(hijosPadre.size()!=0) 						// Comprobamos que el hijo de este nodo(this) que queremos borrar, tenga hijos o no para cambiar su puntero padre
				{
					for(int i2=0;i2<hijosPadre.size();i2++)  	// Recorremos para cambiar la direccion a la que apunta el puntero padre de los hijos
					{	
						addHijo(hijosPadre[i2], i); 			// Anyadimos un hijo nuevo a este nodo en la pos del vector donde se encontraba el nodo que borramos
					}
				}
			}
		}

	}
/*
	if(encontrado == true) 										// Si hemos encontrado el nodo que queriamos borrar en el vector, eliminamos el nodo
	{
		delete n;
		n = nullptr;
	}
*/
 return n;
}
/* Funcion para borrar un hijo del nodo y todos los nodos hijos de ese nodo que queremos borrar, asi hasta que no queden hijos en la rama
	Devuelve el numero de hijos restante
 */
int TNodo::remHijoAll(TNodo* n)
{	
	encontrado = false;
	if(hijos.size()!=0)													//Comprueba que haya hijos que borrar
	{
		for(int i=0;i<hijos.size() && encontrado == false; i++)			//Recorre el array de hijos buscando el nodo a borrar
		{
			if(hijos[i]==n)												//Comprueba el hijo de esta iteracion
			{
				delete hijos[i];										//Borramos el hijo y todos sus hijos
				
				encontrado = true;
			}
		}
	}

	return hijos.size();
}
/* Funcion que le pasamos dos nodos : n1 que es el nodo hijo que queremos borrar y n2 que sera el nuevo nodo padre de los nodos hijos del nodo borrado */
TNodo* TNodo::remHijoChange(TNodo* n1, TNodo* n2)
{
	encontrado = false;
	if(hijos.size()!=0) 										// Comprobamos si tiene algun hijo que borrar
	{
		for(int i=0;i<hijos.size() && !encontrado;i++) 			// Recorremos todos los hijos de este nodo (this)
		{
			if(hijos[i]==n1)
			{
				hijosPadre = hijos[i]->getHijos(); 				// Obtenemos los hijos del nodo que queremos borrar
				hijos.erase(hijos.begin()+i); 					// Borramos el nodo del vector
				encontrado = true;

				if(hijosPadre.size()!=0) 						
				{
					for(int i2=0;i2<hijosPadre.size();i2++)  	
					{	
						n2->addHijoBack(hijosPadre[i2]); 		// Indicamos que n2 es el nodo padre de los hijos del nodo borrado
					}
				}
			}
		}

	}

	if(encontrado == true) 										// Si hemos encontrado el nodo que queriamos borrar en el vector, eliminamos el nodo
	{
		delete n1;
		n1 = nullptr;
	}

	return n1;
}

// Funcion que cambia el padre del nodo por el padre dado por parametros
void TNodo::changePadre(TNodo* nPadre, TNodo* hijo){
	encontrado = false;

	if(hijos.size()!= 0){ //Comprobamos si hay hijos

		for(int i = 0; i<hijos.size() && !encontrado; i++){  	//Buscamos la posicion del nodo a cambiar en el vector hijos
			if(hijos[i] == hijo){								//Una vez encontrado, cambiamos padre del nodo y lo borramos del vector hijos
				hijos.erase(hijos.begin() + i);
				nPadre->addHijoBack(hijo);

				encontrado = true;
			}
		}
	}
}

//Funcion para borrar todos los hijos del nodo
void TNodo::remHijos(){
	if(hijos.size() != 0){ //Comprobamos que haya hijos

		for(int i = 0; i<hijos.size(); i++){					//Recorremos los hijos uno a uno para ir borrandolos

			delete hijos[i];

		}
	} 
}


/* Getters y setters */ 
bool TNodo::setEntidad(TEntidad* ent){
	entidad=ent;
	return true;
}
/* Para indicar cual es el nodo padre del nodo actual*/
void TNodo::setPadre(TNodo *p)
{
	padre = p;
}

void TNodo::setIdent(int i)
{
	identificador = i;
}


TEntidad * TNodo::getEntidad(){
	return entidad;
}
TNodo * TNodo::getPadre(){
	return padre;
}

vector<TNodo*> TNodo::getHijos()
{
	return hijos;
}

int TNodo::getIdent()
{
	return identificador;
}

glm::mat4 TNodo::getMatrix(){
	
	glm::mat4 mResultado = glm::mat4(1.0);
	vector<glm::mat4> mObtenidas;
	TNodo *nodoActual = getPadre();
	TTransf *entActual = nullptr;

	while(nodoActual->getPadre() != nullptr){
		if(nodoActual->getEntidad()->getTipo() == 0){
			entActual = dynamic_cast<TTransf*>(nodoActual->getEntidad());
			mObtenidas.push_back(entActual->getMatriz());
		}		
		nodoActual = nodoActual->getPadre();
	}
	
	if(mObtenidas.size()>0){
		for(int i = mObtenidas.size(); i > 0; i--){
			mResultado *= mObtenidas[i-1];
		}
	}

	return mResultado;
}

glm::vec3 TNodo::getPosicion(){
	glm::vec3 posicion;

	posicion = getMatrix() * glm::vec4(0,0,0,1.0f);

	return posicion;
}


void TNodo::draw(glm::mat4 view, glm::mat4 projection,TNodo* camara, vector<TNodo*> luces, bool bounding, TDatosEntidad* datos)
{
	if(padre == nullptr){			//Solo damos valores nuevos a datos en el nodo escena.
		vector<glm::vec4> colorPuntual;
		glm::vec3 camaraPosicion;
	
		vector<glm::vec3> posicionPuntual;
		vector<vector<float>> atenuacionPuntual;
	
		glm::vec3 directionDir, colorDir;
	
		glm::vec3 positionFlash, directionFlash, colorFlash;
	 	vector<float> atenuacionFlash, corteFlash;

		camaraPosicion = camara->getPosicion();
		//Recorremos en busca de puntuales
		for(int i = 0; i < luces.size(); i++){
			if(dynamic_cast<TLuz*>(luces[i]->getEntidad())->getType() == 0){						//Extraemos datos de las luces puntuales
				posicionPuntual.push_back(luces[i]->getPosicion());
				colorPuntual.push_back(dynamic_cast<TLuz*>(luces[0]->getEntidad())->getColor());
				atenuacionPuntual.push_back(dynamic_cast<TLuz*>(luces[0]->getEntidad())->getPuntualParametros());
	
			}else if(dynamic_cast<TLuz*>(luces[i]->getEntidad())->getType() == 1){ 
				directionFlash = dynamic_cast<TLuz*>(luces[i]->getEntidad())->getDireccion();
				positionFlash = luces[i]->getPosicion();
				colorFlash = dynamic_cast<TLuz*>(luces[i]->getEntidad())->getColor();
				corteFlash = dynamic_cast<TLuz*>(luces[i]->getEntidad())->getCortes();
				atenuacionFlash = dynamic_cast<TLuz*>(luces[i]->getEntidad())->getPuntualParametros();
				
			}else if(dynamic_cast<TLuz*>(luces[i]->getEntidad())->getType() == 2){					//Extraemos datos de las luces dirigidas
				directionDir = dynamic_cast<TLuz*>(luces[i]->getEntidad())->getDireccion();
				colorDir = dynamic_cast<TLuz*>(luces[i]->getEntidad())->getColor();
			}
		}
	
	//	TDatosEntidad datos;
		datos->view = view;
		datos->projection = projection;
		datos->camaraPosicion = camaraPosicion;
	
		datos->colorPuntual = colorPuntual;
		datos->posicionPuntual = posicionPuntual;
		datos->atenuacionPuntual = atenuacionPuntual;
		
		datos->colorDir = colorDir;
		datos->directionDir = directionDir;
	
		datos->directionFlash = directionFlash;
		datos->positionFlash = positionFlash;
		datos->colorFlash = colorFlash;
		datos->corteFlash = corteFlash;
		datos->atenuacionFlash = atenuacionFlash;
	
		datos->bounding = bounding;

	}

	if(entidad!=nullptr){
		if(entidad->getTipo() != 4 /* 4 = sky*/)
			entidad -> beginDraw(datos);
	}
	//para cada nodo hijo i
	for(int i=0; i<hijos.size(); i++){
		hijos[i]->draw(view, projection, camara, luces, bounding, datos);
	}

	if(entidad!=nullptr)
		entidad -> endDraw();
}
