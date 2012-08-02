/*
 * IGraclus.cpp
 *
 *  Created on: 2 août 2012
 *      Author: manuel
 */

#include "IGraclus.hpp"
#include "AbstractGraclus.hpp"

IGraclus::~IGraclus() {
}

IGraclus * IGraclus::Get() {
	return new AbstractGraclus;
}
