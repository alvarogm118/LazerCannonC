#include "teclado.h"

TipoTeclado teclado;

int debounceTime[NUM_ROWS] = {0,0,0,0}; // Timeout to avoid bouncing after pin event

char tecladoTL04[4][4] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'}
};

int initialize(TipoTeclado *p_teclado) {

	// Comenzamos excitacion por primera columna
	p_teclado->columna_actual = COL_1;

	// Inicialmente no hay tecla pulsada
	p_teclado->teclaPulsada.col = -1;
	p_teclado->teclaPulsada.row = -1;

	pinMode (TECLADO_ROW_1, INPUT);
	pullUpDnControl(TECLADO_ROW_1, PUD_DOWN);
	wiringPiISR (TECLADO_ROW_1, INT_EDGE_RISING, row_1_isr);

	pinMode (TECLADO_ROW_2, INPUT);
	pullUpDnControl(TECLADO_ROW_2, PUD_DOWN);
	wiringPiISR (TECLADO_ROW_2, INT_EDGE_RISING, row_2_isr);

	pinMode (TECLADO_ROW_3, INPUT);
	pullUpDnControl(TECLADO_ROW_3, PUD_DOWN);
	wiringPiISR (TECLADO_ROW_3, INT_EDGE_RISING, row_3_isr);

	pinMode (TECLADO_ROW_4, INPUT);
	pullUpDnControl(TECLADO_ROW_4, PUD_DOWN);
	wiringPiISR (TECLADO_ROW_4, INT_EDGE_RISING, row_4_isr);

	pinMode (TECLADO_COL_1, OUTPUT);
	digitalWrite (TECLADO_COL_1, HIGH);

	pinMode (TECLADO_COL_2, OUTPUT);
	digitalWrite (TECLADO_COL_2, LOW);

	pinMode (TECLADO_COL_3, OUTPUT);
	digitalWrite (TECLADO_COL_3, LOW);

	pinMode (TECLADO_COL_4, OUTPUT);
	digitalWrite (TECLADO_COL_4, LOW);

	p_teclado->tmr_duracion_columna = tmr_new (timer_duracion_columna_isr);
	tmr_startms((tmr_t*)(p_teclado->tmr_duracion_columna), COL_REFRESH_TIME);

	printf("\nSystem init complete! keypad ready to process the code!!!\n");
	fflush(stdout);

	return 0;
}
int CompruebaColumnTimeout (fsm_t* this) {
	int result = 0;
	piLock (TECLADO_FLAGS_KEY);
	result = (flags_teclado & FLAG_TMR_TIMEOUT);
	piUnlock (TECLADO_FLAGS_KEY);
	return result;
}

void row_1_isr (void) {
	// Pin event (key / button event) debouncing procedure
	if (millis () < debounceTime[ROW_1]) {
		debounceTime[ROW_1] = millis () + DEBOUNCE_TIME ;
		return;
	}

	piLock (TECLADO_FLAGS_KEY);

	teclado.teclaPulsada.row = ROW_1;
	teclado.teclaPulsada.col = teclado.columna_actual;

	flags_teclado |= FLAG_KEY_PRESSED;

	piUnlock (TECLADO_FLAGS_KEY);

	while (digitalRead (TECLADO_ROW_1) == HIGH){
		delay(1);
	}
	debounceTime[ROW_1] = millis () + DEBOUNCE_TIME ;
}

void row_2_isr (void) {
	// Pin event (key / button event) debouncing procedure
	if (millis () < debounceTime[ROW_2]) {
		debounceTime[ROW_2] = millis () + DEBOUNCE_TIME ;
		return;
	}

	piLock (TECLADO_FLAGS_KEY);

	teclado.teclaPulsada.row = ROW_2;
	teclado.teclaPulsada.col = teclado.columna_actual;

	flags_teclado |= FLAG_KEY_PRESSED;

	piUnlock (TECLADO_FLAGS_KEY);

	while (digitalRead (TECLADO_ROW_2) == HIGH){
		delay(1);
	}
	debounceTime[ROW_2] = millis () + DEBOUNCE_TIME ;
}

void row_3_isr (void) {
	// Pin event (key / button event) debouncing procedure
	if (millis () < debounceTime[ROW_3]) {
		debounceTime[ROW_3] = millis () + DEBOUNCE_TIME ;
		return;
	}

	piLock (TECLADO_FLAGS_KEY);

	teclado.teclaPulsada.row = ROW_3;
	teclado.teclaPulsada.col = teclado.columna_actual;

	flags_teclado |= FLAG_KEY_PRESSED;

	piUnlock (TECLADO_FLAGS_KEY);

	while (digitalRead (TECLADO_ROW_3) == HIGH){
		delay(1);
	}
	debounceTime[ROW_3] = millis () + DEBOUNCE_TIME ;
}

