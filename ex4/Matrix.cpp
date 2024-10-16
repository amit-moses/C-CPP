#include "Matrix.h"
#include <cmath>

#define MIN_VAL 0.1
#define STARS "**"
#define SPACE "  "
#define ERR_INPUT "load data failed"
#define ERR_SIZE "size is not valid"
#define ERR_DIM "dimensions error"
#define ERR_RANGE "range error"

bool Matrix::in_range(int x, int y) const {
    return 0 <= x && x < _rows && 0 <= y && y <= _cols;
}

// Friend function definition for swapping Matrix objects
void swap_value(Matrix &ls, Matrix &rs) {
    using std::swap; // Bring std::swap into scope for local swapping
    swap(ls._value, rs._value); // Swap _value pointers
    swap(ls._rows, rs._rows);   // Swap _rows
    swap(ls._cols, rs._cols);   // Swap _cols
}


int Matrix::get_rows() const { return _rows; }

int Matrix::get_cols() const { return _cols; }

float Matrix::operator()(int row, int col) const {
    if (!(*this).in_range(row, col)) {
        throw std::out_of_range(ERR_RANGE);
    }
    return _value[row * _cols + col];
}

float &Matrix::operator()(int row, int col) {
    if (!(*this).in_range(row, col)) {
        throw std::out_of_range(ERR_RANGE);
    }
    return _value[row * _cols + col];
}

float &Matrix::operator[](int x) {
    if (0 <= x && x < _rows * _cols) {
        return _value[x];
    }
    throw std::out_of_range(ERR_RANGE);

}

float Matrix::operator[](int x) const {
    if (0 <= x && x < _rows * _cols) {
        return _value[x];
    }
    throw std::out_of_range(ERR_RANGE);
}


bool check_dim(const Matrix &ls, const Matrix &rs, bool mult) {
    return mult ? ls.get_cols() == rs.get_rows() :
           ls.get_cols() == rs.get_cols() && ls.get_rows() == rs.get_rows();
}

// Constructor definition
Matrix::Matrix(int rows, int cols) : _rows(rows), _cols(cols) {
    if (rows <= 0 || cols <= 0) {
        throw std::runtime_error(ERR_SIZE);
    }
    _value = new float[rows * cols];
    for (int i = 0; i < rows * cols; ++i) {
        _value[i] = 0;
    }
}

// Copy Constructor definition
Matrix::Matrix(const Matrix &m) : _rows(m._rows), _cols(m._cols) {
    _value = new float[m._rows * m._cols];
    for (int i = 0; i < m._rows * m._cols; ++i) {
        _value[i] = m[i];
    }
}

Matrix::Matrix() : Matrix(1, 1) {}

Matrix &Matrix::operator=(const Matrix &rhs) {
    if (this != &rhs) {
        Matrix cpy(rhs);
        swap_value(*this, cpy);
    }
    return *this;
}

// Destructor definition
Matrix::~Matrix() {
    delete[] _value;
}


// Transpose method definition
Matrix &Matrix::transpose() {
    Matrix trans(_cols, _rows); // Create a new Matrix for transpose
    for (int i = 0; i < _rows; ++i) {
        for (int j = 0; j < _cols; ++j) {
            trans(j, i) = (*this)(i, j); // Perform transpose
        }
    }
    swap_value(*this, trans); // Swap values with transposed matrix
    return *this;
}


Matrix &Matrix::vectorize() {
    _rows *= _cols;
    _cols = 1;
    return *this;
}

void Matrix::plain_print() {
    for (int i = 0; i < _rows; ++i) {
        for (int j = 0; j < _cols; ++j) {
            std::cout << (*this)(i, j) << " ";
        }
        std::cout << std::endl;
    }
}

Matrix Matrix::dot(Matrix &m) {
    if (!check_dim(m, *this, false)) {
        throw std::runtime_error(ERR_DIM);
    }
    Matrix mlt(_rows, _cols);
    for (int i = 0; i < _rows; ++i) {
        for (int j = 0; j < _cols; ++j) {
            mlt(i, j) = (*this)(i, j) * m(i, j);
        }
    }
    return mlt;
}

