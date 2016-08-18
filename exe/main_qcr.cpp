#include "Timer.h"
#include "common.h"
#include <cplex.h>
#include <xprs.h>
#include "mip_solver\LpBuffer.h"
#include "mip_solver\XpressSolver.h"

#include "sdp_solver\SdpProblem.h"
#include "sdp_solver\SdpSolver.h"

#include "bipartite_data\RegisteredModularityBInstance.h"
#include "bipartite_data\BinaryDecompositionOracle.h"

std::string RegisteredModularityBInstance::InstancesPath = "../txt/";

void XPRS_CC my_cb_messsage(XPRSprob prob, void* data, const char *sMsg, int nLen,
	int nMsglvl) {
	std::ostream* stream = (std::ostream*)data;
	if (!data)
		throw std::invalid_argument("optimizermsg data is not XpressSolver");
	switch (nMsglvl) {

		/* Print Optimizer error messages and warnings */
	case 4: /* error */
	case 3: /* warning */
	case 2: /* dialogue */
	case 1: /* information */
		if(stream)
			*stream << sMsg << std::endl;
		break;
		/* Exit and flush buffers */
	default:
		fflush(NULL);
		break;
	}
}



class Qcr {
public:
	Qcr() {
		_max_id = 0;
	}
public:
	void read_linear(std::string const & file_name) {
		std::ifstream file(file_name.c_str());
		std::string line;
		while (std::getline(file, line)) {
			int i;
			double value;
			std::stringstream buffer(line);
			buffer >> i;
			buffer >> value;
			_q[{ i, i}] = value;
			_max_id = std::max(_max_id, i);
		}
		file.close();
	}
	void read_quadratic(std::string const &file_name) {
		std::ifstream file(file_name.c_str());
		std::string line;
		while (std::getline(file, line)) {
			int i;
			int j;
			double value;
			std::stringstream buffer(line);
			buffer >> i;
			buffer >> j;
			buffer >> value;
			_q[{ i, j }] = value;
			_max_id = std::max(_max_id, std::max(i, j));
		}
		file.close();
	}

	int _max_id;
	IntPair2Double _q;
};

int main(int argc, char *argv[])
{


	XpressSolver xpress;
	xpress.stream().push_back(&std::cout);
	//

	AvailableModularityBInstances id(static_cast<AvailableModularityBInstances>(atoi(argv[1]) - 1));
	RegisteredModularityBInstance instance(id);
	instance.out();
	// oracles creation


	//

	Qcr qcr;
	qcr.read_linear("oracle.txt");
	qcr.read_quadratic("q.txt");

	int const n(qcr._max_id + 1);
	std::cout << "n = " << n<< std::endl;

	SdpProblem sdp;
	int const block = sdp.newBlock(n+1);

	// attention index de base 1 et décallé à cause du 1 en haut à gauche
	for (auto const & kvp : qcr._q) {
		int const i(kvp.first.first + 2);
		int const j(kvp.first.second + 2);
		if (i != j) {
			sdp.add(0, block, i, j, -kvp.second);
		}
		else {
			sdp.add(0, block, 1, i, -kvp.second*0.5);
		}
	}
	int ctr;
	// Y[1,1] = 1
	ctr = sdp.newy(1);
	sdp.add(ctr, block, 1, 1, 1);
	for (int i(0); i < n; ++i) {
		ctr = sdp.newy(0);
		sdp.add(ctr, block, i+2, i+2, 1);
		sdp.add(ctr, block, 1, i+2, -0.5);
	}
	sdp.print("toto.dat");
	SdpSolver solver(sdp);
	solver.launch_mosek();

	DoubleVector const & y(solver._y);
	

	BinaryDecompositionOracle bMilpOracle(&instance);
	RowBuffer rowBuffer;
	ColumnBuffer columnBuffer(xpress.continuous());
	bMilpOracle.fill(rowBuffer, columnBuffer);
	xpress.initLp("my_qp");
	xpress.add(columnBuffer);
	xpress.add(rowBuffer);
	
	XPRSprob prob = xpress._lp;
	XPRSchgobjsense(prob, XPRS_OBJ_MAXIMIZE);

	IntVector indexes(xpress.ncols());
	for (int i(0); i < xpress.ncols(); ++i)
		indexes[i] = i;

	DoubleVector obj(xpress.ncols(), 0);
	DoubleVector qobj(xpress.ncols(), 0);
	XPRSchgobj(prob, xpress.ncols(), indexes.data(), obj.data());
	for (auto const & kvp : qcr._q) {
		int const i(kvp.first.first);
		if (i != kvp.first.second) {
			int const j(kvp.first.second);
			XPRSchgqobj(prob, i, j,  kvp.second);
		}
		else {
			obj[i] += kvp.second;
			qobj[i] = -2 * y[i];
		}
	}
	//XPRSwriteprob(prob, "qp.lp", "lp");
	//XPRSmipoptimize(prob, "");
	
	for (int i(0); i < n; ++i) {
		XPRSchgqobj(prob, i, i, qobj[i]);
		obj[i] += y[i];
	}
	XPRSchgobj(prob, n, indexes.data(), obj.data());
	XPRSsetdblcontrol(prob, XPRS_MIPRELSTOP, 0);
	XPRSsetdblcontrol(prob, XPRS_MIPABSSTOP, 0);
	XPRSwriteprob(prob, "qcr.lp", "lp");
	std::cout << "#################################" << std::endl << "solving QCR" << std::endl << "#################################" << std::endl;
	XPRSmipoptimize(prob, "");
	//XPRSreadprob(prob, "oracle.lp", "lp");
	//XPRSmipoptimize(prob, "");




} /* main */
