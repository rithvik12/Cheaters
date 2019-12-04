#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <deque>

using namespace std;

void printNWordChunks(string p, int n);

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

int main(int argc, char *argv[])
{
    string dir = string("sm_doc_set");
    vector<string> files = vector<string>();
    string fileName;
    //int n=stoi(argv[1]);
    int n=6;

    getdir(dir,files);

    for (unsigned int i = 0;i < files.size();i++) {
        cout << i << files[i] << endl;
        fileName="sm_doc_set/"+files[i];

    }

    printNWordChunks("sm_doc_set/"+files[2],n);

    return 0;
}

void printNWordChunks(string p,int n)
{
    ifstream file(p);
    vector<string> chunks;
    string chunk;
    if(file.is_open())
    {
        int i=0;
        cout<<"In if statement"<<endl;
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
                    cout<<"for loop"<<endl;
                    chunk+=wordQ[k];
                }
                chunks.push_back(chunk);
                chunk="";
                wordQ.pop_front();
            }
            i++;
        }
        cout<<"Left all this bs"<<endl;
        for(int b=0;b<chunks.size();b++)
        {
            cout<<"printing chunks" <<endl;
            cout<<chunks[b]<<endl;
        }
        file.close();
    }

}



