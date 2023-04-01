#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include <regex>
#include <iomanip>

using namespace std;

// opens database file and formats it as a vector of strings
/* returns vector like this:
i67 i34 i0
i61 i91 i95 i42 i27
...
i11 i22 i33 i73
*/
vector<vector<int>> openDatabase(string dbfile)
{
  vector<vector<int>> db;
  vector<int> arr;
  string line;
  ifstream fin(dbfile);
  stringstream sline;

  // check file is open
  fin.is_open();
  if (!fin)
  {
    cout << "File can not be opened" << endl;
    exit(0);
  }

  // push all lines of db to a vector of
  while(std::getline(fin, line)) {
           if (fin) {
            //COnvert the fstream to sstream
            sline << fin.rdbuf();
           }
           sline.str(line);
           while (std::getline(sline, var, ' ')) {
            //Push each item into our vector of this transaction
              var.erase(0,1);
              int i = stoi(var);
              arr.push_back(i);
           }
           //Push the transaction into the vector
           transactions.push_back(arr);
           //Clear individual items from array to get next transaction
           arr.clear();
           sline.clear();
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

// gets all unique items from a vector of strings
// takes a vector of strings
/*returns vector of strings like this:
i13
i27
...
i90
*/
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
// 1-itemsets as a vector strings
/* returns a vector like this:
i10
i11
...
i99
 */
vector<string> genFreq1(vector<string> db, float ms, ofstream &outFile)
{
  int support = ms * db.size();
  cout << "Support: " << support << endl;

  // create itemset map
  vector<pair<string, int>> itemsets;

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
      // if item not already in map, place in map
      // if item already in map, increment frequency integer
      int indexInMap = -1;
      for (int i = 0; i < itemsets.size(); i++)
      {
        if (itemsets[i].first == item)
        {
          indexInMap = i;
        }
      }
      if (indexInMap == -1)
      {
        pair<string, int> newItem;
        newItem.first = item;
        newItem.second = 1;
        itemsets.push_back(newItem);
      }
      else
      {
        itemsets[indexInMap].second++;
      }
    }
  }

  // culls itemsets that do not meet the minimum support
  // displays frequent 1-itemsets and outputs to a file
  // loads frequent 1-itemsets into vector to return
  vector<string> L1;
  cout << "Frequent 1-itemsets: " << endl;
  outFile << "Frequent 1-itemsets: " << endl;
  for (int i = 0; i < itemsets.size(); i++)
  {
    if (itemsets[i].second >= support)
    {
      cout << itemsets[i].first << " | " << itemsets[i].second << endl;
      outFile << itemsets[i].first << " | " << itemsets[i].second << endl;
      L1.push_back(itemsets[i].first);
    }
  }
  outFile << endl;
  cout << "Scanned DB 1 time" << endl;
  outFile << "Scanned DB 1 time" << endl;
  outFile << endl;
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
vector<string> genCandidatesByJoin(vector<string> frequent_itemsets, int k)
{
  // create candidate vectors
  vector<string> candidate_itemsets;

  // get unique items in frequent_itemsets
  vector<string> unique_items = getUniqueItems(frequent_itemsets);

  // iterate through frequent itemsets
  for (auto &itemset : frequent_itemsets)
  {
    // iterate through unique items
    for (auto &item : unique_items)
    {
      // if unique item is not in string, find() will return string::npos
      /*
      if (itemset.find(item) == string::npos)
      {
        // create candidate and add to candidate_itemsets
        // duplicates in different order will exist
        string candidate = itemset + " " + item;
        candidate_itemsets.push_back(candidate);
      }
      */

      regex findExact("\\b" + item + "\\b");
      if (!regex_search(itemset, findExact))
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

  cout << "Candidate " << k << "-itemsets: " << endl;
  for (auto &itemset : candidate_itemsets)
  {
    cout << itemset << endl;
  }

  return candidate_itemsets;
}

// generates a vector of strings that is a frequent k-itemset
// from input vector Ck
/* returns vector like this (for L3):
i30 i34 i0
i61 i91 i95
...
i99 i22 i33
*/
vector<string> genFreqKByPrune(vector<string> db, vector<string> candidate_itemsets, float ms, int k, ofstream &outFile)
{
  int support = ms * db.size();
  cout << "Support: " << support << endl;

  // create itemset map
  vector<pair<string, int>> itemsets;

  int tranCheck = 0;
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
        // if item is not in string
        // if this if statement never evaluates true
        // then all stream items in the candidate string must be in itemset
        regex findExact("\\b" + item + "\\b");
        if (!regex_search(transaction, findExact))
        {
          founditemset = false;
          break;
        }
      }
      if (founditemset)
      {
        // cout << candidate << " found in: " << transaction << endl;
        // if item not already in map, place in map
        // if item already in map, increment frequency integer
        int indexInMap = -1;
        for (int i = 0; i < itemsets.size(); i++)
        {
          if (itemsets[i].first == candidate)
          {
            indexInMap = i;
          }
        }
        if (indexInMap == -1)
        {
          pair<string, int> newItem;
          newItem.first = candidate;
          newItem.second = 1;
          itemsets.push_back(newItem);
        }
        else
        {
          itemsets[indexInMap].second++;
        }
      }
    }
    tranCheck++;
    // Display statistics while running
    cout << "Round " << k << ": Checking Transaction: " << tranCheck << " / " << db.size() << endl;
  }

  // culls itemsets that do not meet the minimum support
  // displays frequent k-itemsets and outputs to a file
  // loads frequent k-itemsets into vector to return
  vector<string> Lk;
  cout << "Frequent " << k << "-itemsets:" << endl;
  outFile << "Frequent " << k << "-itemsets:" << endl;
  for (int i = 0; i < itemsets.size(); i++)
  {
    if (itemsets[i].second >= support)
    {
      cout << itemsets[i].first << " | " << itemsets[i].second << endl;
      outFile << itemsets[i].first << " | " << itemsets[i].second << endl;
      Lk.push_back(itemsets[i].first);
    }
  }
  outFile << endl;
  cout << "Scanned DB " << k << " times" << endl;
  outFile << "Scanned DB " << k << " times" << endl;
  outFile << endl;
  return Lk;
}

