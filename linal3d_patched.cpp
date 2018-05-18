#include <iostream>
#include <iomanip>
#include <math.h>

using namespace std;


template<typename Any>
class Vector3D
{
private:
	Any x;
	Any y;
	Any z;
public:
	Vector3D() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}

	Vector3D(const Any& x, const Any& y, const Any& z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	~Vector3D() { }

	Any getX() const{
		return x;
	}

	Any getY() const{
		return y;
	}

	Any getZ() const{
		return z;
	}

	void setX(Any& x) {
		this->x = x;
	}

    void setY(Any& y) {
    	this->y = y;
    }

    void setZ(Any& z) {
    	this->z = z;
    }

	bool operator== (const Vector3D<Any>& v2) {
		return ((x == v2.x) && (y == v2.y) && (z == v2.z));
	}

	bool operator!= (const Vector3D<Any>& v2) {
		return !(operator== (v2));
	}

	Vector3D<Any> operator* (const Any a) {
		Vector3D<Any> v3;
		v3.x = x*a;
		v3.y = y*a;
		v3.z = z*a;
		return v3;
	}
};



template<typename Any>
class Matrix3D {
private: 
	Any mat[3][3];
public:
	Matrix3D() {
		for(int i = 0; i < 3; i++) {
			for(int j = 0; j < 3; j++)
				this->mat[i][j] = 0;
		}
	}

	~Matrix3D() {}

	Matrix3D(Any x1, Any x2, Any x3, Any x4, Any x5, Any x6, Any x7, Any x8, Any x9) {
		mat[0][0] = x1;
		mat[0][1] = x2;
		mat[0][2] = x3;
		mat[1][0] = x4;
		mat[1][1] = x5;
		mat[1][2] = x6;
		mat[2][0] = x7;
		mat[2][1] = x8;
		mat[2][2] = x9;
	}

	Any getEl(int i, int j) const{
		return this->mat[i][j];
	}

	void setEl(int i, int j, Any x) {
		this->mat[i][j] = x;
	}

	Any det() {
		Any det;
		det = 
		  mat[0][0] * (mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2])
		- mat[0][1] * (mat[1][0] * mat[2][2] - mat[2][0] * mat[1][2])
		+ mat[2][2] * (mat[1][0] * mat[2][1] - mat[2][0] * mat[1][1]);
		return det;
	}
};

template<typename Any>
std::ostream& operator<<(std::ostream& osm, const Matrix3D<Any>& m)
{
      osm << m.getEl(0, 0) << " " << m.getEl(0, 1) << " " << m.getEl(0, 2) << endl 
	  << m.getEl(1, 0) << " " << m.getEl(1, 1) << " " << m.getEl(1, 2) << endl
	  << m.getEl(2, 0) << " " << m.getEl(2, 1) << " " << m.getEl(2, 2) << endl;
      return osm;
}

template<typename Any>
std::istream& operator>>(std::istream &ism, Matrix3D<Any>& m)
{
	Any inp[3][3];
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			ism >> inp[i][j];
			m.setEl(i, j, inp[i][j]);
		}
	}
	return ism;
}

template<typename Any>
Matrix3D<Any> operator* (const Matrix3D<Any>& m, Any a) {
    Matrix3D<Any> res;
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			res.setEl(i, j, m.getEl(i, j) * a);
			//cout << m.getEl(i, j) * a << endl;
		}
	}
	return res;
}

template<typename Any>
Matrix3D<Any> operator* (Any a, const Matrix3D<Any>& m) {
    Matrix3D<Any> res;
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			res.setEl(i, j, m.getEl(i, j) * a);
		}
	}
	return res;
}


