#include <iostream>
#include <map>
#include <set>

using namespace std;

class DFA
{
public:
    map<int, map<char, int>> transition;
    int startState;
    set<int> acceptStates;

    void addTransition(int from, char symbol, int to)
    {
        transition[from][symbol] = to;
    }

    bool isAccepted(string input)
    {
        int currentState = startState;
        for (char symbol : input)
        {
            if (transition[currentState].count(symbol))
            {
                currentState = transition[currentState][symbol];
            }
            else
            {
                return false; // Invalid transition, reject input
            }
        }
        return acceptStates.count(currentState);
    }
};

int main()
{
    DFA dfa;
    dfa.startState = 0;
    dfa.acceptStates = {2};

    dfa.addTransition(0, 'a', 1);
    dfa.addTransition(1, 'b', 2);
    dfa.addTransition(2, 'a', 2); // Allows looping in final state

    string input;
    cout << "Enter input string: ";
    cin >> input;

    if (dfa.isAccepted(input))
        cout << "Accepted\n";
    else
        cout << "Rejected\n";
    return 0;
}
