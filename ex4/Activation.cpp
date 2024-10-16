#include "Activation.h"
#include <cmath>

Matrix activation::relu(const Matrix& m){
    Matrix res(m);
    for(int i = 0; i < res.get_rows(); i++){
        for(int j = 0; j < res.get_cols(); j++){
            res(i,j) = 0 < res(i,j) ? res(i,j) : 0;
        }
    }
    return res;
}

Matrix activation::softmax(const Matrix& m){
    Matrix res(m);
    float mult = 0;
    for(int i = 0; i < res.get_rows(); i++){
        for(int j = 0; j < res.get_cols(); j ++){
            res(i,j) = std::exp(res(i,j));
            mult += res(i,j);
        }
    }
    return ((float)1.0 / mult) * res;
}