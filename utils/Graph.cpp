#include "Graph.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>

void Graph::read(const std::string & fileName, std::ostream & out) {
	std::string line;
	std::string tempString;

	size_t tempPoint;
	double tempSimilarity;
	size_t lineNumber = 0;
	size_t nbObs;

	bool isFractionnaire(false);

	TempGraph tempGraph;
	// Open the file which contains the points
	std::ifstream file(fileName.c_str());
	if (file.is_open()) {
		// Initialisation of the iterator for the reading
		// Variable's name reading
		std::getline(file, line);
		//istringstream lineStream(line);
		std::istringstream in(line);
		if (!(in >> nbObs)) {
			out << "n'a pas pu lire le nombre d'elements" << std::endl;
		}
		if (!(in >> _nbEdge)) {
			out << "n'a pas pu lire le nombre d'arêtes" << std::endl;
		}
		in >> isFractionnaire;
		out << std::endl;
		out << std::setw(20) << "Nodes" << std::setw(10) << nbObs << "\n";
		out << std::setw(20) << "Edges" << std::setw(10) << _nbEdge << "\n";
		out << std::setw(20) << (isFractionnaire ? "WEIGHTED" : "UNWEIGHTED")
				<< "\n";

		tempGraph.resize(nbObs);
		//		allocate(nbObs);
		// While there are points
		while (std::getline(file, line)) {
			std::istringstream lineStream(line);
			//       out<<lineNumber<< std::endl;
			while (lineStream >> tempPoint) {
				if (isFractionnaire) {
					if (!(lineStream >> tempSimilarity)) {
						out << "probleme de format l." << lineNumber
								<< std::endl;
					}
				} else
					tempSimilarity = 1.0;
				//					(*this)[lineNumber].insert(tempPoint - 1, tempSimilarity);
				tempGraph[lineNumber][tempPoint - 1] = tempSimilarity;
			}
			++lineNumber;
		}
		file.close();
		fill(tempGraph);
		buildDegrees();

	} else {
		out << "impossible d'ouvrir le fichier : " << fileName << std::endl;
		exit(0);
	}

}
void Graph::fill(TempGraph const & rhs) {
	allocate(rhs.size());
	for (size_t i(0); i < rhs.size(); ++i) {
		_rows[i]->resize(rhs[i].size());
		//		size_t index(0);
		std::copy(rhs[i].begin(), rhs[i].end(), adjacentList(i).begin());
		//		for (TempRow::const_iterator ite(rhs[i].begin()); ite != rhs[i].end(); ++ite) {
		//			if (ite->first != i) {
		//
		//			} else {
		//				std::cout << "LOOP" << std::endl;
		//			}
		//			(*_rows[i])[index].first = ite->first;
		//			(*_rows[i])[index].second = ite->second;
		//			++index;
		//		}
	}
}
void Graph::writeCplusplus(std::string const &fileName) const {
	std::cout << "exporting in\t" << fileName << std::endl;
	std::ofstream file(fileName.c_str());
	file << "	getGraph().allocate(" << nbNodes() << ");" << std::endl;
	for (size_t i(0); i < nbNodes(); ++i) {
		for (auto const & e : adjacentList(i)) {
			file << "getGraph()[" << i << "][" << e.first << "]=" << e.second
					<< ";" << std::endl;
		}
	}
	file.close();
}
Graph::Graph(size_t const & i) {
	allocate(i);
}

Graph::Graph(std::string const& fileName, std::ostream & out) {

	out << std::endl << "Reading " << fileName;
	read(fileName, out);
	buildDegrees();
}

Graph::Graph(Graph const& orig) {
	allocate(orig.nbNodes());
	for (size_t i(0); i < orig.nbNodes(); ++i) {
		*_rows[i] = orig.adjacentList(i);
	}
	_nbEdge = orig._nbEdge;
	_degrees = orig._degrees;
	_degree = orig._degree;
	_squareDegree = orig._squareDegree;

}

Graph::~Graph() {
	desallocate();
}

void Graph::allocate(size_t const&i) {
	_degrees.assign(i, 0);
	desallocate();
	_rows.resize(i);
	for (size_t p = 0; p < i; ++p)
		_rows[p] = new AdjacentList;
	//		rows.push_back(Container::value_type());
	//		rows.push_back(Container::value_type(new row_t));
}

void Graph::desallocate() {
	for (size_t p = 0; p < nbNodes(); ++p)
		delete _rows[p];
}

void Graph::buildDegrees() {
	_degrees.resize(nbNodes());
	_degree = 0;
	for (size_t p = 0; p < nbNodes(); ++p) {
		_degrees[p] = 0;
		for (auto const & e : adjacentList(p)) {
			_degrees[p] += e.second;
			if (e.first == p)
				_degrees[p] += e.second;
		}
		_degree += _degrees[p];
	}
	_squareDegree = _degree * _degree;
}

void Graph::readRotta(std::string const & fileName) {
	typedef std::map<size_t, double> TempRow;
	typedef std::vector<TempRow> TempGraph;
	TempGraph tempGraph;
	{
		std::ifstream file(fileName.c_str());
		std::string line;
		std::getline(file, line);
		size_t nbObs;
		while (std::getline(file, line)) {
			std::istringstream stream(line);
			std::string waste;
			stream >> _nbEdge;
			stream >> nbObs;
			//			std::cout << nbEdge << " | " << waste << " | " << nbObs
			//					<< std::endl;
		}
		++nbObs;
		std::cout << nbObs << std::endl;
		tempGraph.resize(nbObs);
		file.close();
	}
	{
		std::ifstream file(fileName.c_str());
		size_t i, j;
		double v;
		std::string line, waste;
		std::getline(file, line);
		while (std::getline(file, line)) {
			std::istringstream stream(line);
			stream >> waste;
			stream >> i;
			stream >> j;
			stream >> v;
			tempGraph[i].insert(TempRow::value_type(j, v));
		}
		file.close();
	}
	allocate(tempGraph.size());
	for (size_t i(0); i < tempGraph.size(); ++i) {
		AdjacentList & row(*_rows[i]);
		row.resize(tempGraph[i].size());
		size_t index(0);
		for (TempRow::const_iterator ite(tempGraph[i].begin());
				ite != tempGraph[i].end(); ++ite) {
			row[index] = std::pair<size_t, double>(ite->first, ite->second);
			++index;
		}
	}
	buildDegrees();
}

Graph::AdjacentList & Graph::adjacentList(size_t const & i) {
	return *_rows[i];
}
Graph::AdjacentList const & Graph::adjacentList(size_t const & i) const {
	return *_rows[i];
}

std::ostream & operator<<(std::ostream &out, Graph const&g) {
	out << "nbObs:\t" << g.nbNodes() << std::endl;
	for (size_t id(0); id < g.nbNodes(); ++id) {
		out << std::setw(4) << id << " | ";
		for (auto const & e : g.adjacentList(id)) {
			out << e.first << " , " << e.second << " ; ";
		}
		out << std::endl;
	}
	return out;
}

double const & Graph::degree(size_t const & nodeId) const {
	return _degrees[nodeId];
}

double const & Graph::degree() const {
	return _degree;
}

double const & Graph::squareDegree() const {
	return _squareDegree;
}

size_t Graph::nbEdges() const {
	return _nbEdge;
}

size_t & Graph::nbEdges() {
	return _nbEdge;
}

size_t Graph::nbNodes() const {
	return _rows.size();
}

void Graph::clear() {
	_rows.clear();
}
