/*
 * teclado.h
 *
 *  Created on: 19 de Feb. de 2019
 *      Author: FFM
 */

#ifndef _TECLADO_H_
#define _TECLADO_H_

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>

#include "fsm.h"
#include "tmr.h"
#include "piTankGoLib.h"

#define CLK_MS 10
#define COL_REFRESH_TIME 25

#define FLAG_KEY_PRESSED 0x4000
#define FLAG_TMR_TIMEOUT 0x8000

#define NUM_COLS 4
#define NUM_ROWS 4

enum columns_values {
	COL_1,
	COL_2,
	COL_3,
	COL_4,
};

enum rows_values {
	ROW_1,
	ROW_2,
	ROW_3,
	ROW_4
};

enum column_state {
	KEY_COL_1,
	KEY_COL_2,
	KEY_COL_3,
	KEY_COL_4
};

enum keypad_state {
	KEY_WAITING
};

// Distribucion de pines GPIO empleada para el teclado
#define	TECLADO_ROW_1	5
#define	TECLADO_ROW_2	6
#define	TECLADO_ROW_3	12
#define	TECLADO_ROW_4	13

#define	TECLADO_COL_1	0
#define	TECLADO_COL_2	1
#define	TECLADO_COL_3	2
#define	TECLADO_COL_4	3

//#define JOY_PIN_UP	0
//#define JOY_PIN_DOWN	0
//#define JOY_PIN_LEFT	0
//#define JOY_PIN_RIGHT	0
//#define JOY_PIN_CENTER 0

typedef struct {
	int col;
	int row;
} TipoTecla;

typedef struct {
	int columna_actual;
	tmr_t* tmr_duracion_columna;
	TipoTecla teclaPulsada;
} TipoTeclado;

extern TipoTeclado teclado;

//inicializa
int initialize(TipoTeclado *p_teclado);
//procedimientos
int CompruebaColumnTimeout (fsm_t* this);
void row_1_isr (void);
void row_2_isr (void);
void row_3_isr (void);
void row_4_isr (void);
void col_1 (fsm_t* this);
void col_2 (fsm_t* this);
void col_3 (fsm_t* this);
void col_4 (fsm_t* this);

int key_pressed (fsm_t* this);
//Interrupcion
void timer_duracion_columna_isr (union sigval value);

void process_key (fsm_t* this);

#endif /* _TECLADO_TL04_H_ */
