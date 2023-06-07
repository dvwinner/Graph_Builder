#include <cmath>
#include <algorithm>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

using namespace std;

typedef int64_t ll;

struct Stack {
    double number;
    char sign;
    Stack* next;
};

Stack* top1 = nullptr;
Stack* top2 = nullptr;
stack <ll> psp;

void push(Stack*&t, double num, char znak) {
    Stack *p = (Stack*)malloc(sizeof(Stack));
    (*p).number = num;
    (*p).sign = znak;
    (*p).next = t;
    t = p;
}

Stack pop(Stack*&t) {
    Stack st;
    st.sign = (*t).sign;
    st.number = (*t).number;
    t = (*t).next;
    return st;
}

int priority(char o) {
    if (o == '(') {
        return 0;
    } else if (o == '+' || o == '-') {
        return 1;
    } else if (o == '*' || o == '/') {
        return 2;
    } else if (o == '^') {
        return 3;
    } else if (o == 's' || o == 'c' || o == 't' || o == 'l' || o == 'q') {
        return 4;
    }
    return 5;
}

bool isNum(char ch) {
    if (ch >= '0' && ch <= '9') {
        return 1;
    } else {
        return 0;
    }
}

bool isOper2(char ch) {
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^') {
        return 1;
    } else if (ch == 's' || ch == 'c' || ch == 't' || ch == 'l' || ch == 'q') {
        return 1;
    } else {
        return 0;
    }
}

bool isOper(char ch) {
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^') {
        return 1;
    } else {
        return 0;
    }
}

bool isValid(string s) {
    if (isOper(s[0])) {
        return false;
    }
    ll len = s.size() - 1;
    if (isOper(s[len])) {
        return false;
    }
    for (ll i = 0; i < len; i++) {
        if ((s[i] == '(' && s[i + 1] == ')') || (s[i] == ')' && s[i + 1] == '(')) {
            return false;
        }
        if (isOper(s[i]) && isOper(s[i + 1])) {
            return false;
        }
        if ((isOper(s[i]) && s[i + 1] == ')') || (s[i] == '(' && isOper(s[i + 1]))) {
            return false;
        }
        if ((isNum(s[i]) && s[i + 1] == '(') || (s[i] == ')' && isNum(s[i + 1]))) {
            return false;
        }
        if (s[i] == '.' && !isNum(s[i + 1])) {
            return false;
        }
    }
    ll cnt = 0;
    for (ll i = 0; i <= len; i++) {
        if (s[i] == '(') {
            cnt++;
        } else if (s[i] == ')') {
            cnt--;
        }
        if (cnt < 0) {
            return false;
        }
    }
    if (cnt == 0) {
        return 1;
    } else {
        return 0;
    }
    return 1;
}

string fixStr(string s_unfixed) {
    string s;
    if (s_unfixed[0] == '-') {
        s_unfixed = '0' + s_unfixed;
    }
    int size = s_unfixed.size();
    for (int i = 0; i < size; i++) {
        if (s_unfixed[i] == '(' && s_unfixed[i + 1] == '-') {
            s = s + '(' + '0';
        } else if (s_unfixed[i] >= 'a' && s_unfixed[i] <= 'z') {
            if (s_unfixed[i] == 's' && s_unfixed[i + 1] == 'i') {
                s = s + 's';
                i += 2;
            } else if (s_unfixed[i] == 'c') {
                s = s + 'c';
                i += 2;
            } else if (s_unfixed[i] == 't') {
                s = s + 't';
                i += 1;
            } else if (s_unfixed[i] == 's' && s_unfixed[i + 1] == 'q') {
                s = s + 'q';
                i += 3;
            } else if (s_unfixed[i] == 'l') {
                s = s + 'l';
                i += 1;
            } else if (s_unfixed[i] == '.'){
                // vse ok
            } else {
                return "Error.";
            }
        } else {
            s = s + s_unfixed[i];
        }
    }
    return s;
}

string solve(char ch) {
    double a = pop(top1).number;
    double num;
    if (!isOper(ch)) {
        if (ch == 's') {
            num = sin(a);
        } else if (ch == 'c') {
            num = cos(a);
        } else if (ch == 't') {
            num = tan(a);
        } else if (ch == 'q') {
            if (a < 0) {
                return "Error.";
            }
            num = sqrt(a);
        } else if (ch == 'l') {
            if (a <= 0) {
                return "Error.";
            }
            num = log(a);
        }
    } else {
        double b = pop(top1).number;
        if (ch == '*') {
            num = a * b;
        } else if (ch == '-') {
            num = b - a;
        } else if (ch == '+') {
            num = b + a;
        } else if (ch == '/') {
            if (a == 0) {
                return "Error.";
            }
            num = b / a;
        } else if (ch == '^') {
            num = pow(b, a);
        }
    }
    push(top1, num, '#');
    return "ok";
}

void clearAll() {
    top1 = nullptr;
    top2 = nullptr;
    while (psp.size()) {
        psp.pop();
    }
    return;
}

string calc(string s_unfixed, double &answer) {
    string l;
    string s = fixStr(s_unfixed);
    if (!isValid(s) || s == "Error.") {
        return "Error.";
    }
    int size = s.size();
    for (int i = 0; i < size; i++) {
        if (isNum(s[i])) {
            while (isNum(s[i])) {
                l = l + s[i];
                i++;
            }
            if (s[i] == '.') {
                l = l + '.';
                i++;
                while (isNum(s[i])) {
                    l = l + s[i];
                    i++;
                }
            }
            push(top1, stod(l), '#');
            l = "";
        }
        if (s[i] == '(') {
            push(top2, 0.0, s[i]);
        }
        if (s[i] == ')') {
            while ((*top2).sign != '(') {
                string rezz = solve(pop(top2).sign);
                if (rezz == "Error.") {
                    return "Error.";
                }
            }
            pop(top2);
        }
        if (isOper2(s[i])) {
            while (top2 != nullptr && priority(s[i]) <= priority((*top2).sign)) {
                string rezz = (solve(pop(top2).sign));
                if (rezz == "Error.") {
                    clearAll();
                    return "Error.";
                }
            }
            push(top2, 0.0, s[i]);
        }
    }
    while (top2 != nullptr) {
        string rezz = solve(pop(top2).sign);
        if (rezz == "Error.") {
            return "Error.";
        }
    }
    answer = ((*top1).number);
    clearAll();
    return "Ok.";
}
