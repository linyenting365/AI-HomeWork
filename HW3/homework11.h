
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
// #include <vector>

using namespace std;

void readInput(vector<string> query, vector<string> &kb)
{
    ifstream infile;
    int q, k;
    string tmp;
    infile.open("input.txt");
    getline(infile, tmp);
    q = stoi(tmp);
    while (q > 0)
    {
        getline(infile, tmp);
        query.push_back(tmp);
        q--;
    }
    getline(infile, tmp);
    k = stoi(tmp);
    while (k > 0)
    {
        getline(infile, tmp);
        kb.push_back(tmp);
        k--;
    }
}

void writeOutput(vector<bool> &result)
{
    ofstream outfile;
    outfile.open("output.txt");
    for (bool res : result)
    {
        if (res)
            outfile << "TRUE" << endl;
        else
            outfile << "FALSE" << endl;
    }
}

class KB
{
public:
    struct predicate
    {
        bool negate;
        string name;
        vector<string> args;
        predicate(string &s)
        {
            int i = 0;
            if (s[i] == '~')
                negate = true, i = 1;
            while (s[i] != '(')
            {
                name += s[i];
                i++;
            }
            string tmp;
            i++;
            while (s[i] != ')')
            {
                if (s[i] == ',')
                {
                    args.push_back(tmp);
                    tmp = "";
                    i++;
                }
                tmp += s[i];
                i++;
            }
            args.push_back(tmp);
        }
    };

    static void tell(string &fact);
    static bool ask(string &query);
};

class CNF
{
public:
    static vector<string> implicationElimination(pair<vector<string>, int> fact);
    static vector<string> convertToCNF(string fact);
    static pair<vector<string>, int> reArrange(string fact);
};

class Database
{
};

void KB::tell(string &fact)
{
    vector<string> res = CNF::convertToCNF(fact);
}

bool KB::ask(string &query)
{
    return true;
}

vector<string> CNF::implicationElimination(pair<vector<string>, int> fact)
{
    if (fact.second == -1)
        return fact.first;
    fact.first[fact.second] = "|";
    for (int i = 0; i < fact.second; i++)
    {
        if (i - 1 >= 0 && fact.first[i - 1] == "~" && isalpha(fact.first[i][0]))
        {
            fact.first.erase(fact.first.begin() + (i - 1));
        }
        else if (isalpha(fact.first[i][0]))
        {
            fact.first[i] = "~" + fact.first[i];
        }

        if (fact.first[i] == "&")
        {
            fact.first[i] = "|";
        }
    }
    // for (auto str : fact.first)
    // {
    //     cout << str << " ";
    // }
    // cout << endl;

    return fact.first;
}

vector<string> CNF::convertToCNF(string fact)
{
    pair<vector<string>, int> expression = reArrange(fact);
    vector<string> res = implicationElimination(expression);
    return res;
}

pair<vector<string>, int> CNF::reArrange(string fact)
{
    vector<string> res;
    fact.erase(remove_if(fact.begin(), fact.end(), static_cast<int (*)(int)>(isspace)), fact.end());

    int isimplication = -1;
    for (int i = 0; i < fact.size();)
    {

        if (fact[i] == '&')
            res.push_back("&"), i++;
        else if (fact[i] == '~')
            res.push_back("~"), i++;
        else if (fact[i] == '=' && fact[i + 1] == '>')
        {
            res.push_back("=>");
            isimplication = res.size() - 1;
            i += 2;
        }
        else if (isalpha(fact[i]))
        {
            string temp;
            while (fact[i] != ')' && i < fact.size())
            {
                temp += fact[i];
                i++;
            }
            temp += fact[i];
            i++;
            res.push_back(temp);
        }
    }

    // for (auto str : res)
    // {
    //     cout << str << " ";
    // }
    // cout << endl;
    // cout << isimplication << endl;
    return {res, isimplication};
}