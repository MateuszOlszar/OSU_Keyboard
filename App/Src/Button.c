/*
 * Button.c
 *
 *  Created on: May 10, 2024
 *      Author: matio
 */
#include "Button.h"
#include "main.h"
#include "usbd_hid.h"

extern USBD_HandleTypeDef hUsbDeviceFS;

typedef struct {

	uint8_t MODIFIER;
	uint8_t RESERVED;
	uint8_t KEYCODE1;
	uint8_t KEYCODE2;
	uint8_t KEYCODE3;
	uint8_t KEYCODE4;
	uint8_t KEYCODE5;
	uint8_t KEYCODE6;
} KeyboardHID;

KeyboardHID keyboardhid = { 0, 0, 0, 0, 0, 0, 0, 0 };

void Button1Pressed(void) {
	keyboardhid.KEYCODE1 = 0x32;
	USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*) &keyboardhid,
			sizeof(keyboardhid));
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 1);
	return;
}

void Button1Released(void) {
	keyboardhid.KEYCODE1 = 0x00;
	USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*) &keyboardhid,
			sizeof(keyboardhid));
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 0);
	return;
}

void Button2Pressed(void) {
	keyboardhid.KEYCODE2 = 0x29;
	USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*) &keyboardhid,
			sizeof(keyboardhid));
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 1);
	return;
}

void Button2Released(void) {
	keyboardhid.KEYCODE2 = 0x00;
	USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*) &keyboardhid,
			sizeof(keyboardhid));
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 0);
	return;
}

void Button3Pressed(void) {
	keyboardhid.KEYCODE3 = 0x13;
	USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*) &keyboardhid,
			sizeof(keyboardhid));
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 1);
	return;
}

void Button3Released(void) {
	keyboardhid.KEYCODE3 = 0x00;
	USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*) &keyboardhid,
			sizeof(keyboardhid));
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 0);
	return;
}

void Button4Pressed(void) {
	keyboardhid.KEYCODE4 = 0x12;
	USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*) &keyboardhid,
			sizeof(keyboardhid));
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 1);
	return;
}

void Button4Released(void) {
	keyboardhid.KEYCODE4 = 0x00;
	USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*) &keyboardhid,
			sizeof(keyboardhid));
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 0);
	return;
}

void Button5Pressed(void) {
	keyboardhid.KEYCODE5 = 0x06;
	USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*) &keyboardhid,
			sizeof(keyboardhid));
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 1);
	return;
}

void Button5Released(void) {
	keyboardhid.KEYCODE5 = 0x00;
	USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*) &keyboardhid,
			sizeof(keyboardhid));
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 0);
	return;
}
struct Button buttons[5] = { {

.port = KEY1_GPIO_Port, .pin = KEY1_Pin, .pressedAction = Button1Pressed,
		.releaseAction = Button1Released, .state = Idle

},

{

.port = KEY2_GPIO_Port, .pin = KEY2_Pin, .pressedAction = Button2Pressed,
		.releaseAction = Button2Released, .state = Idle

}, {

.port = KEY3_GPIO_Port, .pin = KEY3_Pin, .pressedAction = Button3Pressed,
		.releaseAction = Button3Released, .state = Idle

}, {

.port = KEY4_GPIO_Port, .pin = KEY4_Pin, .pressedAction = Button4Pressed,
		.releaseAction = Button4Released, .state = Idle

}, {

.port = EN_SW_GPIO_Port, .pin = EN_SW_Pin, .pressedAction = Button5Pressed,
		.releaseAction = Button5Released, .state = Idle

}

};

void Buttons_Check() {

for (int i = 0; i<5; i++) {
	struct Button* b=&buttons[i];

	switch (b->state) {
		case Idle:
		if (HAL_GPIO_ReadPin(b->port, b->pin)) {

			b->state = DebouncePressed;
			b->releaseTick = HAL_GetTick();

		}
		break;

		case DebouncePressed:
		if (HAL_GPIO_ReadPin(b->port, b->pin)) {
			if (HAL_GetTick() - b->releaseTick >= 10) {

				b->state = Pressed;
				if (b->pressedAction != NULL) {
					b->pressedAction();
				}
			}
		} else
		b->state = Idle;
		break;

		case Pressed:

		if (!HAL_GPIO_ReadPin(b->port, b->pin)) {

			b->state = DebounceDepressed;
			b->releaseTick = HAL_GetTick();

		}
		break;

		case DebounceDepressed:
		if (!HAL_GPIO_ReadPin(b->port, b->pin)) {
			if (HAL_GetTick() - b->releaseTick >= 10) {

				b->state = Idle;
				if (b->releaseAction != NULL) {
					b->releaseAction();
				}
			}
		} else
		b->state = Pressed;
		break;

		default:

		while (1) {

		}

		break;
	}
}
}
