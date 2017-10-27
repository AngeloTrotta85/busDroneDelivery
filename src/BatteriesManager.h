/*
 * BatteriesManager.h
 *
 *  Created on: 27 ott 2017
 *      Author: angelo
 */

#ifndef BATTERIESMANAGER_H_
#define BATTERIESMANAGER_H_

#include <stdlib.h>
#include <stdio.h>
#include <iostream>     // std::cout
#include <iomanip>      // std::get_time
#include <ctime>        // struct std::tm
#include <time.h>        // struct std::tm
#include <list>        // struct std::tm

#include "Battery.h"


using namespace std;

class BatteriesManager {
public:
	BatteriesManager();
	virtual ~BatteriesManager();

private:
	std::list<Battery> batteryList;
};

#endif /* BATTERIESMANAGER_H_ */
