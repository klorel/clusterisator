#ifndef CSDP_INTERFACE_H_
#define CSDP_INTERFACE_H_

#include <map>
#include <vector>
#include <utility>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iomanip>


class CsdpInterface {
public:
	CsdpInterface();
	~CsdpInterface();
public:
	void clear();
	void add(size_t, double);
	void add(size_t, size_t, double);
	void bestReformulation(std::map<size_t, double> & result) const;
private:
	struct QTerm: public std::pair<size_t, size_t> {
		QTerm(size_t i, size_t j) :
				std::pair<size_t, size_t>(std::min(i, j),
						std::max(i, j)) {
		}
	};
	size_t id(size_t);
private:
	size_t _n;
	std::map<size_t, size_t> _ids;
	std::map<QTerm, double> _qTerms;
	std::map<size_t, double> _cTerms;
};
#endif