template<typename Any>
Matrix3D<Any> operator* (const Matrix3D<Any>& m1, const Matrix3D<Any>& m2) {
		Matrix3D<Any> m3;
		Any tuk[3][3];
		for(int a = 0; a < 3; a++) {
			for(int b = 0; b < 3; b++) {
				tuk[a][b] = 0;
			}
		}

		for(int k = 0; k < 3; k++) {
			for(int i = 0; i < 3; i++) {
				for(int j = 0; j < 3; j++) {
					tuk[k][i] += m1.getEl(k, j) * m2.getEl(j, i);
					m3.setEl(k, i, tuk[k][i]);
				}
				
			}
		}
		return m3;
}

template<typename Any>
Vector3D<Any> operator* (const Matrix3D<Any>& m, const Vector3D<Any>& v) {
	Any x = v.getX() * m.getEl(0, 0) + v.getY() * m.getEl(1, 0) + v.getZ() * m.getEl(2, 0);
	Any y = v.getX() * m.getEl(0, 1) + v.getY() * m.getEl(1, 1) + v.getZ() * m.getEl(2, 1);
	Any z = v.getX() * m.getEl(0, 2) + v.getY() * m.getEl(1, 2) + v.getZ() * m.getEl(2, 2);
	Vector3D<Any> v3;
	v3.setX(x);
	v3.setY(y);
	v3.setZ(z);
	return v3;
}

template<typename Any>
Any operator* (const Vector3D<Any>& v1, const Vector3D<Any>& v2) {
	Any s = v2.getX() * v1.getX() + v2.getY() * v1.getY() + v2.getZ() * v1.getZ();
	return s;
}

template<typename Any>
Vector3D<Any> operator* (const int a, const Vector3D<Any>& v2) {
	Vector3D<Any> v4(v2.getX() * a, v2.getY() * a, v2.getZ() * a);
	return v4;
}

template<typename Any>
std::ostream& operator<<(std::ostream& os, Vector3D<Any>& v) {
      os << "(" << v.getX() << ", " << v.getY() << ", " << v.getZ() << ")";
      return os;
}

template<typename Any>
std::istream& operator>>(std::istream &is, Vector3D<Any> &v) {
	Any x, y, z;
	is >> x;
	v.setX(x);
	is >> y;
	v.setY(y);
	is >> z;
	v.setZ(z);
	return is;
}

int main()
{
	Matrix3D<double> m1(1, 0, 0, 0, 1, 0, 0, 0, 1);
	m1 = m1 * 0.1;
	cout << m1;
	return 0;
}
/*int main()
{
	Vector3D<float> v1(1, 1, 1);
	Vector3D<float> v2(42, 42, 42);

// Умножение вектора на скаляр, скаляр на вектор, вектор на вектор: 
	Vector3D<float> v3 = v1 * 228;
	Vector3D<float> v4 = 2 * v2;
	Vector3D<float> v5 = v1 * v2;

	cout << "v1 * 228 = " << v3 << endl << endl;
	cout << "2 * v2 = " << v4 << endl << endl;
	cout << "v1 * v2 = " << v5 << endl;

	Matrix3D<float> m1(1, 0, 0, 0, 1, 0, 0, 0, 1);
	Matrix3D<float> m2(1, 2, 3, 4, 5, 6, 7, 8, 9);

// Умножение матрицы на скаляр, скаляр на матрицу, матрицу на матрицу: 
	Matrix3D<float> m3 = m1 * 9;
	Matrix3D<float> m4 = 23 * m1;
	Matrix3D<float> m5 = m1 * m2;

	cout << "\nm1 * 9 =\n" << m3 << endl;
	cout << "23 * m1 =\n" << m4 << endl;
	cout << "m1 * m2 =\n" << m5 << endl;

// Ввод матрицы с клавиатуры:
	Matrix3D<float> mi;
	cin >> mi;
    cout << mi;

// Умножение матрицы на вектор дает вектор:
	Vector3D<float> v6 = m2 * v1;
	cout << "\nv6: " << v6 << endl;

// Умножение вектора на матрицу дает вектор:
	Vector3D<float> v7 = v2 * m1;
	cout << "\nv7: " << v7 << endl;

// Детерминант матрицы:
	cout << "\n" << mi.det() << endl;

	return 0;
}
*/