void row_4_isr (void) {
	// Pin event (key / button event) debouncing procedure
	if (millis () < debounceTime[ROW_4]) {
		debounceTime[ROW_4] = millis () + DEBOUNCE_TIME ;
		return;
	}

	piLock (TECLADO_FLAGS_KEY);

	teclado.teclaPulsada.row = ROW_4;
	teclado.teclaPulsada.col = teclado.columna_actual;

	flags_teclado |= FLAG_KEY_PRESSED;

	piUnlock (TECLADO_FLAGS_KEY);

	while (digitalRead (TECLADO_ROW_4) == HIGH){
		delay(1);
	}
	debounceTime[ROW_4] = millis () + DEBOUNCE_TIME ;
}

void col_1 (fsm_t* this) {
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	piLock (TECLADO_FLAGS_KEY);

	digitalWrite (TECLADO_COL_4, LOW);
	digitalWrite (TECLADO_COL_1, HIGH);
	digitalWrite (TECLADO_COL_2, LOW);
	digitalWrite (TECLADO_COL_3, LOW);

	flags_teclado &= (~FLAG_TMR_TIMEOUT);

	p_teclado->columna_actual = COL_1;

	piUnlock (TECLADO_FLAGS_KEY);

	tmr_startms((tmr_t*)(p_teclado->tmr_duracion_columna), COL_REFRESH_TIME);
}

void col_2 (fsm_t* this) {
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	piLock (TECLADO_FLAGS_KEY);

	digitalWrite (TECLADO_COL_1, LOW);
	digitalWrite (TECLADO_COL_2, HIGH);
	digitalWrite (TECLADO_COL_3, LOW);
	digitalWrite (TECLADO_COL_4, LOW);

	flags_teclado &= (~FLAG_TMR_TIMEOUT);

	p_teclado->columna_actual = COL_2;

	piUnlock (TECLADO_FLAGS_KEY);

	tmr_startms((tmr_t*)(p_teclado->tmr_duracion_columna), COL_REFRESH_TIME);
}

void col_3 (fsm_t* this) {
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	piLock (TECLADO_FLAGS_KEY);

	digitalWrite (TECLADO_COL_2, LOW);
	digitalWrite (TECLADO_COL_3, HIGH);
	digitalWrite (TECLADO_COL_4, LOW);
	digitalWrite (TECLADO_COL_1, LOW);

	flags_teclado &= (~FLAG_TMR_TIMEOUT);

	p_teclado->columna_actual = COL_3;

	piUnlock (TECLADO_FLAGS_KEY);

	tmr_startms((tmr_t*)(p_teclado->tmr_duracion_columna), COL_REFRESH_TIME);
}

void col_4 (fsm_t* this) {
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	piLock (TECLADO_FLAGS_KEY);

	digitalWrite (TECLADO_COL_3, LOW);
	digitalWrite (TECLADO_COL_4, HIGH);
	digitalWrite (TECLADO_COL_1, LOW);
	digitalWrite (TECLADO_COL_2, LOW);

	flags_teclado &= (~FLAG_TMR_TIMEOUT);

	p_teclado->columna_actual = COL_4;

	piUnlock (TECLADO_FLAGS_KEY);

	tmr_startms((tmr_t*)(p_teclado->tmr_duracion_columna), COL_REFRESH_TIME);
}

int key_pressed (fsm_t* this) {
	int result = 0;

	piLock (TECLADO_FLAGS_KEY);
	result = (flags_teclado & FLAG_KEY_PRESSED);
	piUnlock (TECLADO_FLAGS_KEY);

	return result;
}

void timer_duracion_columna_isr (union sigval value) {
	piLock (TECLADO_FLAGS_KEY);
	flags_teclado |= FLAG_TMR_TIMEOUT;
	piUnlock (TECLADO_FLAGS_KEY);
}

