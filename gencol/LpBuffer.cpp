#include "LpBuffer.hpp"

#include <cplex.h>

Double & RowBuffer::rhsObj(size_t id) {
	assert(id < _objRhs.size());
	return _objRhs[id];
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

void RowBuffer::reserve(size_t row, size_t col, size_t nz) {
	_begin.reserve(1 + col);
	_index.reserve(nz);
	_value.reserve(nz);

	_type.reserve(row);
	_objRhs.reserve(row);

	_name.reserve(row);
}

void RowBuffer::add(CPXENVptr env, CPXLPptr lp) {
	_begin.push_back(nz());
//	if (_name.empty()) {
	CPXaddrows(env, lp, 0, size(), nz(), _objRhs.data(), _type.data(),
			_begin.data(), index(), value(), NULL, NULL);
//	} else {
////		MY_PRINT(_name.size());
////		MY_PRINT(size());
//		assert(
//				(int )_name.size() == size()
//						&& "you should provide a name for each element");
//
//		std::vector<char*> cpxName(_name.size());
//		for (size_t i(0); i < _name.size(); ++i) {
//			cpxName[i] = const_cast<char*>(_name[i].c_str());
//		}
//		CPXaddrows(env, lp, 0, size(), nz(), _objRhs.data(), _type.data(),
//				_begin.data(), index(), value(), NULL, cpxName.data());
//	}
	_begin.pop_back();
}
int const * RowBuffer::index() const {
	return nz() == 0 ? NULL : _index.data();
}
double const * RowBuffer::value() const {
	return nz() == 0 ? NULL : _value.data();
}
void ColumnBuffer::reserve(size_t row, size_t col, size_t nz) {
	_begin.reserve(1 + row);
	_index.reserve(nz);
	_value.reserve(nz);

	_type.reserve(col);
	_objRhs.reserve(col);

	_name.reserve(col);
}

void ColumnBuffer::add(CPXENVptr env, CPXLPptr lp) {
	_begin.push_back(nz());

//	if (_name.empty()) {
		CPXaddcols(env, lp, size(), nz(), _objRhs.data(), _begin.data(),
				index(), value(), _lower.data(), _upper.data(), NULL);
//	} else {
//		assert(
//				(int )_name.size() == size()
//						&& "you should provide a name for each element");
//
//		std::vector<char*> cpxName(_name.size());
//		for (size_t i(0); i < _name.size(); ++i) {
//			cpxName[i] = const_cast<char*>(_name[i].c_str());
//		}
//		CPXaddcols(env, lp, size(), nz(), _objRhs.data(), _begin.data(),
//				index(), value(), _lower.data(), _upper.data(), cpxName.data());
//	}
	_begin.pop_back();
	std::vector<int> sequence(size());
	for (int i(0); i < size(); ++i)
		sequence[i] = i;
	CPXchgctype(env, lp, size(), sequence.data(), _type.data());

}
void ColumnBuffer::add(Double rhsObj, char type, Double lower, Double upper,
		std::string const & name) {
	_objRhs.push_back(rhsObj);
	_begin.push_back(nz());
	_type.push_back(type);

	_lower.push_back(lower);
	_upper.push_back(upper);
	if (!name.empty())
		_name.push_back(name);
}
