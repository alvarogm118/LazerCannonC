/*
 * player.h
 *
 *  Created on: 21 de enero de 2019
 *      Author: FFM
 */

#ifndef PLAYER_H_
#define PLAYER_H_

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
} TipoEfecto;

typedef struct {
	int posicion_nota_actual;	// Valor correspondiente a la posicion de la nota actual en los arrays de frecuencias y duraciones
	int frecuencia_nota_actual; // Valor correspondiente a la frecuencia de la nota actual
	int duracion_nota_actual; // Valor correspondiente a la duracion de la nota actual

	TipoEfecto* p_efecto; // Variable tipo puntero que apunta al efecto en particular que se quiere reproducir

	TipoEfecto efecto_disparo; // Efecto de disparo
	TipoEfecto efecto_impacto; // Efecto de impacto

	tmr_t* tmr_duracion_nota;
} TipoPlayer;


// Prototipos de procedimientos de inicializacion de los objetos especificos
int InicializaEfecto (TipoEfecto *p_efecto, char *nombre, int *array_frecuencias, int *array_duraciones, int num_notas);
void InicializaPlayer (TipoPlayer *p_player);

// Prototipos de funciones de entrada
int CompruebaStartDisparo (fsm_t* this);
int CompruebaStartImpacto (fsm_t* this);
int CompruebaNuevaNota (fsm_t* this);
int CompruebaNotaTimeout (fsm_t* this);
int CompruebaFinalEfecto (fsm_t* this);

// Prototipos de funciones de salida
void InicializaPlayDisparo (fsm_t* this);
void InicializaPlayImpacto (fsm_t* this);
void ComienzaNuevaNota (fsm_t* this);
void ActualizaPlayer (fsm_t* this);
void FinalEfecto (fsm_t* this);

// Prototipos de procedimientos de atencion a las interrupciones
void timer_player_duracion_nota_actual_isr (union sigval value);

#endif /* PLAYER_H_ */