void process_key (fsm_t* this) {
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	piLock (TECLADO_FLAGS_KEY);

	flags_teclado &= (~FLAG_KEY_PRESSED);

	switch(tecladoTL04[p_teclado->teclaPulsada.row][p_teclado->teclaPulsada.col]){
		case '4':
				piLock (SYSTEM_FLAGS_KEY);
					flags_juego |= FLAG_JOYSTICK_LEFT;
				piUnlock (SYSTEM_FLAGS_KEY);

				piLock (STD_IO_BUFFER_KEY);
				printf("\nIzquierda\n");
				fflush(stdout);
				piUnlock (STD_IO_BUFFER_KEY);

			break;

		case '*':
				full = 1;
				piLock (SYSTEM_FLAGS_KEY);
					flags_juego |= FLAG_JOYSTICK_LEFT;
				piUnlock (SYSTEM_FLAGS_KEY);

				piLock (STD_IO_BUFFER_KEY);
				printf("\nIzquierda Full\n");
				fflush(stdout);
				piUnlock (STD_IO_BUFFER_KEY);

			break;

		case '5':
				piLock (SYSTEM_FLAGS_KEY);
					flags_juego |= FLAG_TRIGGER_BUTTON;
				piUnlock (SYSTEM_FLAGS_KEY);
				piLock (PLAYER_FLAGS_KEY);
					flags_player |= FLAG_START_DISPARO;
					flags_player |= FLAG_NOTA_TIMEOUT;
				piUnlock (PLAYER_FLAGS_KEY);

				piLock (STD_IO_BUFFER_KEY);
				printf("\nDisparo\n");
				fflush(stdout);
				piUnlock (STD_IO_BUFFER_KEY);

			break;

		case '2':
				piLock (SYSTEM_FLAGS_KEY);
					flags_juego |= FLAG_JOYSTICK_UP;
				piUnlock (SYSTEM_FLAGS_KEY);

				piLock (STD_IO_BUFFER_KEY);
				printf("\nArriba\n");
				fflush(stdout);
				piUnlock (STD_IO_BUFFER_KEY);

			break;

		case 'A':
				full = 1;
				piLock (SYSTEM_FLAGS_KEY);
					flags_juego |= FLAG_JOYSTICK_UP;
				piUnlock (SYSTEM_FLAGS_KEY);

				piLock (STD_IO_BUFFER_KEY);
				printf("\nArriba Full\n");
				fflush(stdout);
				piUnlock (STD_IO_BUFFER_KEY);

			break;

		case '8':
				piLock (SYSTEM_FLAGS_KEY);
					flags_juego |= FLAG_JOYSTICK_DOWN;
				piUnlock (SYSTEM_FLAGS_KEY);

				piLock (STD_IO_BUFFER_KEY);
				printf("\nAbajo\n");
				fflush(stdout);
				piUnlock (STD_IO_BUFFER_KEY);

			break;

		case 'B':
				full = 1;
				piLock (SYSTEM_FLAGS_KEY);
					flags_juego |= FLAG_JOYSTICK_DOWN;
				piUnlock (SYSTEM_FLAGS_KEY);

				piLock (STD_IO_BUFFER_KEY);
				printf("\nAbajo Full\n");
				fflush(stdout);
				piUnlock (STD_IO_BUFFER_KEY);

			break;

		case '6':
				piLock (SYSTEM_FLAGS_KEY);
					flags_juego |= FLAG_JOYSTICK_RIGHT;
				piUnlock (SYSTEM_FLAGS_KEY);

				piLock (STD_IO_BUFFER_KEY);
				printf("\nDerecha\n");
				fflush(stdout);
				piUnlock (STD_IO_BUFFER_KEY);

			break;

		case '#':
				full = 1;
				piLock (SYSTEM_FLAGS_KEY);
					flags_juego |= FLAG_JOYSTICK_RIGHT;
				piUnlock (SYSTEM_FLAGS_KEY);

				piLock (STD_IO_BUFFER_KEY);
				printf("\nDerecha Full\n");
				fflush(stdout);
				piUnlock (STD_IO_BUFFER_KEY);

			break;

		case '9':

			if (disparo > 0){
				piLock (PLAYER_FLAGS_KEY);
					flags_player |= FLAG_START_IMPACTO;
				piUnlock (PLAYER_FLAGS_KEY);
				piLock (SYSTEM_FLAGS_KEY);
					flags_juego |= FLAG_TARGET_DONE;
				piUnlock (SYSTEM_FLAGS_KEY);
			}
			else {
				printf("\nNo se ha disparado...\n");
				fflush(stdout);
			}
			break;

		case '0':
				piLock (SYSTEM_FLAGS_KEY);
					flags_juego |= FLAG_SYSTEM_END;
				piUnlock (SYSTEM_FLAGS_KEY);

				piLock (STD_IO_BUFFER_KEY);
				printf("\nFin\n");
				fflush(stdout);
				piUnlock (STD_IO_BUFFER_KEY);

			break;

		default:
			printf("\nERROR!!!! invalid number of column (%d)!!!\n", p_teclado->teclaPulsada.col);
			fflush(stdout);

			p_teclado->teclaPulsada.row = -1;
			p_teclado->teclaPulsada.col = -1;

			break;
	}

	piUnlock (TECLADO_FLAGS_KEY);

}
