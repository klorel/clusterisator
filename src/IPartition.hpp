/*
 * IPartition.hpp
 *
 *  Created on: 4 août 2012
 *      Author: manuel
 */

#ifndef I_PARTITION_HPP_
#define I_PARTITION_HPP_
/**
 * IPartition is aimed to be used in general clustering
 *
 * It represents a dynamic partition of the observations
 */

#include "src/IndexedList.hpp"

class IPartition {
public:
	typedef IntList::iterator NodePosition;
	typedef std::vector<NodePosition> NodePositions;
	typedef std::vector<Int2Double> LabelGraph;
	typedef std::vector<IntList> LabelLists;
	typedef IndexedList::iterator iterator;
	typedef IndexedList::const_iterator const_iterator;
public:

  /**
   * Define the partition
   *
   * @param labels For every observation, its label. Size must equal the number of observations
   */
	virtual void setLabels(IntVector const & labels) = 0;

  /**
   * Define the weight of every observation.
   *
   * @param weights For every observation, its weights. Size mumst equal the number of observation
   */
	virtual void setWeights(DoubleVector const & weights) = 0;

  /// Get the number of observations present in the partition
	virtual size_t nbObs() const = 0;
	/// Get the number of used labels
	virtual size_t nbLabels() const = 0;
  /// Get the maximum number of labels allowed.
	virtual size_t maxNbLabels() const=0;
  /// Get the weight of a given observation
	virtual Double obsWeight(size_t obs) const=0;
  /// Get the sum of the weights of the observations contained in a given label
	virtual Double labelWeight(size_t label) const=0;
	/// Get the label in which belong a given observation
	virtual size_t label(size_t obs) const = 0;
	/// Get the number of observation contained in the given label
	virtual size_t sizeOfLabel(size_t label) const = 0;

  /**
   * Returns an arbitrary unused label, assuming one exist.
   * If every label if used, the behavior is undefined
   */
	virtual size_t getUnUsedLabel() const = 0;
	/// @return TRUE is every label is used, FALSE if at least one is empty
	virtual bool allLabelsUsed() const = 0;
	/// @return The list of every unused labels 
	virtual IndexedList const & unUsed() const = 0;
	/// @return TRUE is at least one observation has the given label 
	virtual bool isUsed(size_t label) const = 0;
	/// @return The list of every used label
	virtual IndexedList const & usedLabels() const =0;
  /// @return The list of every unused label
	virtual IndexedList const & unUsedLabels() const =0;

  /// @returns The observations contained by the given label
	virtual IntList const & observations(size_t label) const = 0;

	/// Change the label of one observation
	virtual bool shift(size_t observation, size_t to) = 0;

	/**
   * Merge one label into another
   *
   * @return The label which contains every observation (the other one is then empty)
   */
	virtual size_t fusion(size_t const & label1, size_t const & label2) = 0;

	virtual ~IPartition();
};

inline IPartition::~IPartition() {
}
#endif /* IPARTITION_HPP_ */
