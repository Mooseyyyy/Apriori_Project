#include "GenDatabase.h"

Database::Database (int transaction) {}

void populateVector() {
    for(int i = 0; i<100; i++) {
        items[i]="i" + std::to_string(i);
    }
}

void GenTransaction(int num) {}


int main() {
  std::vector<Item> items;
  populateVector();
}

