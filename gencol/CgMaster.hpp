#ifndef CG_MASTER_HPP
#define CG_MASTER_HPP
#include "gencol.h"
#include "Column.hpp"

class CgMaster;
class CgInstance;
class Predicate;



class CgMaster{
public:
public:

private:
	CgInstance const * const _instance;
	Columns _columns;
	ColumnTrimmer _columnTrimmer;
};
#endif /* CG_MASTER_HPP */
