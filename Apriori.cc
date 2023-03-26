#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <utility>
#include <tuple>

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
    int parse = 1;

};

class itemsets {
private:
  int calSup;
  //Control is there are frequent itemsets
  bool cont;
public:
    itemsets() {}
    std::vector<std::vector<std::string>> generateL() {
      std::vector<std::vector<std::string>> returned;
      return returned;
    }


    int getSupport(Database &db) {
      if (db.parse == 1) {
          return calSup * db.transactions.size();
      }
    }

    std::vector<std::string> getElements(Database &db) {
      std::vector<std::string> elements;
      for(int i = 0; i < db.transactions.size(); i++) {
        elements.push_back(db.transactions[i][i]);
      }
      return elements;
    }

    std::vector<std::tuple<std::string, int>> generateC(Database &db, int support) {
      std::vector<std::tuple<std::string, int>> candidate;
      std::vector<std::string> elements = getElements(db);
      if (db.parse == 1) {
        for (int i = 0; i<elements.size(); i++) {
          int cnt = count(elements.begin(), elements.end(), elements.at(i));
          if(cnt>=support) {
            std::tuple <std::string, int> freq = make_tuple(elements.at(i), cnt);
            candidate.push_back(freq);
          }
      }
      return candidate;
    }
      //Will need to return joined C list
    }

    void apriori(Database &db, float ms) {
        calSup = ms;
        int support;
        while (cont) {
        support = getSupport(db);
        itemsets test;
      }
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
