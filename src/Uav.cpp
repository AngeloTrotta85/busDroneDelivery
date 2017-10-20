/*
 * Uav.cpp
 *
 *  Created on: 17 lug 2017
 *      Author: angelo
 */

#include "Uav.h"
#include "Simulator.h"

int Uav::idCounter = 0;

Uav::Uav(Simulator *sim) {
	simulator = sim;

	state = UAV_CHARGING;
	charge_state = UAV_CHARGING_ATHOME;
	fly_state = UAV_FLYING_FREE;
	load_weight = 0;
	maxEnergy = 130000;

	resudualEnergy = 0;

	pos_lon = 0;
	pos_lat = 0;

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

void Uav::run(struct std::tm now_time_tm, unsigned int time_step) {
	if ((state == UAV_CHARGING) && (state == UAV_CHARGING)){

	}
}


