#include "Player.h"
#include <sstream>
#include <algorithm>
#include <cstdlib>

using namespace std;


vector<string> Player::words;
unordered_map<string,int> Player::results;

Player::Player(const string& line, char sep)
{
    //ctor
    stringstream ss(line);

    getline(ss, name, sep);
    Remove_spaces(name, false);
    losses=played=points=0;
    wins.clear();
}

int Player::Add_PAW(const string& line, char sep)
{
    stringstream ss(line);
	string paw;

	getline(ss, paw, sep);
	Skip_to_space(paw);

	//No PAW
	if (paw.empty()){
		return -1;
	}

	//PAW found
	played++;
	Caps(paw);
	auto r=results.find(paw);

	//No matching result
	if (r==results.end()){
		return -10;
	}
	//Match replaced
	if (r->second==-2){
		played--;
		return -12;
	}
	//Match not played
	if (r->second==-1){
		return -11;
	}
	//Loss
	if (r->second==0){
		losses++;
		return 0;
	}
	//Win
	points+=r->second;
	wins.push_back(r->second);
	return r->second;
}

void Player::Add_points(const string& line)
{
    //No pick
    if (line.find_first_not_of(' ', line.find_first_of(' '))!=string::npos && line.find("No pick yet")==string::npos){
        played++;
        //No result
        if (auto t=line.find_last_of(' ')+1;t<line.size()){
            string temp(line.substr(t));
            //Valid result
            if (temp.front()>='0' && temp.front()<='9'){
                int p=stoi(temp);
                if (p==0){
                    losses++;
                }
                else{
                    points+=p;
                    wins.push_back(p);
                }
            }
        }
	}
}

string Player::Return(char sep)
{
    string ret;

	ret+=name;
	ret.push_back(sep);
	ret+=to_string(played);
	ret.push_back(sep);
	ret+=to_string(wins.size());
	ret.push_back(sep);
	ret+=to_string(losses);
	ret.push_back(sep);
	ret+=to_string(wins.size()+losses);
	ret.push_back(sep);
	ret+=to_string(points);
	ret.push_back(sep);
	return ret;
}

void Player::Sort_wins()
{
    if (!wins.empty()){
        sort(wins.begin(), wins.end());
    }
}

bool Player::Compare(const Player& a, const Player& b)
{
    //Compare points
	if (a.points>b.points)
		return true;
	if (a.points<b.points)
		return false;
	//Compare wins
	if (a.wins.size()>b.wins.size())
		return true;
	if (a.wins.size()<b.wins.size())
		return false;
	//Compare best win
	for (int i=a.wins.size()-1;i>=0;i--){
		if (a.wins[i]>b.wins[i])
			return true;
		if (a.wins[i]<b.wins[i])
			return false;
	}
	//Compare losses
	if (a.losses<b.losses)
		return true;
	if (a.losses>b.losses)
		return false;
	//Tie
	return false;
}

void Player::Match_result(const string& line, char sep)
{
    stringstream ss(line);
	string p1, p2, pp;
	int p=0;

	//Parse
	getline(ss, p1, sep);
	getline(ss, p2, sep);
	getline(ss, pp, sep);
	if (pp.empty() || pp.front()!='@'){
		p=stoi(pp);
	}

	//Check missing player
	Remove_spaces(p1);
	Remove_spaces(p2);
	if (p1.empty()){
		throw (1);
	}
	if (p2.empty()){
		throw (2);
	}

	//Create result
	if (p>0){
		Create_result (p1, p2, p);
		Create_result (p2, p1, 0);
	}
	else if (p<0){
		Create_result (p1, p2, 0);
		Create_result (p2, p1, abs(p));
	}
	//Match replaced
	else if (!pp.empty() && pp.front()=='@'){
		Create_result (p1, p2, -2);
		Create_result (p2, p1, -2);
	}
	//Match not played
	else{
		Create_result (p1, p2, -1);
		Create_result (p2, p1, -1);
	}
}

void Player::Skip_to_space(string& str)
{
    auto skip=str.find_first_of(' ');
	//No spaces
	if (skip==string::npos){
		str.clear();
	}
	else{
		str=str.substr(skip);
		Remove_spaces(str);
		//Check for PAW xx
		if (str.front()<'A'){
			Skip_to_space(str);
		}
	}
}

void Player::Create_result(const string& w, const string& l, int p)
{
    int check=0;

	for (auto i=words.begin();i!=words.end();i++){
		string paw;
		paw+=w;
		paw.push_back(' ');
		paw+=*i;
		paw.push_back(' ');
		paw+=l;
		Caps(paw);
		check+=results.count(paw);
		results[paw]=p;
	}

	//Check duplicates
	if (check>0){
		throw (-1);
	}
}

void Player::Remove_spaces(string& str, bool remove_double_spaces)
{
    //Remove starting and ending spaces
	auto spaces=str.find_first_not_of(' ');
	if (spaces==string::npos){
		str.clear();
	}
	else{
		str=str.substr(spaces, str.find_last_not_of(' ')+1-spaces);
	}

	//Remove double spaces
	if (remove_double_spaces){
        do{
            spaces=str.find("  ");
            if (spaces!=string::npos){
                str.erase(str.begin()+spaces);
            }
        } while (spaces!=string::npos);
	}
}

void Player::Caps(string& str)
{
    for (auto i=str.begin();i!=str.end();i++){
		if (*i>='A' && *i<='Z'){
			*i=*i-'A'+'a';
		}
	}
}
