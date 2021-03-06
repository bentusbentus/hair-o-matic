#include <Arduino.h>
#include <SPI.h>
#include "AudioUIController.h"

const int AUDIO_INPUT_PIN = 14;
const int AUDIO_OUTPUT_PIN = 8;

const int PACKET_START_TONE = 1500;
const int PACKET_END_TONE = 1000;
const int OFF_TONE = 100;
const int ON_TONE = 500;
const int INT_BITS = sizeof(int) * 8;

const int VOLTAGE_DATA_ID = 1;
const int RESISTANCE_DATA_ID = 2;
const int CURRENT_DATA_ID = 3;

AudioUIController::AudioUIController(ProbeState* state) {
	this->state = state;
}

void AudioUIController::initialize() {
}

void AudioUIController::update(bool ended) {
	if (state->getIsRefreshNeeded()) {

	}
}

void AudioUIController::readInput() {
	sendTone(PACKET_START_TONE);

	sendInt(state->lastInputVoltage, VOLTAGE_DATA_ID);
	sendInt(state->getTargetMicroAmps(), CURRENT_DATA_ID);
	sendInt(state->resistance, RESISTANCE_DATA_ID);

	sendTone(PACKET_END_TONE);

	delay(500);
}

void AudioUIController::sendData (bool* data, int size) {
	for (int i = 0; i < size; i++) {
		Serial.print(data[i]);

		if (data[i])
			sendTone(ON_TONE);
		else
			sendTone(OFF_TONE);
	}
}

void AudioUIController::sendTone(int hz) {
	tone(AUDIO_OUTPUT_PIN, hz);
	delay(200);
	noTone(AUDIO_OUTPUT_PIN);
	delay(200);
}

void AudioUIController::sendInt(int value, int id) {
	bool idBinary[INT_BITS];
	int4ToBinary(value, idBinary);
	sendData(idBinary, INT_BITS);

	bool binary[INT_BITS];
	intToBinary(value, binary);
	sendData(binary, INT_BITS);
}

void AudioUIController::intToBinary(int number, bool* binary) {
	for (int i = 0; i < INT_BITS; i++)
		binary[i] = (number & (1 << i)) ? 1 : 0;
}

void AudioUIController::int4ToBinary(int number, bool* binary) {
	for (int i = INT_BITS - 4; i < INT_BITS; i++)
		binary[i] = (number & (1 << i)) ? 1 : 0;
}