#include "gurobi_c++.h"
#include "stdio.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

int
main(int   argc,
    char* argv[])
{
    ofstream out_to_file;

#if 1
    if (argc < 2)
    {
        cout << "Usage: lpmethod_c++ filename" << endl;
        return 1;
    }
    GRBVar* fvars = 0;
    try {
        // Read model
        GRBEnv env = GRBEnv();
        GRBModel m = GRBModel(env, argv[1]);

        // Solve the model with different values of Method
        int    bestMethod = -1;
        double bestTime = m.get(GRB_DoubleParam_TimeLimit);
        for (int i = 0; i <= 2; ++i) {
            m.reset();
            m.set(GRB_IntParam_Method, i);
            m.optimize();
            if (m.get(GRB_IntAttr_Status) == GRB_OPTIMAL) {
                bestTime = m.get(GRB_DoubleAttr_Runtime);
                bestMethod = i;
                // Reduce the TimeLimit parameter to save time
                // with other methods
                m.set(GRB_DoubleParam_TimeLimit, bestTime);
            }
        }

        // Report which method was fastest
        if (bestMethod == -1) {
            cout << "Unable to solve this model" << endl;
        }
        else {
            cout << "Solved in " << bestTime
                << " seconds with Method: " << bestMethod << endl;
        }

        int numvars = m.get(GRB_IntAttr_NumVars);

        /* Print values of nonzero variables */
        //string last_name_of_file = "ineq_choose.txt";
        //string sulution_filename = int_add_str(ROUND, last_name_of_file);


        //以覆盖方式打开文件，如果不存在即创建
        //out_to_file.open(sulution_filename, ios::trunc);
        fvars = m.getVars();
        for (int j = 0; j < numvars; j++) {
            GRBVar v = fvars[j];
            if (v.get(GRB_DoubleAttr_X) != 0.0) {
                //out_to_file << v.get(GRB_StringAttr_VarName) << " "
                    //<< v.get(GRB_DoubleAttr_X) << endl;
                cout << v.get(GRB_StringAttr_VarName) << " "
                    << v.get(GRB_DoubleAttr_X) << endl;
            }
        }
        //cout << "The value of variables have been written to " << sulution_filename << endl;
        //out_to_file.close();
    }
    catch (GRBException e) {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    }
    catch (...) {
        cout << "Exception during optimization" << endl;
    }

    delete[] fvars;

#endif
    return 0;
}