#ifndef LIBFRCIMA_H
#define LIBFRCIMA_H

#include <armadillo>
#include <vector>
#include <algorithm>

namespace frcima
{
  using arma::mat;
  using std::vector;

  extern double error;

  // using namespace arma;

  /**
 * @brief calculates rank constrained inverse matrix approximation using RCIMA method
 * 
 * @param t_data_x training data 
 * @param t_data_c training data
 * @param rank desired rank constrain, should be less than the size of training data 
 * @return mat rank constrained inverse matrix approximation 
 */
  mat rcima(const vector<vector<double>> &t_data_x, const vector<vector<double>> &t_data_c, const size_t rank);

  mat rcima(const mat &X, const mat &C, const size_t rank);

  /**
 * @brief calculates rank constrained inverse matrix approximation using fast-RCIMA algorithm
 * 
 * @param t_data_x training data
 * @param t_data_c training data
 * @param rank desired rank approximation, should be less than the size of training data
 * @return mat rank constrained inverse matrix approximation
 */
  mat fast_rcima(const vector<vector<double>> &t_data_x, const vector<vector<double>> &t_data_c, const size_t rank);

  mat fast_rcima(const mat &X, const mat &C, const size_t rank);

  /**
 * @brief calculates rank constrained inverse matrix approximation using fast-RCIMA algorithm using AVX instructions
 * 
 * @param t_data_x training data
 * @param t_data_c training data
 * @param rank desired rank approximation, should be less than the size of training data
 * @return mat rank constrained inverse matrix approximation
 */
  mat fast_rcima_AVX(const vector<vector<double>> t_data_x, const vector<vector<double>> t_data_c, const size_t rank);

  /**
 * @brief calculates rank constrained inverse matrix approximation using fast-RCIMA algorithm using OpenCL library
 * 
 * @param t_data_x training data
 * @param t_data_c training data
 * @param rank desired rank approximation, should be less than the size of training data
 * @return mat rank constrained inverse matrix approximation
 */
  mat fast_rcima_OCL(const vector<vector<double>> t_data_x, const vector<vector<double>> t_data_c, const size_t rank);

}

#endif