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
	fly_goal = UAV_FLYING_DELIVERING;
	load_weight = 0;
	maxEnergy = 130000;

	resudualEnergy = 0;
	belongingHome = nullptr;
	carryingPackage = nullptr;
	averageSpeed = 10;
	deliveredPackage = 0;

	pos_lon = 0;
	pos_lat = 0;
	dest_lon = 0;
	dest_lat = 0;

	id = idCounter++;
}

Uav::~Uav() {

}

double Uav::addEnergy(double difference, double seconds) {

	//cout << " Adding/Removing energy: " << (difference * seconds) << endl;
	resudualEnergy += difference * seconds;

	if (resudualEnergy < 0) {
		resudualEnergy = 0;
	} else if (resudualEnergy > maxEnergy) {
		resudualEnergy = maxEnergy;
	}

	return resudualEnergy;
}

bool Uav::check_pkt_feasibility(Package *p) {
	bool ris = true;
	return ris;
}

void Uav::run(struct std::tm now_time_tm, unsigned int time_step) {
	if (state == UAV_CHARGING) {
		if (charge_state == UAV_CHARGING_ATHOME) {
			if (belongingHome == nullptr) {
				cerr << "Error: uav is in charging at home 'without' an home" << endl;
				exit(EXIT_FAILURE);
			}
			else {
				if (belongingHome->wa.getWarehousePktNumber() > 0) {
					Package *pckToCarry = nullptr;
					//for (auto& p : belongingHome->wa.wareHouse) {
					for (auto itP = belongingHome->wa.wareHouse.begin(); itP != belongingHome->wa.wareHouse.end(); itP++) {
						if (check_pkt_feasibility(*itP)) {
							pckToCarry = *itP;
							belongingHome->wa.wareHouse.erase(itP);
							break;
						}
					}
					if (pckToCarry != nullptr) {
						// package to carry...
						load_weight = pckToCarry->weight;
						carryingPackage = pckToCarry;
						state = UAV_FLYING;
						fly_state = UAV_FLYING_WITH_LOAD;
						fly_goal = UAV_FLYING_DELIVERING;
						dest_lat = pckToCarry->dest_dp->getDpLatNum();
						dest_lon = pckToCarry->dest_dp->getDpLonNum();
					}
				}
			}
		}
		else { //UAV_CHARGING_ONBUS

		}
	}
	else if (state == UAV_FLYING) {
		//update the position
		double baseStepMeters = ((double) time_step) * averageSpeed;
		if (Simulator::distanceEarth(pos_lat, pos_lon, dest_lat, dest_lon) < baseStepMeters) {
			//arrived to destination
			pos_lat = dest_lat;
			pos_lon = dest_lon;

			// change state
			if (fly_goal == UAV_FLYING_DELIVERING) {
				//package delivered
				++deliveredPackage;
				delete(carryingPackage);

				fly_goal = UAV_FLYING_GOING_HOME;
				fly_state = UAV_FLYING_FREE;
				load_weight = 0;

				//TODO
				dest_lat = belongingHome->getHomeLatNum();
				dest_lon = belongingHome->getHomeLonNum();
			}
			else { // UAV_FLYING_GOING_HOME
				state = UAV_CHARGING;
				charge_state = UAV_CHARGING_ATHOME;
			}
		}
		else {
			// move
			std::pair<double, double> newCoord = Simulator::moveOnEarth(pos_lat, pos_lon, dest_lat, dest_lon, averageSpeed, time_step);
			pos_lat = newCoord.first;
			pos_lon = newCoord.second;
		}
	}
	else { // UAV_STOP

	}
}


