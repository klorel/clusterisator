#ifndef LP_BUFFER_HPP
#define LP_BUFFER_HPP

#include "gencol.h"

typedef struct cpxlp* CPXLPptr;
typedef struct cpxenv* CPXENVptr;

class RowBuffer {
 public:

 public:
  RowBuffer();
  virtual ~RowBuffer();
 public:
  int size() const;
  int nz() const;
  virtual void add(Double rhsObj, char type, std::string const & name);
  virtual void add(int, Double);
  virtual void add(CPXENVptr, CPXLPptr);
  virtual void reserve(int, int, int);

//	virtual void print(std::ostream &) const;
  int const * index() const;
  int const * begin() const;
  double const * value() const;
  char const * type() const;

  double const * rhsObj() const;

  Double & rhsObj(int);
  virtual void clear();

  void get_names(std::vector<char> & result) const;

  StringVector & name();
  StringVector const & name() const;

  void add_last_begin();
  void rem_last_begin();
 protected:
  IntVector _index;
  IntVector _begin;
  std::vector<double> _value;

  // column : type of variable
  // row : type of row
  std::vector<char> _type;
  // column : obj
  // row : rhs
  std::vector<double> _objRhs;

  StringVector _name;
};

class ColumnBuffer : public RowBuffer {
 public:
  ColumnBuffer(char continuous);
  virtual ~ColumnBuffer();
 public:
  virtual void clear();
  virtual void reserve(int, int, int);
//	virtual void print(std::ostream &) const;
  virtual void add(CPXENVptr, CPXLPptr);
  virtual void add(Double rhsObj, char type, Double lower, Double upper,
                   std::string const & name);
  using RowBuffer::add;

  double const * lower() const;
  double const * upper() const;

  bool only_continous() const;
 private:
  std::vector<double> _lower;
  std::vector<double> _upper;
  bool _only_continous;
  char _continuous;

};

#endif 
