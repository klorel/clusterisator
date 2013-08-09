#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "gencol.h"

class Graph;
std::ostream & operator<<(std::ostream &out, Graph const&);

class Edge;
typedef std::list<Edge> Edges;

class Graph {
public:

public:
	typedef std::pair<size_t, Double> Element;
	typedef std::vector<Element> Row;
	typedef std::vector<Row*> Container;
	typedef Container::iterator iterator;
	typedef Container::const_iterator const_iterator;
	typedef std::map<size_t, Double> TempRow;
	typedef std::vector<TempRow> TempGraph;
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

	Double const & degree(size_t const &) const;
	Double const & degree() const;
	Double const & squareDegree() const;
	size_t & nbEdge(void);
	size_t const & nbEdge(void) const;
	size_t nbNodes() const;

	void clear();
	void writeCplusplus(std::string const &) const;

	Row & operator()(size_t const &);
	Row const & operator()(size_t const &) const;

	Edges const & edges() const;

protected:
	Container _rows;
	DoubleVector _degrees;
	Double _degree;
	Double _squareDegree;
	size_t _nbEdge;

	Edges _edges;

};

inline std::ostream & operator<<(std::ostream &out, Graph const&g) {
	out << "nbObs:\t" << g.nbNodes() << std::endl;
	for (size_t id(0); id < g.nbNodes(); ++id) {
		out << std::setw(4) << id << " | ";
		for (auto const & r : g(id)) {
			out << r.first << " , " << r.second << " ; ";
		}
		out << std::endl;
	}
	return out;
}

inline Double const & Graph::degree(size_t const&nodeId) const {
	return _degrees[nodeId];
}

inline Double const & Graph::degree() const {
	return _degree;
}

inline Double const & Graph::squareDegree() const {
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

class Edge {
public:
	size_t _i;
	size_t _j;
	Double _v;
	Edge(size_t i, size_t j, Double v) :
			_i(i), _j(j), _v(v) {
	}
};

#endif /* GRAPH_HPP */
