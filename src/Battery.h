/*
 * Battery.h
 *
 *  Created on: 27 ott 2017
 *      Author: angelo
 */

#ifndef BATTERY_H_
#define BATTERY_H_

#include <stdlib.h>
#include <stdio.h>
#include <iostream>     // std::cout
#include <iomanip>      // std::get_time
#include <ctime>        // struct std::tm
#include <time.h>        // struct std::tm
#include <list>        // struct std::tm


using namespace std;

class Battery {
public:
	Battery();
	virtual ~Battery();
};

#endif /* BATTERY_H_ */
