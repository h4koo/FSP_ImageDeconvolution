#ifndef FILTERINFO_H
#define FILTERINFO_H

#include <string>

// #define MAX_FILENAME_SIZE 256

namespace AppLogic{

typedef struct{
    std::string filter_name;    
    std::string file_image_source; 
    int image_calc_amount;
    float calc_time_seconds;
    int rank;
    int f_matrix_num_col;
    int f_matrix_num_row;
    std::string noise_type;
    double noise_value;
    std::string calculation_method;
} FilterInfo;

inline std::ostream& operator <<(std::ostream& stream, FilterInfo const& data)
{
//    return stream << data.filter_name.size() << " " <<  data.filter_name << " "
//             << data.file_image_source.size() << " " << data.file_image_source << " "
       return stream << data.filter_name << " "
            << data.file_image_source << " "
           << data.image_calc_amount << " " 
           << data.rank << " "
           << data.f_matrix_num_col << " "
           << data.f_matrix_num_row << " "
           << data.noise_type << " "
           << data.noise_value << " "
           << data.calculation_method << " "
           << data.calc_time_seconds;
    // return stream;
}

inline std::istream& operator >>(std::istream& stream, FilterInfo& data)
{
    
    // std::size_t size;
    // stream >> size;
    // data.filter_name.resize(size);
    // stream.read(&data.filter_name[0],size);
    // stream >> size;
    // data.file_image_source.resize(size);
    // stream.read(&data.file_image_source[0],size);
    stream >> data.filter_name >> data.file_image_source >> data.image_calc_amount >> data.rank >> data.f_matrix_num_col 
            >> data.f_matrix_num_row >> data.noise_type >> data.noise_value 
            >> data.calculation_method >> data.calc_time_seconds;
    
    return stream;
}
}

#endif

// +name : string

// +image_calc_amount : int
// +file_image_source : string
// +calc_time_miliseconds : int
// +rank : int
// +f_matrix_num_col: int
// +f_matrix_num_row: int
// +noise_type : string
// +noise_value : double
// +calculation_method : string
