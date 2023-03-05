#include <random>
#include <vector>
#include <string>
#include <ctime>

int genRandomInt () {
  //Random int generator for transaction amount
  std::default_random_engine re;
  re.seed(time(NULL));
  std::uniform_int_distribution<int> uni(5, 15);

  int randomNum = uni(re);

  return randomNum;
}

void populateVector(std::vector<std::string>& items) {
    for(int i = 0; i<100; i++) {
        items[i]="i" + std::to_string(i);
    }
}

std::string GenTransaction(int num, std::vector<std::string>& items) {
  std::string trans;
  std::default_random_engine re;
  re.seed(time(NULL));
  std::uniform_int_distribution<int> uni(1, 99);

  for (int i=0; i<num; i++) {
    int randomNum = uni(re);
    trans = trans + items[randomNum] + " ";
  }
  return trans;
}

void GenDatabase1K (std::vector<std::string>& items) {
  std::string output;
  //Create file
  //Open File
  for (int i = 0; i<1000; i++) {
    int transaction = genRandomInt();
    output = GenTransaction(transaction, items);
    //Write output to file
  }
  //Close File
}
void GenDatabase10K () {}
void GenDatabase50K () {}
void GenDatabase100K () {}


int main() {
  std::vector<std::string> items;

  //Create vector of "items"
  populateVector(items);

  //Create databases
  GenDatabase1K(items);
  GenDatabase10K();
  GenDatabase50K();
  GenDatabase100K();

  return 0;
}
