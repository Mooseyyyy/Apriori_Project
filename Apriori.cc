#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>

class Database {
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
        //Using sstream for contents copy
        std::string line, var;
        std::stringstream sline;
        std::vector<std::string> arr;
        while(std::getline(fin, temp)) {
           if (fin) {
            //COnvert the fstream to sstream
            sline << fin.rdbuf();
           }
           sline.str(temp);
           while (std::getline(sline, var, '')) {
            //Push each item into our vector of this transaction
            arr.push_back(var);
           }
           //Push the transaction into the vector
           transactions.push_back(arr);
           //Clear individual items from array to get next transaction
           arr.clear();
           sline.clear();
        }
        }

    // Outside vector contains the transactions,
    //Inner contains each item/
    std::vector<std::vector<std::string>> transactions;
    std::ifstream fin;
    
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
