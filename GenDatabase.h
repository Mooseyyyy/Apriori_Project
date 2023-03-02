#include <vector>
#include <string>

class Database {
public:
    Database();
    Database(int trans);
};

class Transaction {
    Transaction();
    GenTransaction(int items);
};

class Item {
public:
    Item() {}

    Item(int num) {}

    std::string name;
};