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
		listUav.push_back(newUav);
	}

	return ris;
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
		flowGraph->execute(t_tm, listUav);

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
	//} while ((difftime(mktime(&end_sim_time_tm), mktime(&t_tm)) >= 0) && alive);
	} while (alive);
	cout << endl;

	finalLifetime = t;
	//cout << "System lifetime: " << finalLifetime << endl;

}

void Simulator::stats(std::string outFileName) {

	std::ofstream statStream(outFileName, std::ofstream::out);
	bool fOutOK = statStream.is_open();

	//if (fOutOK) statStream << "LIFETIME_MIN " << finalLifetime/60.0 << endl;

	if (fOutOK) statStream.close();
}


