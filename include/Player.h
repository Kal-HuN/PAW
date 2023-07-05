#include <string>
#include <vector>
#include <unordered_map>

using namespace std;


class Player
{
    public:
        Player(const string& line, char sep);

        int Add_PAW (const string& line, char sep);
        void Add_points (const string& line);
        string Return (char sep);

        ///Sort
        void Sort_wins ();
        static bool Compare (const Player& a, const Player& b);

        ///Match results
        static void Setwords(const vector<string>& val) { words = val; }
        static void Match_result (const string& line, char sep);

    protected:

    private:
        string name;
        vector<int> wins;
        int losses;
        int played;
        int points;

        void Skip_to_space (string& str);

        ///Match results
        static vector<string> words;
        static unordered_map<string,int> results;

        static void Create_result (const string& w, const string& l, int p);
        static void Remove_spaces (string& str, bool remove_double_spaces=true);
        static void Caps (string& str);
};
