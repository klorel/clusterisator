/*
 * IPartition.hpp
 *
 *  Created on: 4 août 2012
 *      Author: manuel
 */

#ifndef I_EXTENDED_PARTITION_HPP_
#define I_EXTENDED_PARTITION_HPP_

#include "common.h"
#include "IPartition.hpp"
#include "IndexedList.hpp"
class IGraph;
class IExtendedPartition: public IPartition {
public:

	typedef IntList::iterator NodePosition;
	typedef std::vector<NodePosition> NodePositions;
	typedef std::vector<Int2Double> LabelGraph;
	typedef std::vector<IntList> LabelLists;
	typedef IndexedList::iterator iterator;
	typedef IndexedList::const_iterator const_iterator;
public:
	// the number of nodes
	virtual size_t nbNodes() const = 0;
	// the number of edges
	virtual size_t nbEdges() const = 0;
	// the number of used labels
	virtual size_t nbLabels() const = 0;
	// the number of nodes having a label
	virtual size_t & sizeOfLabel(size_t const & label) = 0;
	virtual size_t const & sizeOfLabel(size_t const & label) const = 0;
	// the degree of a label
	virtual double const & degreeOfLabel(size_t const & label) const = 0;
	virtual double & degreeOfLabel(size_t const & label)= 0;
	// one un un used label
	virtual size_t getUnUsedLabel() const = 0;
	// is there any un used label
	virtual bool allLabelsUsed() const = 0;
	// unused label list
	virtual IndexedList & unUsed()= 0;
	virtual IndexedList const & unUsed() const = 0;
	// used label list
	virtual IndexedList & used()= 0;
	virtual IndexedList const & used() const = 0;
	// is the label used
	virtual bool isUsed(size_t const & label) const = 0;
	virtual IndexedList const & usedLabel() const =0;
	virtual IndexedList const & unUsedLabel() const =0;

	virtual IntList & list(size_t const & label) = 0;
	virtual IntList const & list(size_t const & label) const = 0;

	virtual void startWith(IPartition const &) = 0;
public:
	// change the label of one node
	virtual void shift(size_t const & node, size_t const & newLabel) = 0;
	// fusion of two label, return the used label
	virtual size_t fusion(size_t const & label1, size_t const & label2) = 0;
public:
	virtual void intra(size_t const & node, DoubleVector & values, bool const & reset) = 0;
	virtual double intra(size_t const & label) = 0;
	virtual double intra(size_t const & node, size_t const & label) = 0;
public:
	IExtendedPartition(IGraph const & iGraph);
	virtual ~IExtendedPartition();

	IGraph const & graph() const;
private:
	IGraph const & _iGraph;
};

inline IExtendedPartition::IExtendedPartition(IGraph const & iGraph) :
				_iGraph(iGraph) {

}
inline IExtendedPartition::~IExtendedPartition() {
}

inline IGraph const & IExtendedPartition::graph() const {
	return _iGraph;
}
#endif /* I_EXTENDED_PARTITION_HPP_ */
