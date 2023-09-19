#include <iostream>
#include <fstream>
#include "Paw.h"

using namespace std;


const string Configfile="config.txt";
///Back up config values
const char BUsep=',';
const string BUword="over";



int main()
{
    char sep;
    vector<string> words;
    vector<string> filenames(4);
    if (ifstream filein(Configfile);filein.is_open()){
        string line;
        getline(filein,line);
        sep=line.back();
        while (getline(filein,line)){
            words.push_back(line);
        }
    }
    else{
        sep=BUsep;
        words.push_back(BUword);
        cout<<"Missing config file\n\n";
    }
    cout<<"Result file: ";
    getline(cin,filenames[0]);
    if (filenames[0].find(".csv")==string::npos){
        filenames[0]+=".csv";
    }
    cout<<"Players file: ";
    getline(cin,filenames[1]);
    if (filenames[1].find(".csv")==string::npos){
        filenames[1]+=".csv";
    }
    filenames[2]=filenames[1];
    filenames[3]="Ranking.csv";

    cout<<Paw(sep, words, filenames);
    cin.ignore(100, '\n');
    return 0;
}
