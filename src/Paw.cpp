#include "Player.h"
#include <fstream>
#include <algorithm>
#include <sstream>

using namespace std;


string Update (const string& line, char sep, int points);
bool Ranking (char sep, const string& filename, vector<Player>& players);

string Paw (char sep, const vector<string>& words, const vector<string>& filenames)
{
    if (filenames.size()<4){
        return "Missing files";
    }
    vector<Player> players;

    ///Match results
    Player::Setwords(words);
    if (ifstream filein(filenames[0]);filein.is_open()){
        string line;
        int row=1;
        getline(filein,line);
        while (getline(filein,line)){
            row++;
            if (!line.empty() && line.front()!=sep){
                try{
                    Player::Match_result(line, sep);
                }
                catch (int missingp){
                    if (missingp==1){
                        string ret;
                        ret+="Missing player 1 in row ";
                        ret+=to_string(row);
                        return ret;
                    }
                    if (missingp==2){
                        string ret;
                        ret+="Missing player 2 in row ";
                        ret+=to_string(row);
                        return ret;
                    }
                    if (missingp==-1){
                        string ret;
                        ret+="Duplicate result in row ";
                        ret+=to_string(row);
                        if (row==2)
                            ret+="\nPossible double separating word in the config file";
                        return ret;
                    }
                }
                catch (...){
                    string ret;
                    ret+="Missing points in row ";
                    ret+=to_string(row);
                    return ret;
                }
            }
        }
    }
    else{
        string ret(filenames[0]);
        ret+=" can't be opened";
        return ret;
    }

    ///Players
    if (ifstream filein(filenames[1]);filein.is_open()){
        string line;
        stringstream output;
        getline(filein,line);
        output<<line <<endl;
        while (getline(filein,line)){
            //Check starting spaces
            if (!line.empty()){
                auto skipspaces=line.find_first_not_of(' ');
                if (skipspaces==string::npos){
                    line.clear();
                }
                else{
                    line=line.substr(skipspaces);
                }
            }

            //Skip
            if (line.empty() || line.front()==sep){
                output<<line <<endl;
            }
            //Player
            else if (line.front()<'0'){
                players.push_back(Player(line.substr(1), sep));
                output<<line <<endl;
            }
            //PAW
            else{
                if (players.empty()){
                    return "No player found";
                }
                int p=players.back().Add_PAW(line, sep);
                output<<Update(line, sep, p) <<endl;
            }
        }
        filein.close();

        //Output
        if (ofstream fileout(filenames[2], ios::trunc);fileout.is_open()){
            fileout.write(output.str().c_str(), output.str().size());
        }
        else{
            string ret(filenames[2]);
            if (filenames[2]==filenames[1]){
                ret+=" already open";
            }
            else{
                ret+=" can't be opened";
            }
            return ret;
        }
    }
    else{
        string ret(filenames[1]);
        ret+=" can't be opened";
        return ret;
    }

    ///Ranking output
    if (Ranking(sep, filenames[3], players)){
        return "Success";
    }
    else{
        string ret(filenames[3]);
        ret+=" can't be opened";
        return ret;
    }
}

string Update (const string& line, char sep, int points){
	stringstream ss(line);
	string ret;

	//Copy 1st column
	getline(ss, ret, sep);
	ret.push_back(sep);

	//Update
	if (points==-1){
		ret.push_back(sep);
	}
	else if (points==-10){
		ret+="!!!";
		ret.push_back(sep);
	}
	else if (points==-12){
		ret+="@lt";
		ret.push_back(sep);
	}
	else if (points==-11){
		ret.push_back('-');
		ret.push_back(sep);
	}
	else{
		ret+=to_string(points);
		ret.push_back(sep);
	}

	//Copy remaining
	string temp;
	getline(ss, temp, sep);
	temp.clear();
	getline(ss, temp);
	ret+=temp;

	return ret;
}

bool Ranking (char sep, const string& filename, vector<Player>& players){
    ofstream fileout(filename, ios::trunc);

    if (!fileout.is_open())
        return false;

    ///Sort
    for (auto i=players.begin();i!=players.end();i++){
    	i->Sort_wins();
	}
    sort(players.begin(), players.end(), Player::Compare);

    ///Ranking output
    fileout<<"Rank" <<sep <<"Player" <<sep <<"Used PAW" <<sep <<"Wins" <<sep <<"Losses" <<sep <<"Total" <<sep <<"Points" <<sep <<endl;
    for (unsigned int i=0;i<players.size();i++){
        //Check ties
        if (i>0 && !Player::Compare(players[i],players[i-1]) && !Player::Compare(players[i-1],players[i])){
            fileout<<sep;
        }
        else{
            fileout<<i+1 <<sep;
        }
        //Player record
        fileout<<players[i].Return(sep) <<endl;
    }

    return true;
}
