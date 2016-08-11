/*
 * XpressSolver.cpp
 *
 *  Created on: 8 août 2016
 *      Author: manuruiz
 */

#include "XpressSolver.h"
#include "LpBuffer.h"

#include <stdexcept>
#include <cassert>
#include <exception>

bool XpressSolver::WasInit = false;

XpressSolver::XpressSolver() {
  _lp = NULL;
}

XpressSolver::~XpressSolver() {
  freeLp();
}

#ifdef  __LAZY_XPRESS__
void XpressSolver::errormsg(const char *sSubName, int nLineNo, int nErrCode) {
}
#else

#include <xprs.h>

void XPRS_CC optimizermsg(XPRSprob prob, void* data, const char *sMsg, int nLen,
                          int nMsglvl);

char XpressSolver::binary() const {
  return 'B';
}
char XpressSolver::continuous() const {
  return 'C';
}

char XpressSolver::leq() const {
  return 'L';
}
char XpressSolver::eq() const {
  return 'E';
}
char XpressSolver::geq() const {
  return 'G';
}

double XpressSolver::infinity() const {
  return XPRS_PLUSINFINITY;
}

void XpressSolver::run() {
  XPRSchgobjsense(_lp, _is_minimize ? XPRS_OBJ_MINIMIZE : XPRS_OBJ_MAXIMIZE);
  if (_is_mip)
    XPRSmipoptimize(_lp, "");
  else
    XPRSlpoptimize(_lp, "");
}
void XpressSolver::add(RowBuffer & buffer) {
  int nReturn(-1);

  buffer.add_last_begin();
  nReturn = XPRSaddrows(_lp, buffer.size(), buffer.nz(), buffer.type(),
                        buffer.rhsObj(), NULL, buffer.begin(), buffer.index(),
                        buffer.value());
  if (nReturn)
    errormsg("XPRSaddrows", __LINE__, nReturn);
  if (!buffer.name().empty()) {
    std::vector<char> all_names;
    buffer.get_names(all_names);
    XPRSaddnames(_lp, 1, all_names.data(), 0, buffer.size() - 1);
    if (nReturn)
      errormsg("XPRSaddnames", __LINE__, nReturn);
  }
  buffer.rem_last_begin();
}
void XpressSolver::add(ColumnBuffer & buffer) {
  int nReturn(-1);
  buffer.add_last_begin();
  nReturn = XPRSaddcols(_lp, buffer.size(), buffer.nz(), buffer.rhsObj(),
                        buffer.begin(), buffer.index(), buffer.value(),
                        buffer.lower(), buffer.upper());
  if (nReturn)
    errormsg("XPRSaddcols", __LINE__, nReturn);
  if (!buffer.name().empty()) {
    std::vector<char> all_names;
    buffer.get_names(all_names);
    nReturn = XPRSaddnames(_lp, 2, all_names.data(), 0, buffer.size() - 1);
    if (nReturn)
      errormsg("XPRSaddnames", __LINE__, nReturn);
  }
  if (!buffer.only_continous()) {
    _is_mip = true;
    IntVector sequence(buffer.size());
    int ncols;
    nReturn = XPRSgetintattrib(_lp, XPRS_COLS, &ncols);
    if (nReturn)
      errormsg("XPRSgetintattrib", __LINE__, nReturn);
    for (int i(0); i < buffer.size(); ++i) {
      sequence[i] = ncols - buffer.size() + i;
    }
    nReturn = XPRSchgcoltype(_lp, buffer.size(), sequence.data(),
                             buffer.type());
    if (nReturn)
      errormsg("XPRSchgcoltype", __LINE__, nReturn);
  }
  buffer.rem_last_begin();
}

void XpressSolver::write(std::string const & fileName) const {
  XPRSwriteprob(_lp, fileName.c_str(), "lp");
}

void XpressSolver::initLp(std::string const & name) {
  int nReturn = -1;
  if (!WasInit) {
    nReturn = XPRSinit("");
    WasInit = true;
  }

  if (_lp)
    freeLp();
  nReturn = XPRScreateprob(&_lp);
  if (nReturn)
    errormsg("XPRScreateprob", __LINE__, nReturn);

  if (_stream.empty()) {
    XPRSsetintcontrol(_lp, XPRS_OUTPUTLOG, XPRS_OUTPUTLOG_NO_OUTPUT);
    if (nReturn)
      errormsg("XPRScreateprob", __LINE__, nReturn);
  } else {
    nReturn = XPRSsetintcontrol(_lp, XPRS_OUTPUTLOG,
    XPRS_OUTPUTLOG_FULL_OUTPUT);
    if (nReturn)
      errormsg("XPRSsetintcontrol", __LINE__, nReturn);
    nReturn = XPRSsetcbmessage(_lp, optimizermsg, this);
    if (nReturn)
      errormsg("XPRSsetcbmessage", __LINE__, nReturn);
  }

  nReturn = XPRSloadlp(_lp, name.c_str(), 0, 0, NULL, NULL, NULL, NULL, NULL,
  NULL,
                       NULL, NULL, NULL, NULL);
  if (nReturn)
    errormsg("XPRSloadlp", __LINE__, nReturn);

  XPRSsetintcontrol(_lp, XPRS_THREADS, 1);
}
void XpressSolver::freeLp() {
  if (_lp)
    XPRSdestroyprob(_lp);
  _lp = NULL;
}

