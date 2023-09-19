#include <string>
#include <vector>

using namespace std;


/////////////////////////////////////////////////
/// sep: csv separator
/// words: separating words
/// filenames[0]: result file
/// filenames[1]: players file
/// filenames[2]: updated players file
/// filenames[3]: ranking file
/////////////////////////////////////////////////

string Paw (char sep, const vector<string>& words, const vector<string>& filenames);
