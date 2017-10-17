/*
 * Uav.h
 *
 *  Created on: 17 lug 2017
 *      Author: angelo
 */

#ifndef UAV_H_
#define UAV_H_

#include <limits.h>


class Uav {
	static int idCounter;

public:
	typedef enum {
		UAV_FLYING,
		UAV_ONBUS
	} UAV_STATE;

public:
	Uav();
	virtual ~Uav();

	double addEnergy(double difference);

	double getResudualEnergy() const {		return resudualEnergy;	}
	void setResudualEnergy(double resudualEnergy) {		this->resudualEnergy = resudualEnergy;	}
	UAV_STATE getState() const {	return state;	}
	void setState(UAV_STATE state) {		this->state = state;	}
	int getId() const {		return id;	}


private:
	double resudualEnergy;
	int id;



	UAV_STATE state;
};


#endif /* UAV_H_ */
