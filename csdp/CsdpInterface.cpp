#include "CsdpInterface.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "declarations.h"
#ifdef __cplusplus
}
#endif
CsdpInterface::CsdpInterface() :
		_n(0) {
}
CsdpInterface::~CsdpInterface() {
}

size_t CsdpInterface::id(size_t i) {
	std::map<size_t, size_t>::const_iterator it(_ids.find(i));
	if (it == _ids.end()) {
		++_n;
		_ids[i] = _n;
		return _n;
	} else
		return it->second;

}
void CsdpInterface::clear() {
	_qTerms.clear();
	_cTerms.clear();
	_n = 0;
}
void CsdpInterface::add(size_t i, double v) {
	_cTerms[id(i)] += v;
}
void CsdpInterface::add(size_t i, size_t j, double v) {
	_qTerms[QTerm(id(i), id(j))] += v;
}
/**
 * @brief: ordonne les key en variables de 1..n
 */
template<class T> T * allocate(size_t n = 0) {
	T* result = (T *) malloc((n + 1) * sizeof(T));
	if (result == NULL) {
		printf("Couldn't allocate storage !\n");
		exit(1);
	}

	return result;
}
template<class T> T * allocate(size_t n, size_t m) {
	T* result = (T *) malloc(n * m * sizeof(T));
	if (result == NULL) {
		printf("Couldn't allocate storage !\n");
		exit(1);
	}
	return result;
}
template<class T> void zeros(T * t, size_t n) {
	for (size_t i(0); i < n; ++i)
		t[i] = 0.0;
}
size_t my_ijtok(size_t i, size_t j, size_t lda) {
	return ijtok(i,j,lda);
}
void CsdpInterface::bestReformulation(std::map<size_t, double> & result) const {
	result.clear();

	/// Csdp part
	blockmatrix C;
	double *b(NULL);
	constraintmatrix *constraints(NULL);
	///
	blockmatrix X, Z;
	double *y;
	double pobj, dobj;
	///
	sparseblock *blockptr;
	/// Building C
	C.nblocks = 1;
	C.blocks = allocate<blockrec>(C.nblocks);
	/*
	 * Setup the first block.
	 */
	C.blocks[1].blockcategory = MATRIX;
	C.blocks[1].blocksize = _n + 1;
	C.blocks[1].data.mat = allocate<double>(C.blocks[1].blocksize,
			C.blocks[1].blocksize);
	zeros(C.blocks[1].data.mat, C.blocks[1].blocksize * C.blocks[1].blocksize);

	for (auto const & c : _cTerms) {
		C.blocks[1].data.mat[my_ijtok(c.first + 1, c.first + 1, _n + 1)] =
				c.second;
	}

	for (auto const & q : _qTerms) {
		C.blocks[1].data.mat[my_ijtok(q.first.first + 1, q.first.second + 1,
				_n + 1)] = q.second;
		C.blocks[1].data.mat[my_ijtok(q.first.second + 1, q.first.first + 1,
				_n + 1)] = q.second;
	}

///
//	write_prob("toto.dat", _n + 1, 0, C, b, constraints);
//	initsoln(_n + 1, 0, C, b, constraints, &X, &y, &Z);
//	int ret;
//	ret = easy_sdp(_n + 1, 0, C, b, constraints, 0.0, &X, &y, &Z, &pobj, &dobj);
//	free_prob(_n + 1, 0, C, b, constraints, X, y, Z);
	// rhs
	constraints = allocate<constraintmatrix>(_n + 1);
	b = allocate<double>(_n + 1);
	zeros(b, _n + 1);
	for (size_t idCtr(1); idCtr <= _n; ++idCtr) {
		// Xii=xii
		blockptr = allocate<sparseblock>();
		blockptr->blocknum = 1;
		blockptr->blocksize = _n + 1;
		blockptr->constraintnum = idCtr;
		blockptr->next = NULL;
		blockptr->nextbyblock = NULL;
		blockptr->numentries = 2;
		blockptr->entries = allocate<double>(blockptr->numentries);
		blockptr->iindices = allocate<int>(blockptr->numentries);
		blockptr->jindices = allocate<int>(blockptr->numentries);
		blockptr->iindices[1] = 1;
		blockptr->jindices[1] = idCtr + 1;
		blockptr->entries[1] = -0.5;
		blockptr->iindices[2] = idCtr + 1;
		blockptr->jindices[2] = idCtr + 1;
		blockptr->entries[2] = 1;
//		std::cout << blockptr->numentries << std::endl;
		blockptr->next = constraints[idCtr].blocks;
		constraints[idCtr].blocks = blockptr;
	}
	// X11=1
	b[_n + 1] = 1;
	blockptr = allocate<sparseblock>();
	blockptr->blocknum = 1;
	blockptr->blocksize = _n + 1;
	blockptr->constraintnum = _n + 1;
	blockptr->next = NULL;
	blockptr->nextbyblock = NULL;
	blockptr->numentries = 1;
	blockptr->entries = allocate<double>(blockptr->numentries);
	blockptr->iindices = allocate<int>(blockptr->numentries);
	blockptr->jindices = allocate<int>(blockptr->numentries);
	blockptr->iindices[1] = 1;
	blockptr->jindices[1] = 1;
	blockptr->entries[1] = 1;
	//		std::cout << blockptr->numentries << std::endl;
	blockptr->next = constraints[_n + 1].blocks;
	constraints[_n + 1].blocks = blockptr;
	///
	write_prob("toto.dat", _n + 1, _n + 1, C, b, constraints);
	///
	initsoln(_n + 1, _n + 1, C, b, constraints, &X, &y, &Z);
	///
	int ret;
	ret = easy_sdp(_n + 1, _n + 1, C, b, constraints, 0.0, &X, &y, &Z, &pobj,
			&dobj);

	if (ret == 0)
		printf("The objective value is %.7e \n", (dobj + pobj) / 2);
	else
		printf("SDP failed.\n");

	write_sol("prob.sol", _n + 1, _n + 1, X, y, Z);

	free_prob(_n + 1, _n + 1, C, b, constraints, X, y, Z);
}
