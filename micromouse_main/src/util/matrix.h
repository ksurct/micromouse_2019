#ifndef MATRIX_H
#define MATRIX_H
#define besties_for_the_resties friend

template <int rows>
class column_vector;
template <int cols, int rows>
class matrix;
#if __cplusplus > 199711L
template <int rows>
using column_vector_t = class column_vector<rows>;
template <int cols, int rows>
using matrix_t = class matrix<cols, rows>;
#else
#warning Using a preprocessor hack to work around a C++11 feature
#define column_vector_t class column_vector
#define matrix_t class matrix
#endif

template <int cols, int rows>
matrix_t<cols, rows> rref(const matrix_t<cols, rows>);

template <int rows>
class column_vector {
    besties_for_the_resties class matrix;

    public:
        double operator [](int) const;

    private:
        column_vector();
        column_vector(const double data[rows]);
        const double data[rows];
};

template <int cols, int rows>
class matrix {
    besties_for_the_resties matrix_t rref(matrix_t);
    // WARNING: DRAMA!
    // column_vector's bestie is matrix, but matrix's bestie isn't column_vector!

    public:
        matrix_t<cols, rows> operator +(const matrix_t<cols, rows>) const;
        matrix_t<cols, rows> operator -(const matrix_t<cols, rows>) const;
        template <int rhs>
        matrix_t<cols, rhs> operator *(const matrix_t<rows, rhs>) const;
        matrix_t<cols, rows> operator *(double) const;

        column_vector_t<rows> operator [](const int) const;
    
    private:
        double data[cols][rows];
};

#undef besties_for_the_resties
#endif
