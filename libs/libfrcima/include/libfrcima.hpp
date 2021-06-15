/**
 * @file libfrcima.hpp
 * @author Jorge Agüero Zamora
 * @brief Contains the RCIMA and fast-RCIMA methods
 * @version 0.1
 * @date 2021-06-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef LIBFRCIMA_H
#define LIBFRCIMA_H

#define LOW_RANK_APPROX_ITERATIONS 3 //!< amount of iteration to approximate low rank matrix

#include <armadillo>
#include <vector>
#include <algorithm>

namespace frcima
{
  using arma::mat;
  using std::vector;

  extern double _calculated_error; //!< global variable to hold calculated error

  /**
 * @brief calculates rank constrained inverse matrix approximation using RCIMA method
 * 
 * @param t_data_x the original source images. The training data is a vector of vectors 
 *                  containing a vectorized form of an image
 * @param t_data_c the images with a noise. The training data is a vector of vectors 
 *                  containing a vectorized form of an image
 * @param rank desired rank constrain, should be less than the size of training data 
 * @return mat rank constrained inverse matrix approximation 
 */
  mat rcima(const vector<vector<double>> &t_data_x, const vector<vector<double>> &t_data_c, const size_t rank);

  /**
 * @brief calculates rank constrained inverse matrix approximation using RCIMA method
 * 
 * @param t_data_x the original source images. The training data is an arma::mat  
 *                  where each column is a vectorized form of a training image
 * @param t_data_c the images with a noise applied. The training data is an arma::mat  
 *                  where each column is a vectorized form of a training image
 * @param rank desired rank constrain, should be less than the size of training data 
 * @return mat rank constrained inverse matrix approximation 
 */
  mat rcima(const mat &X, const mat &C, const size_t rank);

  /**
 * @brief calculates rank constrained inverse matrix approximation using fast-RCIMA algorithm
 * 
 * @param t_data_x the original source images. The training data is a vector of vectors 
 *                  containing a vectorized form of an image
 * @param t_data_c the images with noise. The training data is a vector of vectors 
 *                  containing a vectorized form of an image
 * @param rank desired rank approximation, should be less than the size of training data
 * @return mat rank constrained inverse matrix approximation
 */
  mat fast_rcima(const vector<vector<double>> &t_data_x, const vector<vector<double>> &t_data_c, const size_t rank);

  /**
 * @brief calculates rank constrained inverse matrix approximation using fast-RCIMA algorithm
 * 
 * @param t_data_x the original source images. The training data is an arma::mat  
 *                  where each column is a vectorized form of a training image
 * @param t_data_c the images with noise. The training data is an arma::mat  
 *                  where each column is a vectorized form of a training image
 * @param rank desired rank approximation, should be less than the size of training data
 * @return mat rank constrained inverse matrix approximation
 */
  mat fast_rcima(const mat &X, const mat &C, const size_t rank);

}

#endif