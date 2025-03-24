#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <algorithm>

using namespace std;

class Polynomial {
private:
    vector<double> coefficients; 
    vector<int> exponents;       
    int degree;                  

public:
    Polynomial() : degree(-1) {}

    Polynomial(int deg) : degree(deg) {
        if (deg < 0) {
            throw invalid_argument("Степень полинома не может быть отрицательной");
        }
        coefficients.resize(deg + 1, 0.0);
        exponents.resize(deg + 1);
        for (int i = 0; i <= deg; ++i) {
            exponents[i] = i;
        }
    }

    Polynomial(const Polynomial& other)
        : coefficients(other.coefficients), exponents(other.exponents), degree(other.degree) {}

    ~Polynomial() {
        coefficients.clear();
        exponents.clear();
    }

    int getDegree() const { return degree; }
    const vector<double>& getCoefficients() const { return coefficients; }
    const vector<int>& getExponents() const { return exponents; }

    void setCoefficient(int index, double value) {
        if (index < 0 || index > degree) {
            throw out_of_range("Индекс выходит за пределы степени полинома");
        }
        coefficients[index] = value;
    }

    void setAllCoefficients(const vector<double>& coeffs) {
        if (coeffs.size() != degree + 1) {
            throw invalid_argument("Неверное количество коэффициентов");
        }
        coefficients = coeffs;
    }

    double evaluate(double x) const {
        double result = 0.0;
        for (size_t i = 0; i < coefficients.size(); ++i) {
            result += coefficients[i] * pow(x, exponents[i]);
        }
        return result;
    }

    void print() const {
        if (degree == -1) {
            cout << "Пустой полином";
            return;
        }

        for (int i = 0; i <= degree; ++i) {
            if (i != 0) {
                cout << (coefficients[i] >= 0 ? " + " : " - ");
            }
            cout << abs(coefficients[i]);

            if (exponents[i] > 0) {
                cout << "x";
                if (exponents[i] > 1) {
                    cout << "^" << exponents[i];
                }
            }
        }
        cout << endl;
    }

    Polynomial operator+(const Polynomial& other) const {
        if (degree != other.degree) {
            throw invalid_argument("Полиномы должны иметь одинаковую степень");
        }

        Polynomial result(degree);
        for (int i = 0; i <= degree; ++i) {
            result.coefficients[i] = coefficients[i] + other.coefficients[i];
        }
        return result;
    }

    Polynomial operator-(const Polynomial& other) const {
        if (degree != other.degree) {
            throw invalid_argument("Полиномы должны иметь одинаковую степень");
        }

        Polynomial result(degree);
        for (int i = 0; i <= degree; ++i) {
            result.coefficients[i] = coefficients[i] - other.coefficients[i];
        }
        return result;
    }
};

class VectPolynomial {
private:
    vector<Polynomial> polynomials;

public:
    VectPolynomial() = default;
    VectPolynomial(const vector<Polynomial>& polys) : polynomials(polys) {}
    VectPolynomial(const VectPolynomial& other) : polynomials(other.polynomials) {}

    ~VectPolynomial() {
        polynomials.clear();
    }

    size_t getCount() const { return polynomials.size(); }
    const vector<Polynomial>& getPolynomials() const { return polynomials; }

    void addPolynomial(const Polynomial& poly) {
        polynomials.push_back(poly);
    }

    Polynomial& getPolynomial(size_t index) {
        if (index >= polynomials.size()) {
            throw out_of_range("Индекс выходит за пределы вектора");
        }
        return polynomials[index];
    }

    void printAll() const {
        for (size_t i = 0; i < polynomials.size(); ++i) {
            cout << "Полином " << i + 1 << ": ";
            polynomials[i].print();
        }
    }
};

VectPolynomial createPolynomialVector(int count, int degree) {
    VectPolynomial vp;
    for (int i = 0; i < count; ++i) {
        Polynomial p(degree);
        for (int j = 0; j <= degree; ++j) {
            p.setCoefficient(j, (i + 1) * (j + 1));
        }

        vp.addPolynomial(p);
    }
    return vp;
}

VectPolynomial processPolynomials(const VectPolynomial& v1, const VectPolynomial& v2) {
    if (v1.getCount() != v2.getCount()) {
        throw invalid_argument("Векторы должны быть одинакового размера");
    }

    VectPolynomial result;
    size_t count = v1.getCount();

    for (size_t i = 0; i < count; ++i) {
        if (i % 2 == 0) {
            result.addPolynomial(v1.getPolynomials()[i] + v2.getPolynomials()[i]);
        }
        else {
            result.addPolynomial(v1.getPolynomials()[i] - v2.getPolynomials()[i]);
        }
    }

    return result;
}