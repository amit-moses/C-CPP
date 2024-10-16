// Matrix.h
#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>

// You don't have to use the struct. Can help you with MlpNetwork.h
struct matrix_dims {
    int rows, cols;
};

class Matrix {
    private:
        int _rows, _cols;
        float * _value;
        bool in_range(int x, int y) const;
        friend void swap_value(Matrix& ls, Matrix& rs);

    public:
        Matrix(int rows, int cols);
        Matrix();
        Matrix(const Matrix& m);
        ~Matrix();
        int get_rows() const;
        int get_cols() const;
        Matrix& transpose();
        Matrix& vectorize();
        void plain_print();
        Matrix dot(Matrix& m);
        float norm();
        Matrix rref();
        int argmax() const;
        float sum() const;
        Matrix& operator+=(const Matrix& rhs);
        Matrix operator+(const Matrix& rhs) const;
        Matrix operator*(const Matrix& rhs) const;
        Matrix operator*(float scalar) const;
        Matrix& operator=(const Matrix& rhs);

        friend Matrix operator*(float scalar, const Matrix& rhs);
        float operator()(int row, int col) const;
        float& operator()(int row, int col);
        float& operator[](int x);
        float operator[](int x) const;

        friend std::ostream& operator<<(std::ostream& s, const Matrix& m);
        friend std::istream& operator>>(std::istream& s, Matrix& m);
};


#endif //MATRIX_H