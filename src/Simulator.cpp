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
	start_sim_time_tm.tm_hour = 6;

	end_sim_time_tm = operationalDay_tm;
	end_sim_time_tm.tm_hour = 9;

	eFLYING_FREE = -50;
	eRECHARGING_HOME = 15;
	eRECHARGING_BUS = 5;

	nUAV = 1;
	initialUavEnergy = 60000;
	uavAvgSpeed = 20.0;

	finalLifetime = 0;

	waDefaultW = 1000;
	waPckInitNum = 0;
	waPckGenRate = 1.0/60.0;

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

	// UAV AVERAGE SPEED
	const std::string &avgSpeedString = inputVal->getCmdOption("-avgS");
	if (!avgSpeedString.empty()) {
		uavAvgSpeed = atof(avgSpeedString.c_str());
	}
	else {
		uavAvgSpeed = 20;
	}

	// WAREHOUSE
	const std::string &defPckWString = inputVal->getCmdOption("-defPW");
	if (!defPckWString.empty()) {
		//wa.setDefaultWeight_grams(atof(defPckWString.c_str()));
		waDefaultW = atof(defPckWString.c_str());
	}
	const std::string &defPckNumString = inputVal->getCmdOption("-defPN");
	if (!defPckNumString.empty()) {
		//wa.setPacketInitNumber(atoi(defPckNumString.c_str()));
		waPckInitNum = atoi(defPckNumString.c_str());
	}
	const std::string &defWAGenString = inputVal->getCmdOption("-waGRsec");
	if (!defWAGenString.empty()) {
		//wa.setPacketGenerationRate(atof(defWAGenString.c_str()));
		waPckGenRate = atof(defWAGenString.c_str());
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
			newHome.setSimulator(this);

			if(newHome.parseInput(str)){
				newHome.init(start_sim_time_tm);
				//newHome.wa.setDefaultWeight_grams(waDefaultW);
				//newHome.wa.setPacketInitNumber(waPckInitNum);
				//newHome.wa.setPacketGenerationRate(waPckGenRate);
				//newHome.setWA_parameters(waDefaultW, waPckInitNum, waPckGenRate);
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
		Uav *newUav = new Uav(this);

		newUav->setResudualEnergy(initialUavEnergy);
		newUav->setMaxEnergy(initialUavEnergy);
		newUav->setAverageSpeed(uavAvgSpeed);

		if (homesMap.size() > 0) {
			auto hIT = homesMap.begin();
			int idxRand = rand() % homesMap.size();

			while (idxRand > 0) {
				hIT++;
				--idxRand;
			}
			newUav->setPosLon(hIT->second.getHomeLonNum());
			newUav->setPosLat(hIT->second.getHomeLatNum());
			newUav->setBelongingHome((Home *) (&(hIT->second)));
		}

		listUav.push_back(newUav);
	}

	// init the homes
	for (auto& h : homesMap) {
		h.second.setWA_parameters(waDefaultW, waPckInitNum, waPckGenRate, deliveryPointsMap);
	}

	return ris;
}

double Simulator::getEnergyLossUav(double load) {
	//TODO
	if (load == 0) {
		return eFLYING_FREE;
	}
	else {
		double multiplier = 1.0 + (load / 3000.0);
		return (eFLYING_FREE * multiplier);
	}
}

void Simulator::updateBatteries(Uav *u, unsigned int time_step) {
	Uav::UAV_STATE us = u->getState();
	Uav::UAV_CHARGING_STATE cs_us = u->getChargeState();
	Uav::UAV_FLYING_STATE fs_us = u->getFlyState();

	if (us == Uav::UAV_FLYING) {
		switch (fs_us) {
		case Uav::UAV_FLYING_WITH_LOAD:
			u->addEnergy(getEnergyLossUav(u->getLoadWeight()), time_step);
			break;

		case Uav::UAV_FLYING_FREE:
		default:
			u->addEnergy(getEnergyLossUav(0), time_step);
			break;
		}

	} else if (us == Uav::UAV_CHARGING) {
		switch (cs_us) {
		case Uav::UAV_CHARGING_ATHOME:
		default:
			u->addEnergy(eRECHARGING_HOME, time_step);
			break;

		case Uav::UAV_CHARGING_ONBUS:
			u->addEnergy(eRECHARGING_BUS, time_step);
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
	unsigned int time_step_sec = 1;

	do {
	//for (unsigned int t = 0; t < maxTime; t++){
		//cout << "Simulation time: " << t << endl;

		std::strftime(buffer, sizeof(buffer), "%a, %d.%m.%Y %H:%M:%S", &t_tm);
		fprintf(stdout, "\rSimulation time: %05u seconds - %s - ", t, buffer);
		for (auto& h : homesMap) {
			fprintf(stdout, "{wa%d|%d} ", h.second.getHomeIdNum(), h.second.getWA_pktNumber());
		}
		cout << "- ";
		for (auto& uav : listUav) {
			fprintf(stdout, "[%d|%.02f|P%d] ", uav->getId(), uav->getResudualEnergy(), uav->getDeliveredPackage());
		}
		cout << " ---!";
		//cout << endl;
		fflush(stdout);

		//flowGraph.execute(t, listUav);
		//flowGraph->execute(t_tm, listUav);

		//update the warehouses
		for (auto& h : homesMap) {
			h.second.update(t_tm);
		}

		// execute the uav
		for(auto& uav : listUav) {
			uav->run(t_tm, time_step_sec);
		}

		//updateThe batteries
		for(auto& uav : listUav) {
			updateBatteries(uav, time_step_sec);
		}

		// check the lifetime
		for(auto& uav : listUav) {
			if (uav->getResudualEnergy() <= 0) {
				alive = false;
				break;
			}
		}

		t += time_step_sec;
		t_tm.tm_sec = t_tm.tm_sec + time_step_sec;
		mktime(&t_tm);
	//} while ((t < maxTime) && alive);
	} while ((difftime(mktime(&end_sim_time_tm), mktime(&t_tm)) > 0) && alive);
	//} while (alive);
	cout << endl;

	if (!alive) {
		cout << "One UAV is dead" << endl;
	}
	else {
		cout << "Simulation time expired" << endl;
	}

	finalLifetime = t;
	//cout << "System lifetime: " << finalLifetime << endl;

}

void Simulator::stats(std::string outFileName) {

	std::ofstream statStream(outFileName, std::ofstream::out);
	bool fOutOK = statStream.is_open();

	cout << endl << "FINAL STATS: " << endl;



	cout << "SYSTEM LIFETIME: " << finalLifetime << " sec" << endl;
	cout << "SYSTEM LIFETIME: " << finalLifetime/60.0 << " min" << endl;



	if (fOutOK)  {
		statStream << "LIFETIME_MIN " << finalLifetime/60.0 << endl;
		statStream << "LIFETIME_SEC " << finalLifetime << endl;

		statStream.close();
	}

}


