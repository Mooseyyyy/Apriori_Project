#include <string>
#include <fstream>
#include <vector>
#include <iostream>

class Database {
public:
    Database();
    openDatbase1K() {
        database.open("Database1K.txt")
    }

    std::vector<std::string> transactions;
    std::ofstream database;
};

void test(Datbase db, float ms) {
    std::string fileCompare, loopCompare;
    while (db.database >> fileCompare) {
        std::cout << fileCompare << " ";
    }
}

int main() {
    Database db;
    float i = 1.0;
    db.openDatbase1K();

    test(db, i);

    return 0;
}

// test
