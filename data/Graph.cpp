#include "Graph.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>

void Graph::read(const std::string & fileName, std::ostream * out) {
	std::string line;
	std::string tempString;

	size_t nbObs;
	_isEdgeWeighted = false;

	std::ifstream file(fileName.c_str());
	if (file.is_open()) {
		std::getline(file, line);
		std::istringstream in(line);
		if (!(in >> nbObs) && out != 0) {
			*out << "n'a pas pu lire le nombre d'elements" << std::endl;
		}
		if (!(in >> _nbEdge) && out != 0) {
			*out << "n'a pas pu lire le nombre d'arêtes" << std::endl;
		}
		in >> _isEdgeWeighted;
		if (out != 0) {
			*out << std::endl;
			*out << std::setw(20) << "Nodes" << std::setw(10) << nbObs << "\n";
			*out << std::setw(20) << "Edges" << std::setw(10) << _nbEdge
					<< "\n";
			*out << std::setw(20)
					<< (_isEdgeWeighted ? "WEIGHTED" : "UNWEIGHTED") << "\n";
		}

		allocate(nbObs);
		// While there are points
		size_t tempPoint;
		double tempSimilarity;
		size_t lineNumber = 0;
		while (std::getline(file, line)) {
			_begin.push_back(_edges.size());
			std::istringstream lineStream(line);
			//       out<<lineNumber<< std::endl;
			while (lineStream >> tempPoint) {
				if (_isEdgeWeighted) {
					if (!(lineStream >> tempSimilarity) && out != 0) {
						*out << "probleme de format l." << lineNumber
								<< std::endl;
					}
				} else
					tempSimilarity = 1.0;
				_edges.push_back(std::make_pair(tempPoint, tempSimilarity));
				//(*this)[lineNumber].insert(tempPoint - 1, tempSimilarity);
				//tempGraph[lineNumber][tempPoint - 1] = tempSimilarity;
			}
			++lineNumber;
			_size.push_back(_edges.size() - _begin.back());

		}
		file.close();
		buildDegrees();

	} else {
		*out << "impossible d'ouvrir le fichier : " << fileName << std::endl;
		exit(0);
	}

}
Graph::Graph(size_t i) {
	allocate(i);
}

Graph::Graph(std::string const& fileName, std::ostream * out) {

	if (out != 0)
		*out << std::endl << "Reading " << fileName;
	read(fileName, out);
	buildDegrees();
}

Graph::~Graph() {
}

void Graph::allocate(size_t n) {
	_edges.clear();
	_begin.clear();
	_size.clear();

	_edges.reserve(2 * _nbEdge);
	_begin.reserve(n);
	_size.reserve(n);
}

void Graph::buildDegrees() {
	_nodeWeights.resize(nbObs());
	for (size_t p = 0; p < nbObs(); ++p) {
		_nodeWeights[p] = 0;
		LinksIterator ite(links(p));
		while (ite.goNext()) {
			_nodeWeights[p] += ite.value();
			if (ite.index() == p)
				_nodeWeights[p] += ite.index();
		}
	}
}
std::ostream & operator<<(std::ostream &out, Graph const&g) {
	//	out << "nbObs:\t" << g.nbNodes() << std::endl;
	//	for (size_t id(0); id < g.nbNodes(); ++id) {
	//		out << std::setw(4) << id << " | ";
	//		FOR_EACH_CONST(e , g.links(id)) {
	//		out << e.first << " , " << e.second << " ; ";
	//	}
	//		out << std::endl;
	//	}
	return out;
}

double const & Graph::weight(size_t nodeId) const {
	return _nodeWeights[nodeId];
}

size_t Graph::nbLinks() const {
	return _nbEdge;
}

size_t & Graph::nbLinks() {
	return _nbEdge;
}

size_t Graph::nbObs() const {
	return _begin.size();
}

LinksIterator Graph::links(size_t node) const {
	return LinksIterator(_edges.begin() + _begin[node],
			_edges.begin() + _begin[node] + _size[node]);

}

double const & Graph::link(size_t i, size_t j) const {
	if (_size[j] < _size[i])
		return link(j, i);
	LinksIterator ite(links(i));
	while (ite.goNext()) {
		if (ite.index() == j)
			return ite.value();

	}
	return Values::Double::Zero;

}
