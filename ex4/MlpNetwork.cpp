#include "MlpNetwork.h"


MlpNetwork::MlpNetwork(Matrix weights[], Matrix biases[]):
        _dns1(weights[0], biases[0], activation::relu),
        _dns2(weights[1], biases[1], activation::relu),
        _dns3(weights[2], biases[2], activation::relu),
        _dns4(weights[3], biases[3], activation::softmax){}

digit MlpNetwork::operator()(Matrix image) const{
    Matrix res(_dns4(_dns3(_dns2(_dns1(image.vectorize())))));
    unsigned int number = res.argmax();
    digit result = {number, res[(int)number]};
    return result;
}