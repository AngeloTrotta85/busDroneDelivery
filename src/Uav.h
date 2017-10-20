/*
 * Uav.h
 *
 *  Created on: 17 lug 2017
 *      Author: angelo
 */

#ifndef UAV_H_
#define UAV_H_

#include <limits.h>
#include <ctime>        // struct std::tm
#include <time.h>

class Simulator;

class Uav {
	static int idCounter;

public:
	typedef enum {
		UAV_STOP,
		UAV_FLYING,
		UAV_CHARGING
	} UAV_STATE;

	typedef enum {
		UAV_CHARGING_ATHOME,
		UAV_CHARGING_ONBUS
	} UAV_CHARGING_STATE;

	typedef enum {
		UAV_FLYING_FREE,
		UAV_FLYING_WITH_LOAD
	} UAV_FLYING_STATE;

public:
	Uav(Simulator *sim);
	virtual ~Uav();

	void run(struct std::tm now_time_tm, unsigned int time_step);

	double addEnergy(double difference, double seconds);

	int getId() const {		return id;	}
	double getResudualEnergy() const {		return resudualEnergy;	}
	void setResudualEnergy(double resudualEnergy) {		this->resudualEnergy = resudualEnergy;	}
	double getMaxEnergy() const {		return maxEnergy;	}
	void setMaxEnergy(double maxEnergy) {		this->maxEnergy = maxEnergy;	}
	UAV_STATE getState() const {	return state;	}
	void setState(UAV_STATE state) {		this->state = state;	}
	UAV_CHARGING_STATE getChargeState() const {		return charge_state;	}
	void setChargeState(UAV_CHARGING_STATE chargeState) {		charge_state = chargeState;	}
	UAV_FLYING_STATE getFlyState() const {		return fly_state;	}
	void setFlyState(UAV_FLYING_STATE flyState) {		fly_state = flyState;	}
	unsigned int getLoadWeight() const {		return load_weight;	}
	void setLoadWeight(unsigned int loadWeight) {		load_weight = loadWeight;	}
	double getPosLon() const {		return pos_lon;	}
	void setPosLon(double posLon) {		pos_lon = posLon;	}
	double getPosLat() const {		return pos_lat;	}
	void setPosLat(double posLat) {		pos_lat = posLat;	}

private:
	Simulator *simulator;

	double resudualEnergy;
	double maxEnergy;
	int id;

	UAV_STATE state;
	UAV_CHARGING_STATE charge_state;
	UAV_FLYING_STATE fly_state;
	unsigned int load_weight;

	double pos_lon;
	double pos_lat;
};


#endif /* UAV_H_ */
