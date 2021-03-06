#include <cstdlib>
#include <vector>
#include <mex.h>
#include "argcheck.h"
#include "spx_operator.hpp"
#include "spx_matarr.hpp"

const char* func_name = "mex_sparse_demo";


#define A_IN prhs[0]
#define S_OUT plhs[0]

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray*prhs[])
{
    check_num_input_args(nrhs, 1, 1);
    try {

        spx::MxSparseMat mat(A_IN);
        mexPrintf("NNZ  : %d\n", mat.nnz());
        mexPrintf("rows: %d\n", mat.rows());
        mexPrintf("columns: %d\n", mat.columns());
        spx::d_vector aa(mat.rows());
        spx::Vec a(aa);
        for (int i=0; i < mat.columns(); ++i){
            mexPrintf("Nz entries in column %d: %d\n", i, mat.nnz_col(i));
            mat.column(i, a.head());
            a.print("A_i");
        }
        spx::d_vector xx(mat.columns());
        spx::Vec x(xx);
        x = 1;
        spx::d_vector yy(mat.rows());
        spx::Vec y(yy);
        mexPrintf("Performing y = A x\n");
        mat.mult_vec(x, y);
        x.print("x");
        y.print("y");
        y = 1;
        mexPrintf("Performing y = A' x\n");
        mat.mult_t_vec(y, x);
        y.print("x");
        x.print("y");

        mwIndex indices[] = {0, 1, 1, 0};
        spx::Matrix cols(mat.rows(), 4);
        mat.extract_columns(indices, 4, cols.head());
        cols.print_matrix("A[: , [0, 1, 1, 0]]");
        {
            // Copy the whole matrix
            spx::Matrix A2(mat.rows(), mat.columns());
            mat.copy_matrix_to(A2);
            A2.print_matrix("A2");        
        }
        {
            mexPrintf("Testing x += alpha * A(:, c)\n");
            y = 4;
            mat.add_column_to_vec(2, 1, y.head());
            y.print("y");
        }
        {
            mexPrintf("Testing y = A(:, ind) x\n");
            mwIndex ind[2] = {0, 2};
            spx::d_vector xx(2);
            spx::Vec x(xx);
            x = 2;
            spx::d_vector yy(mat.rows());
            spx::Vec y(yy);
            x.print("x");
            y.print("y");
            mat.mult_vec(ind, 2, x.head(), y.head());
            y.print("y");
        }
        {
            mexPrintf("Testing extract_rows\n");
            spx::Matrix A3(2, mat.columns());
            mwIndex ind[2] = {0, 2};
            mat.extract_rows(ind, 2, A3.head());
            A3.print_matrix("A3");
        }
        {
            std::vector<std::string> fields;
            fields.push_back("a");
            fields.push_back("b");
            fields.push_back("c");
            fields.push_back("d");
            fields.push_back("e");
            S_OUT = spx::create_struct(fields);
            spx::set_struct_int_field(S_OUT, 0, 10);
            spx::set_struct_bool_field(S_OUT, 1, true);
            spx::set_struct_double_field(S_OUT, 2, 5.0);
            spx::set_struct_string_field(S_OUT, 3, "ABC");
            spx::d_vector vd{10, 20, 30};
            spx::set_struct_d_vec_field(S_OUT, 4, vd);
        }
    } catch (std::exception& e) {
        mexErrMsgTxt(e.what());
        return;
    }
}

