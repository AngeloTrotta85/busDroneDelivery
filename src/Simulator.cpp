/*
 * Simulator.cpp
 *
 *  Created on: 16 ott 2017
 *      Author: angelo
 */

#include "Simulator.h"

using namespace std;

Simulator::~Simulator() {
	// TODO Auto-generated destructor stub
}


Simulator::Simulator() {
	// TODO Auto-generated constructor stub

	operationalDay = std::string("20170522"); // Monday 2017-05-22
	operationalDay_tm.tm_year = 117;
	operationalDay_tm.tm_mon = 4;
	operationalDay_tm.tm_mday = 22;
	operationalDay_tm.tm_wday = 1;
	operationalDay_tm.tm_sec = operationalDay_tm.tm_min = operationalDay_tm.tm_hour = 0;
	operationalDay_tm.tm_yday = 141;
	operationalDay_tm.tm_isdst = 1;

	start_sim_time_tm = operationalDay_tm;
	start_sim_time_tm.tm_hour = 8;

	end_sim_time_tm = operationalDay_tm;
	end_sim_time_tm.tm_hour = 9;

	eFLYING_FREE = 50;
	eRECHARGING_HOME = 15;
	eRECHARGING_BUS = 5;

	nUAV = 1;
	initialUavEnergy = 60000;

	finalLifetime = 0;

}

void Simulator::importSomeParameterFromInputLine(InputParser *inputVal) {
	// INITIAL ENERGY
	const std::string &eInitString = inputVal->getCmdOption("-eINIT");
	if (!eInitString.empty()) {
		initialUavEnergy = atof(eInitString.c_str());
	}
	else {
		initialUavEnergy = 130000;
	}
}


bool Simulator::importHomes(std::string homesFileName) {
	int rowCount;
	std::string str;

	cout << "START PARSING HOMES" << endl;

	std::ifstream fileHomes(homesFileName, std::ifstream::in);
	if(!fileHomes.is_open()) return false;

	std::getline(fileHomes, str); // read first line
	rowCount = 0;
	while (std::getline(fileHomes, str)) {

		str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
		str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());

		if (str.substr(0, 1).compare("#") == 0) {
			continue;
		}
		else {
			Home newHome;

			if(newHome.parseInput(str)){
				homesMap[newHome.getHomeIdNum()] = newHome;
			}
			else {
				cerr << "Error parsing Stops -> " << str << endl;
				return EXIT_FAILURE;
			}
		}

		rowCount++;
		fprintf(stdout, "\rHomes parsing: %d", rowCount);
	}
	//cout << endl;
	cout << "  -  Parsed " << homesMap.size() << " homes" << endl; // out of " << (STOPS_FILE_SMALL_SIZE - 1) << endl;
	fileHomes.close();
	//cout << "END PARSING STOPS. Imported " << db.getStopsSize() << " out of " << (STOPS_FILE_SIZE - 1) << endl << endl;

	return true;
}

bool Simulator::importDeliveryPoints(std::string deliveryPointsFileName) {
	int rowCount;
	std::string str;

	cout << "START PARSING DELIVERY POINTS" << endl;

	std::ifstream fileDP(deliveryPointsFileName, std::ifstream::in);
	if(!fileDP.is_open()) return false;

	std::getline(fileDP, str); // read first line
	rowCount = 0;
	while (std::getline(fileDP, str)) {

		str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
		str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());

		if (str.substr(0, 1).compare("#") == 0) {
			continue;
		}
		else {
			DeliveryPoint newDP;

			if(newDP.parseInput(str)){
				deliveryPointsMap[newDP.getDpIdNum()] = newDP;
			}
			else {
				cerr << "Error parsing Delivery Point -> " << str << endl;
				return EXIT_FAILURE;
			}
		}

		rowCount++;
		fprintf(stdout, "\rDelivery Points parsing: %d", rowCount);
	}
	//cout << endl;
	cout << "  -  Parsed " << deliveryPointsMap.size() << " delivery points" << endl; // out of " << (STOPS_FILE_SMALL_SIZE - 1) << endl;
	fileDP.close();
	//cout << "END PARSING STOPS. Imported " << db.getStopsSize() << " out of " << (STOPS_FILE_SIZE - 1) << endl << endl;

	return true;
}

