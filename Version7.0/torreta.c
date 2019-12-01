
#include "torreta.h"

//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

void InicializaTorreta (TipoTorreta *p_torreta) {

		p_torreta->servo_x.incremento = SERVO_INCREMENTO;
		p_torreta->servo_y.incremento = SERVO_INCREMENTO;

		p_torreta->servo_x.minimo 	= SERVO_MINIMO;
		p_torreta->servo_y.minimo 	= SERVO_MINIMO;

		p_torreta->servo_x.maximo 	= SERVO_MAXIMO;
		p_torreta->servo_y.maximo 	= SERVO_MAXIMO;

		p_torreta->servo_x.inicio 	= SERVO_MINIMO + (SERVO_MAXIMO - SERVO_MINIMO)/2;
		p_torreta->servo_y.inicio 	= SERVO_MINIMO + (SERVO_MAXIMO - SERVO_MINIMO)/2;

		p_torreta->posicion.x	= p_torreta->servo_x.inicio;
		p_torreta->posicion.y 	= p_torreta->servo_y.inicio;

		if(p_torreta->posicion.x > p_torreta->servo_x.maximo)
			p_torreta->posicion.x = p_torreta->servo_x.maximo;
		if(p_torreta->posicion.y > p_torreta->servo_y.maximo)
			p_torreta->posicion.y = p_torreta->servo_y.maximo;

		if(p_torreta->posicion.x < p_torreta->servo_x.minimo)
			p_torreta->posicion.x = p_torreta->servo_x.minimo;
		if(p_torreta->posicion.y < p_torreta->servo_y.minimo)
			p_torreta->posicion.y = p_torreta->servo_y.minimo;

		softPwmCreate (SERVO_HORIZONTAL_PIN, p_torreta->servo_x.inicio, SERVO_PWM_RANGE); // Internamente ya hace: piHiPri (90) ;
		softPwmWrite(SERVO_HORIZONTAL_PIN, p_torreta->posicion.x);

		softPwmCreate (SERVO_VERTICAL_PIN, p_torreta->servo_y.inicio, SERVO_PWM_RANGE); // Internamente ya hace: piHiPri (90) ;
		softPwmWrite(SERVO_VERTICAL_PIN, p_torreta->posicion.y);
		pinMode(IR_TX_PIN, OUTPUT); // TX infrarrojo
		pinMode(IR_RX_PIN, INPUT); //RX infrarrojo
		pullUpDnControl(IR_RX_PIN, PUD_OFF);

		p_torreta->duracion_disparo = SHOOTING_PERIOD;
		p_torreta->tmr_duracion_disparo = tmr_new(timer_duracion_disparo_isr);

		wiringPiISR (IR_RX_PIN, INT_EDGE_RISING, impacto_isr);

		piLock (SYSTEM_FLAGS_KEY);
		flags_juego |= FLAG_SYSTEM_START;
		piUnlock (SYSTEM_FLAGS_KEY);
}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaComienzo (fsm_t* this) {
	int result = 0;

	piLock (SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_SYSTEM_START);
	piUnlock (SYSTEM_FLAGS_KEY);

	return result;
}

int CompruebaJoystickUp (fsm_t* this) {
	int result = 0;

	piLock (SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_JOYSTICK_UP);
	piUnlock (SYSTEM_FLAGS_KEY);

	return result;
}

int CompruebaJoystickDown (fsm_t* fsm_player){
	int result = 0;

	piLock (SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_JOYSTICK_DOWN);
	piUnlock (SYSTEM_FLAGS_KEY);

	return result;
}

int CompruebaJoystickLeft (fsm_t* this) {
	int result = 0;

	piLock (SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_JOYSTICK_LEFT);
	piUnlock (SYSTEM_FLAGS_KEY);

	return result;
}

int CompruebaJoystickRight (fsm_t* this) {
	int result = 0;

	piLock (SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_JOYSTICK_RIGHT);
	piUnlock (SYSTEM_FLAGS_KEY);

	return result;
}

int CompruebaTimeoutDisparo (fsm_t* this) {
	int result = 0;

	piLock (SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_SHOOT_TIMEOUT);
	piUnlock (SYSTEM_FLAGS_KEY);

	return result;
}

