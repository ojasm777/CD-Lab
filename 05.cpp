#include <iostream>
#include <cctype>
using namespace std;

class RecursiveDescentParser
{
    string input;
    int pos;

    char peek() { return pos < input.length() ? input[pos] : '\0'; }
    void advance() { pos++; }

    bool match(char expected)
    {
        if (peek() == expected)
        {
            advance();
            return true;
        }
        return false;
    }

    // Grammar Parsing Functions
    bool E() { return T() && EPrime(); }                          // E → T E'
    bool EPrime() { return match('+') ? T() && EPrime() : true; } // E' → + T E' | ε
    bool T() { return F() && TPrime(); }                          // T → F T'
    bool TPrime() { return match('*') ? F() && TPrime() : true; } // T' → * F T' | ε
    bool F()
    { // F → (E) | id (number)
        if (isdigit(peek()))
        {
            advance();
            return true;
        }
        if (match('('))
        {
            if (E() && match(')'))
                return true;
        }
        return false;
    }

public:
    bool parse(string expr)
    {
        input = expr;
        pos = 0;
        return E() && pos == input.length(); // Ensure complete parsing
    }
};

int main()
{
    RecursiveDescentParser parser;
    string expr;

    cout << "\nEnter expression: ";
    cin >> expr;

    if (parser.parse(expr))
        cout << "Valid Expression \n\n";
    else
        cout << "Invalid Expression \n\n";

    return 0;
}