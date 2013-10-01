#include <stdlib.h>
#include <msp430.h>
#include "HAL_LCD.h"
#include "timer.h"

// P1.7 is button 1
#define BUTTON1_BIT BIT7  // Bit del boton
#define BUTTON1_SEL P1SEL // Seleccion de funciones
#define BUTTON1_DIR P1DIR // Direccion (I/O)
#define BUTTON1_IN  P1IN  // Registro para leer valores de input
#define BUTTON1_OUT P1OUT // Registro para setear valor de output
#define BUTTON1_REN P1REN // Resistencia pull-up o pull-down
#define BUTTON1_IE  P1IE  // Habilitacion de interrupciones
#define BUTTON1_IES P1IES // Flanco de subida o bajada para interrupciones
#define BUTTON1_IFG P1IFG // Flag de interrupciones

// P2.2 is button 2
#define BUTTON2_BIT BIT2  // Bit del boton
#define BUTTON2_SEL P2SEL // Seleccion de funciones
#define BUTTON2_DIR P2DIR // Direccion (I/O)
#define BUTTON2_IN  P2IN  // Registro para leer valores de input
#define BUTTON2_OUT P2OUT // Registro para setear valor de output
#define BUTTON2_REN P2REN // Resistencia pull-up o pull-down
#define BUTTON2_IE  P2IE  // Habilitacion de interrupciones
#define BUTTON2_IES P2IES // Flanco de subida o bajada para interrupciones
#define BUTTON2_IFG P2IFG // Flag de interrupciones


void button_init(void);
