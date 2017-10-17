/*
 * Uav.cpp
 *
 *  Created on: 17 lug 2017
 *      Author: angelo
 */

#include "Uav.h"

int Uav::idCounter = 0;

Uav::Uav() {
	state = UAV_FLYING;

	resudualEnergy = 0;

	id = idCounter++;
}

Uav::~Uav() {
	// TODO Auto-generated destructor stub
}

double Uav::addEnergy(double difference) {

	resudualEnergy += difference;

	return resudualEnergy;
}


