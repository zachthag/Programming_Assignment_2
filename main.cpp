#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <sstream>
#include <iomanip>
#include <stdexcept>

#include "ArrayStack.h"

using namespace std;

struct Token {
    string value;
};

bool isOperator(const string& s) {
    return s == "+" || s == "-" || s == "*" || s == "/";
}

bool isNumber(const string& s) {
    if (s.empty()) {
        return false;
    }

    for (char ch : s) {
        if (!isdigit(static_cast<unsigned char>(ch))) {
            return false;
        }
    }

    return true;
}

vector<Token> tokenize(const string& line) {
    vector<Token> tokens;
    string current = "";

    for (size_t i = 0; i < line.size(); i++) {
        char ch = line[i];

        if (isspace(static_cast<unsigned char>(ch))) {
            if (!current.empty()) {
                Token t;
                t.value = current;
                tokens.push_back(t);
                current = "";
            }
        }
        else if (isdigit(static_cast<unsigned char>(ch))) {
            current += ch;
        }
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(' || ch == ')') {
            if (!current.empty()) {
                Token t;
                t.value = current;
                tokens.push_back(t);
                current = "";
            }

            string one(1, ch);
            Token t;
            t.value = one;
            tokens.push_back(t);
        }
        else {
            if (!current.empty()) {
                Token t;
                t.value = current;
                tokens.push_back(t);
                current = "";
            }

            string bad(1, ch);
            Token t;
            t.value = bad;
            tokens.push_back(t);
        }
    }

    if (!current.empty()) {
        Token t;
        t.value = current;
        tokens.push_back(t);
    }

    return tokens;
}

int precedence(const string& op) {
    if (op == "*" || op == "/") {
        return 2;
    }
    if (op == "+" || op == "-") {
        return 1;
    }
    return 0;
}

bool isValidPostfix(const vector<Token>& tokens) {
    if (tokens.empty()) {
        return false;
    }

    int depth = 0;

    for (size_t i = 0; i < tokens.size(); i++) {
        string val = tokens[i].value;

        if (isNumber(val)) {
            depth++;
        }
        else if (isOperator(val)) {
            if (depth < 2) {
                return false;
            }
            depth--; // two pop, one push
        }
        else {
            return false;
        }
    }

    return depth == 1;
}

bool isValidInfix(const vector<Token>& tokens) {
    if (tokens.empty()) {
        return false;
    }

    int parenCount = 0;
    bool expectOperand = true;

    for (size_t i = 0; i < tokens.size(); i++) {
        string val = tokens[i].value;

        if (expectOperand) {
            if (isNumber(val)) {
                expectOperand = false;
            }
            else if (val == "(") {
                parenCount++;
            }
            else {
                return false;
            }
        }
        else {
            if (isOperator(val)) {
                expectOperand = true;
            }
            else if (val == ")") {
                parenCount--;
                if (parenCount < 0) {
                    return false;
                }
            }
            else {
                return false;
            }
        }
    }

    if (parenCount != 0) {
        return false;
    }

    if (expectOperand) {
        return false;
    }

    return true;
}

vector<Token> infixToPostfix(const vector<Token>& tokens) {
    vector<Token> output;
    ArrayStack<string> ops;

    for (size_t i = 0; i < tokens.size(); i++) {
        string val = tokens[i].value;

        if (isNumber(val)) {
            Token t;
            t.value = val;
            output.push_back(t);
        }
        else if (val == "(") {
            ops.push(val);
        }
        else if (val == ")") {
            while (!ops.empty() && ops.top() != "(") {
                Token t;
                t.value = ops.top();
                output.push_back(t);
                ops.pop();
            }

            if (!ops.empty() && ops.top() == "(") {
                ops.pop();
            }
        }
        else if (isOperator(val)) {
            while (!ops.empty() && isOperator(ops.top()) && precedence(ops.top()) >= precedence(val)) {
                Token t;
                t.value = ops.top();
                output.push_back(t);
                ops.pop();
            }
            ops.push(val);
        }
    }

    while (!ops.empty()) {
        Token t;
        t.value = ops.top();
        output.push_back(t);
        ops.pop();
    }

    return output;
}

double applyOp(double left, double right, const string& op) {
    if (op == "+") {
        return left + right;
    }
    if (op == "-") {
        return left - right;
    }
    if (op == "*") {
        return left * right;
    }
    if (op == "/") {
        return left / right;
    }

    throw runtime_error("bad operator");
}

double evalPostfix(const vector<Token>& tokens) {
    ArrayStack<double> stack;

    for (size_t i = 0; i < tokens.size(); i++) {
        string val = tokens[i].value;

        if (isNumber(val)) {
            double num = stod(val);
            stack.push(num);
        }
        else if (isOperator(val)) {
            double right = stack.top();
            stack.pop();
            double left = stack.top();
            stack.pop();

            double ans = applyOp(left, right, val);
            stack.push(ans);
        }
    }

    return stack.top();
}

string formatResult(double value) {
    ostringstream out;
    out << fixed << setprecision(10) << value;
    string s = out.str();

    while (!s.empty() && s.back() == '0') {
        s.pop_back();
    }
    if (!s.empty() && s.back() == '.') {
        s.pop_back();
    }
    if (s.empty()) {
        s = "0";
    }
    return s;
}

string postfixToString(const vector<Token>& tokens) {
    string result = "";

    for (size_t i = 0; i < tokens.size(); i++) {
        result += tokens[i].value;
        if (i + 1 < tokens.size()) {
            result += " ";
        }
    }

    return result;
}

int main() {
    string line;
    getline(cin, line);

    vector<Token> tokens = tokenize(line);

    try {
        if (isValidPostfix(tokens)) {
            cout << "FORMAT: POSTFIX\n";
            cout << "RESULT: " << formatResult(evalPostfix(tokens)) << "\n";
        }
        else if (isValidInfix(tokens)) {
            vector<Token> postfix = infixToPostfix(tokens);
            cout << "FORMAT: INFIX\n";
            cout << "POSTFIX: " << postfixToString(postfix) << "\n";
            cout << "RESULT: " << formatResult(evalPostfix(postfix)) << "\n";
        }
        else {
            cout << "FORMAT: NEITHER\n";
            cout << "ERROR: invalid expression\n";
        }
    }
    catch (const exception&) {
        cout << "FORMAT: NEITHER\n";
        cout << "ERROR: invalid expression\n";
    }

    return 0;
}
