#include "Dense.h"

Dense::Dense(const Matrix& weights, const Matrix& bias,
      ActivationFunction function):
      _weights(weights), _bias(bias), _activation(function){

}

const Matrix& Dense::get_weights() const{
    return _weights;
}
const Matrix& Dense::get_bias() const{
    return _bias;
}

ActivationFunction Dense::get_activation() const{
    return _activation;
}

Matrix Dense::operator()(const Matrix& x) const{
    return _activation(_weights * x + _bias);
}