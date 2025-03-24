#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <limits>
#include <cctype>

using namespace std;

enum class WarehouseType {
    CENTER,
    WEST,
    EAST
};

string warehouseTypeToString(WarehouseType type) {
    switch (type) {
    case WarehouseType::CENTER: return "центр";
    case WarehouseType::WEST: return "запад";
    case WarehouseType::EAST: return "восток";
    default: return "неизвестно";
    }
}

class Product {
private:
    string barcode;
    string description;
    double price;
    int quantity;
    double longitude;
    double latitude;

public:
    Product() : barcode(""), description(""), price(0.0), quantity(0), longitude(0.0), latitude(0.0) {}

    Product(string desc, double pr, int qty, double lon, double lat)
        : description(desc), price(pr), quantity(qty), longitude(lon), latitude(lat) {
        generateBarcode();
    }

    Product(const Product& other)
        : barcode(other.barcode), description(other.description),
        price(other.price), quantity(other.quantity),
        longitude(other.longitude), latitude(other.latitude) {}

    void generateBarcode() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> part1(1, 9);
        uniform_int_distribution<> part2(0, 999999);

        int randomPart1 = part1(gen) * 1000;
        int randomPart2 = part2(gen);

        stringstream ss;
        ss << "460" << setw(4) << setfill('0') << randomPart1
            << setw(6) << setfill('0') << randomPart2;
        barcode = ss.str();
    }

    string getBarcode() const { return barcode; }
    string getDescription() const { return description; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    double getLongitude() const { return longitude; }
    double getLatitude() const { return latitude; }

    void setDescription(const string& desc) { description = desc; }
    void setPrice(double pr) { price = pr; }
    void setQuantity(int qty) { quantity = qty; }
    void setLongitude(double lon) { longitude = lon; }
    void setLatitude(double lat) { latitude = lat; }

    void print() const {
        cout << "Продукт: " << description << endl;
        cout << "Штрих-код: " << barcode << endl;
        cout << "Цена: " << price << " руб." << endl;
        cout << "Количество: " << quantity << endl;
        cout << "Позиция (долгота, широта): (" << longitude << ", " << latitude << ")" << endl;
    }
};

class Warehouse {
private:
    string id;
    WarehouseType type;
    double longitude;
    double latitude;
    int maxCapacity;
    int totalStock;
    vector<Product> products;
    static int nextId;

public:
    Warehouse() : id(""), type(WarehouseType::CENTER), longitude(0.0), latitude(0.0),
        maxCapacity(0), totalStock(0) {}

    Warehouse(WarehouseType t, double lon, double lat, int maxCap)
        : type(t), longitude(lon), latitude(lat), maxCapacity(maxCap), totalStock(0) {
        generateId();
    }

    Warehouse(const Warehouse& other)
        : id(other.id), type(other.type), longitude(other.longitude),
        latitude(other.latitude), maxCapacity(other.maxCapacity),
        totalStock(other.totalStock), products(other.products) {}

    ~Warehouse() {
        products.clear();
    }

    void generateId() {
        stringstream ss;
        ss << "W" << (100 + nextId++);
        id = ss.str();
    }

    string getId() const { return id; }
    WarehouseType getType() const { return type; }
    double getLongitude() const { return longitude; }
    double getLatitude() const { return latitude; }
    int getMaxCapacity() const { return maxCapacity; }
    int getTotalStock() const { return totalStock; }
    const vector<Product>& getProducts() const { return products; }

    bool addProduct(const Product& product) {
        if (totalStock + product.getQuantity() <= maxCapacity) {
            products.push_back(product);
            totalStock += product.getQuantity();
            return true;
        }
        return false;
    }

    bool removeProduct(const string& barcode, int quantity) {
        for (auto it = products.begin(); it != products.end(); ++it) {
            if (it->getBarcode() == barcode) {
                if (it->getQuantity() >= quantity) {
                    it->setQuantity(it->getQuantity() - quantity);
                    totalStock -= quantity;

                    if (it->getQuantity() == 0) {
                        products.erase(it);
                    }
                    return true;
                }
                return false;
            }
        }
        return false;
    }

    double calculateManhattanDistance(const Product& product) const {
        return abs(latitude - product.getLatitude()) + abs(longitude - product.getLongitude());
    }

    vector<Product> findProductsByDescription(const string& searchTerm) const {
        vector<Product> foundProducts;
        string lowerSearch = searchTerm;
        transform(lowerSearch.begin(), lowerSearch.end(), lowerSearch.begin(), ::tolower);

        for (const auto& product : products) {
            string lowerDesc = product.getDescription();
            transform(lowerDesc.begin(), lowerDesc.end(), lowerDesc.begin(), ::tolower);

            if (lowerDesc.find(lowerSearch) != string::npos) {
                foundProducts.push_back(product);
            }
        }
        return foundProducts;
    }

    void print() const {
        cout << "\nСклад ID: " << id << endl;
        cout << "Тип: " << warehouseTypeToString(type) << endl;
        cout << "Позиция (долгота, широта): (" << longitude << ", " << latitude << ")" << endl;
        cout << "Вместимость: " << totalStock << "/" << maxCapacity << endl;

        if (products.empty()) {
            cout << "На складе нет продуктов." << endl;
        }
        else {
            cout << "Продукты на складе:" << endl;
            for (const auto& product : products) {
                product.print();
                cout << "------------------------" << endl;
            }
        }
    }
};

int Warehouse::nextId = 0;

pair<double, double> generateRussianCoordinates() {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> lonDist(19.0, 169.0);
    uniform_real_distribution<> latDist(41.0, 82.0);

    return make_pair(lonDist(gen), latDist(gen));
}

double inputDouble(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            break;
        }
        else {
            cout << "Ошибка ввода. Пожалуйста, введите число.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    return value;
}

int inputInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            break;
        }
        else {
            cout << "Ошибка ввода. Пожалуйста, введите целое число.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    return value;
}

void displayMainMenu() {
    cout << "\n===== СИСТЕМА УПРАВЛЕНИЯ СКЛАДАМИ =====\n";
    cout << "1. Добавить продукт\n";
    cout << "2. Просмотреть все склады\n";
    cout << "3. Поиск продукта по описанию\n";
    cout << "4. Удалить продукт\n";
    cout << "5. Выход\n";
    cout << "Выберите действие: ";
}