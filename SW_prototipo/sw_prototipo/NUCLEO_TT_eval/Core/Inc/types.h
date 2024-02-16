/*
 * types.h
 *
 *  Created on: Feb 6, 2024
 *      Author: fede
 */

#ifndef INC_TYPES_H_
#define INC_TYPES_H_

// Este tipo de dato solo se utiliza para errores de propósito general, no de la aplicaicón en sí.
// Por ejemplo, cuando se pasa un puntero nulo como argument en una función, etc.
typedef enum
{
	OK = 0,
	ERROR_NULL_POINTER,
	ERROR_BUFFER_TOO_SMALL,
	ERROR_TIMEOUT
} status_t;

// Booleano auxiliar
typedef enum
{
	FALSE = 0,
	TRUE
} bool_t;


#endif /* INC_TYPES_H_ */
