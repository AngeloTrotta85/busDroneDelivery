/*
 * Home.cpp
 *
 *  Created on: 16 ott 2017
 *      Author: angelo
 */

#include "Home.h"

Home::Home() {
	// TODO Auto-generated constructor stub

}

Home::~Home() {
	// TODO Auto-generated destructor stub
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

