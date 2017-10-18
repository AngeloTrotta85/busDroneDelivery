/*
 * Uav.cpp
 *
 *  Created on: 17 lug 2017
 *      Author: angelo
 */

#include "Uav.h"

int Uav::idCounter = 0;

Uav::Uav() {
	state = UAV_CHARGING;
	charge_state = UAV_CHARGING_ATHOME;
	fly_state = UAV_FLYING_FREE;
	load_weight = 0;

	resudualEnergy = 0;

	id = idCounter++;
}

Uav::~Uav() {
	// TODO Auto-generated destructor stub
}

double Uav::addEnergy(double difference, double seconds) {

	resudualEnergy += difference * seconds;

	if (resudualEnergy < 0) {
		resudualEnergy = 0;
	} else if (resudualEnergy > maxEnergy) {
		resudualEnergy = maxEnergy;
	}

	return resudualEnergy;
}


