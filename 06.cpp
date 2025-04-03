#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;

// Function to find common prefixes in the productions of a given non-terminal
string findCommonPrefix(const vector<string>& productions) {
    if (productions.empty()) return "";

    string prefix = productions[0];

    for (const string& prod : productions) {
        int i = 0;
        while (i < prefix.length() && i < prod.length() && prefix[i] == prod[i]) {
            i++;
        }
        prefix = prefix.substr(0, i);
        if (prefix.empty()) break; // No common prefix found
    }

    return prefix;
}

// Function to perform left factoring on the given grammar
void leftFactorGrammar(map<string, vector<string>>& grammar) {
    map<string, vector<string>> newGrammar;
    int count = 1; // For creating new non-terminal names

    for (auto& rule : grammar) {
        string nonTerminal = rule.first;
        vector<string> productions = rule.second;

        string commonPrefix = findCommonPrefix(productions);

        if (!commonPrefix.empty() && commonPrefix.length() > 1) { // Left factoring needed
            string newNonTerminal = nonTerminal + "_fact" + to_string(count++); // Creating new non-terminal

            vector<string> newProductions = { commonPrefix + newNonTerminal }; // Main rule
            vector<string> newNonTerminalProductions; // Productions for the new non-terminal

            for (const string& prod : productions) {
                if (prod.find(commonPrefix) == 0) { // Starts with common prefix
                    string remainder = prod.substr(commonPrefix.length());
                    newNonTerminalProductions.push_back(remainder.empty() ? "ε" : remainder);
                } else {
                    newProductions.push_back(prod);
                }
            }

            newGrammar[nonTerminal] = newProductions;
            newGrammar[newNonTerminal] = newNonTerminalProductions;
        } else {
            newGrammar[nonTerminal] = productions; // No left factoring needed
        }
    }

    // Display the left-factored grammar
    cout << "Left Factored Grammar:\n";
    for (const auto& rule : newGrammar) {
        cout << rule.first << " -> ";
        for (size_t i = 0; i < rule.second.size(); i++) {
            cout << rule.second[i];
            if (i != rule.second.size() - 1) cout << " | ";
        }
        cout << endl;
    }
}
int main() {
    map<string, vector<string>> grammar;
    int numRules;

    cout << "Enter the number of production rules: ";
    cin >> numRules;

    for (int i = 0; i < numRules; i++) {
        string nonTerminal, arrow, production;
        cout << "Enter rule (format: A -> a | b): ";
        cin >> nonTerminal >> arrow;

        vector<string> productions;
        while (cin.peek() != '\n') {
            cin >> production;
            if (production != "|") productions.push_back(production);
        }

        grammar[nonTerminal] = productions;
    }

    leftFactorGrammar(grammar);

    return 0;
}