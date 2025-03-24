#include <iostream>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include "Polynomial.h"

int main() {
    try {
        int polyCount = 5;
        int polyDegree = 3;

        VectPolynomial v1 = createPolynomialVector(polyCount, polyDegree);
        VectPolynomial v2 = createPolynomialVector(polyCount, polyDegree);

        cout << "Вектор полиномов V1:" << endl;
        v1.printAll();

        cout << "\nВектор полиномов V2:" << endl;
        v2.printAll();

        VectPolynomial vout = processPolynomials(v1, v2);

        cout << "\nРезультирующий вектор Vout:" << endl;
        vout.printAll();

        double x = 2.0;
        cout << "\nВычисление значения первого полинома Vout при x = " << x << ": ";
        cout << vout.getPolynomials()[0].evaluate(x) << endl;

    }
    catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }

    return 0;
}