float Matrix::norm() {
    double rtn = 0;
    for (int i = 0; i < _rows * _cols; ++i) {
        rtn += _value[i] * _value[i];
    }
    return (float) sqrt(rtn);
}

Matrix Matrix::rref() {
    Matrix res = *this;
    int pivot = 0;
    for (int r = 0; r < res._rows && pivot < res._cols; r++, pivot++) {
        int i = r;

        /* checking if the rest of matrix is already rref*/
        while (res(i, pivot) == 0) {
            if (++i == res._rows) {
                i = r;
                if (++pivot == res._cols) {
                    return res;
                }
            }
        }

        /* bring the row with non-zero entry to the current row position */
        if (i != r) {
            for (int j = 0; j < res._cols; j++) {
                std::swap(res(r, j), res(i, j));
            }
        }

        /* dividing all elements by the pivot value */
        float pivot_value = res(r, pivot);
        if (pivot_value != 0) {
            for (int j = pivot; j < res._cols; j++) {
                res(r, j) /= pivot_value;
            }
        }

        /* clearing entries in column except the pivot for leading */
        for (int k = 0; k < res._rows; k++) {
            if (k != r) {
                float scalar = res(k, pivot);
                for (int j = pivot; j < res._cols; j++) {
                    res(k, j) -= scalar * res(r, j);
                }
            }
        }
    }

    return res;
}



int Matrix::argmax() const {
    int arg = 0;
    for (int i = 1; i < _rows * _cols; ++i) {
        if (_value[arg] < _value[i]) {
            arg = i;
        }
    }
    return arg;
}

float Matrix::sum() const {
    float sum = 0;
    for (int i = 0; i < _rows; ++i) {
        for (int j = 0; j < _cols; ++j) {
            sum += (*this)(i, j);
        }
    }
    return sum;
}

Matrix &Matrix::operator+=(const Matrix &rhs) {
    if (!check_dim(*this, rhs, false)) {
        throw std::runtime_error(ERR_DIM);
    }
    for (int i = 0; i < _rows; ++i) {
        for (int j = 0; j < _cols; ++j) {
            (*this)(i, j) += rhs(i, j);
        }
    }
    return *this;
}

Matrix Matrix::operator+(const Matrix &rhs) const {
    Matrix res(*this);
    res += rhs;
    return res;
}

Matrix Matrix::operator*(const Matrix &rhs) const {
    // Check dimensions compatibility
    if (_cols != rhs._rows) {
        throw std::runtime_error(ERR_DIM);
    }

    // Create the result matrix with appropriate dimensions
    Matrix res(_rows, rhs._cols);

    // Perform matrix multiplication
    for (int i = 0; i < _rows; ++i) {
        for (int j = 0; j < rhs._cols; ++j) {
            float sum = 0.0;
            for (int k = 0; k < _cols; ++k) {
                sum += (*this)(i, k) * rhs(k, j);
            }
            res(i, j) = sum;
        }
    }
    return res;
}

Matrix Matrix::operator*(float scalar) const {
    Matrix res(*this);
    for (int i = 0; i < res._rows * res._cols; ++i) {
        for (int j = 0; j < res._cols; ++j) {
            res[i] = scalar * _value[i];
        }
    }
    return res;
}

Matrix operator*(float scalar, const Matrix &rhs) {
    return rhs * scalar;
}

std::ostream &operator<<(std::ostream &s, const Matrix &m) {
    for (int i = 0; i < m._rows; ++i) {
        for (int j = 0; j < m._cols; ++j) {
            s << (m(i, j) > MIN_VAL ? STARS : SPACE);
        }
        s << std::endl;
    }
    return s;
}

std::istream &operator>>(std::istream &s, Matrix &m) {
    float buf = 0;
    Matrix tmp(m._rows, m._cols);
    for (int i = 0; i < tmp._rows; ++i) {
        for (int j = 0; j < tmp._cols; ++j) {
            s.read(reinterpret_cast<char *>(&buf), sizeof(buf));
            if (!s.good()) {
                throw std::runtime_error(ERR_INPUT);
            }
            tmp(i, j) = buf;
        }
    }
    swap_value(m, tmp);
    return s;
}

