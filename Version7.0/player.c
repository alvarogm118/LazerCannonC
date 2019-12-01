
#include "player.h"
#include <string.h>

//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

//------------------------------------------------------
// void InicializaEfecto (TipoEfecto *p_efecto)
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
int InicializaEfecto (TipoEfecto *p_efecto, char *nombre, int *array_frecuencias, int *array_duraciones, int num_notas) {

 int i;

 strcpy(p_efecto->nombre,nombre);
 p_efecto->num_notas=num_notas;
 for (i=0; i<num_notas; i++) {
	 p_efecto->frecuencias[i]=array_frecuencias[i];
	 p_efecto->duraciones[i]=array_duraciones[i];
 }

	return p_efecto->num_notas;
}


// Procedimiento de inicializacion del objeto especifico
void InicializaPlayer (TipoPlayer *p_player) {
 p_player->posicion_nota_actual=0;

 //lo inicializamos en piTankGo1.c Inicializa Sistema()
// p_player->p_efecto=&(p_player->efecto_disparo);

 p_player->frecuencia_nota_actual=p_player->p_efecto->frecuencias[p_player->posicion_nota_actual]; //Lo que se busca es la direccion del objeto anidado, para lo que sea
 p_player->duracion_nota_actual=p_player->p_efecto->duraciones[p_player->posicion_nota_actual];

 p_player->tmr_duracion_nota = tmr_new(timer_player_duracion_nota_actual_isr);
 tmr_startms(p_player->tmr_duracion_nota,p_player->duracion_nota_actual);
 softToneWrite(23,p_player->frecuencia_nota_actual);
 pinMode (26, OUTPUT); //LED impacto


 piLock (STD_IO_BUFFER_KEY);
 printf("[PLAYER][InicializaPlayer][NOTA %d][FREC %d][DURA %d]\n",p_player->posicion_nota_actual,
		 p_player->frecuencia_nota_actual,p_player->duracion_nota_actual);
 fflush(stdout);
 piUnlock (STD_IO_BUFFER_KEY);


}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaStartDisparo (fsm_t* this) {

	int result = 0;

	piLock (PLAYER_FLAGS_KEY);
	result = (flags_player & FLAG_START_DISPARO); // El 2o termino es una mascara, dara el resultado en decimal de los 32 bits.
	piUnlock (PLAYER_FLAGS_KEY);

	return result;
}

int CompruebaStartImpacto (fsm_t* this) {

	int result = 0;

	piLock (PLAYER_FLAGS_KEY);
	result = (flags_player & FLAG_START_IMPACTO);
	piUnlock (PLAYER_FLAGS_KEY);

	return result;
}

int CompruebaNuevaNota (fsm_t* this){

	int result = 0;

	piLock (PLAYER_FLAGS_KEY);
	result = !(flags_player & FLAG_PLAYER_END);
	piUnlock (PLAYER_FLAGS_KEY);

	return result;
}

int CompruebaNotaTimeout (fsm_t* this) {

	int result = 0;

	piLock (PLAYER_FLAGS_KEY);
	result = (flags_player & FLAG_NOTA_TIMEOUT);
	piUnlock (PLAYER_FLAGS_KEY);

	return result;
}

int CompruebaFinalEfecto (fsm_t* this) {

	int result = 0;

	piLock (PLAYER_FLAGS_KEY);
	result = (flags_player & FLAG_PLAYER_END);
	piUnlock (PLAYER_FLAGS_KEY);

	return result;
}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

void InicializaPlayDisparo (fsm_t* this) {

	TipoPlayer *p_player;
	p_player = (TipoPlayer*) (this->user_data);
	p_player->p_efecto=&(p_player->efecto_disparo);
	piLock (PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_START_DISPARO;
	flags_player &= ~FLAG_NOTA_TIMEOUT;
	piUnlock (PLAYER_FLAGS_KEY);

	piLock (STD_IO_BUFFER_KEY);
	printf("\n[PLAYER][InicializaPlayDisparo]\n");
	fflush(stdout);
	piUnlock (STD_IO_BUFFER_KEY);

	InicializaPlayer(p_player);
}

void InicializaPlayImpacto (fsm_t* this) {

	TipoPlayer *p_player;
	p_player = (TipoPlayer*) (this->user_data);
	p_player->p_efecto=&(p_player->efecto_impacto);
	piLock (PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_START_IMPACTO;
	flags_player &= ~FLAG_NOTA_TIMEOUT;
	piUnlock (PLAYER_FLAGS_KEY);

	digitalWrite (26, HIGH); //LED impacto

	piLock (STD_IO_BUFFER_KEY);
	printf("\n[PLAYER][InicializaPlayImpacto]\n");
	fflush(stdout);
	piUnlock (STD_IO_BUFFER_KEY);

	InicializaPlayer(p_player);

}

void ComienzaNuevaNota (fsm_t* this) {

	TipoPlayer *p_player;
	p_player = (TipoPlayer*) (this->user_data);
	piLock (PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_PLAYER_END;
	piUnlock (PLAYER_FLAGS_KEY);

	tmr_startms(p_player->tmr_duracion_nota,p_player->duracion_nota_actual);

	piLock (STD_IO_BUFFER_KEY);
	printf("[PLAYER][ComienzaNuevaNota][NOTA %d][FREC %d][DURA %d]\n",p_player->posicion_nota_actual,
			p_player->frecuencia_nota_actual,p_player->duracion_nota_actual);
	fflush(stdout);
	piUnlock (STD_IO_BUFFER_KEY);

}

void ActualizaPlayer (fsm_t* this) {

	TipoPlayer *p_player;
	p_player = (TipoPlayer*) (this->user_data);

	p_player->posicion_nota_actual++;
	p_player->frecuencia_nota_actual=p_player->p_efecto->frecuencias[p_player->posicion_nota_actual];
	p_player->duracion_nota_actual=p_player->p_efecto->duraciones[p_player->posicion_nota_actual];

	softToneWrite(23,p_player->frecuencia_nota_actual);

	piLock (PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_NOTA_TIMEOUT;
	if((p_player->posicion_nota_actual) == (p_player->p_efecto->num_notas)) {
		flags_player |= FLAG_PLAYER_END;
	}
	piUnlock (PLAYER_FLAGS_KEY);


	piLock (STD_IO_BUFFER_KEY);
	if((p_player->posicion_nota_actual) == (p_player->p_efecto->num_notas)) {
		printf("[PLAYER][ActualizaPlayer][REPRODUCIDAS TODAS LAS NOTAS]\n");
		fflush(stdout);
	}
	else { printf("[PLAYER][ActualizaPlayer][NUEVA NOTA %d DE %d]\n",p_player->posicion_nota_actual,p_player->p_efecto->num_notas);
		   fflush(stdout);
	}
	piUnlock (STD_IO_BUFFER_KEY);

}

void FinalEfecto (fsm_t* this) {

	piLock (PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_PLAYER_END;
	flags_player &= ~FLAG_START_DISPARO;
	piUnlock (PLAYER_FLAGS_KEY);

	digitalWrite (26, LOW); //LED impacto

	piLock (STD_IO_BUFFER_KEY);
	printf("[PLAYER][FinalMelodia]\n");
	fflush(stdout);
	piUnlock (STD_IO_BUFFER_KEY);

}

//------------------------------------------------------
// PROCEDIMIENTOS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

void timer_player_duracion_nota_actual_isr (union sigval value) {
	piLock (PLAYER_FLAGS_KEY);
	flags_player |= FLAG_NOTA_TIMEOUT;
	piUnlock (PLAYER_FLAGS_KEY);
}
