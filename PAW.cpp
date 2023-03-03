#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;


const string configname="config.txt";
//Back up config values
const char BUsep=',';
const string BUword="over";



//Class
class Player{
	private:
		string name;
		int losses;
		int played;
		int points;
		vector<int> wins;
		static void Create_result (const string& w, const string& l, int p);
		static void Remove_spaces (string& str);
		static void Skip_to_space (string& str);
		static string Caps (const string& str);
	public:
		static map<string,int> results;
		static vector<string> words;
		Player (const string& line, char sep){
			stringstream ss(line);
			string temp;
			
			getline(ss, temp, sep);
			name=temp.substr(1);
			losses=played=points=0;
			wins.clear();
		}
		int Add (const string& line, char sep);
		string Return (char sep);
		void Sort_wins ();
		static bool Compare (const Player& a, const Player& b);
		static void Match_result (const string& line, char sep);
		static void New_word (string word);
};
//Members
int Player::Add (const string& line, char sep){
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
string Player::Return(char sep){
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

//Sort
bool Player::Compare (const Player& a, const Player& b){
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
void Player::Sort_wins(){
	if (!wins.empty()){
		sort (wins.begin(),wins.end());
	}
}

//Handle spaces
void Player::Remove_spaces (string& str){
	
	//Remove starting and ending spaces
	auto spaces=str.find_first_not_of(' ');
	if (spaces==string::npos){
		str.clear();
	}
	else{
		str=str.substr(spaces, str.find_last_not_of(' ')+1-spaces);
	}
	
	//Remove double spaces
	do{
		spaces=str.find("  ");
		if (spaces!=string::npos){
			str.erase(str.begin()+spaces);
		}
	} while (spaces!=string::npos);
	
}
void Player::Skip_to_space (string& str){
	
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
string Player::Caps (const string& str){
	string ret;
	
	for (auto i=str.begin();i!=str.end();i++){
		if (*i>='a' && *i<='z'){
			ret.push_back(*i-'a'+'A');
		}
		else{
			ret.push_back(*i);
		}
	}
	return ret;
}

//Match results
map<string,int> Player::results;
void Player::Create_result (const string& w, const string& l, int p){
	int check=0;
	
	for (auto i=words.begin();i!=words.end();i++){
		string paw;
		paw+=w;
		paw.push_back(' ');
		paw+=*i;
		paw.push_back(' ');
		paw+=l;
		check+=results.count(paw);
		results[paw]=p;
	}
	
	//Check duplicates
	if (check>0){
		throw (-1);
	}
}
void Player::Match_result (const string& line, char sep){
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
	
	string pp1=Caps(p1);
	string pp2=Caps(p2);
	//Create result
	if (p>0){
		Create_result (p1, p2, p);
		Create_result (p2, p1, 0);
		Create_result (pp1, pp2, p);
		Create_result (pp2, pp1, 0);
	}
	else if (p<0){
		Create_result (p1, p2, 0);
		Create_result (p2, p1, abs(p));
		Create_result (pp1, pp2, 0);
		Create_result (pp2, pp1, abs(p));
	}
	//Match replaced
	else if (!pp.empty() && pp.front()=='@'){
		Create_result (p1, p2, -2);
		Create_result (p2, p1, -2);
		Create_result (pp1, pp2, -2);
		Create_result (pp2, pp1, -2);
	}
	//Match not played
	else{
		Create_result (p1, p2, -1);
		Create_result (p2, p1, -1);
		Create_result (pp1, pp2, -1);
		Create_result (pp2, pp1, -1);
	}
}
vector<string> Player::words;
void Player::New_word(string word){
	Remove_spaces(word);
	if (!word.empty()){
		words.push_back(word);
	}
}

//Update csv
string Update (const string& line, char sep, int points){
	stringstream ss(line);
	string ret, temp;
	
	//Copy 1st column
	getline(ss, temp, sep);
	ret+=temp;
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
	getline(ss, temp, sep);
	temp.clear();
	getline(ss, temp);
	ret+=temp;
	
	return ret;
}

//Press enter to leave
void Exitprompt (){
	cout<<endl <<"Press enter to leave" <<endl;
	cin.ignore(100, '\n');
}



int main()
{
    vector<Player> players;
    string filename, line;
    ifstream infile;
    ofstream outfile;
    char sep;
    
    //Config file
    infile.open(configname);
    if (infile.is_open()){
    	getline(infile,line);
    	sep=line.back();
    	while (getline(infile,line)){
    		Player::New_word(line);
		}
	}
	//No config file found
	else{
		sep=BUsep;
		Player::New_word(BUword);
	}
	infile.close();
    
    //Results
    do{
    	cout<<"Results file: ";
    	getline(cin,filename);
    	//Strip .csv
    	if (filename.size()>4 && filename.substr(filename.size()-4)==".csv"){
    		filename=filename.substr(0,filename.size()-4);
		}
    	infile.open(filename+".csv");
	} while (!infile.is_open());
	
    int row=1;
    getline(infile, line);
    
    while (getline(infile, line)){
    	
    	row++;
    	if (!line.empty() && line.front()!=sep){
    		try{
    			Player::Match_result(line, sep);
			}
			catch (int missingp){
				if (missingp==1){
					cout<<endl <<"Missing player 1 in row " <<row <<endl;
					Exitprompt();
					return 0;
				}
				if (missingp==2){
					cout<<endl <<"Missing player 2 in row " <<row <<endl;
					Exitprompt();
					return 0;
				}
				if (missingp==-1){
					cout<<endl <<"Duplicate result in row " <<row <<endl;
					Exitprompt();
					return 0;
				}
			}
			catch (...){
				cout<<endl <<"Missing points in row " <<row <<endl;
				Exitprompt();
				return 0;
			}
		}
		
	}
	infile.close();
	
    //Players
    do{
    	cout<<"Players file: ";
    	getline(cin,filename);
    	//Strip .csv
    	if (filename.size()>4 && filename.substr(filename.size()-4)==".csv"){
    		filename=filename.substr(0,filename.size()-4);
		}
    	infile.open(filename+".csv");
	} while (!infile.is_open());
    outfile.open(filename+"_updated.csv", ios::trunc);
    if (!outfile.is_open()){
    	cout<<endl <<filename <<"_updated.csv can't be opened" <<endl;
    	Exitprompt();
    	return 0;
	}
    
    getline(infile, line);
    outfile<<line <<endl;
	
    while (getline(infile, line)){
    	
    	//Check spaces
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
			outfile<<line <<endl;
		}
		//Player
		else if (line.front()<'0'){
			players.push_back(Player(line, sep));
			outfile<<line <<endl;
		}
		//PAW
    	else{
    		if (players.empty()){
    			cout<<endl <<"No player found" <<endl;
    			Exitprompt();
    			return 0;
			}
    		int p=players.back().Add(line, sep);
    		outfile<<Update(line, sep, p) <<endl;
		}
		
	}		
    infile.close();
    outfile.close();
    
    //Sort
    for (auto i=players.begin();i!=players.end();i++){
    	i->Sort_wins();
	}
    sort (players.begin(), players.end(), Player::Compare);
    
	//Table output
    outfile.open("Table.csv", ios::trunc);
    if (!outfile.is_open()){
    	cout<<endl <<"Table.csv can't be opened" <<endl;
    	Exitprompt();
    	return 0;
	}
    outfile<<"Rank" <<sep <<"Player" <<sep <<"Used PAW" <<sep <<"Wins" <<sep <<"Losses" <<sep <<"Total" <<sep <<"Points" <<sep <<endl;
    for (int i=0;i<players.size();i++){
    	//Check ties
    	if (i>0 && !Player::Compare(players[i],players[i-1]) && !Player::Compare(players[i-1],players[i])){
    		outfile<<sep;
		}
		else{
			outfile<<i+1 <<sep;
		}
		//Player record
		outfile<<players[i].Return(sep) <<endl;
    }
    outfile.close();
    cout<<endl <<"Success!!!" <<endl;
    Exitprompt();
    return 0;
}
