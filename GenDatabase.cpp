#include "GenDatabase.h"
#include <random>
#include <vector>
#include <string>

void GenDatabase1K () {
  std::string output;
  //Create file
  //Open File
  for (int i = 0; i<1000; i++) {
    int transaction = genRandomInt();
    output = GenTransaction(transaction); 
    //Write output to file
  }
  //Close File
}
void GenDatabase10K () {}
void GenDatabase50K () {}
void GenDatabase100K () {}

int genRandomInt () {
  //Random int generator for transaction amount
  std::default_random_engine re;
  re.seed(time(NULL)); 
  std::uniform_int_distribution<int> uni(5, 15);

  int randomNum = uni(re);
  
  return randomNum;
}

void populateVector(vector<std::string>& items) {
    for(int i = 0; i<100; i++) {
        items[i]="i" + std::to_string(i);
    }
}

string GenTransaction(int num) {
  std::string trans;
  std::default_random_engine re;
  re.seed(time(NULL)); 
  std::uniform_int_distribution<int> uni(1, 99);

  for (int i=0; i<num; i++) {
    int randomNum = uni(re);
    trans = trans + items[randomNum] + " ";
  }
  return trans
}


int main() {
  std::vector<std::string> items;

  //Create vector of "items"
  populateVector(vector items);

  //Create databases
  GenDatabase1K();
  GenDatabase10K();
  GenDatabase50K();
  GenDatabase100K();

  return 0;
}

