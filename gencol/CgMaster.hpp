#ifndef CG_MASTER_HPP
#define CG_MASTER_HPP

#include "src/common.h"

class CgMaster;

class CgInstance;
class Predicate;
typedef std::set<size_t> Column;
typedef std::vector<Column*> Columns;
typedef std::set<Column,Predicate > ColumnTrimmer;

class Predicate{
public:
	bool operator()(Column const * const p, Column const * const q)const{
		return (*p)<(*q);
	}
};

class CgMaster{
public:
public:

private:
	CgInstance const * const _instance;
	Columns _columns;
	ColumnTrimmer _columnTrimmer;
};
#endif /* CG_MASTER_HPP */
