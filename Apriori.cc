#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>

class Database {
public:
    Database() {}
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
        while(std::getline(fin, line)) {
           if (fin) {
            //COnvert the fstream to sstream
            sline << fin.rdbuf();
           }
           sline.str(line);
           while (std::getline(sline, var, ' ')) {
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
private:
  std::vector<std::vector<std::string>> C, L;
  int calSup;
public:
    itemsets() {}
    std::vector<std::vector<std::string>> generateL() {
      std::vector<std::vector<std::string>> returned;
      return returned;
    }

/*
    int getSupport(Database &db) {
      int ret;
      for(auto&row:db.transactions) {
        int i, j;
        if(row.size() < item.size()) continue;
        for
      }
      return calSup * db.transactions.size();
    }
    */

    void apriori(Database &db, float ms) {
        calSup = ms;
        // getSupport(db);
        itemsets test;
    }
};



int main() {
    Database db1, db10, db50, db100;
    itemsets temp;
    float i = 0.1;
    db1.openDatabase("Database1K.txt");
    db10.openDatabase("Database10K.txt");
    db50.openDatabase("Database50K.txt");
    db100.openDatabase("Database100K.txt");

    temp.apriori(db1, i);
    temp.apriori(db10, i);
    temp.apriori(db50, i);
    temp.apriori(db100, i);

    return 0;
}
