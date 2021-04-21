#include "homework11.h"

int main()
{
    KB kb;
    vector<string> querys, facts;
    readInput(querys, facts);
    // KB::CNF::convertToCNF("Learn(Come,x) => Train(Sit,x)");
    for (string &fact : facts)
        kb.tell(fact);
    vector<bool> res;
    cout << querys[0] << endl;
    for (string &query : querys)
    {
        bool tmp = kb.ask(query);
        res.push_back(tmp);
    }
    writeOutput(res);
    return 0;
}