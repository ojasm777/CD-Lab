#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <regex>

using namespace std;

class TokenAnalyzer
{
private:
    unordered_set<string> keywords = {
        "int", "float", "double", "char", "void", "return",
        "if", "else", "while", "for", "switch", "case",
        "break", "continue", "const", "class", "struct", "main"};

    unordered_set<string> operators = {
        "+", "-", "*", "/", "%", "=", "==", "!=",
        "<", ">", "<=", ">=", "&&", "||", "!",
        "&", "|", "^", "~", "<<", ">>", "+=",
        "-=", "*=", "/=", "%="};

    unordered_set<string> punctuations = {
        "(", ")", "{", "}", ";", ","};

    vector<pair<string, string>> tokenCategories;

    bool isSpecialChar(char c)
    {
        string specialChars = "+-*/=<>(){}[];,";
        return specialChars.find(c) != string::npos;
    }

    vector<string> tokenize(const string &code)
    {
        vector<string> tokens;
        string currentToken;

        for (size_t i = 0; i < code.length(); ++i)
        {
            char c = code[i];

            if (isspace(c))
            {
                if (!currentToken.empty())
                {
                    tokens.push_back(currentToken);
                    currentToken.clear();
                }
                continue;
            }

            if (isSpecialChar(c))
            {
                if (!currentToken.empty())
                {
                    tokens.push_back(currentToken);
                    currentToken.clear();
                }
                tokens.push_back(string(1, c));
                continue;
            }

            currentToken += c;
        }

        if (!currentToken.empty())
        {
            tokens.push_back(currentToken);
        }

        return tokens;
    }

    string categorizeToken(const string &token)
    {
        if (isKeyword(token))
            return "Keyword";
        if (isOperator(token))
            return "Operator";
        if (isPunctuation(token))
            return "Punctuation";
        if (isIdentifier(token))
            return "Identifier";
        if (isNumericLiteral(token))
            return "Numeric Literal";
        if (isStringLiteral(token))
            return "String Literal";
        return "Unknown";
    }

public:
    bool isKeyword(const string &token)
    {
        return keywords.find(token) != keywords.end();
    }

    bool isOperator(const string &token)
    {
        return operators.find(token) != operators.end();
    }

    bool isPunctuation(const string &token)
    {
        return punctuations.find(token) != punctuations.end();
    }

    bool isIdentifier(const string &token)
    {
        if (token.empty())
            return false;

        regex identifierPattern("^[a-zA-Z_][a-zA-Z0-9_]*$");
        return regex_match(token, identifierPattern);
    }

    bool isNumericLiteral(const string &token)
    {
        regex numericPattern("^[0-9]+(\\.[0-9]+)?([eE][-+]?[0-9]+)?$");
        return regex_match(token, numericPattern);
    }

    bool isStringLiteral(const string &token)
    {
        regex stringPattern("^'.'$|^\".*\"$");
        return regex_match(token, stringPattern);
    }

    void analyzeTokens(const string &code)
    {
        tokenCategories.clear();
        vector<string> tokens = tokenize(code);

        for (const auto &token : tokens)
        {
            string category = categorizeToken(token);
            tokenCategories.push_back({token, category});
        }

        printTokens();
    }

    void printTokens()
    {
        cout << "Token Analysis:\n";
        for (const auto &p : tokenCategories)
        {
            auto token = p.first;
            auto category = p.second;
            cout << "Token: " << token << " | Category: " << category << endl;
        }
    }
};

int main()
{
    TokenAnalyzer analyzer;

    string sampleCode = R"(
          int main() {
              int x = 10;  
              
              if (x > 5) {
                  return x + 20;
              }
              
              return 0;
          }
      )";

    analyzer.analyzeTokens(sampleCode);
    return 0;
}