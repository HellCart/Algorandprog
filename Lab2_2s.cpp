#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <limits>
#include <cctype>
#include "Warehouse.h"

int main() {
    vector<Warehouse> warehouses;
    warehouses.emplace_back(WarehouseType::CENTER, 55.75, 37.61, 1000); // Москва
    warehouses.emplace_back(WarehouseType::WEST, 30.31, 59.93, 800);     // Санкт-Петербург
    warehouses.emplace_back(WarehouseType::EAST, 135.07, 48.48, 1200);   // Владивосток

    int choice;
    do {
        displayMainMenu();
        choice = inputInt("");

        switch (choice) {
        case 1: {
            cin.ignore();
            cout << "Введите описание продукта: ";
            string description;
            getline(cin, description);

            double price = inputDouble("Введите цену продукта: ");
            int quantity = inputInt("Введите количество: ");

            auto coords = generateRussianCoordinates();
            Product newProduct(description, price, quantity, coords.first, coords.second);

            auto minIt = min_element(warehouses.begin(), warehouses.end(),
                [&newProduct](const Warehouse& a, const Warehouse& b) {
                    return a.calculateManhattanDistance(newProduct) < b.calculateManhattanDistance(newProduct);
                });

            if (minIt != warehouses.end()) {
                if (minIt->addProduct(newProduct)) {
                    cout << "Продукт успешно добавлен на склад " << minIt->getId() << endl;
                }
                else {
                    cout << "Ошибка: недостаточно места на складе " << minIt->getId() << endl;
                }
            }
            break;
        }

        case 2: {
            for (const auto& warehouse : warehouses) {
                warehouse.print();
                cout << "====================================" << endl;
            }
            break;
        }

        case 3: {
            cin.ignore();
            cout << "Введите поисковый запрос: ";
            string searchTerm;
            getline(cin, searchTerm);

            bool found = false;
            for (const auto& warehouse : warehouses) {
                auto results = warehouse.findProductsByDescription(searchTerm);
                if (!results.empty()) {
                    cout << "\nНайдены продукты на складе " << warehouse.getId() << ":\n";
                    for (const auto& product : results) {
                        product.print();
                        cout << "------------------------" << endl;
                    }
                    found = true;
                }
            }

            if (!found) {
                cout << "Продукты не найдены.\n";
            }
            break;
        }

        case 4: {
            cin.ignore();
            cout << "Введите штрих-код продукта: ";
            string barcode;
            getline(cin, barcode);

            int quantity = inputInt("Введите количество для удаления: ");

            bool removed = false;
            for (auto& warehouse : warehouses) {
                if (warehouse.removeProduct(barcode, quantity)) {
                    cout << "Продукт успешно удален со склада " << warehouse.getId() << endl;
                    removed = true;
                    break;
                }
            }

            if (!removed) {
                cout << "Продукт не найден или недостаточное количество.\n";
            }
            break;
        }

        case 5:
            cout << "Выход из программы.\n";
            break;

        default:
            cout << "Неверный выбор. Попробуйте снова.\n";
        }
    } while (choice != 5);

    return 0;
}