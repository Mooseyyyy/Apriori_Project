#include <string>
#include <fstream>
#include <vector>
#include <iostream>

class Database {
private:
    ifstream fin;
public:
    Database();
    void openDatabase(std::string dbf) {
        fin.open(dbf);
        if(!fin) {
            std::cout << "File can not be opened" << std::endl;
            exit(0);
        }
        TakeIn();
    }

    void TakeIn() {
        std::string temp;
        while (std::cin >> temp && temp != "#") {
            transactions.push_back(s);
        }

    }

    std::vector<std::vector<std::string>> transactions;
    
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
    float i = 0.1;
    db.openDatabase("Database1K.txt");

    apriori(db, i);

    return 0;
}

// test
