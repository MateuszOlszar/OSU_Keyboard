/*
 * Button.h
 *
 *  Created on: May 10, 2024
 *      Author: matio
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_
#include "main.h"

typedef void (*callback)(void);

typedef enum ButtonState
{
	Idle,
	Pressed,
	DebouncePressed,
	DebounceDepressed,
	Release

}ButtonState;

struct Button
{
	GPIO_TypeDef* port;
	uint32_t pin;

	callback pressedAction;
	callback releaseAction;

	uint32_t releaseTick;
	ButtonState state;
};



enum ButtonNames {
	Key1,
	Key2,
	Key3,
	Key4,
	ENCODER_KEY,

};

#endif /* INC_BUTTON_H_ */
