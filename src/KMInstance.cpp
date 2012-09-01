/*
 * RawData.cpp
 *
 *  Created on: 9 août 2012
 *      Author: manuel
 */

#include "src/KMInstance.hpp"
#include "src/IPartition.hpp"

KMInstance::KMInstance() {
	_cst = 0;
}

KMInstance::KMInstance(KMInstance const & instance,
		Aggregations const & aggregations) {
	allocate(aggregations.v.size(), instance.nbAtt());
	std::fill_n(_weights.begin(), _weights.size(), Zero<Double>());

	for (size_t i(0); i < aggregations.v.size(); ++i) {
		for (auto const & j : aggregations.v[i])
			for (size_t d(0); d < nbAtt(); ++d)
				_data.plus(i, d, instance.get(j, d));

		assert(aggregations.v[i].size() > 0);
		_weights[i] = static_cast<Double>(aggregations.v[i].size());
		assert(_weights[i]> 0);
		for (size_t d(0); d < nbAtt(); ++d)
			_data.get(i, d) /= _weights[i];
		if (_weights[i] > 1) {
			for (auto const & j : aggregations.v[i])
				for (size_t d(0); d < nbAtt(); ++d)
					_cst += std::pow(get(i, d) - instance.get(j, d), 2);
		}
	}
	// les contraintes cannot link
	for (size_t i(0); i < instance.nbObs(); ++i) {
		for (auto const & j : instance.cannotLinks().get(i))
			addCannotLink(aggregations.newIds[i], aggregations.newIds[j]);

	}
}

void KMInstance::allocate(size_t n, size_t m) {
	_data = RectMatrix(n, m);
	_cst = Zero<Double>();
	_weights.assign(n, One<Double>());
	_must = KMConstraints(n);
	_cannot = KMConstraints(n);
}
void KMInstance::readData(std::string const & fileName) {
	std::ifstream file(fileName.c_str());
	if (file.good()) {
		size_t n;
		size_t m;
		file >> n;
		file >> m;
		allocate(n, m);
		for (size_t i(0); i < n; ++i) {
			for (size_t j(0); j < m; ++j) {
				file >> _data.get(i, j);
			}
		}
	} else {
		OUT<< "unable to open file "<<fileName<<"\n";
	}
}

void KMInstance::readConstraints(std::string const & fileName) {
	std::ifstream file(fileName.c_str());
	if (file.good()) {
		size_t i;
		size_t j;
		int v;
		while (file >> i && file >> j && file >> v) {
			if (v > 0)
				_must.newCtr(i, j);
			else
				_cannot.newCtr(i, j);
		}
	}
}

void KMInstance::buildMustLink(Aggregations & result) const {
	std::list<IntSet> aggregations;
	std::vector<std::list<IntSet>::iterator> temp(nbObs(), aggregations.end());
	size_t n(0);
	result.newIds.assign(nbObs(), -1);

	for (size_t i(0); i < nbObs(); ++i) {
		auto it(temp[i]);
		if (it == aggregations.end()) {
			aggregations.push_front(IntSet());
			temp[i] = aggregations.begin();
			temp[i]->insert(i);
			result.newIds[i] = n;
			++n;
		}
		for (auto const & j : _must.get(i)) {
			temp[j] = temp[i];
			result.newIds[j] = result.newIds[i];
			temp[j]->insert(j);
		}
	}

	OUT<< "found "<<n<<" aggregated point\n";
	result.v.assign(n, IntSet());
	std::copy(aggregations.rbegin(), aggregations.rend(), result.v.begin());
	for (auto const & id : result.newIds) {
		assert(id < n);
	}
}


bool KMInstance::feasible(IPartition const & p) const {
	for (auto const & ctr : _must) {
		if (p.label(ctr.first) != p.label(ctr.second)) {
			OUT<< ctr.first << " should be with "<<ctr.second<<"\n";
			return false;
		}
	}
	for(auto const & ctr : _cannot) {
		if (p.label(ctr.first) == p.label(ctr.second)) {
			OUT << ctr.first << " should not be with "<<ctr.second<<"\n";
			return false;
		}
	}
	return true;
}

void KMInstance::cpp(std::ostream & stream) const {
	std::string s;

	stream << "_weights.assign(" << nbObs() << ",One<Double>());\n";
	stream << "_must = KMConstraints( ";
	stream << nbObs() << " , ";
	stream << " {";
	for (auto const & p : _must) {
		stream << p.first << " , " << p.second << " , ";
	}
	stream << " } );\n";
	stream << "_cannot = KMConstraints( ";
	stream << nbObs() << " , ";
	stream << " {";
	for (auto const & p : _cannot) {
		stream << p.first << " , " << p.second << " , ";
	}
	stream << " } );\n";
	stream << "_cst = Zero<Double>();\n";
	stream << "_data  = RectMatrix(";
	stream << nbObs() << " , " << 13 << " , { ";
	std::copy(_data.matrix().begin(), _data.matrix().end(),
			std::ostream_iterator<Double>(stream, " , "));
	stream << "} );\n";
}
std::ostream & operator<<(std::ostream &stream, KMInstance const &rhs) {
	stream << rhs.data();
	return stream;

}
