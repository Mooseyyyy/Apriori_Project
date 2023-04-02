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

class Itemset
{
public:
  vector<int> transaction;
  int frequency;
  Itemset()
  {
    frequency = 1;
  }
};

vector<Itemset> openDatabase(string dbFile)
{
  vector<Itemset> transactions;
  string line;
  ifstream fileIn(dbFile);
  if (fileIn.is_open())
  {
    while (getline(fileIn, line))
    {
      Itemset itemSet;
      vector<int> items;
      istringstream iss;
      iss.str(line);
      string iXX;
      while (iss >> iXX)
      {
        iXX.erase(0, 1);
        items.push_back(stoi(iXX));
      }
      // cout << line << endl;
      itemSet.transaction = items;
      transactions.push_back(itemSet);
    }
    fileIn.close();
  }
    else
    {
    cout << "File can not be opened" << endl;
    exit(0);
    }
    return transactions;
};

void outputItemsets(vector<Itemset> itemSet, string message, int round)
{
    cout << "Scanned Database " << round << " Time(s)" << endl;
    cout << message << endl;
    for (int i = 0; i < itemSet.size(); i++)
    {
    for (int j = 0; j < itemSet[i].transaction.size(); j++)
    {
      cout << "i" << itemSet[i].transaction[j] << " ";
    }
    cout << "| " << itemSet[i].frequency << endl;
    }
}

vector<Itemset> checkSupport(vector<Itemset> itemSet, int support)
{
    cout << "Support: " << support << endl;
    for (int i = 0; i < itemSet.size(); i++)
    {
    if (itemSet[i].frequency < support)
    {
      itemSet.erase(itemSet.begin() + i);
      i--;
    }
    }
    return itemSet;
}

vector<Itemset> genFrequent1(vector<Itemset> db, float ms)
{
  int support = ms * db.size();
  vector<Itemset> L1;
  // iterate through database
  for (int i = 0; i < db.size(); i++)
  {
    // iterate through each transaction in database
    for (int j = 0; j < db[i].transaction.size(); j++)
    {
      int indexInItemset = -1;
      // iterate through L1
      for (int k = 0; k < L1.size(); k++)
      {
        // iterate through each transaction in L1
        for (int l = 0; l < L1[k].transaction.size(); l++)
          // if true then item already in L1
          if (L1[k].transaction[l] == db[i].transaction[j])
          {
            indexInItemset = k;
          }
      }
      // if not already in itemset, place in new itemset
      // if item already in itemset, increment frequency integer
      if (indexInItemset == -1)
      {
        Itemset newItemset;
        newItemset.transaction.push_back(db[i].transaction[j]);
        L1.push_back(newItemset);
      }
      else
      {
        L1[indexInItemset].frequency++;
      }
    }
  }
  L1 = checkSupport(L1, support);
  outputItemsets(L1, "Frequent 1-Itemsets:", 1);
  return L1;
}

vector<int> genUniqueItemsets(vector<Itemset> fi)
{
  vector<int> L1;
  // iterate through database
  for (int i = 0; i < fi.size(); i++)
  {
    // iterate through each transaction in database
    for (int j = 0; j < fi[i].transaction.size(); j++)
    {
      // if not already in itemset, place in new itemset
      // if item already in itemset, increment frequency integer
      int indexInItemset = -1;
      for (int k = 0; k < L1.size(); k++)
      {
        if (L1[k] == fi[i].transaction[j])
        {
          indexInItemset = k;
        }
      }
      if (indexInItemset == -1)
      {
        L1.push_back(fi[i].transaction[j]);
      }
    }
  }
  return L1;
};

vector<Itemset> genCandidateItemsets(vector<Itemset> frequentItemsets, int round)
{
  string outNum = to_string(round);
  string out = "Candidate " + outNum + "-Itemsets";
  vector<Itemset> Ck;
  vector<Itemset> Lkp = frequentItemsets;
  vector<int> uniqueItems = genUniqueItemsets(frequentItemsets);
  for (int i = 0; i < Lkp.size(); i++)
  {
    cout << "Round " << round << ": Finding Candidates: " << i + 1 << "/" << Lkp.size() << endl;
    for (int k = 0; k < uniqueItems.size(); k++)
    {
      // unique item not found in Lkp[i].transaction
      // create candidate transaction
      if (find(Lkp[i].transaction.begin(), Lkp[i].transaction.end(), uniqueItems[k]) == Lkp[i].transaction.end())
      {
        vector<int> trans = Lkp[i].transaction;
        trans.push_back(uniqueItems[k]);
        Itemset candidate;
        candidate.transaction = trans;
        sort(candidate.transaction.begin(), candidate.transaction.end());
        // if candidate transaction not already in Ck, push back
        int indexInItemset = false;
        for (int k = 0; k < Ck.size(); k++)
        {
          // if true then item already in Ck
          if (Ck[k].transaction == candidate.transaction)
          {
            indexInItemset = true;
          }
        }
        if (!indexInItemset)
        {
          Ck.push_back(candidate);
        }
      }
    }
  }
  // outputItemsets(Ck, out, 0);
  return Ck;
}

vector<Itemset> genFrequentK(vector<Itemset> db, vector<Itemset> candidates, float ms, int round)
{
  string outNum = to_string(round);
  string out = "Frequent " + outNum + "-Itemsets";
  vector<Itemset> Lk;
  int support = ms * db.size();
  // iterate through database
  for (int i = 0; i < db.size(); i++)
  {
    cout << "Round" << round << ": Checking transaction: " << i + 1 << "/" << db.size() << endl;
    // iterate through candidates
    for (int j = 0; j < candidates.size(); j++)
    {
      bool foundItemset = true;
      // iterate through candidate itemset
      for (int k = 0; k < candidates[j].transaction.size(); k++)
      {
        // if this if statement never evaluates true
        // then all items in the candidate must be in the db transaction
        if (find(db[i].transaction.begin(), db[i].transaction.end(), candidates[j].transaction[k]) == db[i].transaction.end())
        {
          foundItemset = false;
          break;
        }
      }
      if (foundItemset)
      { //  if not already in itemset, place in new itemset
        //  if item already in itemset, increment frequency integer
        int indexInItemset = -1;
        for (int k = 0; k < Lk.size(); k++)
        {
          // if true then item already in Lk
          if (Lk[k].transaction == candidates[j].transaction)
          {
            indexInItemset = k;
          }
        }
          if (indexInItemset == -1)
          {
          Itemset newItemset;
          newItemset.transaction = candidates[j].transaction;
          Lk.push_back(newItemset);
          }
          else
          {
          Lk[indexInItemset].frequency++;
          }
      }
    }
  }
  Lk = checkSupport(Lk, support);
  outputItemsets(Lk, out, round);
  return Lk;
}

void apriori(vector<Itemset> db, float ms)
{
  vector<Itemset> Lk = genFrequent1(db, ms);
  int k = 1;
  while (!Lk.empty())
  {
    k++;
    vector<Itemset> Ck = genCandidateItemsets(Lk, k);
    Lk = genFrequentK(db, Ck, ms, k);
  }
}

int main()
{
  // Start timer
  time_t start, end;
  time(&start);
  ios_base::sync_with_stdio(false);

  // vector<Itemset> db = openDatabase("DatabaseTest.txt");
  // apriori(db, 0.6);

  vector<Itemset> db = openDatabase("Database1K.txt");
  apriori(db, 0.01);

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
