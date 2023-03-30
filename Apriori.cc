#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>

using namespace std;

// opens database file and formats it as a vector of strings
/* returns vector like this:
i67 i34 i0
i61 i91 i95 i42 i27
...
i11 i22 i33 i73
*/
vector<string> openDatabase(string dbfile)
{
  vector<string> db;
  string line;
  ifstream fin(dbfile);

  // check file is open
  fin.is_open();
  if (!fin)
  {
    cout << "File can not be opened" << endl;
    exit(0);
  }

  // push all lines of db to a vector of
  while (getline(fin, line))
  {
    db.push_back(line);
  }
  return db;
};

// sorts items in lexicographic order in itemset
// takes an itemset items as a string
// returns sorted string
string sortItems(string items)
{
  vector<int> itemlist;
  istringstream iss;
  iss.str(items);
  string item;
  while (iss >> item)
  {
    item.erase(0, 1);
    int i = stoi(item);
    itemlist.push_back(i);
  }
  sort(itemlist.begin(), itemlist.end());
  string sorted;
  int i = 0;
  for (auto &item : itemlist)
  {
    if (i == 0)
    {
      sorted += "i" + to_string(item);
      i++;
    }
    else
    {
      sorted += " i" + to_string(item);
    }
  }
  return sorted;
}

vector<string> getUniqueItems(vector<string> frequent_itemsets)
{
  // create itemset map
  map<string, int> unique_items;

  // iterate through vector of strings
  for (auto &itemset : frequent_itemsets)
  {
    // start string stream
    istringstream iss;
    iss.str(itemset);
    string item;

    //  iterate through string
    //  item is the current iXX value in the itemset
    while (iss >> item)
    {
      // place item in map if not already in map
      // if item already in map, increment frequency integer
      if (!unique_items.emplace(item, 1).second)
      {
        unique_items[item]++;
      }
    }
  }

  vector<string> UI;
  // display unique itemsets
  // load unique itemsets into vector to return
  // cout << "Unique Items: " << endl;
  for (auto it = unique_items.cbegin(); it != unique_items.cend(); ++it)
  {
    // cout << it->first << " | " << it->second << endl;
    UI.push_back(it->first);
  }
  return UI;
}

// takes database vector of strings and returns frequent
// 1-itemsets as a map of item string and count int
/* returns map like this:
i10 111
i11 120
...
i99 133
 */
vector<string> genFreq1(vector<string> db, float ms)
{
  int support = ms * db.size();
  cout << "Support: " << support << endl;

  // create itemset map
  map<string, int> itemsets;

  // iterate through vector of strings
  for (auto &transaction : db)
  {
    // start string stream
    istringstream iss;
    iss.str(transaction);
    string item;

    //  iterate through string
    //  item is the current iXX value in the transaction
    while (iss >> item)
    {
      // place item in map if not already in map
      // if item already in map, increment frequency integer
      if (!itemsets.emplace(item, 1).second)
      {
        itemsets[item]++;
      }
    }
  }

  // culls itemsets that do not meet the minimum support
  for (auto it = itemsets.cbegin(); it != itemsets.cend(); ++it)
  {
    if (it->second < support)
    {
      itemsets.erase(it->first);
    }
  }

  // display frequent 1-itemsets
  // load frequent 1-itemsets into vector to return
  vector<string> L1;
  cout << "Frequent 1-itemsets: " << endl;
  for (auto it = itemsets.cbegin(); it != itemsets.cend(); ++it)
  {
    cout << it->first << " | " << it->second << endl;
    L1.push_back(it->first);
  }
  return L1;
};

// generates a vector of strings that is a candidate k-itemset
// from input vector Lk-1
/* returns vector like this (for C3):
i30 i34 i0
i61 i91 i95
...
i99 i22 i33
*/
vector<string> genCandidatesByJoin(vector<string> frequent_itemsets)
{
  // create candidate vectors
  vector<string> candidate_itemsets;

  // use freq-1 function with 0 support to get all
  // unique items in frequent_itemsets
  vector<string> unique_items = getUniqueItems(frequent_itemsets);

  // iterate through frequent itemsets
  for (auto &itemset : frequent_itemsets)
  {
    // iterate through unique items
    for (auto &item : unique_items)
    {
      // if unique item is not in string, find() will return string::npos
      if (itemset.find(item) == string::npos)
      {
        // create candidate and add to candidate_itemsets
        // duplicates in different order will exist
        string candidate = itemset + " " + item;
        candidate_itemsets.push_back(candidate);
      }
    }
    // cout << endl;
  }

  // sort candidate itemsets
  for (auto &itemset : candidate_itemsets)
  {
    itemset = sortItems(itemset);
  }

  // delete duplicate candidate itemsets
  std::sort(candidate_itemsets.begin(), candidate_itemsets.end());
  auto it = std::unique(candidate_itemsets.begin(), candidate_itemsets.end());
  candidate_itemsets.erase(it, candidate_itemsets.end());

  // display candidate itemsets
  /*
  cout << "Candidate k-itemsets: " << endl;
  for (auto &itemset : candidate_itemsets)
  {
    cout << itemset << endl;
  }
  */
  return candidate_itemsets;
}

vector<string> genFreqKByPrune(vector<string> db, vector<string> candidate_itemsets, float ms, int k)
{
  int support = ms * db.size();

  // create itemset map
  map<string, int> frequent_itemsets;

  // iterate through vector of strings
  for (auto &transaction : db)
  {
    // iterate through candidate itemsets
    for (auto &candidate : candidate_itemsets)
    {
      bool founditemset = true;
      // start string stream
      istringstream iss;
      iss.str(candidate);
      string item;
      //  iterate through string
      //  item is the current iXX value in the candidate
      while (iss >> item)
      {
        // if item is not in string, find() will return string::npos
        // if this if statement never holds true
        // then all stream items in the candidate string must be in itemset
        if (transaction.find(item) == string::npos)
        {
          founditemset = false;
        }
      }
      if (founditemset)
      {
        // place item in map if not already in map
        // if item already in map, increment frequency integer
        if (!frequent_itemsets.emplace(candidate, 1).second)
        {
          frequent_itemsets[candidate]++;
        }
      }
    }
  }

  // culls itemsets that do not meet the minimum support
  for (auto it = frequent_itemsets.cbegin(); it != frequent_itemsets.cend(); ++it)
  {
    if (it->second < support)
    {
      frequent_itemsets.erase(it->first);
    }
  }

  // display frequent k-itemsets
  // load frequent k-itemsets into vector to return
  vector<string> Lk;
  cout << "Frequent " << k << "-itemsets" << endl;
  for (auto it = frequent_itemsets.cbegin(); it != frequent_itemsets.cend(); ++it)
  {
    cout << it->first << " | " << it->second << endl;
    Lk.push_back(it->first);
  }
  return Lk;
}

void apriori(vector<string> db, float ms)
{
  // get frequent 1-itemsets
  vector<string> Lk = genFreq1(db, ms);
  int k = 1;
  while (!Lk.empty())
  {
    k++;
    vector<string> Ck = genCandidatesByJoin(Lk);
    Lk = genFreqKByPrune(db, Ck, ms, k);
  }
}

int main()
{
  vector<string> db1 = openDatabase("Database1K.txt");
  apriori(db1, 0.1);
  vector<string> test = {"i1 i2 i3 i4 i5 i6", "i2 i3 i4 i5 i6 i7", "i1 i4 i5 i8", "i1 i4 i6 i9 i10", "i2 i4 i5 i10 i11"};
  apriori(test, 0.6);
  return 0;
}
