#ifndef LP_BUFFER_HPP
#define LP_BUFFER_HPP

#include "gencol.h"

typedef struct cpxlp* CPXLPptr;
typedef struct cpxenv* CPXENVptr;

class RowBuffer {
public:
public:
	RowBuffer() {

	}
	virtual ~RowBuffer() {
	}
public:
	int size() const;
	int nz() const;
	virtual void add(Double rhsObj, char type, std::string const & name);
	virtual void add(size_t, Double);
	virtual void add(CPXENVptr, CPXLPptr);
	virtual void reserve(size_t, size_t, size_t);

//	virtual void print(std::ostream &) const;
	int const * index() const;
	double const * value() const;

	Double & rhsObj(size_t);
	virtual void clear();
protected:
	std::vector<int> _index;
	std::vector<int> _begin;
	std::vector<double> _value;

	// column : type of variable
	// row : type of row
	std::vector<char> _type;
	// column : obj
	// row : rhs
	std::vector<double> _objRhs;

	std::vector<std::string> _name;
};

class ColumnBuffer: public RowBuffer {
public:
	ColumnBuffer() :
			RowBuffer() {
		_only_continous = true;
	}
	virtual ~ColumnBuffer() {
	}
public:
	virtual void clear();
	virtual void reserve(size_t, size_t, size_t);
//	virtual void print(std::ostream &) const;
	virtual void add(CPXENVptr, CPXLPptr);
	virtual void add(Double rhsObj, char type, Double lower, Double upper,
			std::string const & name);
	using RowBuffer::add;
private:
	std::vector<double> _lower;
	std::vector<double> _upper;
	bool _only_continous;

};

#endif 