int CompruebaImpacto (fsm_t* this) {
	int result = 0;

	piLock (SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_TARGET_DONE);
	piUnlock (SYSTEM_FLAGS_KEY);

	return result;
}

int CompruebaTriggerButton (fsm_t* this) {
	int result = 0;

	piLock (SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_TRIGGER_BUTTON);
	piUnlock (SYSTEM_FLAGS_KEY);

	return result;
}

int CompruebaFinalJuego (fsm_t* this) {
	int result = 0;

	piLock (SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_SYSTEM_END);
	piUnlock (SYSTEM_FLAGS_KEY);

	return result;
}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

void ComienzaSistema (fsm_t* this) {
	piLock (SYSTEM_FLAGS_KEY);
	flags_juego &= ~FLAG_SYSTEM_START;
	piUnlock (SYSTEM_FLAGS_KEY);

	piLock (STD_IO_BUFFER_KEY);
	printf("\n[TORRETA][ComienzaSistema]\n");
	fflush(stdout);
	piUnlock (STD_IO_BUFFER_KEY);
}

void MueveTorretaArriba (fsm_t* this) {
	TipoTorreta *p_torreta;
	p_torreta = (TipoTorreta*)(this->user_data);

	piLock (SYSTEM_FLAGS_KEY);
	flags_juego &= (~FLAG_JOYSTICK_UP);
	piUnlock (SYSTEM_FLAGS_KEY);

	if(p_torreta->posicion.y + p_torreta->servo_y.incremento <= p_torreta->servo_y.maximo) {

		if(full == 1){
			p_torreta->posicion.y = p_torreta->servo_y.maximo - 1;
			full = 0;
		}
		else {
			p_torreta->posicion.y = p_torreta->posicion.y + p_torreta->servo_y.incremento;
		}

		softPwmWrite(SERVO_VERTICAL_PIN, p_torreta->posicion.y);

		piLock (STD_IO_BUFFER_KEY);
		printf("[TORRETA][POSICION]=[%d]\n", p_torreta->posicion.y);
		fflush(stdout);
		piUnlock (STD_IO_BUFFER_KEY);
	}
}

void MueveTorretaAbajo (fsm_t* this) {
	TipoTorreta *p_torreta;
	p_torreta = (TipoTorreta*)(this->user_data);

	piLock (SYSTEM_FLAGS_KEY);
	flags_juego &= (~FLAG_JOYSTICK_DOWN);
	piUnlock (SYSTEM_FLAGS_KEY);

	if(p_torreta->posicion.y - p_torreta->servo_y.incremento >= p_torreta->servo_y.minimo) {

		if(full == 1){
			p_torreta->posicion.y = p_torreta->servo_y.minimo + 1;
			full = 0;
		}
		else {
			p_torreta->posicion.y = p_torreta->posicion.y - p_torreta->servo_y.incremento;
		}

		softPwmWrite(SERVO_VERTICAL_PIN, p_torreta->posicion.y);

		piLock (STD_IO_BUFFER_KEY);
		printf("[TORRETA][POSICION]=[%d]\n", p_torreta->posicion.y);
		fflush(stdout);
		piUnlock (STD_IO_BUFFER_KEY);
	}
}

void MueveTorretaIzquierda (fsm_t* this) {
	TipoTorreta *p_torreta;
	p_torreta = (TipoTorreta*)(this->user_data);

	piLock (SYSTEM_FLAGS_KEY);
	flags_juego &= (~FLAG_JOYSTICK_LEFT);
	piUnlock (SYSTEM_FLAGS_KEY);

	if(p_torreta->posicion.x - p_torreta->servo_x.incremento >= p_torreta->servo_x.minimo) {

		if(full == 1){
			p_torreta->posicion.x = p_torreta->servo_x.minimo + 1;
			full = 0;
		}
		else {
			p_torreta->posicion.x = p_torreta->posicion.x - p_torreta->servo_x.incremento;
		}

		softPwmWrite(SERVO_HORIZONTAL_PIN, p_torreta->posicion.x);

		piLock (STD_IO_BUFFER_KEY);
		printf("[TORRETA][POSICION]=[%d]\n", p_torreta->posicion.x);
		fflush(stdout);
		piUnlock (STD_IO_BUFFER_KEY);
	}
}

