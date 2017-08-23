#include "matrix.hpp"

template<typename T>
Matrix<T>::Matrix() : m_side(0), m_mat(nullptr), m_conv(nullptr), m_sizeC(0)
{

}

template<typename T>
Matrix<T>::Matrix(int side) : m_side(side), m_mat(new T[m_side*m_side]), m_conv(nullptr), m_sizeC(0)
{
	for(int i(0); i < m_side; i++)
	{
		for(int j(0); j < m_side; j++)
		{
			m_mat[i*m_side+j] = (i != j) ? std::numeric_limits<T>::infinity() : 0;
		}
	}
}

template<typename T>
Matrix<T>::Matrix(const Matrix<T>& m, bool *activate) : m_side(0), m_mat(nullptr), m_conv(new int[m.m_side]), m_sizeC(m.m_side)
{
	for(int i(0); i < m.m_side; i++)
	{
		m_conv[i] = activate[i]*m_side;
		m_side += activate[i];
	}

	m_mat = new T[m_side*m_side];

	int *tmp(new int[m_side]);


	for(int i(0),j(0); i < m.m_side; i++)
	{
		if(activate[i])
			tmp[j++] = i;
	}

	for(int i(0); i < m_side; i++)
	{
		for(int j(0); j < m_side; j++)
		{
			m_mat[i*m_side+j] = m.m_mat[tmp[i]*m.m_side+tmp[j]];
		}
	}

	delete[] tmp;
}

template<typename T>
Matrix<T>::~Matrix()
{
	delete[] m_mat;
	delete[] m_conv;
}

template<typename T>
void Matrix<T>::floyd_warshall()
{
	for(int k(0); k < m_side; k++)
	{
		int kms(k*m_side);
		for(int i(0); i < m_side; i++)
		{	
			int ims(i*m_side);
			T ik(m_mat[kms+i]);
			for(int j(0); j < m_side; j++)
			{
				T ij(m_mat[ims+j]);
				T kj(m_mat[kms+j]);
				T ikkj(ik+kj);

				m_mat[ims+j] = ((ij > ikkj)) ? ikkj : ij;
			}
		}
	}
}

template<typename T>
int Matrix<T>::getSide() const
{
	return m_side;
}

template<typename T>
T& Matrix<T>::operator[](int index) const
{
	return m_mat[index];
}

template<typename T>
T& Matrix<T>::operator()(int i, int j) const
{
	return m_mat[m_conv[i]*m_side+m_conv[j]];
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& m)
{
	delete[] m_mat;
	delete[] m_conv;

	m_side = m.m_side;
	m_sizeC = m.m_sizeC;
	m_mat = new T[m_side*m_side];

	if(m.m_conv)
	{
		m_conv = new int[m_sizeC];

		for(int i(0); i < m_sizeC; i++)
		{
			m_conv[i] = m.m_conv[i];
		}
	}
	else
	{
		m_conv = nullptr;
	}

	for(int i(0); i < m_side*m_side; i++)
	{
		m_mat[i] = m.m_mat[i];
	}

	return *this;
}


template class Matrix<float>;





