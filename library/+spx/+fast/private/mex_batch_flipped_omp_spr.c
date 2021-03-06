

#include <mex.h>
#include "argcheck.h"
#include "spxblas.h"
#include "omp.h"


#define Y_IN prhs[0]
#define K_IN prhs[1]
#define EPS_IN prhs[2]
#define SPARSE_IN prhs[3]

#define C_OUT plhs[0]

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray*prhs[])
{
    // Initialize input arguments
    double *m_dataset = 0;
    mwSize M = 0;
    mwSize S = 0;
    mwSize K = 0;
    double res_norm_bnd = 0;
    int sparse_output = 0;
    // Verify number of arguments
    check_num_input_args(nrhs, 2, 4);
    check_num_output_args(nlhs, 0,1);

    check_is_double_matrix(Y_IN, "mex_batch_omp_spr", "D");
    check_is_double_scalar(K_IN,  "mex_batch_omp_spr", "K");
    // Read the value of K
    K = (int)(mxGetScalar(K_IN) + 1e-2);
    if (nrhs > 2){
        check_is_double_scalar(EPS_IN,  "mex_batch_omp_spr", "eps");
        res_norm_bnd  = mxGetScalar(EPS_IN);
    }
    if (nrhs > 3){
        check_is_double_scalar(SPARSE_IN, "mex_batch_omp_spr","sparse");
        sparse_output = (int) mxGetScalar(SPARSE_IN);
    }
    // Check an argument for empty matrix before taking it.
    if (!mxIsEmpty(Y_IN)){
        m_dataset = mxGetPr(Y_IN);
    }else{
        mexErrMsgTxt("Data set is empty.");
    }
    if (m_dataset){
        // Number of signal space dimension
        M = mxGetM(Y_IN);
        // Number of Signals
        S = mxGetN(Y_IN);
    }
    if(S < 1){
        mexErrMsgTxt("No signals for sparse coding.");
    }
    // Create Sparse Representation matrix
    C_OUT = batch_flipped_omp_spr(m_dataset, M, S, K, res_norm_bnd, sparse_output);
}

