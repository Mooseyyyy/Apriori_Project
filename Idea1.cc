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
#include <stack>

using namespace std;

const int MAX_TRANSACTIONS = 100000;
string DATABASE_FILE;
float MINIMUM_SUPPORT = 0.01;

void readDatabase(stack<pair<set<string>, bitset<MAX_TRANSACTIONS>>> &candidates, int &num_transactions, time_t &start)
{
  string line;
  ifstream fileIn(DATABASE_FILE);
  if (fileIn.is_open())
  {
    int transaction_ID = 0;
    map<set<string>, bitset<MAX_TRANSACTIONS>> candidates_set;
    while (getline(fileIn, line))
    {
      istringstream iss;
      iss.str(line);
      string item;
      while (iss >> item)
      {
        set<string> itemset;
        itemset.emplace(item);
        candidates_set[itemset].set(transaction_ID);
      }
      transaction_ID++;
    }
    num_transactions = transaction_ID;
    fileIn.close();

    for (auto it = candidates_set.begin(); it != candidates_set.end(); it++)
    {
      candidates.push(pair<set<string>, bitset<MAX_TRANSACTIONS>>(it->first, it->second));
    }

    cout << "Finish reading database in " << time(NULL) - start << "s" << endl;
    cout << "Number of transactions: " << num_transactions << endl;
  }
  else
  {
    cout << "File can not be opened" << endl;
    exit(0);
  }
}

/*
void printFrequentItemsets(const map<set<string>, bitset<MAX_TRANSACTIONS>> Lk_k, const int &k, const int &num_transactions, const int &start, ofstream &Database)
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
    cout << "Support: " << it->second.count() << "/" << num_transactions << " = " << it->second.count() / (float)num_transactions << endl;
    Database << "Support: " << it->second.count() << "/" << num_transactions << " = " << it->second.count() / (float)num_transactions << endl;
  }
}
*/

int apriori(stack<pair<set<string>, bitset<MAX_TRANSACTIONS>>> &candidates, const int &num_transactions, time_t &start, ofstream &Database)
{
  // Vector Lk contains all frequent itemsets
  // First item in vector Lk is a map of all the frequent 1-itemsets
  // Second item in vector Lk is a map of all the frequent 2-itemsets
  // Third item in vector Lk is a map of all the frequent 3-itemsets
  // etc.
  vector<map<set<string>, bitset<MAX_TRANSACTIONS>>> Lk_temp;
  vector<map<set<string>, bitset<MAX_TRANSACTIONS>>> Lk_final;
  Lk_temp.assign(100, map<set<string>, bitset<MAX_TRANSACTIONS>>());
  Lk_final.assign(100, map<set<string>, bitset<MAX_TRANSACTIONS>>());
  pair<set<string>, bitset<MAX_TRANSACTIONS>> big_itemset;
  int k = 1;
  bool found_one = false;

  while (!candidates.empty())
  {
    // Get the next candidate
    pair<set<string>, bitset<MAX_TRANSACTIONS>> candidate = candidates.top();
    candidates.pop();

    // If the candidate is frequent, add it to Lk
    if (candidate.second.count() >= MINIMUM_SUPPORT * num_transactions)
    {
      Lk_temp[candidate.first.size() - 1][candidate.first] = candidate.second;
      pair<set<string>, bitset<MAX_TRANSACTIONS>> big_itemset = candidate;
      found_one = true;
      break;
    }
  }

  if (found_one)
  {
    while (!candidates.empty())
    {
      // Get the next candidate
      pair<set<string>, bitset<MAX_TRANSACTIONS>> candidate = candidates.top();
      candidates.pop();

      // If the candidate is frequent, add it to Lk
      if (candidate.second.count() >= MINIMUM_SUPPORT * num_transactions)
      {
        Lk_temp[candidate.first.size() - 1][candidate.first] = candidate.second;

        pair<set<string>, bitset<MAX_TRANSACTIONS>> combined;
        set_union(candidate.first.begin(), candidate.first.end(), big_itemset.first.begin(), big_itemset.first.end(), inserter(combined.first, combined.first.begin()));
        combined.second = candidate.second & big_itemset.second;
        if (combined.second.count() >= MINIMUM_SUPPORT * num_transactions)
        {
          Lk_temp[combined.first.size() - 1][combined.first] = combined.second;
          big_itemset = combined;
        }
      }
    }
    while (!Lk_temp.empty())
    {
      // Largest frequent itemset is now the bi_itemset
      for (auto it = Lk_temp.rbegin(); it != Lk_temp.rend(); it++)
      {
        if (!it->empty())
        {
          big_itemset = pair<set<string>, bitset<MAX_TRANSACTIONS>>(it->begin()->first, it->begin()->second);
          break;
        }
      }

      // Push from Lk_temp to candidates stack
      for (auto it = Lk_temp.rbegin(); it != Lk_temp.rend(); it++)
      {
        if (!it->empty())
        {
          candidates.push(pair<set<string>, bitset<MAX_TRANSACTIONS>>(it->begin()->first, it->begin()->second));
        }
      }
      k++;

      while (!candidates.empty())
      {
        // Get the next candidate
        pair<set<string>, bitset<MAX_TRANSACTIONS>> candidate = candidates.top();
        candidates.pop();

        // If the candidate is frequent, add it to Lk
        if (candidate.second.count() >= MINIMUM_SUPPORT * num_transactions)
        {
          Lk_temp[candidate.first.size() - 1][candidate.first] = candidate.second;

          pair<set<string>, bitset<MAX_TRANSACTIONS>> combined;
          set_union(candidate.first.begin(), candidate.first.end(), big_itemset.first.begin(), big_itemset.first.end(), inserter(combined.first, combined.first.begin()));
          combined.second = candidate.second & big_itemset.second;
          if (combined.second.count() >= MINIMUM_SUPPORT * num_transactions)
          {
            Lk_temp[combined.first.size() - 1][combined.first] = combined.second;
            big_itemset = combined;
          }
        }
      }
      Lk_final[big_itemset.first.size() - 1][big_itemset.first] = big_itemset.second;
      Lk_temp[big_itemset.first.size() - 1].erase(big_itemset.first);
    }

    // Find all subsets of the frequent itemsets in L_k and insert them Lk_final
    for (auto it = Lk_final.rbegin(); it != Lk_final.rend(); it++)
    {
      for (auto it2 = it->begin(); it2 != it->end(); it2++)
      {
        for (int i = 0; i < it2->first.size(); i++)
        {
          set<string> subset;
          set_difference(it2->first.begin(), it2->first.end(), next(it2->first.begin(), i), next(it2->first.begin(), i + 1), inserter(subset, subset.begin()));
          //
          if (Lk_final[subset.size() - 1].find(subset) == Lk_final[subset.size() - 1].end())
          {
            Lk_final[subset.size() - 1][subset] = it2->second;
          }
        }
      }
    }

    for (auto it = Lk_final.begin(); it != Lk_final.end(); it++)
    {
      for (auto it2 = it->begin(); it2 != it->end(); it2++)
      {
        cout << "Itemset: ";
        Database << "Itemset: ";
        for (auto it3 = it2->first.begin(); it3 != it2->first.end(); it3++)
        {
          cout << *it3 << " ";
          Database << *it3 << " ";
        }
        cout << "Support: " << it2->second.count() << "/" << num_transactions << " = " << it2->second.count() / (float)num_transactions << endl;
        Database << "Support: " << it2->second.count() << "/" << num_transactions << " = " << it2->second.count() / (float)num_transactions << endl;
      }
    }
  }

  return k;
}

