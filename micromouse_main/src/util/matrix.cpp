#include "matrix.h"

template <int cols, int rows>
matrix_t<cols, rows> matrix<cols, rows>::operator +(const matrix_t<cols, rows> other) const {
    matrix_t<cols, rows> res;
    for (int j = cols - 1; j >= 0; --j) {
        for (int i = rows - 1; i >= 0; --i) {
            res.data[j][i] = data[j][i] + other.data[j][i];
        }
    }
    return res;
}

template <int cols, int rows>
matrix_t<cols, rows> matrix<cols, rows>::operator -(const matrix_t<cols, rows> other) const {
    matrix_t<cols, rows> res;
    for (int j = cols - 1; j >= 0; --j) {
        for (int i = rows - 1; i >= 0; --i) {
            res.data[j][i] = data[j][i] - other.data[j][i];
        }
    }
    return res;
}

template <int cols, int rows>
template <int rhs>
matrix_t<cols, rhs> matrix<cols, rows>::operator *(const matrix_t<rows, rhs> other) const {
    matrix_t<cols, rhs> res;
    for (int j = cols - 1; j >= 0; --j) {
        for (int i = rhs - 1; i >= 0; --i) {
            res[j][i] = 0;
            for (int k = rows - 1; k >= 0; --k) {
                res[j][i] += data[j][k] * other.data[k][i];
            }
        }
    }
    return res;
}

template <int cols, int rows>
matrix_t<cols, rows> matrix<cols, rows>::operator *(double scalar) const {
    matrix_t<cols, rows> res;
    for (int j = cols - 1; j >= 0; --j) {
        for (int i = rows - 1; i >= 0; --i) {
            res.data[j][i] = data[j][i] * scalar;
        }
    }
    return res;
}

template <int cols, int rows>
column_vector_t<rows> matrix<cols, rows>::operator [](int col) const {
    if (col < 0 || col >= cols) {
        return column_vector_t<rows>();
    }
    return column_vector_t<rows>(data[col]);
}

template <int rows>
column_vector<rows>::column_vector() : data({0}) {
}

template <int rows>
column_vector<rows>::column_vector(const double data[rows]) : data(data) {
}

template <int rows>
double column_vector<rows>::operator [](int row) const {
    return data[row];
}

template <int cols, int rows>
matrix_t<cols, rows> rref(const matrix_t<cols, rows> _mat) {
    matrix_t<cols, rows> mat = _mat;
    for (int i = 0; i < rows && i < cols; ++i) {
        int j;
        for (j = i; j < rows && mat.data[j][i] != 0; ++j);
        if (j != i) {
            for (int k = 0; k < cols; ++k) {
                double tmp = mat[k][i];
                mat[k][i] = mat[k][j];
                mat[k][j] = tmp;
            }
        }
        if (mat.data[i][j] != 0) {
            double factor = 1.0 / mat.data[i][j];
            for (int k = 0; k < cols; ++k) {
                mat[i][k] *= factor;
            }
            for (int k = j; k < rows; ++k) {
                factor = mat.data[i][k] / mat.data[j][k];
                for (int l = 0; l < cols; ++l) {
                    mat.data[l][k] -= mat.data[j][k] * factor;
                }
            }
        }
    }
    for (int i = rows > cols ? cols : rows; i >= 0; --i) {
        if (mat.data[i][i] != 0) {
            for (int j = i; j >= 0; --j) {
                double factor = mat.data[i][j] / mat.data[i][i];
                for (int k = 0; k < cols; ++k) {
                    mat.data[j][k] -= mat.data[i][k] * factor;
                }
            }
        }
    }
    return mat;
}
