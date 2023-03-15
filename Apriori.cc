#include <string>
#include <fstream>
#include <vector>
#include <iostream>

class Database {
public:
    Database();
    void openDatabase(std::string dbf) {
        
        
std::string line;
        std::ifstream database;
        database.open(dbf);
        if (database.is_open()) {
            while (getline(database, line)) {
                std::cout << line << '\n';
            }
        database.close();
        } else std::cout << "Unable to open file";
    }

    std::vector<std::string> transactions;
    
};

class itemsets {
public:
    itemsets();
};

itemsets apriori(Database &db, float ms) {
    std::string fileCompare, loopCompare;
    for (int i = 0; i < db.transactions.size(); i++) {
        
    }
}

int main() {
    Database db;
    float i = 1.0;
    db.openDatabase("Database1K.txt");

    apriori(db, i);

    return 0;
}

// test
