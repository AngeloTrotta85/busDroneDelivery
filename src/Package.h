/*
 * Package.h
 *
 *  Created on: 20 ott 2017
 *      Author: angelo
 */

#ifndef PACKAGE_H_
#define PACKAGE_H_

class Package {
public:
	Package(double p, double l, double w, double h);
	virtual ~Package();

private:
	double weight;
	double length;
	double width;
	double height;

	static int id_counter;
	int id;
};

#endif /* PACKAGE_H_ */
