/*
 * Home.h
 *
 *  Created on: 16 ott 2017
 *      Author: angelo
 */

#ifndef HOME_H_
#define HOME_H_

#include <stdlib.h>
#include <stdio.h>
#include <iostream>     // std::cout
#include <boost/tokenizer.hpp>
#include <iomanip>      // std::get_time
#include <ctime>        // struct std::tm
#include <time.h>        // struct std::tm
#include <list>        // struct std::tm

#include "Warehouse.h"

using namespace std;
using namespace boost;

class Simulator;

class Home {
public:
	typedef enum {
		HOME_ID = 0,
		HOME_NAME,
		HOME_LAT,
		HOME_LON
	} ENUM_HOME;

public:
	Home();
	virtual ~Home();

	void setSimulator(Simulator *sim) {simulator = sim;};

	void init(struct std::tm sim_time_tm);
	void update(struct std::tm now_time_tm);
	bool parseInput(const std::string toParse);
	unsigned int getWA_pktNumber(void);
	void setWA_parameters(double defW, int initPck, double genRate, std::map<unsigned int, DeliveryPoint> &dp);

	const std::string& getHomeId() const {		return home_id;	}
	unsigned int getHomeIdNum() const {		return home_id_num;	}
	const std::string& getHomeLat() const {		return home_lat;	}
	const std::string& getHomeLon() const {		return home_lon;	}
	const std::string& getHomeName() const { return home_name;	}
	double getHomeLatNum() const {		return home_lat_num;	}
	double getHomeLonNum() const {		return home_lon_num;	}

public:
	Warehouse wa;

private:
	Simulator *simulator;

	std::string home_id;
	std::string home_name;
	std::string home_lat;
	std::string home_lon;

	unsigned int home_id_num;
	double home_lat_num;
	double home_lon_num;
};

#endif /* HOME_H_ */
