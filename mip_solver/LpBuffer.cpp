#include "LpBuffer.h"

Double & RowBuffer::rhsObj(int id) {
	assert(id < _objRhs.size());
	return _objRhs[id];
}
double const * RowBuffer::rhsObj()const {
	return _objRhs.data();
}
int RowBuffer::nz() const {
	return (int) _value.size();
}
int RowBuffer::size() const {
	return (int) _type.size();
}
void RowBuffer::add(Double rhsObj, char type, std::string const & name) {
	//	MY_PRINT(name);
	_objRhs.push_back(rhsObj);
	_begin.push_back(nz());
	_type.push_back(type);
	if (!name.empty())
		_name.push_back(name);
}
void RowBuffer::add(size_t index, Double value) {
	_index.push_back((int) index);
	_value.push_back(value);

}
void RowBuffer::get_names(std::vector<char> & result)const {
	assert((int)_name.size() == size() && "you should provide a name for each element");
	size_t n(0);
	for (auto const & s : _name)
		n += (s.size() + 1);
	result.assign(n, '\0');
	size_t pos(0);
	for (auto const & s : _name) {
		std::copy(s.begin(), s.end(), result.begin() + pos);
		pos += s.size() + 1;
	}

}
void RowBuffer::reserve(int row, int col, int nz) {
	_begin.reserve(1 + col);
	_index.reserve(nz);
	_value.reserve(nz);

	_type.reserve(row);
	_objRhs.reserve(row);

	_name.reserve(row);
}

void RowBuffer::clear() {
	_begin.clear();
	_index.clear();
	_value.clear();

	_type.clear();
	_objRhs.clear();

	_name.clear();
}
StringVector & RowBuffer::name() {
	return _name;
}
StringVector const & RowBuffer::name()const {
	return _name;
}

void RowBuffer::add(CPXENVptr env, CPXLPptr lp) {

}


char const * RowBuffer::type() const {
	return size() == 0 ? NULL : _type.data();
}
double const * ColumnBuffer::lower()const {
	return size() == 0 ? NULL : _lower.data();
}

double const * ColumnBuffer::upper()const {
	return size() == 0 ? NULL : _upper.data();
}


int const * RowBuffer::index() const {
	return nz() == 0 ? NULL : _index.data();
}
int const * RowBuffer::begin() const {
	return nz() == 0 ? NULL : _begin.data();
}
double const * RowBuffer::value() const {
	return nz() == 0 ? NULL : _value.data();
}


void RowBuffer::add_last_begin() {
	_begin.push_back(nz());
}
void RowBuffer::rem_last_begin() {
	_begin.pop_back();
}
void ColumnBuffer::reserve(int row, int col, int nz) {
	_begin.reserve(1 + row);
	_index.reserve(nz);
	_value.reserve(nz);

	_type.reserve(col);
	_objRhs.reserve(col);

	_name.reserve(col);
}

void ColumnBuffer::clear() {
	_begin.clear();
	_index.clear();
	_value.clear();

	_type.clear();
	_objRhs.clear();

	_name.clear();
}

void ColumnBuffer::add(CPXENVptr env, CPXLPptr lp) {

}
void ColumnBuffer::add(Double rhsObj, char type, Double lower, Double upper,
		std::string const & name) {
	_objRhs.push_back(checkCoeff(rhsObj));
	_begin.push_back(nz());
	_type.push_back(type);
	if (type != _continuous)
		_only_continous = false;
	_lower.push_back(lower);
	_upper.push_back(upper);
	if (!name.empty())
		_name.push_back(name);
}

RowBuffer::RowBuffer() :
		_index(), _begin(), _value(), _type(), _objRhs(), _name() {

}
RowBuffer::~RowBuffer() {
}

ColumnBuffer::ColumnBuffer(char continuous) :
		RowBuffer() {
	_only_continous = true;
	_continuous  = continuous;
}
ColumnBuffer::~ColumnBuffer() {
}

bool ColumnBuffer::only_continous()const {
	return _only_continous;
}