/*
 * IPartition.hpp
 *
 *  Created on: 4 août 2012
 *      Author: manuel
 */

#ifndef I_PARTITION_HPP_
#define I_PARTITION_HPP_
/*
 * IPartition is aimed to be used in general clustering
 */

#include "../src/IndexedList.hpp"

class IPartition {
public:
	typedef IntList::iterator NodePosition;
	typedef std::vector<NodePosition> NodePositions;
	typedef std::vector<Int2Double> LabelGraph;
	typedef std::vector<IntList> LabelLists;
	typedef IndexedList::iterator iterator;
	typedef IndexedList::const_iterator const_iterator;
public:
	//
	virtual void set(IntVector const &) = 0;
	// the number of nodes
	virtual size_t nbObs() const = 0;
	// the number of used labels
	virtual size_t nbLabels() const = 0;
	virtual size_t maxNbLabels() const=0;
	//
	virtual void setWeights(DoubleVector const & rhs) = 0;
	virtual Double obsWeight(size_t) const=0;
	virtual Double & obsWeight(size_t)=0;
	virtual Double labelWeight(size_t) const=0;
	virtual Double & labelWeight(size_t)=0;
	// clustering information
	virtual size_t & label(size_t node) = 0;
	virtual size_t label(size_t node) const = 0;
	// the number of nodes having a label
	virtual size_t & sizeOfLabel(size_t label) = 0;
	virtual size_t sizeOfLabel(size_t label) const = 0;
	// one un-used label
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
	virtual bool isUsed(size_t label) const = 0;
	virtual IndexedList const & usedLabel() const =0;
	virtual IndexedList const & unUsedLabel() const =0;

	virtual IntList & list(size_t label) = 0;
	virtual IntList const & list(size_t label) const = 0;

	// change the label of one node
	virtual bool shift(size_t node, size_t newLabel) = 0;
	// fusion of two label, return the used label
	virtual size_t fusion(size_t const & label1, size_t const & label2) = 0;
	virtual ~IPartition();
};

inline IPartition::~IPartition() {
}
#endif /* IPARTITION_HPP_ */
