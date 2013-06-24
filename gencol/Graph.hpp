#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "gencol.h"

class Graph;
std::ostream & operator<<(std::ostream &out, Graph const&);
class Graph {
public:
	class Edge;
public:
	typedef std::pair<size_t, double> Element;
	typedef std::vector<Element> Row;
	typedef std::vector<Row*> Container;
	typedef Container::iterator iterator;
	typedef Container::const_iterator const_iterator;
	typedef std::map<size_t, double> TempRow;
	typedef std::vector<TempRow> TempGraph;
	typedef std::list<Edge> Edges;
public:

	Graph(size_t const & = 0);
	Graph(std::string const &, std::ostream & = std::cout);
	Graph(Graph const &);

	virtual ~Graph();

	void fill(TempGraph const &);

	void allocate(size_t const &);
	void desallocate();
	void readRotta(std::string const &);
	void finalize();

	void read(std::string const &, std::ostream & = std::cout);


	double const & degree(size_t const &) const;
	double const & degree() const;
	double const & squareDegree() const;
	size_t & nbEdge(void);
	size_t const & nbEdge(void) const;
	size_t nbNodes() const;

	void clear();
	void writeCplusplus(std::string const &) const;

	Row & operator()(size_t const &);
	Row const & operator()(size_t const &)const;

	Edges const & edges()const;

protected:
	Container _rows;
	DoubleVector _degrees;
	double _degree;
	double _squareDegree;
	size_t _nbEdge;

	Edges _edges;

};

inline std::ostream & operator<<(std::ostream &out, Graph const&g) {
	out << "nbObs:\t" << g.nbNodes() << std::endl;
	for (size_t id(0); id < g.nbNodes(); ++id) {
		out << std::setw(4) << id << " | ";
		for (auto const  & r : g(id) ) {
			out << r.first << " , " << r.second << " ; ";
		}
		out << std::endl;
	}
	return out;
}

inline double const & Graph::degree(size_t const&nodeId) const {
	return _degrees[nodeId];
}

inline double const & Graph::degree() const {
	return _degree;
}

inline double const & Graph::squareDegree() const {
	return _squareDegree;
}

inline size_t const & Graph::nbEdge() const {
	return _nbEdge;
}

inline size_t & Graph::nbEdge() {
	return _nbEdge;
}

inline size_t Graph::nbNodes() const {
	return _rows.size();
}

inline void Graph::clear() {
	_rows.clear();
}

class Graph::Edge
{
public:
	size_t _i;
	size_t _j;
	Double _v;
	Edge(size_t i, size_t j, Double v):_i(i),_j(j),_v(v){}
};

#endif /* GRAPH_HPP */
