#ifndef MATRIX_HPP_INCLUDED
#define MATRIX_HPP_INCLUDED

#include <iostream>
#include <limits>

template<typename T>
class Matrix
{
	public:

	Matrix();
	Matrix(int side);
	Matrix(const Matrix<T>& m, bool *activate);
	~Matrix();

	void floyd_warshall();
	void floyd_warshall(bool *activate);

	int getSide() const;
	T& operator[](int index) const;
	T& operator()(int i, int j) const;
	Matrix<T>& operator=(const Matrix<T>& m);

	private:

	int m_side;
	T *m_mat;
	int *m_conv;
	int m_sizeC;
};

#endif