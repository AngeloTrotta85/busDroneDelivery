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

	estimatedEnergyLossOnPackage = 0;
	energyAtPackageLoad = 0;

	belongingHome = nullptr;
	carryingPackage = nullptr;
	batt = nullptr;
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

	return batt->addEnergy(difference, seconds);
}

bool Uav::check_pkt_feasibility(Package *p) {
	bool ris = false;
	double requestedEnergy = 0;
	double energy_loss, distance_travel, speed_travel, time_travel;

	// calculate first part
	energy_loss = simulator->getEnergyLossUav(p->weight);
	distance_travel = Simulator::distanceEarth(pos_lat, pos_lon, p->dest_dp->getDpLatNum(), p->dest_dp->getDpLonNum());
	speed_travel = getSpeedWithLoad(averageSpeed, p->weight);
	time_travel = distance_travel / speed_travel;
	requestedEnergy += energy_loss * time_travel;
	//cout << "Andata: el=" << energy_loss << "W; dist=" << distance_travel << "m; speed=" << speed_travel << "m/s; time="
	//		<< time_travel << "s; reqE=" << requestedEnergy << endl;

	// calculate coming back
	energy_loss = simulator->getEnergyLossUav(0);
	distance_travel = Simulator::distanceEarth(p->dest_dp->getDpLatNum(), p->dest_dp->getDpLonNum(), pos_lat, pos_lon);
	speed_travel = getSpeedWithLoad(averageSpeed, 0);
	time_travel = distance_travel / speed_travel;
	requestedEnergy += energy_loss * time_travel;
	//cout << "Ritorno: el=" << energy_loss << "W; dist=" << distance_travel << "m; speed=" << speed_travel << "m/s; time="
	//		<< time_travel << "s; reqE=" << (energy_loss * time_travel) << endl;
	//cout << "Total req = " << requestedEnergy << "J. Mia energia: " << resudualEnergy << endl << endl;

	if ((batt->getResudualEnergy() + requestedEnergy) > (batt->getResudualEnergy() * 0.1)) {
		//cout << "OK, posso trasportarlo" << endl;
		estimatedEnergyLossOnPackage = requestedEnergy;
		energyAtPackageLoad = batt->getResudualEnergy();
		ris = true;
	}

	return ris;
}

double Uav::getSpeedWithLoad(double speedNoLoad, double loadW) {
	if (loadW <= 0) {
		return speedNoLoad;
	}
	else {
		double maxW = 4000.0;
		if (loadW >= maxW) {
			return speedNoLoad * 0.5;
		}
		else {
			//return (speedNoLoad * 0.5) + ((1.0 - (loadW / maxW)) * (speedNoLoad * 0.5));
			return (speedNoLoad - ((speedNoLoad * loadW) / (2.0 * maxW)));
		}
	}
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
		double actSpeed = getSpeedWithLoad(averageSpeed, load_weight);
		double baseStepMeters = ((double) time_step) * actSpeed;

		if (Simulator::distanceEarth(pos_lat, pos_lon, dest_lat, dest_lon) < baseStepMeters) {
			//arrived to destination
			pos_lat = dest_lat;
			pos_lon = dest_lon;

			// change state
			if (fly_goal == UAV_FLYING_DELIVERING) {
				//package delivered
				++deliveredPackage;
				delete(carryingPackage);

				state = UAV_FLYING;
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
			std::pair<double, double> newCoord = Simulator::moveOnEarth(pos_lat, pos_lon, dest_lat, dest_lon, actSpeed, time_step);
			pos_lat = newCoord.first;
			pos_lon = newCoord.second;
		}
	}
	else { // UAV_STOP

	}

	// update the batteries
	updateBattery(time_step);
}

void Uav::updateBattery(unsigned int time_step) {
//TODO
}


