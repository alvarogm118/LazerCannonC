
#ifndef _PITANKGOLIB_H_
#define _PITANKGOLIB_H_

#include <stdio.h>
#include <string.h> //metida por nosotros dudasss
#include <errno.h>

#include "fsm.h"
#include "tmr.h"

// ATENCION: Valores a modificar por el alumno
// INTERVALO DE GUARDA ANTI-REBOTES
#define	DEBOUNCE_TIME	150
// DURACION DISPARO IR, duracion del tiempo que esta encendido el infrarrojo de la torreta
#define SHOOTING_PERIOD 5000

// CLAVES PARA MUTEX
// ATENCION: Valores a modificar por el alumno
#define	SYSTEM_FLAGS_KEY	0
#define	PLAYER_FLAGS_KEY	1
#define	TECLADO_FLAGS_KEY	2
#define	STD_IO_BUFFER_KEY	3

//TORRETA
// PWM SW: el SO provoca que cuanto más alta sea la frecuencia de la señal PWM de control menos estable resulta esta (i.e. cimbreo del servo)
// Interesa una frecuencia para la señal de control relativamente baja para minimizar el cimbreo
// El problema es que tampoco puede ser demasiado baja o de lo contrario el control del servo se resiente y su respuesta es peor
// (más lenta ante variaciones rápidas del ciclo de trabajo de la señal de control)
#define SERVO_PWM_RANGE		400 // 100 * 400 = 40,000 �S = 40 ms // 25 Hz

#define SERVO_INCREMENTO	2
#define SERVO_MINIMO		9
#define SERVO_MAXIMO		22



// Distribucion de pines GPIO empleada para el enlace IR
// ATENCION: Valores a modificar por el alumno
#define	IR_TX_PIN		24
#define	IR_RX_PIN		16

// Distribucion de pines GPIO empleada para la reproducción de efectos
// ATENCION: Valores a modificar por el alumno
#define PLAYER_PWM_PIN 	0

// Distribucion de pines GPIO empleada para el control de los servos
// ATENCION: Valores a modificar por el alumno
#define	SERVO_VERTICAL_PIN		22
#define	SERVO_HORIZONTAL_PIN	18

// FLAGS FSM CONTROL DE JUEGO Y TORRETA
// ATENCION: Valores a modificar por el alumno
#define FLAG_SYSTEM_START 		0x01
#define FLAG_JOYSTICK_UP 		0x02
#define FLAG_JOYSTICK_DOWN 		0x04
#define FLAG_JOYSTICK_LEFT		0x08
#define FLAG_JOYSTICK_RIGHT		0x10
#define FLAG_TRIGGER_BUTTON		0x20
#define FLAG_SHOOT_TIMEOUT		0x40
#define FLAG_TARGET_DONE		0x80
#define FLAG_SYSTEM_END			0x100

// FLAGS FSM REPRODUCCION DE EFECTOS DE SONIDO
// ATENCION: Valores a modificar por el alumno
#define FLAG_START_DISPARO 		0x200
#define FLAG_START_IMPACTO 		0x400
#define FLAG_PLAYER_STOP 		0x800
#define FLAG_PLAYER_END			0x1000
#define FLAG_NOTA_TIMEOUT		0x2000

#define FLAG_START_CANCION		0x10000

extern int flags_juego;
extern int flags_player;
extern int flags_teclado;
extern int flags_cancion;

extern int disparo;
extern int full;

extern int frecuenciaDespacito[];
extern int tiempoDespacito[];
extern int frecuenciaGOT[];
extern int tiempoGOT[];
extern int frecuenciaTetris[];
extern int tiempoTetris[];
extern int frecuenciaStarwars[];
extern int tiempoStarwars[];

extern int frecuenciasDisparo[];
extern int tiemposDisparo[];
extern int frecuenciasImpacto[];
extern int tiemposImpacto[];

#endif /* _PITANKGOLIB_H_ */
