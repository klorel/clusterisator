/*
 * RawData.cpp
 *
 *  Created on: 9 août 2012
 *      Author: manuel
 */

#include "KMInstance.hpp"
#include "IPartition.hpp"

KMInstance::KMInstance(size_t nbObs, size_t nbAtt) {
	allocate(nbObs, nbAtt);
}

KMInstance::KMInstance(KMInstance const & instance,
		Aggregations const & aggregations) {
	allocate(aggregations.v.size(), instance.nbAtt());
	std::fill_n(_weights.begin(), _weights.size(), 0);

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

Double KMInstance::distance(size_t i, size_t j)const{
	Double result( 0 );
	for (size_t d(0); d < nbAtt(); ++d)
		result+=std::pow(get(i, d) - get(j, d), 2);
	return std::sqrt(result);
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
	typedef std::map<std::pair<size_t,size_t>, size_t > Toto;
	Toto toto;
	size_t n(0);
	result.newIds.assign(nbObs(), -1);
	for (auto const & ij : _must.all()) {
		Toto::iterator it(toto.end());
		for(auto const & j : _must.get(ij.first)){
			std::pair<size_t,size_t> const p(std::min(j, ij.first), std::max(j, ij.first));
			Toto::iterator jt(toto.find(p));
			if(jt != toto.end()){
				it = jt;
				break;
			}
		}
		for(auto const & j : _must.get(ij.second)){
			std::pair<size_t,size_t> const p(std::min(j, ij.second), std::max(j, ij.second));
			Toto::iterator jt(toto.find(p));
			if(jt != toto.end()){				
				if(it != toto.end()&&it->second!=jt->second){					
					std::map<size_t,size_t> sort;
					// fusion
					sort[it->second] = 0;
					sort[jt->second] = 0;
					size_t p(0);
					for(auto & t : toto){
						std::map<size_t,size_t>::const_iterator found(sort.find(t.second));
						if(found == sort.end()){
							++p;
							found = sort.insert( std::make_pair(t.second, p)).first;
						}
						t.second = found->second;
					}
					assert(n==p+2);
					n = p+1;
				}else{
					it = jt;
					break;
				}
			}
		}

		if(it == toto.end()){
			it = toto.insert(std::make_pair(ij, n)).first;
			++n;
		}else{
			toto[ij] = it->second;
		}
	}
	OUT<< "found "<<n<<" aggregated point\n";
	result.v.reserve(nbObs());
	result.v.assign(n, IntSet());
	for(auto const & ij : toto){
		result.newIds[ij.first.first]=ij.second;
		result.newIds[ij.first.second]=ij.second;
		result.v[ij.second].insert(ij.first.first);
		result.v[ij.second].insert(ij.first.second);
	}
	
	
	for(size_t i(0); i<nbObs(); ++i){
		if(result.newIds[i]> nbObs()){
			result.newIds[i]=n;
			result.v.push_back(IntSet());
			result.v[n].insert(i);
			++n;
		}
	}

	for (auto const & id : result.newIds) {
		assert(id < n);
	}
	for(auto const & i : result.v){
		assert(!i.empty());
		size_t const l(result.newIds[*i.begin()]);
		for(auto const & j : i)
			assert( result.newIds[j] == l);
	}
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
