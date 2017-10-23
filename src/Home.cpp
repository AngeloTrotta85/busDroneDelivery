/*
 * Home.cpp
 *
 *  Created on: 16 ott 2017
 *      Author: angelo
 */

#include "Home.h"
#include "Simulator.h"

Home::Home() {

}

Home::~Home() {
}

bool Home::parseInput(const std::string toParse) {
	bool ris = true;
	int idx = 0;
	typedef boost::tokenizer<boost::escaped_list_separator<char>> tokenizer;

	tokenizer tok{toParse};
	for (const auto &t : tok){
		//std::cout << "|" << t << "|" << '\n';

		switch (idx) {
		case HOME_ID:
			home_id = t;
			home_id_num = atol(home_id.c_str());
			break;
		case HOME_NAME:
			home_name = t;
			break;
		case HOME_LAT:
			home_lat = t;
			home_lat_num = atof(home_lat.c_str());
			break;
		case HOME_LON:
			home_lon = t;
			home_lon_num = atof(home_lon.c_str());
			break;
		default:
			cerr << "Too much columns in Home file" << endl;
			ris = false;
			break;
		}

		idx++;
	}

	if (idx != 4) {
		ris = false;
	}

	return ris;
}

void Home::init(struct std::tm sim_time_tm) {

	// warehouse initialization
	wa.initTime(sim_time_tm);

}

void Home::update(struct std::tm now_time_tm) {
	// warehouse initialization
	wa.update(now_time_tm, simulator->deliveryPointsMap);
}

unsigned int Home::getWA_pktNumber(void) {
	return wa.getWarehousePktNumber();
}

void Home::setWA_parameters(double defW, int initPck, double genRate, std::map<unsigned int, DeliveryPoint> &dp) {
	wa.setDefaultWeight_grams(defW);
	wa.setPacketInitNumber(initPck, dp);
	wa.setPacketGenerationRate(genRate);
}


