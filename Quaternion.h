#pragma once

#include <iostream>
#include <math.h>
#include <string>
using namespace std;

class Quaternion {
private:
    double a;
    double b;
    double c;
    double d;
public:
    Quaternion(double valueA, double valueB, double valueC, double valueD) {
        a = valueA;
        b = valueB;
        c = valueC;
        d = valueD;
    }

    Quaternion() : a(1), b(1), c(1), d(1) {}

    Quaternion(const Quaternion& other)
    {

    }

    double norm() const {
        return sqrt(a * a + b * b + c * c + d * d);
    }

    Quaternion conjugate() const {
        return Quaternion(a, -b, -c, -d);
    }

    Quaternion normalize() const {
        double n = norm();
        if (n == 0) {
            throw invalid_argument("Ќевозможно нормализовать кватернион: норма равна нулю.");
        }
    }

    Quaternion inverse() const {
        double n = norm();
        if (n == 0) {
            throw std::invalid_argument("Ќевозможно вычислить инверсию: норма равна нулю.");
        }
        return conjugate() * (1.0 / (n * n));
    }

    friend std::ostream& operator<<(std::ostream& os, const Quaternion& q) {
        os << q.a << " + " << q.b << "i + " << q.c << "j + " << q.d << "k";
        return os;
    }

    Quaternion operator*(double scalar) const {
        return Quaternion(a * scalar, b * scalar, c * scalar, d * scalar);
    }


    Quaternion operator+(const Quaternion& other) const {
        return Quaternion(a + other.a, b + other.b, c + other.c, d + other.d);
    }


    Quaternion operator-(const Quaternion& other) const {
        return Quaternion(a - other.a, b - other.b, c - other.c, d - other.d);
    }


    Quaternion operator*(const Quaternion& other) const {
        return Quaternion(
            a * other.a - b * other.b - c * other.c - d * other.d,
            a * other.b + b * other.a + c * other.d - d * other.c,
            a * other.c - b * other.d + c * other.a + d * other.b,
            a * other.d + b * other.c - c * other.b + d * other.a
        );
    }


    double dot(const Quaternion& other) const {
        return a * other.a + b * other.b + c * other.c + d * other.d;
    }

    Quaternion divide(const Quaternion& q1, const Quaternion& q2) {
        return q1 * q2.inverse();
    }

    static double euclideanDistance(const Quaternion& q1, const Quaternion& q2) {
        double da = q1.a - q2.a;
        double db = q1.b - q2.b;
        double dc = q1.c - q2.c;
        double dd = q1.d - q2.d;
        return sqrt(da * da + db * db + dc * dc + dd * dd);
    }

    static double chebyshevNorm(const Quaternion& q1, const Quaternion& q2) {
        double da = std::abs(q1.a - q2.a);
        double db = std::abs(q1.b - q2.b);
        double dc = std::abs(q1.c - q2.c);
        double dd = std::abs(q1.d - q2.d);
        return max({ da, db, dc, dd });
    }

    double GetA()
    {
        return a;
    }

    double GetB()
    {
        return b;
    }

    double GetC()
    {
        return c;
    }

    double GetD()
    {
        return d;
    }

    void SetA(double valueA)
    {
        a = valueA;
    }

    void SetB(double valueB)
    {
        b = valueB;
    }

    void SetC(double valueC)
    {
        c = valueC;
    }

    void SetD(double valueD)
    {
        d = valueD;
    }

    void Print() {
        cout << "q = " << a << " + " << b << "*i + " << c << "*j + " << d << "*k";
    }
};