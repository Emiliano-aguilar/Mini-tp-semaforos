#include <stdio.h>      // libreria estandar
#include <stdlib.h>     // para usar exit y funciones de la libreria standard
#include <string.h>
#include <pthread.h>    // para usar threads
#include <semaphore.h>  // para usar semaforos
#include <unistd.h>


#define LIMITE 50

//creo estructura de semaforos 
struct semaforos {
		sem_t sem_mezclar;
	sem_t sem_PonerSal;
	sem_t sem_Empanar;


	sem_t sem_panHorneado;
	sem_t sem_milaCocinada;
	sem_t sem_cocinarMila;


	sem_t sem_sarten;
	sem_t sem_sal;
	sem_t sem_horno;
	//poner demas semaforos aqui
};

//creo los pasos con los ingredientes
struct paso {
   char accion [LIMITE];
   char ingredientes[4][LIMITE];
   
};

//creo los parametros de los hilos 
struct parametro {
 int equipo_param;
 struct semaforos semaforos_param;
 struct paso pasos_param[8];
};

//funcion para imprimir las acciones y los ingredientes de la accion
void* imprimirAccion(void *data, char *accionIn) {
	struct parametro *mydata = data;
	//calculo la longitud del array de pasos 
	int sizeArray = (int)( sizeof(mydata->pasos_param) / sizeof(mydata->pasos_param[0]));
	//indice para recorrer array de pasos 
	int i;
	for(i = 0; i < sizeArray; i ++){
		//pregunto si la accion del array es igual a la pasada por parametro (si es igual la funcion strcmp devuelve cero)
		if(strcmp(mydata->pasos_param[i].accion, accionIn) == 0){
		printf("\tEquipo %d - accion %s \n " , mydata->equipo_param, mydata->pasos_param[i].accion);
		//calculo la longitud del array de ingredientes
		int sizeArrayIngredientes = (int)( sizeof(mydata->pasos_param[i].ingredientes) / sizeof(mydata->pasos_param[i].ingredientes[0]) );
		//indice para recorrer array de ingredientes
		int h;
		printf("\tEquipo %d -----------ingredientes : ----------\n",mydata->equipo_param); 
			for(h = 0; h < sizeArrayIngredientes; h++) {
				//consulto si la posicion tiene valor porque no se cuantos ingredientes tengo por accion 
				if(strlen(mydata->pasos_param[i].ingredientes[h]) != 0) {
							printf("\tEquipo %d ingrediente  %d : %s \n",mydata->equipo_param,h,mydata->pasos_param[i].ingredientes[h]);
				}
			}
		}
	}
}

//funcion para tomar de ejemplo
void* cortar(void *data) {
	//creo el nombre de la accion de la funcion 
	char *accion = "cortar";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 20000 );
	//doy la señal a la siguiente accion (cortar me habilita mezclar)
    sem_post(&mydata->semaforos_param.sem_mezclar);
	
    pthread_exit(NULL);
}

void* revolver(void *data) {
	//creo el nombre de la accion de la funcion 
	char *accion = "revolver";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	sem_wait(&mydata->semaforos_param.sem_mezclar);
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 20000 );
	// Habilita el poner sal.
	sem_post(&mydata->semaforos_param.sem_PonerSal);


    pthread_exit(NULL);
}

void* ponerSal(void *data) {
	//creo el nombre de la accion de la funcion 
	char *accion = "poner Sal";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
		sem_wait(&mydata->semaforos_param.sem_PonerSal);


	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	usleep( 900000 );

	imprimirAccion(mydata,accion);

	// poner sal a la mezcla habilita a empanar la milanesa.
	sem_post(&mydata->semaforos_param.sem_Empanar);


    pthread_exit(NULL);
}

void* ponerCarne(void *data) {
	//creo el nombre de la accion de la funcion 
	char *accion = "Agregar Carne y empanar";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
			
		sem_wait(&mydata->semaforos_param.sem_Empanar);
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 20000 );

	// habilita a cocinar la milanesa
    sem_post(&mydata->semaforos_param.sem_cocinarMila);


    pthread_exit(NULL);
}

void* cocinar(void *data) {
	//creo el nombre de la accion de la funcion 
	char *accion = "Cocinar milanesa";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;

    sem_wait(&mydata->semaforos_param.sem_cocinarMila);

	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	usleep( 500000 );

	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo

	// Avisa que la milanesa ya esta cocinada.
	// habilita a que se pueda armar el sanguche
    sem_post(&mydata->semaforos_param.sem_milaCocinada);


    pthread_exit(NULL);
}

