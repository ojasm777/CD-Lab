#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;

map<char, vector<string>> grammar; // Stores grammar rules
map<char, set<char>> firsts, follows; // FIRST and FOLLOW sets
set<char> nonTerminals, terminals;
char startSymbol;

// Function to compute FIRST set
void computeFirst(char symbol) {
    if (firsts[symbol].size() > 0) return; // Already computed

    for (string production : grammar[symbol]) {
        char firstChar = production[0];

        if (terminals.count(firstChar)) {
            firsts[symbol].insert(firstChar); // If terminal, add directly
        } else if (nonTerminals.count(firstChar)) {
            computeFirst(firstChar);
            firsts[symbol].insert(firsts[firstChar].begin(), firsts[firstChar].end()); // Copy FIRST of non-terminal
        }

        if (production == "ε") {
            firsts[symbol].insert('ε'); // If epsilon production
        }
    }
}

// Function to compute FOLLOW set
void computeFollow(char symbol) {
    if (follows[symbol].size() > 0) return; // Already computed

    if (symbol == startSymbol) {
        follows[symbol].insert('$'); // FOLLOW(start) = { $ }
    }

    for (auto rule : grammar) {
        for (string production : rule.second) {
            for (size_t i = 0; i < production.length(); i++) {
                if (production[i] == symbol) { // If symbol found in production
                    if (i + 1 < production.length()) {
                        char nextChar = production[i + 1];
                        if (terminals.count(nextChar)) {
                            follows[symbol].insert(nextChar);
                        } else if (nonTerminals.count(nextChar)) {
                            follows[symbol].insert(firsts[nextChar].begin(), firsts[nextChar].end());
                            follows[symbol].erase('ε'); // Remove epsilon
                        }
                    } else {
                        follows[symbol].insert(follows[rule.first].begin(), follows[rule.first].end());
                    }
                }
            }
        }
    }
}
int main() {
    int numProductions;
    cout << "Enter number of productions: ";
    cin >> numProductions;

    cout << "Enter grammar (e.g., A->aAb|ε):\n";
    for (int i = 0; i < numProductions; i++) {
        char nonTerminal;
        string arrow, production;
        cin >> nonTerminal >> arrow >> production;

        grammar[nonTerminal].push_back(production);
        nonTerminals.insert(nonTerminal);
        for (char ch : production) {
            if (isupper(ch) && ch != 'ε') terminals.insert(ch);
        }
    }

    cout << "Enter start symbol: ";
    cin >> startSymbol;

    // Compute FIRST and FOLLOW sets
    for (char nt : nonTerminals) computeFirst(nt);
    for (char nt : nonTerminals) computeFollow(nt);

    // Print FIRST sets
    cout << "\nFIRST sets:\n";
    for (auto &pair : firsts) {
        cout << "FIRST(" << pair.first << ") = { ";
        for (char ch : pair.second) cout << ch << " ";
        cout << "}\n";
    }

    // Print FOLLOW sets
    cout << "\nFOLLOW sets:\n";
    for (auto &pair : follows) {
        cout << "FOLLOW(" << pair.first << ") = { ";
        for (char ch : pair.second) cout << ch << " ";
        cout << "}\n";
    }

    return 0;
}