void apriori(vector<string> db, float ms)
{
  // initialize file
  string fileSize = to_string(db.size() / 1000);
  ofstream outFile;
  string fileName = "D" + fileSize + "K_Apriori_1.freq";
  outFile.open(fileName);
  outFile << "Apriori D" << fileSize << "K" << endl;
  outFile << endl;

  // get frequent 1-itemsets
  vector<string> Lk = genFreq1(db, ms, outFile);
  int k = 1;
  while (!Lk.empty())
  {
    k++;
    vector<string> Ck = genCandidatesByJoin(Lk, k);
    Lk = genFreqKByPrune(db, Ck, ms, k, outFile);
  }

  outFile.close();
}

int main()
{
  // Start timer
  time_t start, end;
  time(&start);
  ios_base::sync_with_stdio(false);

  vector<string> dbTest = openDatabase("DatabaseTest.txt");
  apriori(dbTest, 0.6);

  // vector<string> db1 = openDatabase("Database1K.txt");
  // apriori(db1, 0.01);

  // vector<string> db10 = openDatabase("Database10K.txt");
  // apriori(db10, 0.01);

  // vector<string> db50 = openDatabase("Database50K.txt");
  // apriori(db50, 0.01);

  // vector<string> db100 = openDatabase("Database100K.txt");
  // apriori(db100, 0.01);

  /*
  int main(int argc, char *argv[]) {
    switch (argc) {
      case 1:
        //If only executeable name is given
        cout << "Missing additional arguments" << endl;
        break;
      case 2:
        //If only exe name 1 value given
        cout << "Missing additional arguments" << endl;
        break;
      default:
        //Take name of database using
        std::string database_name = argv[1];
        float min_support = argv[2];
        vector<string> db1 = openDatabase(database_name);
        itemsets temp;

        //Output file code
        float min_support = atof(argv[2]);
        string output_name = database_name+"_Apriori_"+to_string(min_support)+".freq";
        ofstream output(output_name);

        float i = 0.1;

        apriori(db1, min_support);

    }
  */

  // Record end time
  time(&end);
  // Calculating time taken
  double time_taken = double(end - start);
  cout << "Execution Time: " << fixed
       << time_taken << setprecision(5);
  cout << "s " << endl;

  return 0;
}
