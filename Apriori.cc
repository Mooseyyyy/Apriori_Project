#include <string>
#include <fstream>
#include <vector>
#include <iostream>

class Database {
public:
    Database();
    void openDatabase1K() {
        database.open("Database1K.txt");
    }

    std::vector<std::string> transactions;
    std::ofstream database;
};

void test(Database &db, float ms) {
    std::string fileCompare, loopCompare;
    while (db.database) {
        std::cout << fileCompare << " ";
    }
}

int main() {
    Database db;
    float i = 1.0;
    db.openDatabase1K();

    test(db, i);

    return 0;
}

// test
