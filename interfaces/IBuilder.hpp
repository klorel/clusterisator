/*
 * IBuilder.hpp
 *
 *  Created on: 10 août 2012
 *      Author: manuel
 */

#ifndef IBUILDER_HPP_
#define IBUILDER_HPP_

#include <vector>

class IBuilder {
public:
	template<class T> static bool Get(T &, std::string const &);
	template<class T> static bool Get(T &, std::ifstream &);
};

template<class T> inline bool IBuilder::Get(T &t, std::string const &fileName) {
	std::ifstream file(fileName.c_str());
	bool result(false);
	if (file.good()) {
		result = Get(t, file);
	}
	file.close();
	return result;
}

template<> inline bool IBuilder::Get(IPartition & partition,
		std::ifstream & stream) {
	std::vector<size_t> v;
	size_t i;
	while (stream >> i)
		v.push_back(i);
	partition.set(v);

	return true;
}

#endif /* IBUILDER_HPP_ */
