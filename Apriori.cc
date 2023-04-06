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
#include <bitset>

using namespace std;

string DATABASE_FILE;
const int NUM_TRANSACTIONS = 1000;
float MINIMUM_SUPPORT;

void readDatabase(map<set<string>, bitset<NUM_TRANSACTIONS>> &candidates, time_t &start)
{
  string line;
  ifstream fileIn(DATABASE_FILE);
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
        set<string> itemset;
        itemset.emplace(item);
        candidates[itemset].set(transaction_ID);
      }
      transaction_ID++;
    }
    fileIn.close();

    cout << "Finish reading database in " << time(NULL) - start << "s" << endl;
    cout << "Number of transactions: " << NUM_TRANSACTIONS << endl;
  }
  else
  {
    cout << "File can not be opened" << endl;
    exit(0);
  }
}

void printFrequentItemsets(const map<set<string>, bitset<NUM_TRANSACTIONS>> Lk_k, const int &k, const int &start, ofstream& Database)
{
  // Print frequent itemsets
  cout << endl;
  Database << endl;
  cout << "Frequent " << k << "-itemsets" << endl;
  Database << "Frequent " << k << "-itemsets" << endl;
  cout << "-----------------" << endl;
  Database << "-----------------" << endl;
  cout << "Number of frequent " << k << "-itemsets: " << Lk_k.size() << endl;
  Database << "Number of frequent " << k << "-itemsets: " << Lk_k.size() << endl;
  cout << "Execution time: " << time(NULL) - start << "s" << endl;
  Database << "Execution time: " << time(NULL) - start << "s" << endl;
  cout << "-----------------" << endl;
  Database << "-----------------" << endl;
  for (auto it = Lk_k.begin(); it != Lk_k.end(); it++)
  {
    for (auto it2 = it->first.begin(); it2 != it->first.end(); it2++)
    {
      cout << *it2 << " ";
      Database << *it2 << " ";
    }
    cout << "Support: " << it->second.count() << "/" << NUM_TRANSACTIONS << " = " << it->second.count() / (float)NUM_TRANSACTIONS << endl;
    Database << "Support: " << it->second.count() << "/" << NUM_TRANSACTIONS << " = " << it->second.count() / (float)NUM_TRANSACTIONS << endl;
  }
}

void apriori(map<set<string>, bitset<NUM_TRANSACTIONS>> &candidates, time_t &start, ofstream& Database)
{
  // Vector Lk contains all frequent itemsets
  // First item in vector Lk is a map of all the frequent 1-itemsets
  // Second item in vector Lk is a map of all the frequent 2-itemsets
  // Third item in vector Lk is a map of all the frequent 3-itemsets
  // etc.
  vector<map<set<string>, bitset<NUM_TRANSACTIONS>>> Lk;
  int k = 1;

  do
  {
    // Generate candidates
    if (k > 1)
    {
      map<set<string>, bitset<NUM_TRANSACTIONS>> old_candidates = candidates;
      candidates.clear();
      for (auto it = old_candidates.begin(); it != old_candidates.end(); it++)
      {
        for (auto it2 = next(it); it2 != old_candidates.end(); it2++)
        {
          set<string> new_itemset;
          set_union(it->first.begin(), it->first.end(), it2->first.begin(), it2->first.end(), inserter(new_itemset, new_itemset.begin()));
          if (int(new_itemset.size()) == k)
          {
            candidates[new_itemset] = it->second & it2->second;
          }
        }
      }
    }

    // Prune candidates
    for (auto it = candidates.begin(); it != candidates.end();)
    {
      if (it->second.count() < MINIMUM_SUPPORT * NUM_TRANSACTIONS)
      {
        it = candidates.erase(it);
      }
      else
      {
        it++;
      }
    }

    // Add candidates to Lk
    if (!candidates.empty())
    {
      Lk.push_back(candidates);
      printFrequentItemsets(Lk[k - 1], k, start, Database);
    }
    k++;
  } while (!candidates.empty());
}

int main(int argc, char *argv[])
{
  // Start timer
  time_t start, end;
  time(&start);
  ios_base::sync_with_stdio(false);

  std::string database_name = argv[1];
  MINIMUM_SUPPORT = atof(argv[2]);
  // string temp_db_name=;
  DATABASE_FILE = database_name;
  std::string temp_name = database_name+"_Apriori_"+to_string(MINIMUM_SUPPORT)+".freq";
  ofstream Database(DATABASE_FILE);

  switch (argc)
  {
  case 1:
      //If only executeable name is given
    cout << "Missing additional arguments" << endl;
    break;
  case 2:
      //If only exe name 1 value given
    cout << "Missing additional arguments" << endl;
    break;
  default:
    map<set<string>, bitset<NUM_TRANSACTIONS>> candidates;
    readDatabase(candidates, start);
    apriori(candidates, start, Database);
    break;

}
  // Record end time
  time(&end);
  // Calculating time taken
  double time_taken = double(end - start);
  cout << endl
       << "Time taken by program is: " << fixed
       << time_taken << setprecision(5) << "s " << endl;
 Database << endl
      << "Time taken by program is: " << fixed
      << time_taken << setprecision(5) << "s " << endl;

  Database.close();
  return 0;
}
