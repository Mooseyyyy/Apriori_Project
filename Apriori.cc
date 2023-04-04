#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <iterator>
#include <vector>
#include <iomanip>

using namespace std;

const string DATABASE_FILE = "Database1k.txt";
const float MINIMUM_SUPPORT = 0.01;

void readDatabase(string dbFile, map<string, set<int>> &transactions, int &num_transactions, time_t &start)
{
  string line;
  ifstream fileIn(dbFile);
  if (fileIn.is_open())
  {
    int transaction_ID = 0;
    while (getline(fileIn, line))
    {
      istringstream iss;
      iss.str(line);
      string item;
      while (iss >> item)
      {
        transactions[item].emplace(transaction_ID);
      }
      transaction_ID++;
    }
    fileIn.close();

    num_transactions = transaction_ID;
    cout << "Finish reading database in " << time(NULL) - start << "s" << endl;
    cout << "Number of transactions: " << num_transactions << endl;
  }
  else
  {
    cout << "File can not be opened" << endl;
    exit(0);
  }
}

void printFrequentItemsets(const vector<map<set<string>, set<int>>> &Lk, int num_transactions)
{
  // Print frequent itemsets
  for (int i = 0; i < int(Lk.size()); i++)
  {
    for (auto it = Lk[i].begin(); it != Lk[i].end(); it++)
    {
      cout << "Frequent " << i + 1 << "-itemset: ";
      for (auto it2 = it->first.begin(); it2 != it->first.end(); it2++)
      {
        cout << *it2 << " ";
      }
      cout << "Support: " << it->second.size() << "/" << num_transactions << endl;
    }
  }
}

void apriori(map<string, set<int>> &db, float ms, int num_transactions, time_t &start)
{
  // Contains all frequent itemsets
  // First item in vector Lk is a map of all the frequent 1-itemsets
  // Second item in vector Lk is a map of all the frequent 2-itemsets
  // Third item in vector Lk is a map of all the frequent 3-itemsets
  // etc.
  vector<map<set<string>, set<int>>> Lk;
  map<set<string>, set<int>> current;

  // Generate frequent 1-itemsets
  for (auto it = db.begin(); it != db.end(); it++)
  {
    // If the item is frequent, add it to the map
    if (it->second.size() >= ms * db.size())
    {
      set<string> item;
      item.emplace(it->first);
      current[item] = it->second;
    }
  }

  // Generate frequent k-itemsets
  int k = 2;
  do
  {

    // Add the map of frequent k-itemsets to the vector Lk
    Lk.push_back(current);
    current.clear();
    cout << "Found " << Lk[k - 2].size() << " Frequent " << k - 1 << "-itemsets in " << time(NULL) - start << "s" << endl;

    set<set<string>> previous_items;

    for (auto it = Lk[k - 2].begin(); it != Lk[k - 2].end(); it++)
    {
      previous_items.emplace(it->first);
    }

    set<set<string>> candidates;
    // Generate candidates
    for (auto it = previous_items.begin(); it != previous_items.end(); it++)
    {
      for (auto it2 = previous_items.begin(); it2 != previous_items.end(); it2++)
      {
        if (it != it2)
        {
          set<string> candidate;
          set_union(it->begin(), it->end(), it2->begin(), it2->end(), inserter(candidate, candidate.begin()));
          if (int(candidate.size()) == k)
          {
            candidates.emplace(candidate);
          }
        }
      }
    }

    // Check if candidates are frequent
    for (auto it = candidates.begin(); it != candidates.end(); it++)
    {
      set<int> candidate_transactions;
      for (auto it2 = it->begin(); it2 != it->end(); it2++)
      {
        if (candidate_transactions.empty())
        {
          candidate_transactions = db[*it2];
        }
        else
        {
          set<int> temp;
          set_intersection(candidate_transactions.begin(), candidate_transactions.end(), db[*it2].begin(), db[*it2].end(), inserter(temp, temp.begin()));
          candidate_transactions = temp;
        }
      }
      if (candidate_transactions.size() >= ms * db.size())
      {
        current[*it] = candidate_transactions;
      }
    }

    k++;

  } while (current.size() > 0);

  printFrequentItemsets(Lk, num_transactions);
}

int main()
{
  // Start timer
  time_t start, end;
  time(&start);
  ios_base::sync_with_stdio(false);

  int num_transactions = 0;
  map<string, set<int>> db;
  readDatabase(DATABASE_FILE, db, num_transactions, start);
  apriori(db, MINIMUM_SUPPORT, num_transactions, start);

  // Record end time
  time(&end);
  // Calculating time taken
  double time_taken = double(end - start);
  cout << "Execution Time: " << fixed
       << time_taken << setprecision(5);
  cout << "s " << endl;

  return 0;
}
