#include "libfrcima.hpp"

namespace frcima
{
    using arma::mat;
    using arma::vec;
    using std::vector;

    double _calculated_error;
    // using namespace arma;

    /**
     * @brief calculates the error from the calculated filter
     * 
     * @param A Calculated filter we want to get the error from
     * @param C Training matrix with vectorized noisy images
     * @param X Training matrix with vectorized source images
     */
    void calculate_error(mat &A, const mat &C, const mat &X);

    /**
     * @brief calculates low rank matrix approximation using the modified bilateral random projections (MBRP)
     *         method.
     * 
     * @param A input matrix to calculate low rank approximation
     * @param r desired rank, must be less than the minimun between the number of columns and rows of A
     * @param B output left bilateral matrix
     * @param C output right bilateral matrix
     * @return true if low rank approximation is calculated
     * @return false if there is a problem during calculation
     */
    bool low_rank_approx(const mat &A, const size_t r, mat &B, mat &C);

    /**
     * @brief calculates low rank matrix using SVD method
     * 
     * @param A input matrix to calculate low rank 
     * @param r desired rank, must be less than the minimun between the number of columns and rows of A
     * @param B output left bilateral matrix
     * @param C output right bilateral matrix
     * @return true if low rank approximation is calculated
     * @return false if there is a problem during calculation
     */
    bool low_rank(const mat &A, const size_t r, mat &B, mat &C);

    /**
     * @brief generates training matrices from vectors 
     * 
     * @param X input training vector
     * @param C input training vector
     * @param X_tr ouput training matrix
     * @param C_tr output training matrixñ
     * @return true if successfull generating matrices
     * @return false if falure in process
     */
    bool generate_training_matrices(const vector<vector<double>> &X, const vector<vector<double>> &C, mat &X_tr, mat &C_tr);

    /**
     * @brief Calculates pseudo inverse matrix using the Tensor Product Matrix (TPM) method
     * 
     * @param A Matrix to calculate pseudoinverse
     * @param Ap Output calculated pseudoinverse matrix
     * @return true and Ap matrix contains the calculated pseudo inverse matrix
     * @return false 
     */
    bool pseudo_inverse_tpm(const mat &A, mat &Ap);

    //#####################################################################################################################################

    mat rcima(const vector<vector<double>> &t_data_x, const vector<vector<double>> &t_data_c, const size_t input_rank)
    {
        mat X, C, result;

        if (!generate_training_matrices(t_data_x, t_data_c, X, C))
            return result;

        return rcima(X, C, input_rank);
    }

    mat rcima(const mat &X, const mat &C, const size_t input_rank)
    {
        mat result;
        mat p_inv_C = arma::pinv(C);

        mat U, V;
        vec s;
        arma::svd_econ(U, s, V, C, "right");

        int rank_k = arma::rank(C);
        mat Vk = V.head_cols(rank_k);

        mat P = X * Vk * Vk.t();

        mat B, D;
        if (!low_rank(P, input_rank, B, D))
            return result;

        result = B * D;
        result = result * p_inv_C;
        // calculate_error(result, C, X);

        return result;
    }

    //#####################################################################################################################################

    mat fast_rcima(const vector<vector<double>> &t_data_x, const vector<vector<double>> &t_data_c, const size_t input_rank)
    {
        mat result;
        mat X, C;

        if (!generate_training_matrices(t_data_x, t_data_c, X, C))
            return result;
        return fast_rcima(X, C, input_rank);
    }

    mat fast_rcima(const mat &X, const mat &C, const size_t input_rank)
    {
        mat result;
        mat Cp;

        pseudo_inverse_tpm(C, Cp);

        mat P = X * Cp * C;

        mat B, D;
        if (!low_rank_approx(P, input_rank, B, D))
            return result;
        result = B * D * Cp;

        // calculate_error(result, C, X);

        return result;
    }

    // ----------------------------------------------------------------------------------------------------------------------------------------
    void calculate_error(mat &A, const mat &C, const mat &X)
    {
        _calculated_error = arma::norm(A * C - X, "fro");
        _calculated_error = (_calculated_error * _calculated_error) / X.n_cols;
    }

    bool low_rank(const mat &A, const size_t r, mat &B, mat &C)
    {
        // r must be less than the minimun number of cols and rows of A
        if (r > std::min(A.n_cols, A.n_rows) || r <= 0)
            return false;

        mat U, V;
        vec s;

        if (!arma::svd_econ(U, s, V, A))
            return false;

        mat smat = mat(arma::diagmat(s))(arma::span(0, r - 1), arma::span(0, r - 1));

        B = U.head_cols(r) * smat;
        C = V.head_cols(r).t();

        return true;
    }

    bool low_rank_approx(const mat &A, const size_t r, mat &B, mat &C)
    {
        // r must be less than the minimun number of cols and rows of A
        if (r > std::min(A.n_cols, A.n_rows) || r <= 0)
            return false;
        mat Y2(A.n_cols, r, arma::fill::randn);
        mat Y1;

        for (size_t i = 0; i < LOW_RANK_APPROX_ITERATIONS; ++i)
        {
            Y1 = A * Y2;
            Y2 = A.t() * Y1;
        }

        mat Q, R;
        if (!arma::qr(Q, R, Y2))
            return false;

        mat Qr = Q.head_cols(r);
        B = A * Qr;
        C = Qr.t();

        return true;
    }

    bool generate_training_matrices(const vector<vector<double>> &X, const vector<vector<double>> &C, mat &X_mat, mat &C_mat)
    {
        size_t tset_columns = X.size(); //training set amount of images
        size_t tset_rows = X[0].size(); // training set size of vectorized image

        mat X_tr(tset_rows, tset_columns);
        mat C_tr(tset_rows, tset_columns);
        // if size of image sets don't match
        if (C.size() != tset_columns)
        {
            // Error size of x data does not match size of c data
            //return empty result
            return false;
        }
        //go through vectors and fill the matrices
        for (size_t col = 0; col < tset_columns; ++col)
        {
            tset_rows = X[col].size();
            // check image size
            if (C[col].size() != tset_rows)
            {
                // Error size of images must be the same for all images;
                return false;
            }

            for (size_t row = 0; row < tset_rows; ++row)
            {
                X_tr(row, col) = double(X[col][row]);
                C_tr(row, col) = double(C[col][row]);
            }
        }
        X_mat = X_tr;
        C_mat = C_tr;
        return true;
    }

    bool pseudo_inverse_tpm(const mat &A, mat &Ap)
    {

        size_t r = arma::rank(A);
        size_t mn = A.n_rows, S = A.n_cols;

        if (r == S || r == mn) // it is full rank
        {
            if (mn >= S)
                Ap = arma::inv(A.t() * A) * A.t();
            else
                Ap = A.t() * arma::inv(A * A.t());
        }
        else // it is rank deficient
        {
            double alpha = 0.001;

            if (mn >= S)
                Ap = arma::inv((A.t() * A) + (alpha * arma::eye(S, S))) * A.t();
            else
                Ap = A.t() * arma::inv((A * A.t()) + (alpha * arma::eye(mn, mn)));
        }
        return true;
    }
}
