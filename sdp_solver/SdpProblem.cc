#include "SdpProblem.h"

void SdpProblem::add(int i1, int i2, int i3, int i4, double v) {
	_matrix[{i1, i2, i3, i4}] += v;
}

int SdpProblem::newy(double cost) {
	_b.push_back(cost);
	return static_cast<int>(_b.size());
}

int SdpProblem::newBlock(int size) {
	Block block;
	block._size = size;

	if (!_blocks.empty()) {
		block._begin = _blocks.back()._begin + _blocks.back().nz();
		block._previous_dim = _blocks.back()._previous_dim + std::abs(_blocks.back()._size);
	}
	else {
		block._begin = 0;
		block._previous_dim = 0;
	}
	_blocks.push_back(block);
	return static_cast<int>(_blocks.size());
}
void SdpProblem::print(std::string const & fileName)const {
	std::ofstream file(fileName.c_str());
	print(file);
	file.close();
}

void SdpProblem::print(std::ostream & stream)const {
	stream << _b.size() << std::endl;
	stream << _blocks.size() << std::endl;
	for (auto const & block : _blocks) {
		stream << block._size << " ";
	}
	stream << std::endl;
	for (auto const rhs : _b)
		stream << rhs << " ";
	stream << std::endl;
	for (auto const & kvp : _matrix) {
		for (auto const & i : kvp.first) {
			stream << i << " ";
		}
		stream << kvp.second << std::endl;
	}
}

void SdpProblem::clear() {
	_b.clear();
	_matrix.clear();
	_blocks.clear();

}
//void SdpProblem::sdprelaxation(Problem const & problem) {
//	clear();
//	int const ini = newBlock(problem.nvars() + 1);
//	{
//		FunctionReal const	 f(problem.minimize());
//		if (f.constant() != 0) {
//			add(0, ini, 1, 1, -f.constant());
//		}
//		for (auto const & kvp : f.linear()) {
//			add(0, ini, 1, kvp.first + 1, -0.5*kvp.second);
//		}
//		for (auto const & kvp : f.quadratic()) {
//			add(0, ini, kvp.first.first + 1, kvp.first.second + 1, -kvp.second*(kvp.first.first == kvp.first.second ? 1.0 : 0.5));
//		}
//	}
//	{
//		int const idctr = newy(1);
//		add(idctr, ini, 1, 1, 1);
//
//	}
//	for (int ctr(0); ctr < problem.nctrs(); ++ctr) {
//		int const idctr = newy(problem.ctr(ctr).lb());
//		FunctionReal const	 f(problem.ctr(ctr).f());
//		if (f.constant() != 0) {
//			add(idctr, ini, 1, 1, f.constant());
//		}
//		for (auto const & kvp : f.linear()) {
//			add(idctr, ini, 1, kvp.first + 1, 0.5*kvp.second);
//		}
//		for (auto const & kvp : f.quadratic()) {
//			add(idctr, ini, kvp.first.first + 1, kvp.first.second + 1, kvp.second*(kvp.first.first == kvp.first.second ? 1.0 : 0.5));
//		}
//	}
//
//}

int SdpProblem::nvars() const {
	return  _blocks.back()._begin + _blocks.back()._size;
}

// void SdpProblem::addSparsityPattern(SparsityPattern & result)const {
	// result.clear();
	// result.assign(nvars(), IntSet());
	// for (auto const & coef : _matrix) {
		// int const ctr(coef.first[0]);
		// int const block(coef.first[1]);
		// int const bi(coef.first[2]);
		// int const bj(coef.first[3]);
		// int const i(_blocks[block - 1]._begin + bi - 1);
		// int const j(_blocks[block - 1]._begin + bj - 1);
		// result[i].insert(j);
		// result[j].insert(i);
	// }
