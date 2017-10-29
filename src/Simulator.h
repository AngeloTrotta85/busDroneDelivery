/*
 * Simulator.h
 *
 *  Created on: 16 ott 2017
 *      Author: angelo
 */

#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include <stdlib.h>
#include <stdio.h>
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include <algorithm>    // std::find
#include <map>
#include <list>
#include <limits>       // std::numeric_limits
#include <ctime>        // struct std::tm
#include <time.h>

#include "Battery.h"
#include "Uav.h"
#include "Home.h"
#include "DeliveryPoint.h"

class InputParser{
public:
	InputParser (int &argc, char **argv){
		for (int i=1; i < argc; ++i)
			this->tokens.push_back(std::string(argv[i]));
	}
	const std::string& getCmdOption(const std::string &option) const{
		std::vector<std::string>::const_iterator itr;
		itr =  std::find(this->tokens.begin(), this->tokens.end(), option);
		if (itr != this->tokens.end() && ++itr != this->tokens.end()){
			return *itr;
		}
		static const std::string empty_string("");
		return empty_string;
	}
	bool cmdOptionExists(const std::string &option) const{
		return std::find(this->tokens.begin(), this->tokens.end(), option)
		!= this->tokens.end();
	}
private:
	std::vector <std::string> tokens;
};

class Simulator {
public:
	Simulator();
	virtual ~Simulator();

	void importSomeParameterFromInputLine(InputParser *inputVal);

	void run(void);
	bool init(void);
	void stats(std::string outFileName);

	bool importHomes(std::string homesFileName);
	bool importDeliveryPoints(std::string deliveryPointsFileName);

	double getEnergyLossUav(double load);

	unsigned int getUav() const {		return nUAV;	}
	void setUav(unsigned int uav) {		nUAV = uav;	}

public:
	// This function converts decimal degrees to radians
	static double deg2rad(double deg) { return (deg * M_PI / 180.0); }

	//  This function converts radians to decimal degrees
	static double rad2deg(double rad) { return (rad * 180.0 / M_PI); }

	/**
	 * Returns the distance between two points on the Earth.
	 * Direct translation from http://en.wikipedia.org/wiki/Haversine_formula
	 * @param lat1d Latitude of the first point in degrees
	 * @param lon1d Longitude of the first point in degrees
	 * @param lat2d Latitude of the second point in degrees
	 * @param lon2d Longitude of the second point in degrees
	 * @return The distance between the two points in meters
	 */
	static double distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d) {
		double lat1r, lon1r, lat2r, lon2r, u, v;
		lat1r = deg2rad(lat1d);
		lon1r = deg2rad(lon1d);
		lat2r = deg2rad(lat2d);
		lon2r = deg2rad(lon2d);
		u = sin((lat2r - lat1r)/2);
		v = sin((lon2r - lon1r)/2);
		return 2.0 * 6371.0 * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v)) * 1000.0;
	}

	static std::pair<double,double> moveOnEarth(double latSrc_d, double lonSrc_d, double latDest_d, double lonDest_d, double speed, double time_s) {
		double dist_src_dest = distanceEarth( latSrc_d, lonSrc_d, latDest_d, lonDest_d);
		double dist_on_step = speed * time_s;
		if (dist_on_step >= dist_src_dest) {
			return make_pair(latDest_d,lonDest_d);
		}
		else {
			double latStep_d, lonStep_d;
			double ratio = dist_src_dest / dist_on_step;

			latStep_d = latSrc_d + ((latDest_d - latSrc_d) / ratio);
			lonStep_d = lonSrc_d + ((lonDest_d - lonSrc_d) / ratio);

			return make_pair(latStep_d, lonStep_d);
		}
	}
private:
	void updateBatteries(Uav *u, unsigned int time_step);

public:
	std::map<unsigned int, Home> homesMap;
	std::map<unsigned int, DeliveryPoint> deliveryPointsMap;

private:
	unsigned int nUAV;
	std::list<Uav *> listUav;

	unsigned int finalLifetime;

	std::string operationalDay;
	struct std::tm operationalDay_tm;
	struct std::tm start_sim_time_tm;
	struct std::tm end_sim_time_tm;

	double waDefaultW;
	int waPckInitNum;
	double waPckGenRate;

	int initialNumberBatteryInWA;
	int initialNumberChargerInWA;

	double initialUavEnergy; 	// Joule
	double uavAvgSpeed;		 	// m/s

	// Watt
	double eFLYING_FREE;
	double eRECHARGING_HOME;
	double eRECHARGING_BUS;

};

#endif /* SIMULATOR_H_ */
