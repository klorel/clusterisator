/*
 * RawData.cpp
 *
 *  Created on: 9 août 2012
 *      Author: manuel
 */

#include "src/KMInstance.hpp"
#include "src/IPartition.hpp"

KMInstance::KMInstance(size_t nbObs, size_t nbAtt) {
	allocate(nbObs, nbAtt);
}

KMInstance::KMInstance(KMInstance const & instance,
		Aggregations const & aggregations) {
	allocate(aggregations.v.size(), instance.nbAtt());
	std::fill_n(_weights.begin(), _weights.size(), 0);

	size_t n(0);
	for (size_t i(0); i < aggregations.v.size(); ++i) {
		if(!aggregations.v[i].empty()){
			for (auto const & j : aggregations.v[i])
				for (size_t d(0); d < nbAtt(); ++d)
					_data.plus(n, d, instance.get(j, d));

			assert(aggregations.v[i].size() > 0);
			_weights[n] = static_cast<Double>(aggregations.v[i].size());
			assert(_weights[n]> 0);
			for (size_t d(0); d < nbAtt(); ++d)
				_data.get(n, d) /= _weights[n];
			if (_weights[n] > 1) {
				for (auto const & j : aggregations.v[i])
					for (size_t d(0); d < nbAtt(); ++d)
						_cst += std::pow(get(n, d) - instance.get(j, d), 2);
			}
			++n;
		}
	}
	// les contraintes cannot link
	for (size_t i(0); i < instance.nbObs(); ++i) {
		for (auto const & j : instance.cannotLinks().get(i))
			addCannotLink(aggregations.newIds[i], aggregations.newIds[j]);

	}
}

void KMInstance::allocate(size_t nbObs, size_t nbAtt) {
	_data = RectMatrix(nbObs, nbAtt);
	_cst = 0;
	_weights.assign(nbObs, 1);
	_must = KMConstraints(nbObs);
	_cannot = KMConstraints(nbObs);
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
	result.v.clear();	
	size_t n(0);
	std::vector<IntSet> temp(nbObs());
	// au départ on a des singletons
	for(size_t i(0); i<nbObs(); ++i){
		temp[i].insert(i);
	}
	result.newIds.assign(nbObs(), -1);
	// on fusionne les 
	for(auto const & p : _must){
		size_t const i(p.first);
		size_t const j(p.second);
		for(auto & si : temp[i]){
			for(auto & sj : temp[j]){
				temp[si].insert(temp[sj].begin(),temp[sj].end());
				temp[sj].insert(temp[si].begin(),temp[si].end());
			}
		}
	}
	std::set<IntSet> trimmer;
	for(auto & it : temp)
		if(!trimmer.insert(it).second)
			it.clear();
	result.v.reserve(nbObs());
	for(size_t i(0); i<nbObs(); ++i){
		if(!(temp[i].empty())){
			for(auto const & j :temp[i])
				result.newIds[j]=n;
			result.v.push_back( temp[i] );
			++n;
		}
	}
	OUT<< "found "<<n<<" aggregated point\n";
	//for (auto const & id : result.newIds) {
	//	assert(id < n);
	//}
}

bool KMInstance::feasible(IPartition const & p) const {
	assert(p.nbObs() == nbObs());

	for (auto const & ctr : _must) {
		if (p.label(ctr.first) != p.label(ctr.second)) {
			OUT<< ctr.first << " should be with "<<ctr.second<<"\n";
			return false;
		}
	}

	for (auto const & ctr : _cannot) {
		if (p.label(ctr.first) == p.label(ctr.second)) {
			OUT<< ctr.first << " should not be with "<<ctr.second<<"\n";
			return false;
		}
	}

	return true;
}

void KMInstance::cpp(std::ostream & stream) const {
	std::string s;

	stream << "_weights.assign(" << nbObs() << ",1);\n";
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
	stream << "_cst = 0;\n";
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