void* hornear(void *data) {
	//creo el nombre de la accion de la funcion 
	char *accion = "hornear panes";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;

	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
		usleep( 10000000 );

	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	//doy la señal a la siguiente accion (habilita armar sanguche)
    sem_post(&mydata->semaforos_param.sem_panHorneado);
	
    pthread_exit(NULL);
}

void* armado(void *data) {
	//creo el nombre de la accion de la funcion 
	char *accion = "Armar sanguche";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;

	    sem_wait(&mydata->semaforos_param.sem_panHorneado);
	    sem_wait(&mydata->semaforos_param.sem_milaCocinada);


	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 20000 );
	//doy la señal a la siguiente accion (habilita armar sanguche)
    sem_post(&mydata->semaforos_param.sem_panHorneado);
	
    pthread_exit(NULL);
}




void* ejecutarReceta(void *i) {
	
	//variables semaforos
	sem_t sem_mezclar;
	
	//crear variables semaforos aqui
	//sem_t sem_sarten;
	sem_t sem_sal;
	//sem_t sem_horno;



	sem_t sem_PonerSal;
	sem_t sem_Empanar;
	sem_t sem_panHorneado;
	sem_t sem_milaCocinada;
	sem_t sem_cocinarMila;

	
	//variables hilos
	pthread_t p1; 
	//crear variables hilos aqui
	pthread_t p2; 
	pthread_t p3;
	pthread_t p4;
	pthread_t p5;
	pthread_t p6;
	pthread_t p7;



	
	//numero del equipo (casteo el puntero a un int)
	int p = *((int *) i);
	
	printf("Ejecutando equipo %d \n", p);

	//reservo memoria para el struct
	struct parametro *pthread_data = malloc(sizeof(struct parametro));

	//seteo los valores al struct
	
	//seteo numero de grupo
	pthread_data->equipo_param = p;

	//seteo semaforos
	pthread_data->semaforos_param.sem_mezclar = sem_mezclar;
	//setear demas semaforos al struct aqui
	pthread_data->semaforos_param.sem_PonerSal= sem_PonerSal;

	pthread_data->semaforos_param.sem_Empanar= sem_Empanar;

		pthread_data->semaforos_param.sem_panHorneado = sem_panHorneado;
		pthread_data->semaforos_param.sem_milaCocinada = sem_milaCocinada;
		pthread_data->semaforos_param.sem_cocinarMila = sem_cocinarMila;



	//seteo las acciones y los ingredientes (Faltan acciones e ingredientes) ¿Se ve hardcodeado no? ¿Les parece bien?	

	// corto el ajo y perejil	
	strcpy(pthread_data->pasos_param[1].accion, "cortar");

	strcpy(pthread_data->pasos_param[1].ingredientes[0], "ajo");
	strcpy(pthread_data->pasos_param[1].ingredientes[1], "perejil");

	// mezclo lo cortado del ajo y perejil con el huevo
	strcpy(pthread_data->pasos_param[2].accion, "revolver");
	strcpy(pthread_data->pasos_param[2].ingredientes[0], "huevo");
	strcpy(pthread_data->pasos_param[2].ingredientes[1], "Cortado de ajo y perejil");

	// Agrego sal a la mezcla
	strcpy(pthread_data->pasos_param[3].accion, "poner Sal");
	strcpy(pthread_data->pasos_param[3].ingredientes[0], "sal");
	strcpy(pthread_data->pasos_param[3].ingredientes[1], "Mezcla de huevo, ajo y perejil");





	// agrego la carne
	strcpy(pthread_data->pasos_param[4].accion, "Agregar Carne y empanar");
	strcpy(pthread_data->pasos_param[4].ingredientes[0], "carne");
	strcpy(pthread_data->pasos_param[4].ingredientes[1], "Mezcla de huevo, ajo y perejil , sal");





	// Cocinar en sarten
	strcpy(pthread_data->pasos_param[5].accion, "Cocinar milanesa");
	strcpy(pthread_data->pasos_param[5].ingredientes[0], "Milaneza ya preparada.");



	// honear panes
	strcpy(pthread_data->pasos_param[6].accion, "hornear panes");
	strcpy(pthread_data->pasos_param[6].ingredientes[0], "pan");



	// cortar lechuga , tomate , cebolla morada y pepino
	strcpy(pthread_data->pasos_param[7].accion, "cortar");
	strcpy(pthread_data->pasos_param[7].ingredientes[0], "lechuga");
	strcpy(pthread_data->pasos_param[7].ingredientes[1], "Tomate");
	strcpy(pthread_data->pasos_param[7].ingredientes[2], "Cebolla morada");
	strcpy(pthread_data->pasos_param[7].ingredientes[3], "Pepino");



	// armar todo
	strcpy(pthread_data->pasos_param[0].accion, "Armar sanguche");
	strcpy(pthread_data->pasos_param[0].ingredientes[0], "Milanesa cocinada");
	strcpy(pthread_data->pasos_param[0].ingredientes[1], "Panes honeados");
	strcpy(pthread_data->pasos_param[0].ingredientes[2], "Luchuga,tomates ,cebolla morada y pepinos cortados");

	//inicializo los semaforos

	sem_init(&(pthread_data->semaforos_param.sem_mezclar),0,0);
	//inicializar demas semaforos aqui
	sem_init(&(pthread_data->semaforos_param.sem_PonerSal),0,0);
//	sem_init(&(pthread_data->semaforos_param.sem_sarten),0,0);	
	sem_init(&(pthread_data->semaforos_param.sem_panHorneado),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_milaCocinada),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_cocinarMila),0,0);


