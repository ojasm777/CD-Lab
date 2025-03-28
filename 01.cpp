#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <queue>

using namespace std;

class NFAToDFAConverter
{
private:
    int numStates;
    int numSymbols;
    vector<vector<set<int>>> nfaTransitions;
    set<int> nfaStartStates;
    set<int> nfaFinalStates;

    set<int> epsilonClosure(const set<int> &states)
    {
        set<int> closure = states;
        queue<int> stateQueue;

        for (int state : states)
        {
            stateQueue.push(state);
        }

        while (!stateQueue.empty())
        {
            int currentState = stateQueue.front();
            stateQueue.pop();

            for (int nextState : nfaTransitions[currentState][0])
            {
                if (closure.find(nextState) == closure.end())
                {
                    closure.insert(nextState);
                    stateQueue.push(nextState);
                }
            }
        }

        return closure;
    }

public:
    NFAToDFAConverter(int states, int symbols) : numStates(states),
                                                 numSymbols(symbols),
                                                 nfaTransitions(states, vector<set<int>>(symbols)) {}

    void addTransition(int fromState, int symbol, int toState)
    {
        nfaTransitions[fromState][symbol].insert(toState);
    }

    void setStartStates(const set<int> &startStates)
    {
        nfaStartStates = startStates;
    }

    void setFinalStates(const set<int> &finalStates)
    {
        nfaFinalStates = finalStates;
    }

    map<set<int>, map<int, set<int>>> convertToDFA()
    {
        map<set<int>, map<int, set<int>>> dfaTransitions;
        queue<set<int>> unmarkedStates;

        set<int> startState = epsilonClosure(nfaStartStates);
        unmarkedStates.push(startState);

        while (!unmarkedStates.empty())
        {
            set<int> currentState = unmarkedStates.front();
            unmarkedStates.pop();

            for (int symbol = 1; symbol < numSymbols; ++symbol)
            {
                set<int> nextStateSet;

                for (int state : currentState)
                {
                    for (int nextState : nfaTransitions[state][symbol])
                    {
                        nextStateSet.insert(nextState);
                    }
                }

                set<int> epsilonNextStateSet = epsilonClosure(nextStateSet);

                if (!epsilonNextStateSet.empty())
                {
                    dfaTransitions[currentState][symbol] = epsilonNextStateSet;

                    if (dfaTransitions.find(epsilonNextStateSet) == dfaTransitions.end())
                    {
                        unmarkedStates.push(epsilonNextStateSet);
                    }
                }
            }
        }

        return dfaTransitions;
    }

    bool isAcceptState(const set<int> &dfaState)
    {
        for (int state : dfaState)
        {
            if (nfaFinalStates.find(state) != nfaFinalStates.end())
            {
                return true;
            }
        }
        return false;
    }

    void printDFATransitions(const map<set<int>, map<int, set<int>>> &dfaTransitions)
    {
        cout << "DFA Transitions:\n";
        for (const auto &p : dfaTransitions)
        {
            auto fromState = p.first;
            auto transitions = p.second;
            cout << "State {";
            for (int state : fromState)
            {
                cout << state << " ";
            }
            cout << "} transitions:\n";

            for (const auto &p : transitions)
            {
                auto symbol = p.first;
                auto toState = p.second;
                cout << "  Symbol " << symbol << " -> State {";
                for (int state : toState)
                {
                    cout << state << " ";
                }
                cout << "}\n";
            }

            if (isAcceptState(fromState))
            {
                cout << "  [ACCEPT STATE]\n";
            }
        }
    }
};

int main()
{
    NFAToDFAConverter nfaToDfa(3, 2);

    nfaToDfa.addTransition(0, 0, 1);
    nfaToDfa.addTransition(1, 0, 2);

    nfaToDfa.addTransition(0, 1, 1);
    nfaToDfa.addTransition(1, 1, 2);

    nfaToDfa.setStartStates({0});
    nfaToDfa.setFinalStates({2});

    auto dfaTransitions = nfaToDfa.convertToDFA();

    nfaToDfa.printDFATransitions(dfaTransitions);

    return 0;
}