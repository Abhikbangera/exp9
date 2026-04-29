// Shift-Reduce Parser
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

int numProd;
string lhs[10];
string rhs[10];

string tryReduce(string stk) {
    for (int i = 0; i < numProd; i++) {
        int pos = stk.rfind(rhs[i]);
        if (pos != string::npos) {
            string reduced = stk.substr(0, pos) + lhs[i] + stk.substr(pos + rhs[i].size());
            return reduced;
        }
    }
    return "";
}

int main() {
    cout << "Enter number of productions: ";
    cin >> numProd;
    cout << "Enter productions (e.g. S->aSb):\n";
    for (int i = 0; i < numProd; i++) {
        string prod;
        cin >> prod;
        int arrow = prod.find("->");
        lhs[i] = prod.substr(0, arrow);
        rhs[i] = prod.substr(arrow + 2);
    }

    string input;
    cout << "Enter input string: ";
    cin >> input;
    input += "$";

    string stk = "$";
    int idx = 0;

    cout << "\n" << left << setw(20) << "Stack"
         << setw(20) << "Input" << "Action\n";
    cout << string(55, '-') << "\n";

    while (true) {
        string inp = input.substr(idx);
        string action = "";
        string newStk = "";

        newStk = tryReduce(stk);
        if (!newStk.empty()) {
            // Find which production was used
            for (int i = 0; i < numProd; i++) {
                int pos = stk.rfind(rhs[i]);
                if (pos != string::npos) {
                    action = "Reduce " + lhs[i] + "->" + rhs[i];
                    break;
                }
            }
            cout << left << setw(20) << stk << setw(20) << inp << action << "\n";
            stk = newStk;
        } else if (stk == "$S" && inp == "$") {
            cout << left << setw(20) << stk << setw(20) << inp << "Accept\n";
            break;
        } else if (idx < (int)input.size() && input[idx] != '$') {
            action = "Shift " + string(1, input[idx]);
            cout << left << setw(20) << stk << setw(20) << inp << action << "\n";
            stk += input[idx++];
        } else {
            cout << left << setw(20) << stk << setw(20) << inp << "Reject\n";
            break;
        }
    }
    return 0;
}