bool Simulator::init() {

	bool ris = true;

	// init the UAVs
	cout << "INIT THE " << nUAV << " UAVs" << endl; fflush(stdout);
	for (unsigned int i = 0; i < nUAV; i++) {
		Uav *newUav = new Uav();

		newUav->setResudualEnergy(initialUavEnergy);
		newUav->setMaxEnergy(initialUavEnergy);

		listUav.push_back(newUav);
	}

	return ris;
}

void Simulator::updateBatteries(Uav *u) {
	Uav::UAV_STATE us = u->getState();
	Uav::UAV_CHARGING_STATE cs_us = u->getChargeState();
	Uav::UAV_FLYING_STATE fs_us = u->getFlyState();
	unsigned int lw = u->getLoadWeight();
	double eFLYING_withLoad;

	if (us == Uav::UAV_FLYING) {
		switch (fs_us) {
		case Uav::UAV_FLYING_WITH_LOAD:
			eFLYING_withLoad = eFLYING_FREE + lw; //TODO
			u->addEnergy(eFLYING_withLoad, 1);
			break;

		case Uav::UAV_FLYING_FREE:
		default:
			u->addEnergy(eFLYING_FREE, 1);
			break;
		}

	} else if (us == Uav::UAV_CHARGING) {
		switch (cs_us) {
		case Uav::UAV_CHARGING_ATHOME:
		default:
			u->addEnergy(eRECHARGING_HOME, 1);
			break;

		case Uav::UAV_CHARGING_ONBUS:
			u->addEnergy(eRECHARGING_BUS, 1);
			break;
		}
	}
	else { //if (us == Uav::UAV_STOP) {
		// EXAMPLE: on-bus without charging
		// NOTHING ???
	}


}

void Simulator::run(void) {
	char buffer[64];
	unsigned int t = 0;
	struct std::tm t_tm = start_sim_time_tm;
	bool alive = true;

	do {
	//for (unsigned int t = 0; t < maxTime; t++){
		//cout << "Simulation time: " << t << endl;

		std::strftime(buffer, sizeof(buffer), " %a, %d.%m.%Y %H:%M:%S", &t_tm);
		fprintf(stdout, "\rSimulation time: %u seconds - %s", t, buffer);fflush(stdout);

		//flowGraph.execute(t, listUav);
		//flowGraph->execute(t_tm, listUav);

		//updateThe batteries
		for(auto& uav : listUav) {
			updateBatteries(uav);
		}

		// check the lifetime
		for(auto& uav : listUav) {
			if (uav->getResudualEnergy() <= 0) {
				alive = false;
				break;
			}
		}

		t++;
		t_tm.tm_sec = t_tm.tm_sec + 1;
		mktime(&t_tm);
	//} while ((t < maxTime) && alive);
	} while ((difftime(mktime(&end_sim_time_tm), mktime(&t_tm)) > 0) && alive);
	//} while (alive);
	cout << endl;

	finalLifetime = t;
	//cout << "System lifetime: " << finalLifetime << endl;

}

void Simulator::stats(std::string outFileName) {

	std::ofstream statStream(outFileName, std::ofstream::out);
	bool fOutOK = statStream.is_open();

	cout << "FINAL STATS: " << endl;



	cout << "SYSTEM LIFETIME: " << finalLifetime << " sec" << endl;
	cout << "SYSTEM LIFETIME: " << finalLifetime/60.0 << " min" << endl;



	if (fOutOK)  {
		statStream << "LIFETIME_MIN " << finalLifetime/60.0 << endl;
		statStream << "LIFETIME_SEC " << finalLifetime << endl;

		statStream.close();
	}

}


