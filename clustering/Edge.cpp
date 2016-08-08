#include "Edge.h"

Edges::Edges() {

}
Edges::~Edges() {

}

bool Edges::read(std::string const & fileName, std::ostream & stream) {
	std::ifstream file(fileName.c_str());
	std::string line;
	int i;
	int j;
	double v;
	while (std::getline(file, line)) {
		std::stringstream buffer(line);
		buffer >> i;
		buffer >> j;
		if (buffer >> v)
			push_back(Edge(i - 1, j - 1, v));
		else
			push_back(Edge(i - 1, j - 1));
	}
	file.close();
	return true;
}
void Edges::print(std::ostream & stream) const {
	for (auto const & edge : *this) {
		stream << std::setw(6) << edge._i;
		stream << std::setw(6) << edge._j;
		stream << std::setw(6) << edge._v;
		stream << std::endl;
	}
}
std::ostream & operator<<(std::ostream & stream, Edges const & rhs) {
	rhs.print(stream);
	return stream;
}
