#include "Player.h"
#include <fstream>

using namespace std;


///Standard inputs
const char DEF_sep=',';
const string DEF_inputfile="Table.txt";
const string DEF_outputfile="Ranking.csv";

bool Ranking (char sep, const string& filename, vector<Player>& players);

int main (int argc, char* argv[])
{
    vector<Player> players;
    char sep;
    string inputfile, outputfile, line;

    if (argc>1){
        inputfile=argv[1];
    }
    else{
        inputfile=DEF_inputfile;
    }
    if (argc>2){
        outputfile=argv[2];
    }
    else{
        outputfile=DEF_outputfile;
    }
    if (argc>3){
        sep=argv[3][0];
    }
    else{
        sep=DEF_sep;
    }

    ifstream filein(inputfile);
    while (getline(filein,line)){
        if (line.find_first_not_of(' ')==string::npos){

        }
        else if (line.front()=='#'){
            players.push_back(Player(line.substr(line.find_first_not_of(' ', 1)), '_'));
        }
        else if (!players.empty()){
            players.back().Add_points(line);
        }
    }
    filein.close();
    Ranking (sep, outputfile, players);
    return 0;
}
