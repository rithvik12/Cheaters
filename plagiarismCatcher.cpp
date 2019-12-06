#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <deque>
#include <list>
#include <cmath>

using namespace std;



/*function... might want it in some class?*/
int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

class Hash
{
    int NUMENTRIES;

    //int primes[25] = {97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,73,79,83,89,53,59,61,67,71};

public:
    list<int> *table;
    Hash(int size);
    void insertItem(string s, int fileIndex);         //add key to hash table
    void deleteItem(int key);       //delete key from hash table
    int hashFunction(string s);        //function for hash mapping
    void displayHashTable();
    int size(){
        return NUMENTRIES;
    }
};

//Hash Table Constructor
Hash::Hash(int size)
{
    this->NUMENTRIES = size;
    table = new list<int>[NUMENTRIES];
}

//Add key (file index) to hash table based on chunk
void Hash::insertItem(string s, int fileIndex) {
    int index = hashFunction(s);
    table[index].push_back(fileIndex);
}

//Hash function: multiply each character by a different prime number, then mod by table size
int Hash::hashFunction(string s) {
    int value = 0;
    for (int i = 0; i < s.length(); i++)
    {
        //value = value + (s[i] * (137^i));
        /*
        if(s[i] >= '0'){
            if(s[i]<= '9') {
                value = value + (s[i] * (137^i));
            }
        }
         */

        if(s[i] >= 'A'){
            if(s[i]<= 'Z') {
                value = value + (s[i] * (137^i));
            }
        }
        else if(s[i] >= 'a'){
            if(s[i] <= 'z'){
                value = value + (s[i] * (137^i));
            }
        }

    }
    value = (value%NUMENTRIES);
    return value;
}

void Hash::displayHashTable() {
    for(int i = 0; i < NUMENTRIES; i++)
    {
        cout << i;
        for(auto x : table[i])
            cout << " --> " << x;
        cout << endl;
    }
}

void printNWordChunks(string p, int n, Hash hashTable, int fileIndex);

int main(int argc, char *argv[])
{
    string dir = string("sm_doc_set");
    vector<string> files = vector<string>();
    string fileName;
    //int n=stoi(argv[1]);
    int n=20;

    getdir(dir,files);

    Hash hashTable(1000003);

    for (unsigned int i = 0;i < files.size();i++) {
        //cout << i << files[i] << endl;
        fileName="sm_doc_set/"+files[i];
        printNWordChunks("sm_doc_set/"+files[i],n, hashTable, i);
    }

    //hashTable.displayHashTable();

    //make lists unique
    for(int i = 0; i < hashTable.size(); i++)
    {
        hashTable.table[i].unique();
    }
    //cout << files[2] << endl;
    //printNWordChunks("sm_doc_set/"+files[2],n, hashTable);


    //2D array to count similarities
    int similarityTable[files.size()][files.size()];
    for (int i = 0; i < files.size(); i++) {
        for (int j = 0; j < files.size(); j++) {
            similarityTable[i][j] = 0;
        }
    }

    for(int j = 0; j < hashTable.size(); j++)
    {
        //cout << "in for loop" << endl;
        int flag = 1;
        while(flag)
        {
            if(hashTable.table[j].size() >= 2) {
                int val1 = hashTable.table[j].front();
                hashTable.table[j].pop_front();
                std::list<int>::iterator it;


                for (auto x : hashTable.table[j]) {
                    int val2 = x;
                    similarityTable[val1][val2]++;
                }

                if (hashTable.table[j].size() < 2)
                    flag = 0;
            }
            else{
                flag = 0;
            }
        }
    }


    //search through 2D array
    int flag2 = 1;
    while(flag2) {
        int maxScore = 200;
        int fileA = 0;
        int fileB = 0;
        for (int i = 0; i < files.size(); i++) {
            for (int j = 0; j < files.size(); j++) {
                if (similarityTable[i][j] > maxScore) {
                    fileA = i;
                    fileB = j;
                    maxScore = similarityTable[i][j];
                }
            }
        }
        if (maxScore != 200) {
            cout << similarityTable[fileA][fileB] << ':' << files[fileA] << ", " << files[fileB] << endl;
            similarityTable[fileA][fileB] = 0;

        }
        else{
            flag2 = 0;
        }
    }

    return 0;
}

void printNWordChunks(string p,int n, Hash hashTable, int fileIndex)
{
    ifstream file(p);
    //vector<string> chunks;
    string chunk;
    if(file.is_open())
    {
        int i=0;
        string word;
        std::deque<string> wordQ;
        while(file>>word)
        {
            //cout <<"in while loop"<<endl;
            wordQ.push_back(word);
            if(wordQ.size()==n)
            {
                for(int k=0;k<wordQ.size();k++)
                {
                    chunk+=wordQ[k];
                }
                //chunks.push_back(chunk);
                hashTable.insertItem(chunk, fileIndex);
                chunk="";
                wordQ.pop_front();
            }
            i++;
        }
        /*
        for(int b=0;b<chunks.size();b++)
        {
            cout<<chunks[b]<<endl;
        }
         */
        file.close();
    }

}



