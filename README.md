# SOR-Semáforos-1S-2021
Trabajo Práctico Semáforos primer semestre año 2021


En esta ocasión, no logre la resolución completa del trabajo practico, por cuestiones personales. Más allá del tiempo adicional que se nos brindó, no logre darle el tiempo correspondiente para resolver los problemas que se me fueron dando a la hora de avanzar con el trabajo práctico, principalmente me resto más lectura para implementar en C mi planificación .
Pseudocodigo planificando el armado del trabajo practico

4 equipos. (4 hilos)
1 solo sarten.
2 hornos.
1 salero.
Para armar sanguche:

1.	cortar 2 dientes de ajocortar 2 dientes de ajo

un puñado de perejil, con huevo.

sal a gusto.

agregar carne, empanar .

5 segundos tarda en cocinarse en la sarten.

Panes tardan 10m en hornearse.

Junto a lechuga fresca, tomate, cebolla morada y pepino recién cortados.

W = wait() ---    P()

S =  signal() ---- V()

Semaforos del hilo

Mezclar, poner_sal, cocinarMila, milaCocinada, panHorneado, cortados  todos iniciados en 0;

Semaforos intra-equipos:
sem_t sem_sarten  1
sem_t sem_sal  1
sem_t sem_horno  2


 
  CortarAjoYPer();   	V(mezclar);	
  
revolver();	P(mezc lar); V(poner_sal);                   

ponerCarne();	P(poner sal); V(cocinarMila);	P y V (sal);

Cocinar();	p(cocinarMila); v(milaCocinada);	P y V (Sarten); 

hornear();	V(panHorneado);	P y V (horno); 

CortarOtros();	V(cortados);	

Armado();	p(milaCocinada;panHorneado;cortados);	
		

Inconvenientes encontrados no resueltos. 
Armar semáforos intra-equipos.
Lograr leer la receta de un archivo txt
Guardar los resultados del programa en un archivo txt.