//	sem_init(&(pthread_data->semaforos_param.sem_horno),0,0);



	//creo los hilos a todos les paso el struct creado (el mismo a todos los hilos) ya que todos comparten los semaforos 
    int rc;
    rc = pthread_create(&p1,                           //identificador unico
                            NULL,                          //atributos del thread
                                cortar,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
	    rc = pthread_create(&p2,                           //identificador unico
                            NULL,                          //atributos del thread
                                revolver,             //funcion a ejecutar
                                pthread_data);


 
	    rc = pthread_create(&p3,                           //identificador unico
                            NULL,                          //atributos del thread
                                ponerSal,             //funcion a ejecutar
                                pthread_data); 

	    rc = pthread_create(&p4,                           //identificador unico
                            NULL,                          //atributos del thread
                                ponerCarne,             //funcion a ejecutar
                                pthread_data); 
	    rc = pthread_create(&p5,                           //identificador unico
                            NULL,                          //atributos del thread
                                cocinar,             //funcion a ejecutar
                                pthread_data); 
	    rc = pthread_create(&p6,                           //identificador unico
                            NULL,                          //atributos del thread
                                hornear,             //funcion a ejecutar
                                pthread_data);
	    rc = pthread_create(&p7,                           //identificador unico
                            NULL,                          //atributos del thread
                                armado,             //funcion a ejecutar
                                pthread_data);









	//crear demas hilos aqui
	
	
	//join de todos los hilos
	pthread_join (p1,NULL);
	//crear join de demas hilos
	pthread_join (p2,NULL);
	pthread_join (p3,NULL);
	pthread_join (p4,NULL);
	pthread_join (p5,NULL);
	pthread_join (p6,NULL);
	pthread_join (p7,NULL);





	//valido que el hilo se alla creado bien 
    if (rc){
       printf("Error:unable to create thread, %d \n", rc);
       exit(-1);
     }

	 
	//destruccion de los semaforos 
	sem_destroy(&sem_mezclar);

	//destruir demas semaforos 
	
	//salida del hilo
	 pthread_exit(NULL);
}


int main ()
{
	//creo los nombres de los equipos 
	int rc;
	int *equipoNombre1 =malloc(sizeof(*equipoNombre1));
	int *equipoNombre2 =malloc(sizeof(*equipoNombre2));
	int *equipoNombre3 =malloc(sizeof(*equipoNombre3));
	int *equipoNombre4 =malloc(sizeof(*equipoNombre4));

//faltan equipos
  
	*equipoNombre1 = 1;
	*equipoNombre2 = 2;
	*equipoNombre3 = 3;
	*equipoNombre4 = 4;

	//creo las variables los hilos de los equipos
	pthread_t equipo1; 
	pthread_t equipo2;
	pthread_t equipo3; 
	pthread_t equipo4; 
//faltan hilos
  
	//inicializo los hilos de los equipos
    rc = pthread_create(&equipo1,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre1); 

    rc = pthread_create(&equipo2,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre2);
    rc = pthread_create(&equipo3,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre3);

    rc = pthread_create(&equipo4,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre4);

  //faltn inicializaciones


   if (rc){
       printf("Error:unable to create thread, %d \n", rc);
       exit(-1);
     } 

	//join de todos los hilos
	pthread_join (equipo1,NULL);
	pthread_join (equipo2,NULL);
	pthread_join (equipo3,NULL);
	pthread_join (equipo4,NULL);


//.. faltan joins


    pthread_exit(NULL);
}


//Para compilar:   gcc subwayArgento.c -o ejecutable -lpthread
//Para ejecutar:   ./ejecutable
