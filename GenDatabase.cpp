#include "GenDatabase.h"

populateVector() {
    for(int i = 0; i<100; i++) {
        items[i]=Item(i);
    }
}

GenTransaction(int num) {}

Item::Item(int num) {
    Item.name = "i" + std::to_string(num);
}

int main() {
  std::vector<Item> items;
  populateVector();
}

