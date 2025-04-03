#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Function to eliminate left recursion
void removeLeftRecursion(string nonTerminal, vector<string>& productions) {
    vector<string> alpha, beta;
    string newNonTerminal = nonTerminal + "'";

    // Separating left-recursive and non-left-recursive rules
    for (const string& prod : productions) {
        if (prod.find(nonTerminal) == 0) { // Left recursion detected
            alpha.push_back(prod.substr(nonTerminal.length())); // Remove the left-recursive non-terminal
        } else {
            beta.push_back(prod); // Non-left-recursive rule
        }
    }

    // If left recursion exists, transform the grammar
    if (!alpha.empty()) {
        cout << "Converted Grammar:\n";

        // New production for the original non-terminal
        cout << nonTerminal << " -> ";
        for (int i = 0; i < beta.size(); i++) {
            cout << beta[i] << newNonTerminal;
            if (i < beta.size() - 1) cout << " | ";
        }
        cout << endl;

        // New productions for the introduced non-terminal
        cout << newNonTerminal << " -> ";
        for (int i = 0; i < alpha.size(); i++) {
            cout << alpha[i] << newNonTerminal;
            if (i < alpha.size() - 1) cout << " | ";
        }
        cout << " | ε\n"; // Adding epsilon to allow termination
    } else {
        cout << "No left recursion detected in " << nonTerminal << ".\n";
    }
}

int main() {
    string nonTerminal;
    int numProductions;

    cout << "Enter the non-terminal: ";
    cin >> nonTerminal;

    cout << "Enter the number of productions: ";
    cin >> numProductions;

    vector<string> productions;
    cout << "Enter the productions (each on a new line, without spaces):\n";

    cin.ignore();
    for (int i = 0; i < numProductions; i++) {
        string prod;
        getline(cin, prod);
        productions.push_back(prod);
    }

    removeLeftRecursion(nonTerminal, productions);

    return 0;
}