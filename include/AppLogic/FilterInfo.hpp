#ifndef FILTERINFO_H
#define FILTERINFO_H

#include <string>

typedef struct{
    std::string name;
    int image_calc_amount;
    std::string file_image_source;
    int calc_time_seconds;
    int rank;
    int f_matrix_num_col;
    int f_matrix_num_row;
    std::string noise_type;
    double noise_value;
    std::string calculation_method;
} FilterInfo;

#endif

// +name : string

// +image_calc_amount : int
// +file_image_source : string
// +calc_time_seconds : int
// +rank : int
// +f_matrix_num_col: int
// +f_matrix_num_row: int
// +noise_type : string
// +noise_value : double
// +calculation_method : string