void MueveTorretaDerecha (fsm_t* this) {
		TipoTorreta *p_torreta;
		p_torreta = (TipoTorreta*)(this->user_data);

		piLock (SYSTEM_FLAGS_KEY);
		flags_juego &= (~FLAG_JOYSTICK_RIGHT);
		piUnlock (SYSTEM_FLAGS_KEY);

		if(p_torreta->posicion.x + p_torreta->servo_x.incremento <= p_torreta->servo_x.maximo) {

			if(full == 1){
				p_torreta->posicion.x = p_torreta->servo_x.maximo - 1;
				full = 0;
			}
			else{
				p_torreta->posicion.x = p_torreta->posicion.x + p_torreta->servo_x.incremento;
			}

			softPwmWrite(SERVO_HORIZONTAL_PIN, p_torreta->posicion.x);

			piLock (STD_IO_BUFFER_KEY);
			printf("[TORRETA][POSICION]=[%d]\n", p_torreta->posicion.x);
			fflush(stdout);
			piUnlock (STD_IO_BUFFER_KEY);
		}
}

void DisparoIR (fsm_t* this) {
	TipoTorreta *p_torreta;
	p_torreta = (TipoTorreta*)(this->user_data);

	piLock (SYSTEM_FLAGS_KEY);
	flags_juego &= (~FLAG_TRIGGER_BUTTON);
	piUnlock (SYSTEM_FLAGS_KEY);

	tmr_startms(p_torreta->tmr_duracion_disparo, p_torreta->duracion_disparo);

	digitalWrite (IR_TX_PIN, HIGH); //Enciende LED

	disparo = 1;

	piLock (STD_IO_BUFFER_KEY);
	printf("\n[TORRETA][DisparoIR]\n");
	fflush(stdout);
	piUnlock (STD_IO_BUFFER_KEY);
}

void FinalDisparoIR (fsm_t* this) {
	piLock (SYSTEM_FLAGS_KEY);
	flags_juego &= (~FLAG_SHOOT_TIMEOUT);
	piUnlock (SYSTEM_FLAGS_KEY);

	digitalWrite (IR_TX_PIN, LOW); //Apaga LED

	disparo = 0;

	piLock (STD_IO_BUFFER_KEY);
	printf("\n[TORRETA][FinalDisparo_ObjetivoFallido]\n");
	fflush(stdout);
	piUnlock (STD_IO_BUFFER_KEY);
}

void ImpactoDetectado (fsm_t* this) {
	piLock (SYSTEM_FLAGS_KEY);
	flags_juego &= (~FLAG_TARGET_DONE);
	piUnlock (SYSTEM_FLAGS_KEY);

	disparo = 0;

	digitalWrite (IR_TX_PIN, LOW); //Apaga LED

	piLock (STD_IO_BUFFER_KEY);
	printf("\n[TORRETA][FinalDisparo_ObjetivoIMPACTADO]\n");
	fflush(stdout);
	piUnlock (STD_IO_BUFFER_KEY);

	piLock (SYSTEM_FLAGS_KEY);
	flags_juego &= (~FLAG_SHOOT_TIMEOUT);
	piUnlock (SYSTEM_FLAGS_KEY);
}

void FinalizaJuego (fsm_t* this) {
	piLock (SYSTEM_FLAGS_KEY);
	flags_juego &= (~FLAG_SYSTEM_END);
	piUnlock (SYSTEM_FLAGS_KEY);

	piLock (STD_IO_BUFFER_KEY);
	printf("\n[TORRETA][FinalJuego]\n");
	fflush(stdout);
	piUnlock (STD_IO_BUFFER_KEY);

	exit(0);
}

//------------------------------------------------------
// PROCEDIMIENTOS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

void timer_duracion_disparo_isr (union sigval value) {
	if (disparo > 0){
		piLock (SYSTEM_FLAGS_KEY);
		flags_juego |= FLAG_SHOOT_TIMEOUT;
		piUnlock (SYSTEM_FLAGS_KEY);
	}
}

void impacto_isr (void) {
	if(disparo > 0){
		piLock (SYSTEM_FLAGS_KEY);
		flags_juego |= FLAG_TARGET_DONE;
		piUnlock (SYSTEM_FLAGS_KEY);
		delay(10);
		piLock (PLAYER_FLAGS_KEY);
			flags_player |= FLAG_START_IMPACTO;
		piUnlock (PLAYER_FLAGS_KEY);
	}
}
