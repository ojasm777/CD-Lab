#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <regex>
#include <algorithm>

class LexicalAnalyzer
{
private:
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
        std::string value;
        TokenType type;
        int lineNumber;
    };

    const std::unordered_set<std::string> keywords = {
        "int", "float", "double", "char", "void", "return",
        "if", "else", "while", "for", "switch", "case",
        "break", "continue", "const", "class", "struct"
    };

    const std::unordered_set<std::string> operators = {
        "+", "-", "*", "/", "%", "=", "==", "!=",
        "<", ">", "<=", ">=", "&&", "||", "!",
        "&", "|", "^", "~", "<<", ">>", "+=",
        "-=", "*=", "/=", "%="
    };

    const std::unordered_set<std::string> punctuation = {
        "(", ")", "{", "}", "[", "]", ";", ",", "."
    };

    std::vector<Token> tokens;

    TokenType identifyTokenType(const std::string &token)
    {
        // Check for keywords, operators, and punctuation first
        if (keywords.count(token))
            return KEYWORD;
        if (operators.count(token))
            return OPERATOR;
        if (punctuation.count(token))
            return PUNCTUATION;

        // Check for identifiers (variable names)
        if (std::regex_match(token, std::regex("^[a-zA-Z_][a-zA-Z0-9_]*$")))
            return IDENTIFIER;

        // Check for literals (numbers, characters, strings)
        if (std::regex_match(token, std::regex("^[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?$")) || // Numbers
            std::regex_match(token, std::regex("^'.'$")) || // Character literals
            std::regex_match(token, std::regex("^\".*\"$"))) // String literals
            return LITERAL;

        return UNKNOWN;
    }

    void tokenize(const std::string &input)
    {
        std::istringstream iss(input);
        std::string line;
        int lineNumber = 1;

        while (std::getline(iss, line))
        {
            // Remove comments (single-line C++ style comments)
            size_t commentPos = line.find("//");
            if (commentPos != std::string::npos)
                line = line.substr(0, commentPos);

            // Remove leading and trailing whitespace
            line.erase(0, line.find_first_not_of(" \t\r\n"));
            line.erase(line.find_last_not_of(" \t\r\n") + 1);

            if (line.empty())
            {
                lineNumber++;
                continue;
            }

            // Advanced tokenization to handle operators and punctuation
            std::string currentToken;
            for (size_t i = 0; i < line.length(); ++i)
            {
                char c = line[i];

                // Handle string literals
                if (c == '"')
                {
                    size_t endQuote = line.find('"', i + 1);
                    if (endQuote != std::string::npos)
                    {
                        std::string stringLiteral = line.substr(i, endQuote - i + 1);
                        addToken(stringLiteral, lineNumber);
                        i = endQuote;
                        continue;
                    }
                }

                // Handle character literals
                if (c == '\'')
                {
                    size_t endQuote = line.find('\'', i + 1);
                    if (endQuote != std::string::npos)
                    {
                        std::string charLiteral = line.substr(i, endQuote - i + 1);
                        addToken(charLiteral, lineNumber);
                        i = endQuote;
                        continue;
                    }
                }

                // Check for multi-character operators
                if (isOperatorStart(c))
                {
                    std::string potentialOperator = extractOperator(line, i);
                    if (!potentialOperator.empty())
                    {
                        addToken(currentToken, lineNumber);
                        currentToken.clear();
                        addToken(potentialOperator, lineNumber);
                        i += potentialOperator.length() - 1;
                        continue;
                    }
                }

                // Check for punctuation
                if (isPunctuation(c))
                {
                    addToken(currentToken, lineNumber);
                    currentToken.clear();
                    addToken(std::string(1, c), lineNumber);
                    continue;
                }

                // Build tokens
                if (std::isspace(c))
                {
                    addToken(currentToken, lineNumber);
                    currentToken.clear();
                }
                else
                {
                    currentToken += c;
                }
            }

            // Add last token of the line
            addToken(currentToken, lineNumber);
            lineNumber++;
        }
    }

    void addToken(const std::string &token, int lineNumber)
    {
        if (!token.empty())
        {
            Token t;
            t.value = token;
            t.type = identifyTokenType(token);
            t.lineNumber = lineNumber;
            tokens.push_back(t);
        }
    }

    bool isOperatorStart(char c)
    {
        return c == '+' || c == '-' || c == '*' || c == '/' || 
               c == '=' || c == '<' || c == '>' || c == '!' || 
               c == '&' || c == '|';
    }

    std::string extractOperator(const std::string &line, size_t &pos)
    {
        std::vector<std::string> multiCharOperators = {
            "==", "!=", "<=", ">=", "&&", "||", 
            "<<", ">>", "+=", "-=", "*=", "/=", "%="
        };

        for (const auto &op : multiCharOperators)
        {
            if (line.substr(pos, op.length()) == op)
            {
                return op;
            }
        }
        return "";
    }

    bool isPunctuation(char c)
    {
        return punctuation.count(std::string(1, c)) > 0;
    }

    std::string getTokenTypeName(TokenType type)
    {
        switch (type)
        {
        case KEYWORD:    return "Keyword";
        case IDENTIFIER: return "Identifier";
        case OPERATOR:   return "Operator";
        case LITERAL:    return "Literal";
        case PUNCTUATION:return "Punctuation";
        default:         return "Unknown";
        }
    }

public:
    void analyze(const std::string &code)
    {
        tokens.clear();
        tokenize(code);
    }

    void printTokens()
    {
        std::cout << "Lexical Analysis:\n";
        for (const auto &token : tokens)
        {
            std::cout << "Line " << token.lineNumber
                      << " | Token: " << token.value
                      << " | Type: " << getTokenTypeName(token.type) << std::endl;
        }
    }

    std::vector<Token> getTokens()
    {
        return tokens;
    }
};

int main()
{
    LexicalAnalyzer lexer;

    std::string sampleCode = R"(
        int main() {
            int x = 10;  // Integer declaration
            float y = 3.14;  // Float declaration
            char z = 'a';  // Character declaration
            
            if (x > 5) {
                return x + 20;
            }
            
            return 0;
        }
    )";

    lexer.analyze(sampleCode);
    lexer.printTokens();

    return 0;
}