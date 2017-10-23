//============================================================================
// Name        : busDroneDelivery.cpp
// Author      : Angelo Trotta
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include <stdlib.h>
#include <stdio.h>
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include <algorithm>    // std::find
#include <vector>       // std::vector
#include <list>       // std::list
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>

#include <boost/range/irange.hpp>

#include "MyCoord.h"
#include "Simulator.h"

using namespace std;

int main(int argc, char **argv) {

	Simulator sim;
	InputParser input(argc, argv);
	sim.importSomeParameterFromInputLine(&input);

	const std::string &fileDeliveryPoints = input.getCmdOption("-fDP");
	if (!fileDeliveryPoints.empty()) {
		sim.importDeliveryPoints(fileDeliveryPoints);
	}
	else {
		cerr << "Insert the delivery points file input" << endl;
		exit (EXIT_FAILURE);
	}

	const std::string &fileHomeStations = input.getCmdOption("-fH");
	if (!fileHomeStations.empty()) {
		sim.importHomes(fileHomeStations);
	}
	else {
		cerr << "Insert the home stations file input" << endl;
		exit (EXIT_FAILURE);
	}

	//std::srand(100);
	const std::string &seedString = input.getCmdOption("-seed");
	if (!seedString.empty()) {
		std::srand(atol(seedString.c_str()));
	}
	else {
		std::srand(std::time(0)); // use current time as seed for random generator
	}

	//set the number of UAV
	const std::string &nUavString = input.getCmdOption("-nUAV");
	if (!nUavString.empty()) {
		sim.setUav(atol(nUavString.c_str()));
	}
	else {
		sim.setUav(12);
	}

	// tests
	/*{
		cout << "Distance from 44.505765754071,11.3431758861234 to 44.505122,11.331807 -> "
				<< Simulator::distanceEarth(44.505765754071,11.3431758861234, 44.505122,11.331807) << endl;

		std::pair<double, double> newCoord = Simulator::moveOnEarth(44.505765754071,11.3431758861234, 44.505122,11.331807, 20, 8);

		cout << "Distance from 44.505765754071,11.3431758861234 to " << newCoord.first << "," << newCoord.second <<  " -> "
						<< Simulator::distanceEarth(44.505765754071,11.3431758861234, newCoord.first,newCoord.second) << endl;
	}*/

	// init the simulation
	if (!sim.init()) {
		cout << "Error initializing the simulation" << endl;
		return EXIT_FAILURE;
	}

	if (input.cmdOptionExists("-noRun")) exit (EXIT_SUCCESS);

	// run the simulation
	cout << "Running the simulation" << endl;
	sim.run();

	// make stats
	cout << "Making stats" << endl;
	//sim.stats();
	const std::string &statsFileName = input.getCmdOption("-oSTAT");
	if (!statsFileName.empty()) {
		sim.stats(std::string(statsFileName));
	}
	else {
		sim.stats(std::string(""));

	}

	cout << "End Simulation!" << endl;
	return EXIT_SUCCESS;
}





