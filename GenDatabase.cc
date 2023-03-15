#include <random>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

int genRandomInt () {
  int randomNum = rand() % 5 + 11;
  return randomNum;
}


void populateVector(std::vector<std::string>& items) {
    for(int i = 0; i<100; i++) {
        std::string temp;
        temp="i" + std::to_string(i);
        items.push_back(temp);
    }
}

std::string GenTransaction(int num, std::vector<std::string>& items) {
  std::string trans;

  for (int i=0; i<num; i++) {
    int randomNum = rand() % 100;
    trans = trans + items[randomNum] + " ";
  }
  return trans;
}

void GenDatabase1K (std::vector<std::string>& items) {
  std::string output;
  std::ofstream Database1K ("Database1K.txt");
  for (int i = 0; i<1000; i++) {
    int transaction = genRandomInt();
    output = GenTransaction(transaction, items);
    Database1K << output << std::endl;
  }
  Database1K.close();
}

void GenDatabase10K (std::vector<std::string>& items) {
  std::string output;
  std::ofstream Database10K ("Database10K.txt");
  for (int i = 0; i<10000; i++) {
    int transaction = genRandomInt();
    output = GenTransaction(transaction, items);
    Database10K << output << std::endl;
  }
  Database10K.close();
}

void GenDatabase50K (std::vector<std::string>& items) {
  std::string output;
  std::ofstream Database50K ("Database50K.txt");
  for (int i = 0; i<50000; i++) {
    int transaction = genRandomInt();
    output = GenTransaction(transaction, items);
    Database50K << output << std::endl;
  }
  Database50K.close();
}
void GenDatabase100K (std::vector<std::string>& items) {
  std::string output;
  std::ofstream Database100K ("Database100K.txt");
  for (int i = 0; i<100000; i++) {
    int transaction = genRandomInt();
    output = GenTransaction(transaction, items);
    Database100K << output << std::endl;
  }
  Database100K.close();
}


int main() {
  std::vector<std::string> items;

  //Create vector of "items"
  populateVector(items);

  //Create databases
  GenDatabase1K(items);
  GenDatabase10K(items);
  GenDatabase50K(items);
  GenDatabase100K(items);


  return 0;
}
