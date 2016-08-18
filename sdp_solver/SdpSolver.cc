#include "SdpSolver.h"

#include "mosek.h"    /* Include the MOSEK definition file.  */


SdpSolver::SdpSolver(SdpProblem const & input) :_input(input) {

}

SdpSolver::~SdpSolver() {

}



static void MSKAPI printstr(void *handle, MSKCONST char str[])
{
	std::cout << str;;
}


void SdpSolver::launch_mosek() {
	int const NUMCON = (int)_input._b.size();
	int const NUMBARVAR = (int)_input._blocks.size();
	std::vector<int> DIMBARVAR(NUMBARVAR);
	std::vector<int> LENBARVAR(NUMBARVAR);
	for (size_t i(0); i < _input._blocks.size(); ++i) {
		auto const & block(_input._blocks[i]);
		DIMBARVAR[i] = std::abs(block._size);
		LENBARVAR[i] = block._size > 0 ? DIMBARVAR[i] * (DIMBARVAR[i] + 1) / 2 : DIMBARVAR[i];
	}

	//
	MSKrescodee  r;

	MSKenv_t     env = NULL;
	MSKtask_t    task = NULL;
	/* Create the mosek environment. */
	r = MSK_makeenv(&env, NULL);
	if (r == MSK_RES_OK)
	{
		/* Create the optimization task. */
		r = MSK_maketask(env, NUMCON, 0, &task);
		if (r == MSK_RES_OK)
		{
			MSK_linkfunctotaskstream(task, MSK_STREAM_LOG, NULL, printstr);

			/* Append 'NUMCON' empty constraints.
			The constraints will initially have no bounds. */
			if (r == MSK_RES_OK)
				r = MSK_appendcons(task, NUMCON);
			/* Append 'NUMBARVAR' semidefinite variables. */
			if (r == MSK_RES_OK) {
				r = MSK_appendbarvars(task, NUMBARVAR, DIMBARVAR.data());
			}
			// sorted by ctr then by block and then UPPER triangle 1-based
			{
				IntVector bara_i;
				IntVector bara_j;
				IntVector bara_k;
				IntVector bara_l;
				DoubleVector bara_v;

				IntVector barc_j;
				IntVector barc_k;
				IntVector barc_l;
				DoubleVector barc_v;
				for (auto const & kvp : _input._matrix) {
					int const ctr(kvp.first[0]);
					int const block(kvp.first[1]);
					int const xi(kvp.first[2]);
					int const xj(kvp.first[3]);
					if (ctr == 0) {
						barc_j.push_back(block - 1);
						barc_k.push_back(xj - 1);
						barc_l.push_back(xi - 1);
						barc_v.push_back(kvp.second);
					}
					else {
						bara_i.push_back(ctr - 1);
						bara_j.push_back(block - 1);
						bara_k.push_back(xj - 1);
						bara_l.push_back(xi - 1);
						bara_v.push_back(kvp.second);
					}
				}
				std::cout << "MSK_putbarcblocktriplet " << std::endl;
				if (r == MSK_RES_OK)
					r = MSK_putbarcblocktriplet(task, (int)barc_v.size(), barc_j.data(), barc_k.data(), barc_l.data(), barc_v.data());
				std::cout << "MSK_putbarablocktriplet " << std::endl;
				if (r == MSK_RES_OK)
					r = MSK_putbarablocktriplet(task, (int)bara_v.size(), bara_i.data(), bara_j.data(), bara_k.data(), bara_l.data(), bara_v.data());
			}
		}
		if (r == MSK_RES_OK)
			r = MSK_putobjsense(task, MSK_OBJECTIVE_SENSE_MAXIMIZE);
		{
			//MSK_putconbound
			std::vector<MSKboundkey_enum> fx(NUMCON, MSK_BK_FX);
			IntVector conidx(NUMCON);
			DoubleVector bnd(NUMCON);
			for (int i(0); i < NUMCON; ++i) {
				conidx[i] = i;
				bnd[i] = _input._b[i];
			}
			std::cout << "MSK_putconboundlist " << std::endl;
			r = MSK_putconboundlist(task, NUMCON, conidx.data(), fx.data(), bnd.data(), bnd.data());
		}

		if (r == MSK_RES_OK)
		{
			MSKrescodee trmcode;

			/* Run optimizer */
			r = MSK_optimizetrm(task, &trmcode);

			/* Print a summary containing information
			about the solution for debugging purposes*/
			MSK_solutionsummary(task, MSK_STREAM_MSG);

			DoubleVector barx;

			if (r == MSK_RES_OK)
			{
				MSKsolstae solsta;

				MSK_getsolsta(task, MSK_SOL_ITR, &solsta);

				switch (solsta)
				{
				case MSK_SOL_STA_OPTIMAL:
				case MSK_SOL_STA_NEAR_OPTIMAL:
					for (int i(0); i < NUMBARVAR; ++i) {						
						barx.assign(LENBARVAR[i], -1);
						MSK_getbarxj(task,
							MSK_SOL_ITR,    /* Request the interior solution. */
							i,
							barx.data());
						//for (int j(0); j < LENBARVAR[i]; ++j) {
						//	std::cout << std::setw(5) << i;
						//	std::cout << std::setw(5) << j;
						//	std::cout << std::setw(15) << barx[j] << std::endl;;
						//}
					}
					_y.assign(_input.nctr(), 0);
					MSK_gety(task,
						MSK_SOL_ITR,    /* Request the interior solution. */
						_y.data()
					);

					break;
				case MSK_SOL_STA_DUAL_INFEAS_CER:
				case MSK_SOL_STA_PRIM_INFEAS_CER:
				case MSK_SOL_STA_NEAR_DUAL_INFEAS_CER:
				case MSK_SOL_STA_NEAR_PRIM_INFEAS_CER:
					printf("Primal or dual infeasibility certificate found.\n");
					break;

				case MSK_SOL_STA_UNKNOWN:
					printf("The status of the solution could not be determined.\n");
					break;
				default:
					printf("Other solution status.");
					break;
				}
			}
			else
			{
				printf("Error while optimizing.\n");
			}
		}

		if (r != MSK_RES_OK)
		{
			/* In case of an error print error code and description. */
			char symname[MSK_MAX_STR_LEN];
			char desc[MSK_MAX_STR_LEN];

			printf("An error occurred while optimizing.\n");
			MSK_getcodedesc(r,
				symname,
				desc);
			printf("Error %s - '%s'\n", symname, desc);
		}
		/* Delete the task and the associated data. */
		MSK_deletetask(&task);
	}
	/* Delete the environment and the associated data. */
	MSK_deleteenv(&env);
}



