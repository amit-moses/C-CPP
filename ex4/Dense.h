// Dense.h
#ifndef DENSE_H
#define DENSE_H

#include "Activation.h"

//typedef for activation function will be given to class.
typedef Matrix (* ActivationFunction)(const Matrix&);

class Dense{
    private:
        Matrix _weights, _bias;
        ActivationFunction _activation;
    public:
        Dense(const Matrix& weights, const Matrix& bias,
              ActivationFunction function);
        const Matrix& get_weights() const;
        const Matrix& get_bias() const;
        ActivationFunction get_activation() const;
        Matrix operator()(const Matrix& x) const;
};









#endif //DENSE_H