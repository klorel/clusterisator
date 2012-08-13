#include "Solution.hpp"
#include "../interfaces/ILinks.hpp"

Solution::Solution(Problem const & problem, IPartition & partition) :
		ISolution(problem, partition) {
	computeSumOfLabels();
}

Solution::~Solution() {

}

void Solution::shift(size_t n, size_t l) {

	if (partition().label(n) != l) {
		sumOfLabel(partition().label(n)) -= links().sumOfNodeLinks(n);
		sumOfLabel(l) += links().sumOfNodeLinks(n);
	}
	partition().shift(n, l);

}

size_t Solution::fusion(size_t l1, size_t l2) {
	return -1;
}
void Solution::computeSumOfLabels() {
	for (size_t l(0); l < partition().nbObs(); ++l)
		sumOfLabel(l) += computeSumOfLabel(l);
}

Double Solution::computeSumOfLabel(size_t l) const {
	Double result(0);
	for (auto const & n : partition().list(l))
		result += links().sumOfNodeLinks(n);
	return result;
}
