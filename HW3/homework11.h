
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <sstream>

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
        vector<pair<vector<predicate>, int>> Fetch(predicate p);
    };

    Database db;

public:
    void tell(string &fact);
    bool ask(string &query);
    vector<predicate> negateQuery(vector<predicate> s);
    predicate negate(predicate p);
    static void printPredicate(predicate p);
};

void KB::tell(string &fact)
{
    vector<predicate> sentence = CNF::convertToCNF(fact);
    for (predicate str : sentence)
    {
        printPredicate(str);
    }
    cout << endl;
    // db.Store(sentence);
    // cout << db.position["Vaccinated"].predicateLocation["negatedLiteral"][0].first << " ";
    // cout << position["Vaccinated"].predicateLocation["negatedLiteral"][0].second << endl;
}

bool KB::ask(string &querys)
{
    // db.Fetch();
    vector<predicate> query = CNF::convertToCNF(querys);
    vector<predicate> notQuery = negateQuery(query);

    db.Store(notQuery);
    queue<vector<predicate>> que;
    que.push(notQuery);

    while (!que.empty())
    {

        vector<predicate> cur_sentence = que.front();
        que.pop();

        for (int i = 0; i < cur_sentence.size(); i++)
        {
            predicate resolve = negate(cur_sentence[i]);
            vector<pair<vector<predicate>, int>> possibleResolve = db.Fetch(resolve);
            for (int j = 0; j < possibleResolve.size(); j++)
            {
                cout << possibleResolve[i].first[possibleResolve[i].second].negate << possibleResolve[i].first[possibleResolve[i].second].name << endl;
                cout << resolve.negate << resolve.name << endl;
                break;
            }
        }
    }

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

vector<KB::predicate> KB::negateQuery(vector<predicate> s)
{
    s[0].negate = true;
    return s;
}

KB::predicate KB::negate(predicate p)
{
    p.negate = (p.negate == true) ? false : true;
    return p;
}

vector<KB::predicate> KB::CNF::convertToCNF(string fact)
{
    vector<string> splitSpace;
    vector<KB::predicate> sentence;
    stringstream ss(fact);
    string word;
    while (ss >> word)
    {
        splitSpace.push_back(word);
    }

    int n = splitSpace.size();
    if (n == 1)
    {
        KB::predicate p(splitSpace[0]);
        sentence.push_back(p);
    }
    else
    {
        for (int i = 0; i < n - 1; i++)
        {
            if (splitSpace[i] != "=>" && splitSpace[i] != "&")
            {
                KB::predicate p(splitSpace[i]);
                p.negate = (p.negate == true) ? false : true;
                sentence.push_back(p);
            }
        }
        KB::predicate p(splitSpace[n - 1]);
        sentence.push_back(p);
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
            }
        }
    }
}

void KB::Database::Store(vector<predicate> p)
{
    standardizeVariable(p);
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

vector<pair<vector<KB::predicate>, int>> KB::Database::Fetch(predicate p)
{
    vector<pair<vector<predicate>, int>> ans;
    vector<pair<int, int>> literals;
    vector<pair<int, int>> predicates;

    if (!p.negate)
    {
        literals = position[p.name].predicateLocation["literal"];
        predicates = position[p.name].predicateLocation["predicate"];
    }
    else
    {
        literals = position[p.name].predicateLocation["~literal"];
        predicates = position[p.name].predicateLocation["~predicate"];
    }

    for (int i = 0; i < literals.size(); i++)
    {
        ans.push_back({allSentences[literals[i].first], literals[i].second});
    }

    for (int i = 0; i < predicates.size(); i++)
    {
        ans.push_back({allSentences[predicates[i].first], predicates[i].second});
    }

    return ans;
}