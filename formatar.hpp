#include <string>

using namespace std;

string formatar_palavra(const string &input)
{
    string output;
    for (char c : input)
    {
        if (isalpha(c))
        {
            output += tolower(c);
        }
    }
    return output;
}