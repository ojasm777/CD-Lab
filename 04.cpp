#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <regex>
using namespace std;

class LexicalAnalyzer
{
    enum TokenType
    {
        KEYWORD,
        IDENTIFIER,
        OPERATOR,
        LITERAL,
        PUNCTUATION,
        UNKNOWN
    };
    struct Token
    {
        string value;
        TokenType type;
        int lineNumber;
    };

    const unordered_set<string> keywords = {"int", "float", "char", "return", "if", "else", "while", "for"};
    const unordered_set<string> operators = {"+", "-", "*", "/", "=", "==", "!=", "<", ">", "<=", ">="};
    const unordered_set<string> punctuation = {"(", ")", "{", "}", ";", ","};

    vector<Token> tokens;

    TokenType identifyType(const string &token)
    {
        if (keywords.count(token))
            return KEYWORD;
        if (operators.count(token))
            return OPERATOR;
        if (punctuation.count(token))
            return PUNCTUATION;
        if (regex_match(token, regex("^[a-zA-Z_][a-zA-Z0-9_]*$")))
            return IDENTIFIER;
        if (regex_match(token, regex("^[-+]?[0-9]*\\.?[0-9]+$")) || regex_match(token, regex("^'.'$")) || regex_match(token, regex("^\".*\"$")))
            return LITERAL;
        return UNKNOWN;
    }

    void addToken(const string &token, int lineNumber)
    {
        if (!token.empty())
            tokens.push_back({token, identifyType(token), lineNumber});
    }

    void tokenize(const string &input)
    {
        istringstream iss(input);
        string line, token;
        int lineNumber = 1;

        while (getline(iss, line))
        {
            line = regex_replace(line, regex("//.*"), ""); // Remove comments
            istringstream lineStream(line);
            while (lineStream >> token)
                addToken(token, lineNumber);
            lineNumber++;
        }
    }

public:
    void analyze(const string &code)
    {
        tokens.clear();
        tokenize(code);
    }
    void printTokens()
    {
        for (const auto &t : tokens)
            cout << "Line " << t.lineNumber << " | " << t.value << " | "
                 << (t.type == KEYWORD ? "Keyword" : t.type == IDENTIFIER ? "Identifier"
                                                 : t.type == OPERATOR     ? "Operator"
                                                 : t.type == LITERAL      ? "Literal"
                                                 : t.type == PUNCTUATION  ? "Punctuation"
                                                                          : "Unknown")
                 << endl;
    }
};

int main()
{ 
    string sampleCode = R"(
          int main() {
              int x = 10; // Integer
              if (x > 5) return x + 20;
              return 0;
          }
      )";

    LexicalAnalyzer lexer;
    lexer.analyze(sampleCode);
    lexer.printTokens();  
}