#include "GenDatabase.h"
#include <random>
#include <vector>
#include <string>

void GenDatabase1K () {}
void GenDatabase10K () {}
void GenDatabase50K () {}
void GenDatabase100K () {}

void populateVector(vector<std::string>& items) {
    for(int i = 0; i<100; i++) {
        items[i]="i" + std::to_string(i);
    }
}

string GenTransaction(int num) {

  return trans
}


int main() {
  std::vector<std::string> items;

//Random int generator for transaction amount
  std::default_random_engine re;
  re.seed(time(NULL)); 
  std::uniform_int_distribution<int> uni(5, 15);

  int randomNum = uni(re);

  populateVector(vector items);

}