void XpressSolver::chgObj(IntVector const & indexe,
                          DoubleVector const & values) {
  XPRSchgobj(_lp, (int) indexe.size(), indexe.data(), values.data());
}

void XpressSolver::delMipStarts() {
}
bool XpressSolver::isOptimal() const {

  int status = -1;
  if (_is_mip)
    XPRSgetintattrib(_lp, XPRS_MIPSTATUS, &status);
  else
    XPRSgetintattrib(_lp, XPRS_LPSTATUS, &status);

  return status == XPRS_MIP_OPTIMAL || status == XPRS_LP_OPTIMAL;
}
double XpressSolver::objValue() const {
  double result;
  if (_is_mip)
    XPRSgetdblattrib(_lp, XPRS_MIPOBJVAL, &result);
  else
    XPRSgetdblattrib(_lp, XPRS_LPOBJVAL, &result);
  return result;
}
void XpressSolver::objValue(int i, Double & obj) const {
  obj = objValue();
}
void XpressSolver::solution(int i, DoubleVector & result) const {
  result.resize(ncols());
  XPRSgetmipsol(_lp, result.data(), NULL);
}
int XpressSolver::ncols() const {
  int result;
  XPRSgetintattrib(_lp, XPRS_COLS, &result);
  return result;

}
int XpressSolver::nrows() const {
  int result;
  XPRSgetintattrib(_lp, XPRS_ROWS, &result);
  return result;
}
/************************************************************************************\
* Name:         optimizermsg                                                              *
 * Purpose:      Display Optimizer error messages and warnings.                            *
 * Arguments:    const char *sMsg    Message string                                   *
 *               int nLen            Message length                                   *
 *               int nMsgLvl         Message type                                     *
 * Return Value: None.                                                                *
 \************************************************************************************/

void XPRS_CC optimizermsg(XPRSprob prob, void* data, const char *sMsg, int nLen,
                          int nMsglvl) {
  XpressSolver * ptr = (XpressSolver*) data;
  if (!data)
    throw std::invalid_argument("optimizermsg data is not XpressSolver");
  switch (nMsglvl) {

    /* Print Optimizer error messages and warnings */
    case 4: /* error */
    case 3: /* warning */
    case 2: /* dialogue */
    case 1: /* information */
      for (auto const & stream : ptr->stream())
        *stream << sMsg << std::endl;
      break;
      /* Exit and flush buffers */
    default:
      fflush(NULL);
      break;
  }
}

/************************************************************************************\
* Name:         errormsg                                                             *
 * Purpose:      Display error information about failed subroutines.                  *
 * Arguments:    const char *sSubName    Subroutine name                              *
 *               int nLineNo             Line number                                  *
 *               int nErrCode            Error code                                   *
 * Return Value: None                                                                 *
 \************************************************************************************/

void XpressSolver::errormsg(const char *sSubName, int nLineNo, int nErrCode) {
  int nErrNo; /* Optimizer error number */
  /* Print error message */
  printf("The subroutine %s has failed on line %d\n", sSubName, nLineNo);

  /* Append the error code if it exists */
  if (nErrCode != -1)
    printf("with error code %d.\n\n", nErrCode);

  /* Append Optimizer error number, if available */
  if (nErrCode == 32) {
    XPRSgetintattrib(_lp, XPRS_ERRORCODE, &nErrNo);
    printf("The Optimizer eror number is: %d\n\n", nErrNo);
  }

  /* Free memory close files and exit */
  XPRSdestroyprob(_lp);
  XPRSfree();
  exit(nErrCode);
}

void XpressSolver::setLog() {
	XPRSsetintcontrol(_lp, XPRS_OUTPUTLOG, XPRS_OUTPUTLOG_FULL_OUTPUT);
}
void XpressSolver::setNoLog() {
	XPRSsetintcontrol(_lp, XPRS_OUTPUTLOG, XPRS_OUTPUTLOG_NO_OUTPUT);
}
void XpressSolver::setNbThreads(int n) {
	XPRSsetintcontrol(_lp, XPRS_THREADS, n);
}


#endif
