
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
// #include <vector>

using namespace std;

void readInput(vector<string> &query, vector<string> &kb)
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

private:
    struct predicate
    {
        bool negate = false;
        string name = "";
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
    static bool isLiteral(predicate &p);
    static bool isVariable(string &s);

    class CNF
    {
    public:
        static vector<predicate> convertToCNF(string fact);
    };
    class Database
    {
    private:
        struct index
        {
            unordered_map<string, vector<pair<int, int>>> predicateLocation;
        };
        unordered_map<string, int> variable;
        vector<vector<predicate>> allSentences;
        unordered_map<string, index> position;

    public:
        void standardizeVariable(vector<predicate> &p);
        void Store(vector<predicate> sentence);
        void Fetch();
    };

    Database db;

public:
    void tell(string &fact);
    bool ask(string &query);
    static void printPredicate(predicate p);
};

void KB::tell(string &fact)
{
    vector<predicate> sentence = CNF::convertToCNF(fact);
    db.Store(sentence);
    // cout << db.position["Vaccinated"].predicateLocation["negatedLiteral"][0].first << " ";
    // cout << position["Vaccinated"].predicateLocation["negatedLiteral"][0].second << endl;
}

bool KB::ask(string &query)
{
    db.Fetch();
    return true;
}

void KB::printPredicate(predicate p)
{
    cout << (p.negate == true ? "~" : "");
    cout << p.name << "(";
    for (string str : p.args)
    {
        cout << str << ",";
    }
    cout << ") ";
}

bool KB::isLiteral(predicate &p)
{
    for (string s : p.args)
    {
        if (isVariable(s))
            return false;
    }
    return true;
}

bool KB::isVariable(string &s)
{
    if (s.size() > 1)
        return false;
    return islower(s[0]);
}

vector<KB::predicate> KB::CNF::convertToCNF(string fact)
{
    vector<KB::predicate> sentence;
    fact.erase(remove_if(fact.begin(), fact.end(), static_cast<int (*)(int)>(isspace)), fact.end());
    int index = 0;
    for (int i = 0; i < fact.size();)
    {
        if (isalpha(fact[i]) && index == 0)
        {
            string temp;
            bool negate = (fact[i - 1] == '~') ? true : false;
            while (fact[i - 1] != ')' && i >= 0)
            {
                temp += fact[i];
                i++;
            }
            KB::predicate p(temp);
            p.negate = negate;
            p.negate = (p.negate == true) ? false : true;
            sentence.push_back(p);
        }
        else if (fact[i] == '=' && fact[i + 1] == '>')
        {
            index = i;
            i += 2;
        }
        else if (isalpha(fact[i]) && index != 0)
        {
            string temp;
            bool negate = (fact[i - 1] == '~') ? true : false;
            while (fact[i - 1] != ')' && i < fact.size())
            {
                temp += fact[i];
                i++;
            }
            KB::predicate p(temp);
            p.negate = negate;
            sentence.push_back(p);
        }
        else
            i++;
    }
    return sentence;
}

void KB::Database::standardizeVariable(vector<predicate> &s)
{
    unordered_set<string> cur_var;
    for (int i = 0; i < s.size(); i++)
    {
        vector<string> args = s[i].args;
        for (int j = 0; j < args.size(); j++)
        {
            if (isVariable(args[j]))
            {
                string tmp = args[j];
                if (!cur_var.count(tmp))
                {
                    cur_var.insert(tmp);
                    variable[tmp]++;
                }
                args[j] = tmp + to_string(variable[tmp]);
                // cout << args[j] << ",";
            }
        }
        // cout << endl;
    }
}

void KB::Database::Store(vector<predicate> p)
{
    standardizeVariable(p);
    // cout << "standardize" << endl;
    allSentences.push_back(p);
    int pos = allSentences.size() - 1;
    for (int i = 0; i < p.size(); i++)
    {
        if (isLiteral(p[i]))
        {
            if (p[i].negate)
            {
                cout << "literal ~" << p[i].name << endl;
                position[p[i].name].predicateLocation["~literal"].push_back({pos, i});
            }
            else
            {
                cout << "literal " << p[i].name << endl;
                position[p[i].name].predicateLocation["literal"].push_back({pos, i});
            }
        }
        else
        {
            if (p[i].negate)
            {
                cout << "predicate ~" << p[i].name << endl;
                position[p[i].name].predicateLocation["~predicate"].push_back({pos, i});
            }
            else
            {
                cout << "predicate " << p[i].name << endl;
                position[p[i].name].predicateLocation["predicate"].push_back({pos, i});
            }
        }
    }
}

void KB::Database::Fetch()
{
    vector<pair<int, int>> res = position["Healthy"].predicateLocation["~literal"];
    // cout << res << endl;
    for (auto r : res)
    {
        cout << r.first << " ";
        cout << r.second << endl;
    }
}