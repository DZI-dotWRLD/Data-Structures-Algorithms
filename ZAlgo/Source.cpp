#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

void getZarr(string str, vector<int>& Z);


void searchPatternInText(const vector<string>& lines, const string& pattern)
{
    int lineNumber = 1; 
    bool found = false; 

    for (const string& line : lines)
    {
        string concat = pattern + "$" + line;
        int lenConcat = concat.length();
        vector<int> Z(lenConcat, 0);
        getZarr(concat, Z);

        vector<int> offsets; 
        for (int i = 0; i < lenConcat; ++i)
        {
            if (Z[i] == pattern.length()) // Found pattern
            {
                offsets.push_back(i - pattern.length() - 1);
            }
        }

        // Print results if offsets were found
        if (!offsets.empty()) {
            found = true;
            cout << "Pattern found on line " << lineNumber << " at offsets: ";
            for (int offset : offsets)
                cout << offset << " ";
            cout << endl;
        }

        lineNumber++;
    }

    if (!found)
        cout << "Pattern not found in the text." << endl;
}

//  to fill the Z array
void getZarr(string str, vector<int>& Z)
{
    int n = str.length();
    int L, R, k;
    L = R = 0;

    for (int i = 1; i < n; ++i)
    {
        if (i > R)
        {
            L = R = i;
            while (R < n && str[R - L] == str[R])
                R++;
            Z[i] = R - L;
            R--;
        }
        else
        {
            k = i - L;
            if (Z[k] < R - i + 1)
                Z[i] = Z[k];
            else
            {
                L = i;
                while (R < n && str[R - L] == str[R])
                    R++;
                Z[i] = R - L;
                R--;
            }
        }
    }
}

int main()
{
    vector<string> lines;
    string pattern, line, filename = "KingJamesBibleProjGutenberg.txt";

    
    cout << "Enter the search pattern: ";
    getline(cin, pattern);

    
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Could not open file '" << filename << "'." << endl;
        return 1;
    }

   
    while (getline(file, line)) {
        lines.push_back(line);
    }
    file.close();

 
    searchPatternInText(lines, pattern);

    return 0;
}
