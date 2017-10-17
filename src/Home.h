/*
 * Home.h
 *
 *  Created on: 16 ott 2017
 *      Author: angelo
 */

#ifndef HOME_H_
#define HOME_H_

#include <stdlib.h>
#include <stdio.h>
#include <iostream>     // std::cout
#include <boost/tokenizer.hpp>
#include <iomanip>      // std::get_time
#include <ctime>        // struct std::tm
#include <time.h>        // struct std::tm
#include <list>        // struct std::tm

using namespace std;
using namespace boost;

class Home {
public:
	typedef enum {
		HOME_ID = 0,
		HOME_NAME,
		HOME_LAT,
		HOME_LON
	} ENUM_HOME;

public:
	Home();
	virtual ~Home();

	bool parseInput(const std::string toParse);

	const std::string& getHomeId() const {		return home_id;	}
	unsigned int getHomeIdNum() const {		return home_id_num;	}
	const std::string& getHomeLat() const {		return home_lat;	}
	const std::string& getHomeLon() const {		return home_lon;	}
	const std::string& getHomeName() const { return home_name;	}
	double getHomeLatNum() const {		return home_lat_num;	}
	double getHomeLonNum() const {		return home_lon_num;	}

private:
	std::string home_id;
	std::string home_name;
	std::string home_lat;
	std::string home_lon;

	unsigned int home_id_num;
	double home_lat_num;
	double home_lon_num;
};

#endif /* HOME_H_ */