// }
//void SdpProblem::sparsesdp(SdpProblem & output) {
//	output.clear();
//	SparsityPattern sparsityPattern;
//	addSparsityPattern(sparsityPattern);
//
//	SparseMatrix matrix;
//	build(sparsityPattern, matrix);
//
//	//std::cout << matrix << std::endl;
//	IntSetPtrSet cliqueDecomposition;
//	IntPairSet chordalExtension;
//	build(matrix, chordalExtension, cliqueDecomposition);
//
//	std::vector<IntSetPtr> cliqueVector;
//	cliqueVector.reserve(cliqueDecomposition.size());
//	// 1 block by clique
//	for (auto const & clique : cliqueDecomposition) {
//		cliqueVector.push_back(clique);
//	}
//	std::vector<Int2Int> iToClique(nvars());
//	for (auto const & clique : cliqueVector) {
//		int const b(output.newBlock(static_cast<int>(clique->size())));
//		int index(0);
//		for (auto const & i : *clique) {
//			iToClique[i][b] = index;
//			++index;
//		}
//	}
//
//	// same rhs
//	for (auto const b : _b)
//		output.newy(b);
//	std::vector<IntSet> iToCliqueSet(nvars());
//	for (int i(0); i < nvars(); ++i) {
//		for (auto const & clique : iToClique[i])
//			iToCliqueSet[i].insert(clique.first);
//	}
//	std::map<IntPair, IntVector> chordalToClique;
//	for (auto const & edge : chordalExtension) {
//		int const i(edge.first);
//		int const j(edge.second);
//		IntVector inter_result;
//		std::set_intersection(iToCliqueSet[i].begin(), iToCliqueSet[i].end(), iToCliqueSet[j].begin(), iToCliqueSet[j].end(), std::back_inserter(inter_result));
//		chordalToClique[{std::min(i, j), std::max(i, j)}] = inter_result;
//	}
//	// conversion of the matrix
//	for (auto const & kvp : _matrix) {
//		int const ctr(kvp.first[0]);
//		int const block(kvp.first[1]);
//		int const bi(kvp.first[2]);
//		int const bj(kvp.first[3]);
//		int const i(_blocks[block - 1]._begin + bi - 1);
//		int const j(_blocks[block - 1]._begin + bj - 1);
//
//		if (i != j) {
//			IntPair const edge({ i,j });
//			IntVector const & inter(chordalToClique.find(edge)->second);
//			if (!inter.empty()) {
//				int const r(chordalToClique.find(edge)->second.front());
//				int const newi(iToClique[i].find(r)->second);
//				int const newj(iToClique[j].find(r)->second);
//				output.add(ctr, r, std::min(newi, newj) + 1, std::max(newi, newj) + 1, kvp.second);
//			}
//		}
//		else {
//			int const r(iToClique[i].begin()->first);
//			int const newi(iToClique[i].find(r)->second);
//			output.add(ctr, r, newi + 1, newi + 1, kvp.second);
//		}
//	}
//	std::cout << "CHORDAL COUPLING" << std::endl;
//	for (auto const & frontier : chordalToClique) {
//		IntSet interClique;
//		if (frontier.second.size() > 1) {
//			int const i(frontier.first.first);
//			int const j(frontier.first.second);
//			int const r0(frontier.second.front());
//			int const i0(iToClique[i].find(r0)->second);
//			int const j0(iToClique[j].find(r0)->second);
//
//			for (auto const & r : frontier.second) {
//				if (r != r0) {
//					int const ctr(output.newy(0));
//					int const ir(iToClique[i].find(r)->second);
//					int const jr(iToClique[j].find(r)->second);
//					//std::cout << r << "(";
//					//std::cout << std::setw(6) << ir;
//					//std::cout << ", ";
//					//std::cout << std::setw(6) << jr;
//					//std::cout << ")  ";
//					output.add(ctr, r0, std::min(i0, j0) + 1, std::max(i0, j0) + 1, 1);
//					output.add(ctr, r, std::min(ir, jr) + 1, std::max(ir, jr) + 1, -1);
//				}
//				else {
//					//std::cout << r0 << "(";
//					//std::cout << std::setw(6) << i0;
//					//std::cout << ", ";
//					//std::cout << std::setw(6) << j0;
//					//std::cout << ") with ";
//				}
//			}
//			//std::cout << std::endl;
//		}
//	}
//	std::cout << "DIAGONAL COUPLING" << std::endl;
//	// diagonal term
//	for (int i(0); i < nvars(); ++i) {
//		if (iToClique[i].size() > 1) {
//			int const ci0(iToClique[i].begin()->first);
//			int const i0(iToClique[i].begin()->second);
//			for (auto const & clique : iToClique[i]) {
//				int const ci(clique.first);
//				if (ci != ci0) {
//					int const ctr(output.newy(0));
//					int const i(clique.second);
//					//std::cout << ci << "(";
//					//std::cout << std::setw(6) << i;
//					//std::cout << ", ";
//					//std::cout << std::setw(6) << i;
//					//std::cout << ")  ";
//					output.add(ctr, ci0, i0 + 1, i0 + 1, 1);
//					output.add(ctr, ci, i + 1, i + 1, -1);
//				}
//				else {
//					//std::cout << ci0 << "(";
//					//std::cout << std::setw(6) << i0;
//					//std::cout << ", ";
//					//std::cout << std::setw(6) << i0;
//					//std::cout << ") with ";
//				}
//			}
//			//std::cout << std::endl;
//		}
//	}
//
//	//size_t total_size(0);
//	//size_t max_size(0);
//	//for (auto const & clique : cliqueDecomposition) {
//	//	total_size += clique->size();
//	//	max_size = std::max(max_size, clique->size());
//	//}
//}

