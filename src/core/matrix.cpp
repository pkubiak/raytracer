#include <core/matrix.h>
#include <core/assert.h>

#include <core/vector.h>
#include <core/point.h>

namespace rt {

Matrix::Matrix(const Float4& r1, const Float4& r2, const Float4& r3, const Float4& r4) {
    row[0] = r1;
    row[1] = r2;
    row[2] = r3;
    row[3] = r4;
}

Float4& Matrix::operator[](int idx) {
    assert(idx>=0 && idx<4);
    return row[idx];
}

Float4 Matrix::operator[](int idx) const {
    assert(idx>=0 && idx<4);
    return row[idx];
}

Matrix Matrix::operator+(const Matrix& b) const {
    return Matrix(
        row[0]+b.row[0],
        row[1]+b.row[1],
        row[2]+b.row[2],
        row[3]+b.row[3]
    );
}

Matrix Matrix::operator-(const Matrix& b) const {
    return Matrix(
        row[0]-b.row[0],
        row[1]-b.row[1],
        row[2]-b.row[2],
        row[3]-b.row[3]
    );
}

Matrix Matrix::transpose() const {
    return Matrix(
        Float4(row[0][0], row[1][0], row[2][0], row[3][0]),
        Float4(row[0][1], row[1][1], row[2][1], row[3][1]),
        Float4(row[0][2], row[1][2], row[2][2], row[3][2]),
        Float4(row[0][3], row[1][3], row[2][3], row[3][3])
        );
}

Matrix Matrix::invert() const {
    Matrix result;
    const Matrix& m = *this;

    //Taken and modified from http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
    result[0][0] = m[1][1] * m[2][2] * m[3][3] - m[1][1] * m[2][3] * m[3][2] - m[2][1] * m[1][2] * m[3][3] + m[2][1] * m[1][3] * m[3][2] + m[3][1] * m[1][2] * m[2][3] - m[3][1] * m[1][3] * m[2][2];
    result[1][0] = -m[1][0] * m[2][2] * m[3][3] + m[1][0] * m[2][3] * m[3][2] + m[2][0] * m[1][2] * m[3][3] - m[2][0] * m[1][3] * m[3][2] - m[3][0] * m[1][2] * m[2][3] + m[3][0] * m[1][3] * m[2][2];
    result[2][0] = m[1][0] * m[2][1] * m[3][3] - m[1][0] * m[2][3] * m[3][1] - m[2][0] * m[1][1] * m[3][3] + m[2][0] * m[1][3] * m[3][1] + m[3][0] * m[1][1] * m[2][3] - m[3][0] * m[1][3] * m[2][1];
    result[3][0] = -m[1][0] * m[2][1] * m[3][2] + m[1][0] * m[2][2] * m[3][1] + m[2][0] * m[1][1] * m[3][2] - m[2][0] * m[1][2] * m[3][1] - m[3][0] * m[1][1] * m[2][2] + m[3][0] * m[1][2] * m[2][1];

    float det = m[0][0] * result[0][0] + m[0][1] * result[1][0] + m[0][2] * result[2][0] + m[0][3] * result[3][0];
    if (det == 0)
        return Matrix::zero();

    result[0][1] = -m[0][1] * m[2][2] * m[3][3] + m[0][1] * m[2][3] * m[3][2] + m[2][1] * m[0][2] * m[3][3] - m[2][1] * m[0][3] * m[3][2] - m[3][1] * m[0][2] * m[2][3] + m[3][1] * m[0][3] * m[2][2];
    result[1][1] = m[0][0] * m[2][2] * m[3][3] - m[0][0] * m[2][3] * m[3][2] - m[2][0] * m[0][2] * m[3][3] + m[2][0] * m[0][3] * m[3][2] + m[3][0] * m[0][2] * m[2][3] - m[3][0] * m[0][3] * m[2][2];
    result[2][1] = -m[0][0] * m[2][1] * m[3][3] + m[0][0] * m[2][3] * m[3][1] + m[2][0] * m[0][1] * m[3][3] - m[2][0] * m[0][3] * m[3][1] - m[3][0] * m[0][1] * m[2][3] + m[3][0] * m[0][3] * m[2][1];
    result[3][1] = m[0][0] * m[2][1] * m[3][2] - m[0][0] * m[2][2] * m[3][1] - m[2][0] * m[0][1] * m[3][2] + m[2][0] * m[0][2] * m[3][1] + m[3][0] * m[0][1] * m[2][2] - m[3][0] * m[0][2] * m[2][1];
    result[0][2] = m[0][1] * m[1][2] * m[3][3] - m[0][1] * m[1][3] * m[3][2] - m[1][1] * m[0][2] * m[3][3] + m[1][1] * m[0][3] * m[3][2] + m[3][1] * m[0][2] * m[1][3] - m[3][1] * m[0][3] * m[1][2];
    result[1][2] = -m[0][0] * m[1][2] * m[3][3] + m[0][0] * m[1][3] * m[3][2] + m[1][0] * m[0][2] * m[3][3] - m[1][0] * m[0][3] * m[3][2] - m[3][0] * m[0][2] * m[1][3] + m[3][0] * m[0][3] * m[1][2];
    result[2][2] = m[0][0] * m[1][1] * m[3][3] - m[0][0] * m[1][3] * m[3][1] - m[1][0] * m[0][1] * m[3][3] + m[1][0] * m[0][3] * m[3][1] + m[3][0] * m[0][1] * m[1][3] - m[3][0] * m[0][3] * m[1][1];
    result[3][2] = -m[0][0] * m[1][1] * m[3][2] + m[0][0] * m[1][2] * m[3][1] + m[1][0] * m[0][1] * m[3][2] - m[1][0] * m[0][2] * m[3][1] - m[3][0] * m[0][1] * m[1][2] + m[3][0] * m[0][2] * m[1][1];
    result[0][3] = -m[0][1] * m[1][2] * m[2][3] + m[0][1] * m[1][3] * m[2][2] + m[1][1] * m[0][2] * m[2][3] - m[1][1] * m[0][3] * m[2][2] - m[2][1] * m[0][2] * m[1][3] + m[2][1] * m[0][3] * m[1][2];
    result[1][3] = m[0][0] * m[1][2] * m[2][3] - m[0][0] * m[1][3] * m[2][2] - m[1][0] * m[0][2] * m[2][3] + m[1][0] * m[0][3] * m[2][2] + m[2][0] * m[0][2] * m[1][3] - m[2][0] * m[0][3] * m[1][2];
    result[2][3] = -m[0][0] * m[1][1] * m[2][3] + m[0][0] * m[1][3] * m[2][1] + m[1][0] * m[0][1] * m[2][3] - m[1][0] * m[0][3] * m[2][1] - m[2][0] * m[0][1] * m[1][3] + m[2][0] * m[0][3] * m[1][1];
    result[3][3] = m[0][0] * m[1][1] * m[2][2] - m[0][0] * m[1][2] * m[2][1] - m[1][0] * m[0][1] * m[2][2] + m[1][0] * m[0][2] * m[2][1] + m[2][0] * m[0][1] * m[1][2] - m[2][0] * m[0][2] * m[1][1];

    result = result*(1.0f/det);
    return result;
}

bool Matrix::operator==(const Matrix& b) const {
    return row[0] == b.row[0] &&
        row[1] == b.row[1] &&
        row[2] == b.row[2] &&
        row[3] == b.row[3];
}

bool Matrix::operator!=(const Matrix& b) const { return !((*this)==b); }

Matrix product(const Matrix& a, const Matrix& b) {
    Matrix bT = b.transpose();
    Matrix result;
    result[0][0] = dot(a[0],bT[0]);
    result[0][1] = dot(a[0],bT[1]);
    result[0][2] = dot(a[0],bT[2]);
    result[0][3] = dot(a[0],bT[3]);
    result[1][0] = dot(a[1],bT[0]);
    result[1][1] = dot(a[1],bT[1]);
    result[1][2] = dot(a[1],bT[2]);
    result[1][3] = dot(a[1],bT[3]);
    result[2][0] = dot(a[2],bT[0]);
    result[2][1] = dot(a[2],bT[1]);
    result[2][2] = dot(a[2],bT[2]);
    result[2][3] = dot(a[2],bT[3]);
    result[3][0] = dot(a[3],bT[0]);
    result[3][1] = dot(a[3],bT[1]);
    result[3][2] = dot(a[3],bT[2]);
    result[3][3] = dot(a[3],bT[3]);
    return result;
}

Matrix operator*(const Matrix& a, float scalar) {
    return Matrix(
        a.row[0]*scalar,
        a.row[1]*scalar,
        a.row[2]*scalar,
        a.row[3]*scalar
        );
}

Matrix operator*(float scalar, const Matrix& a) {
    return a*scalar;
}

Float4 Matrix::operator*(const Float4& b) const {
    return Float4(
        dot(row[0],b),
        dot(row[1],b),
        dot(row[2],b),
        dot(row[3],b));
}

Vector Matrix::operator*(const Vector& b) const {
    return Vector((*this)*Float4(b));
}

Point Matrix::operator*(const Point& b) const {
    return Point((*this)*Float4(b));
}


float Matrix::det() const {
    const Matrix& m = *this;
    float inv0 = m[1][1] * m[2][2] * m[3][3] - m[1][1] * m[2][3] * m[3][2] - m[2][1] * m[1][2] * m[3][3] + m[2][1] * m[1][3] * m[3][2] + m[3][1] * m[1][2] * m[2][3] - m[3][1] * m[1][3] * m[2][2];
    float inv1 = -m[1][0] * m[2][2] * m[3][3] + m[1][0] * m[2][3] * m[3][2] + m[2][0] * m[1][2] * m[3][3] - m[2][0] * m[1][3] * m[3][2] - m[3][0] * m[1][2] * m[2][3] + m[3][0] * m[1][3] * m[2][2];
    float inv2 = m[1][0] * m[2][1] * m[3][3] - m[1][0] * m[2][3] * m[3][1] - m[2][0] * m[1][1] * m[3][3] + m[2][0] * m[1][3] * m[3][1] + m[3][0] * m[1][1] * m[2][3] - m[3][0] * m[1][3] * m[2][1];
    float inv3 = -m[1][0] * m[2][1] * m[3][2] + m[1][0] * m[2][2] * m[3][1] + m[2][0] * m[1][1] * m[3][2] - m[2][0] * m[1][2] * m[3][1] - m[3][0] * m[1][1] * m[2][2] + m[3][0] * m[1][2] * m[2][1];
    float det = m[0][0] * inv0 + m[0][1] * inv1 + m[0][2] * inv2 + m[0][3] * inv3;
    return det;
}

Matrix Matrix::zero() {
    return Matrix(
        Float4::rep(0),
        Float4::rep(0),
        Float4::rep(0),
        Float4::rep(0));
}

Matrix Matrix::identity() {
    return Matrix(
        Float4(1.0f, 0.0f, 0.0f, 0.0f),
        Float4(0.0f, 1.0f, 0.0f, 0.0f),
        Float4(0.0f, 0.0f, 1.0f, 0.0f),
        Float4(0.0f, 0.0f, 0.0f, 1.0f)
        );
}

Matrix Matrix::system(const Vector& e1, const Vector& e2, const Vector& e3) {
    return Matrix(
        Float4(e1),
        Float4(e2),
        Float4(e3),
        Float4(0.0f, 0.0f, 0.0f, 1.0f)).transpose();
}

}