int main(int argc, char *argv[])
{
  // Start timer
  time_t start, end;
  time(&start);
  ios_base::sync_with_stdio(false);

  DATABASE_FILE = argv[1];
  MINIMUM_SUPPORT = atof(argv[2]);

  // setup for output filename
  unsigned first = DATABASE_FILE.find("e");
  first++;
  unsigned last = DATABASE_FILE.find(".");
  string outSize = DATABASE_FILE.substr(first, last - first);
  string outUnroundedSupport = to_string(MINIMUM_SUPPORT);
  string outSupport = outUnroundedSupport.substr(2, 2);
  string output_name = "D" + outSize + "_Idea1_" + outSupport + ".freq";
  ofstream Database(output_name);
  int scanCount;

  switch (argc)
  {
  case 1:
    // If only executeable name is given
    cout << "Missing additional arguments" << endl;
    break;
  case 2:
    // If only exe name 1 value given
    cout << "Missing additional arguments" << endl;
    break;
  default:
    stack<pair<set<string>, bitset<MAX_TRANSACTIONS>>> candidates;
    int num_transactions = 0;
    readDatabase(candidates, num_transactions, start);
    scanCount = apriori(candidates, num_transactions, start, Database);
    break;
  }

  // Record end time
  time(&end);
  // Calculating time taken
  double time_taken = double(end - start);
  cout << endl
       << "The frequent itemsets are stored in " << output_name
       << ", under ms = " << outUnroundedSupport.substr(0, 4)
       << "." << endl
       << "The time spent is " << fixed
       << time_taken << setprecision(5)
       << "s, to get the frequent itemsets." << endl
       << "The number of times scanning the database is " << scanCount
       << ", to get the frequent itemsets." << endl;
  Database << endl
           << "The frequent itemsets are stored in " << output_name
           << ", under ms = " << outUnroundedSupport.substr(0, 4)
           << "." << endl
           << "The time spent is " << fixed
           << time_taken << setprecision(5)
           << "s, to get the frequent itemsets." << endl
           << "The number of times scanning the database is " << scanCount
           << ", to get the frequent itemsets." << endl;
  return 0;
}
