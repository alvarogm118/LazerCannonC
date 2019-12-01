/*
 * player.h
 *
 *  Created on: 21 de enero de 2019
 *      Author: FFM
 */

#ifndef CANCION_H_
#define CANCION_H_

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <softTone.h>

#include "piTankGoLib.h"

#define MAX_NUM_NOTAS 		2000
#define MAX_NUM_CHAR_NOMBRE	100

typedef struct {
	char nombre[MAX_NUM_CHAR_NOMBRE]; // String con el nombre del efecto
	int frecuencias[MAX_NUM_NOTAS]; // Array con las frecuencias de las notas del efecto
	int duraciones[MAX_NUM_NOTAS]; // Array con las duraciones de las notas del efecto
	int num_notas; // Numero de notas de que consta el efecto
} TipoMelodia;

typedef struct {
	int posicion_nota_actual;	// Valor correspondiente a la posicion de la nota actual en los arrays de frecuencias y duraciones
	int frecuencia_nota_actual; // Valor correspondiente a la frecuencia de la nota actual
	int duracion_nota_actual; // Valor correspondiente a la duracion de la nota actual

	TipoMelodia* p_melodia;

	TipoMelodia melodia_continua;

	tmr_t* tmr_duracion_nota;
} TipoCancion;

extern int flags_cancion;

// Prototipos de procedimientos de inicializacion de los objetos especificos
int InicializaMelodia (TipoMelodia *p_melodia, char *nombre, int *array_frecuencias, int *array_duraciones, int num_notas);
void InicializaCancion (TipoCancion *p_cancion);

// Prototipos de funciones de entrada
int CompruebaCancion (fsm_t* this);
int CompruebaNewNota (fsm_t* this);
int CompruebaNotaTimer (fsm_t* this);
int CompruebaFinalCancion (fsm_t* this);

// Prototipos de funciones de salida
void InicializaTono (fsm_t* this);
void ComienzaNewNota (fsm_t* this);
void ActualizaCancion (fsm_t* this);
void FinalCancion (fsm_t* this);

// Prototipos de procedimientos de atencion a las interrupciones
void timer_cancion_duracion_nota_actual_isr (union sigval value);

#endif /* PLAYER_H_ */
