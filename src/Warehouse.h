/*
 * Warehouse.h
 *
 *  Created on: 18 ott 2017
 *      Author: angelo
 */

#ifndef WAREHOUSE_H_
#define WAREHOUSE_H_

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

class Package {
public:
	Package(double p, double l, double w, double h) : weight(w), length(l), width(w), height(h) {};
	virtual ~Package();

private:
	double weight;
	double length;
	double width;
	double height;
};

class Warehouse {
public:
	Warehouse();
	virtual ~Warehouse();

	void setDefaultWeight_grams(double defaultWeight) { this->defaultWeight = defaultWeight;	}
	void setPacketInitNumber(double packetNumber) { this->packetNumber = packetNumber;	}

private:
	std::list<Package *> wareHouse;

	double defaultWeight;
	unsigned int packetNumber;
};

#endif /* WAREHOUSE_H_ */
