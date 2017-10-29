/*
 * Charger.h
 *
 *  Created on: 29 ott 2017
 *      Author: angelo
 */

#ifndef CHARGER_H_
#define CHARGER_H_

#include <stdlib.h>
#include <stdio.h>
#include <iostream>     // std::cout
#include <iomanip>      // std::get_time
#include <ctime>        // struct std::tm
#include <time.h>        // struct std::tm
#include <list>        // struct std::tm

#include "Battery.h"

using namespace std;

class Charger {
public:
	Charger();
	virtual ~Charger();
};

#endif /* CHARGER_H_ */
