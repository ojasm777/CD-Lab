#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

struct PairHash
{
    template <class T1, class T2>
    unsigned int operator()(const pair<T1, T2> &p) const
    {
        unsigned int h1 = hash<T1>{}(p.first);
        unsigned int h2 = hash<T2>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

class DFA
{
private:
    int numStates;
    int startState;
    vector<int> finalStates;
    unordered_map<pair<int, char>, int, PairHash> transitions;

public:
    DFA(int states, int start) : numStates(states), startState(start) {}

    void addTransition(int fromState, char symbol, int toState)
    {
        transitions[{fromState, symbol}] = toState;
    }

    void addFinalState(int state)
    {
        finalStates.push_back(state);
    }

    bool isAcceptState(int state)
    {
        for (int finalState : finalStates)
        {
            if (state == finalState)
                return true;
        }
        return false;
    }

    bool acceptsString(const string &input)
    {
        int currentState = startState;

        for (char symbol : input)
        {
            auto transition = transitions.find({currentState, symbol});

            if (transition == transitions.end())
            {
                return false;
            }

            currentState = transition->second;
        }

        return isAcceptState(currentState);
    }
};

int main()
{
    DFA dfa(4, 0);

    dfa.addTransition(0, '0', 1);
    dfa.addTransition(0, '1', 0);
    dfa.addTransition(1, '0', 2);
    dfa.addTransition(1, '1', 0);
    dfa.addTransition(2, '0', 2);
    dfa.addTransition(2, '1', 3);
    dfa.addTransition(3, '0', 2);
    dfa.addTransition(3, '1', 3);

    dfa.addFinalState(3);

    string testStrings[] = {
        "0110", "1001", "00110", "101010", "111", "000"};

    for (const auto &str : testStrings)
    {
        cout << "String: " << str
             << " - "
             << (dfa.acceptsString(str) ? "Accepted" : "Rejected")
             << endl;
    }

    return 0;
}