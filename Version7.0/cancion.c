
#include "cancion.h"
#include <string.h>

//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

//------------------------------------------------------
// void InicializaEfecto (TipoEfecto *p_cancion)
//
// Descripcion: inicializa los parametros del objeto efecto.
// Ejemplo de uso:
//
// ...
//
// TipoEfecto efecto_demo;
//
// if ( InicializaEfecto (&efecto_demo, "DISPARO", frecuenciasDisparo, tiemposDisparo, 16) < 0 ) {
// 		printf("\n[ERROR!!!][InicializaEfecto]\n");
// 		fflush(stdout);
// 		}
//
// ...
//
//------------------------------------------------------
int InicializaMelodia (TipoMelodia *p_melodia, char *nombre, int *array_frecuencias, int *array_duraciones, int num_notas) {

 int i;

 strcpy(p_melodia->nombre,nombre);
 p_melodia->num_notas=num_notas;
 for (i=0; i<num_notas; i++) {
	 p_melodia->frecuencias[i]=array_frecuencias[i];
	 p_melodia->duraciones[i]=array_duraciones[i];
 }
	return p_melodia->num_notas;
}

// Procedimiento de inicializacion del objeto especifico
void InicializaCancion (TipoCancion *p_cancion) {
	p_cancion->posicion_nota_actual=0;

 //lo inicializamos en piTankGo1.c Inicializa Sistema()
// p_cancion->p_cancion=&(p_cancion->melodia_continua);

	p_cancion->frecuencia_nota_actual=p_cancion->p_melodia->frecuencias[p_cancion->posicion_nota_actual]; //Lo que se busca es la direccion del objeto anidado, para lo que sea
	p_cancion->duracion_nota_actual=p_cancion->p_melodia->duraciones[p_cancion->posicion_nota_actual];

	p_cancion->tmr_duracion_nota = tmr_new(timer_cancion_duracion_nota_actual_isr);
 tmr_startms(p_cancion->tmr_duracion_nota,p_cancion->duracion_nota_actual);
 softToneWrite(23,p_cancion->frecuencia_nota_actual);
}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaCancion (fsm_t* this) {

	int result = 0;

	piLock (PLAYER_FLAGS_KEY);
	result = (flags_cancion & FLAG_START_CANCION); // El 2o termino es una mascara, dara el resultado en decimal de los 32 bits.
	piUnlock (PLAYER_FLAGS_KEY);

	return result;
}

int CompruebaNewNota (fsm_t* this){

	int result = 0;

	piLock (PLAYER_FLAGS_KEY);
	result = !(flags_cancion & FLAG_PLAYER_END);
	piUnlock (PLAYER_FLAGS_KEY);

	return result;
}

int CompruebaNotaTimer (fsm_t* this) {

	int result = 0;

	piLock (PLAYER_FLAGS_KEY);
	result = (flags_cancion & FLAG_NOTA_TIMEOUT);
	piUnlock (PLAYER_FLAGS_KEY);

	return result;
}

int CompruebaFinalCancion (fsm_t* this) {

	int result = 0;

	piLock (PLAYER_FLAGS_KEY);
	result = (flags_cancion & FLAG_PLAYER_END);
	piUnlock (PLAYER_FLAGS_KEY);

	return result;
}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

void InicializaTono (fsm_t* this) {

	TipoCancion *p_cancion;
	p_cancion = (TipoCancion*) (this->user_data);
	p_cancion->p_melodia=&(p_cancion->melodia_continua);
	piLock (PLAYER_FLAGS_KEY);
	flags_cancion &= ~FLAG_START_CANCION;
	flags_cancion &= ~FLAG_NOTA_TIMEOUT;
	piUnlock (PLAYER_FLAGS_KEY);

	InicializaCancion(p_cancion);
}

void ComienzaNewNota (fsm_t* this) {

	TipoCancion *p_cancion;
	p_cancion = (TipoCancion*) (this->user_data);
	piLock (PLAYER_FLAGS_KEY);
	flags_cancion &= ~FLAG_PLAYER_END;
	piUnlock (PLAYER_FLAGS_KEY);

	tmr_startms(p_cancion->tmr_duracion_nota,p_cancion->duracion_nota_actual);
}

void ActualizaCancion (fsm_t* this) {

	TipoCancion *p_cancion;
	p_cancion = (TipoCancion*) (this->user_data);

	p_cancion->posicion_nota_actual++;
	p_cancion->frecuencia_nota_actual=p_cancion->p_melodia->frecuencias[p_cancion->posicion_nota_actual];
	p_cancion->duracion_nota_actual=p_cancion->p_melodia->duraciones[p_cancion->posicion_nota_actual];

	softToneWrite(23,p_cancion->frecuencia_nota_actual);

	piLock (PLAYER_FLAGS_KEY);
	flags_cancion &= ~FLAG_NOTA_TIMEOUT;
	if((p_cancion->posicion_nota_actual) == (p_cancion->p_melodia->num_notas)) {
		flags_cancion |= FLAG_PLAYER_END;
	}
	piUnlock (PLAYER_FLAGS_KEY);

}

void FinalCancion (fsm_t* this) {

	piLock (PLAYER_FLAGS_KEY);
	flags_cancion &= ~FLAG_PLAYER_END;
	flags_cancion |= FLAG_START_CANCION;
	piUnlock (PLAYER_FLAGS_KEY);

}

//------------------------------------------------------
// PROCEDIMIENTOS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

void timer_cancion_duracion_nota_actual_isr (union sigval value) {
	piLock (PLAYER_FLAGS_KEY);
	flags_cancion |= FLAG_NOTA_TIMEOUT;
	piUnlock (PLAYER_FLAGS_KEY);
}
