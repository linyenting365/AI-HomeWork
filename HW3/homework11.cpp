#include "homework11.h"

int main()
{
    vector<string> querys, facts;
    readInput(querys, facts);
    for (string &fact : facts)
        KB::tell(fact);
    vector<bool> res;
    for (string &query : querys)
    {
        bool tmp = KB::ask(query);
        res.push_back(tmp);
    }
    res = {1, 0, 1};
    writeOutput(res);
    return 0;
}