int numBefore(int n, int line) {
	return line*(2 * n - line + 1) / 2;
}
int SdpProblem::nz()const {
	int const last_id(std::abs(_blocks.back()._size));
	int const i(id((int)_blocks.size() - 1, last_id - 1, last_id - 1));
	return 1 + i;
}
int SdpProblem::dim()const {
	if (_blocks.empty())
		return 0;
	else
		return _blocks.back()._previous_dim + std::abs(_blocks.back()._size);

}

int SdpProblem::nblock()const {
	return (int)_blocks.size();
}
int SdpProblem::nctr()const {
	return (int)_b.size();
}
int SdpProblem::id(int i, int j, int k)const {
	int const begin(_blocks[i]._begin);
	int const size(_blocks[i]._size);
	if (size < 0) {
		// diagonal
		return  begin + k;
	}
	else {
		// dense
		return  begin + numBefore(size, j) + k - j;
	}
}
bool isZero(double value) {
	return std::fabs(value)< 1e-10;
}
std::string format(double value) {
		std::stringstream buffer;
		if (value > 0) {
			buffer << "+";
			if (value != 1)
				buffer << value;
		}
		else if (isZero(value + 1)) {
			buffer << "-";
		}
		else {
			buffer << value;
		}
		return buffer.str();
}
std::ostream & SdpProblem::print(std::ostream & stream, std::string const &name) const {
	int lastCtr(-1);
	for (auto const & coef : _matrix) {
		int const ctr(coef.first[0]);
		int const block(coef.first[1]);
		int const bi(coef.first[2]);
		int const bj(coef.first[3]);
		if (ctr != lastCtr) {
			lastCtr = ctr;
			stream << std::endl;
			if (ctr != 0)
				stream << "ctr[" << ctr << "] : " << _b[ctr - 1] << " = ";
			else
				stream << "maximize : ";
		}
		stream << format(coef.second) << " " << name << "[" << block << ", " << bi << ", " << bj << "]";
	}
	stream << std::endl;
	return stream;
}



void get_sdp_1(SdpProblem & sdp) {
	sdp.clear();
	int const b1 = sdp.newBlock(2);
	int const b2 = sdp.newBlock(3);
	int const b3 = sdp.newBlock(-2);

	int y1 = sdp.newy(1);
	int y2 = sdp.newy(2);

	sdp.add(0, b1, 1, 1, 2);
	sdp.add(0, b1, 1, 2, 1);
	sdp.add(0, b1, 2, 2, 2);

	sdp.add(0, b2, 1, 1, 3);
	sdp.add(0, b2, 1, 3, 1);
	sdp.add(0, b2, 2, 2, 2);
	sdp.add(0, b2, 3, 3, 3);


	sdp.add(y1, b1, 1, 1, 3);
	sdp.add(y1, b1, 1, 2, 1);
	sdp.add(y1, b1, 2, 2, 3);

	sdp.add(y1, b3, 1, 1, 1);

	sdp.add(y2, b2, 1, 1, 3);
	sdp.add(y2, b2, 1, 3, 1);
	sdp.add(y2, b2, 2, 2, 4);
	sdp.add(y2, b2, 3, 3, 5);

	sdp.add(y2, b3, 2, 2, 1);

}

void SdpProblem::dual(Matrix & result)const {
	result.clear();
	for (auto const & kvp : _matrix) {
		int const ctr(kvp.first[0]);
		int const block(kvp.first[1]);
		int const bi(kvp.first[2]);
		int const bj(kvp.first[3]);
		result[{block, bi, bj, ctr}] = kvp.second;
	}

	for (int i(0); i < nblock(); ++i) {
		SdpProblem::Block const & block(_blocks[i]);
		if (block._size > 0) {
			for (int j(0); j < block._size; ++j)
				for (int k(j); k < block._size; ++k) {
					result[{i + 1, j + 1, k + 1, 0}] += 0;
				}
		}
		else {
			for (int j(0); j < -block._size; ++j) {
				result[{i + 1, j + 1, j + 1, 0}] += 0;
			}
		}
	}
}

void SdpProblem::read(std::string const & file_name) {
	clear();

	std::ifstream file(file_name.c_str());
	std::string line;
	size_t n = 0;
	size_t n_block(0);
	size_t n_constraint(0);
	while (std::getline(file, line)) {
		++n;
		std::stringstream buffer(line);
		if (n == 1) {
			buffer >> n_constraint;
		}
		else if (n == 2) {
			buffer >> n_block;
		}
		else if (n == 3) {
			for (size_t i(0); i < n_block; ++i) {
				int s;
				buffer >> s;
				newBlock(s);
			}
		}
		else if (n == 4) {
			for (size_t i(0); i < n_constraint; ++i) {
				double s;
				buffer >> s;
				newy(s);
			}
		}
		else {
			int ctr;
			int block;
			int i;
			int j;
			double value;
			buffer >> ctr;
			buffer >> block;
			buffer >> i;
			buffer >> j;
			buffer >> value;
			add(ctr, block, i, j, value);
		}
	}
}
