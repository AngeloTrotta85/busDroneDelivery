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

int main(int argc, char **argv) {

	Simulator sim;
	InputParser input(argc, argv);

	const std::string &fileHomeStations = input.getCmdOption("-fH");
	if (!fileHomeStations.empty()) {
		sim.importHomes(fileHomeStations);
	}
	else {
		cerr << "Insert the home stations file input" << endl;
		exit (EXIT_FAILURE);
	}

	const std::string &fileDeliveryPoints = input.getCmdOption("-fDP");
	if (!fileDeliveryPoints.empty()) {
		sim.importDeliveryPoints(fileDeliveryPoints);
	}
	else {
		cerr << "Insert the delivery points file input" << endl;
		exit (EXIT_FAILURE);
	}

	cout << "End Simulation!" << endl;
	return EXIT_SUCCESS;
}





