#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <unordered_map>

using namespace std;

// Fungsi untuk mengecek apakah sebuah karakter adalah alfanumerik
bool alpha(char c) {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9');
}

// Fungsi untuk menemukan posisi penutupan tag HTML
int closetag(const string& html, int start) {
    int pos = start;
    while (pos < html.size()) {
        if (html[pos] == '>') {
            return pos;
        }
        ++pos;
    }
    return -1;
}

// Fungsi untuk melakukan tokenisasi HTML
vector<string> tagHTML(const string& html_content) {
    vector<string> tokens;
    int pos = 0;

    while (pos < html_content.size()) {
        if (html_content[pos] == '<') {
            int endpos = closetag(html_content, pos);
            string token = html_content.substr(pos, endpos - pos + 1);
            tokens.push_back(token);
            pos = endpos + 1;
        } else {
            ++pos;
        }
    }

    return tokens;
}

// Fungsi untuk melakukan parsing dengan menggunakan stack dan parse table
bool parser(const vector<string>& tokens, const unordered_map<string, unordered_map<string, string>>& parse_table) {
    stack<string> parseStack;
    parseStack.push("#");
    parseStack.push("S");
    vector<string> input(tokens);
    input.push_back("EOS");
    size_t i = 0;

    while (parseStack.top() != "#") {
        string top = parseStack.top();
        parseStack.pop();
        string read = input[i];

        if (top == read) {
            ++i;
        } else if (parse_table.find(top) != parse_table.end() && parse_table.at(top).find(read) != parse_table.at(top).end()) {
            string production = parse_table.at(top).at(read);
            if (!production.empty()) {
                vector<string> productionTokens = tagHTML(production);
                for (auto it = productionTokens.rbegin(); it != productionTokens.rend(); ++it) {
                    parseStack.push(*it);
                }
            }
        } else {
            return false;
        }
    }

    return parseStack.top() == "#" && i == tokens.size();
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "Usage: ./html_parser <filename>" << endl;
        return 1;
    }

    string filename = argv[1];
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "File not found: " << filename << endl;
        return 1;
    }

    string html_content;
    string line;
    while (getline(file, line)) {
        html_content += line;
    }
    file.close();

    cout << "Isi file tag pada file HTML:" << endl;
    cout << html_content << endl;

    vector<string> tokens = tagHTML(html_content);

    unordered_map<string, unordered_map<string, string>> parse_table = {
        {"S", {{"<html>", "<html> A </html>"}}},
        {"A", {{"<head>", "<head> D </head> B"}, {"<body>", "B"}, {"</html>", ""}}},
        {"B", {{"<body>", "<body> C </body>"}, {"</html>", ""}}},
        {"C", {{"<h1>", "<h1> </h1> C"}, {"<p>", "<p> </p> C"}, {"</body>", ""}}},
        {"D", {{"<title>", "<title> </title>"}, {"</head>", ""}}}
    };

    if (parser(tokens, parse_table)) {
        cout << "Accepted" << endl;
    } else {
        cout << "Rejected" << endl;
    }

    return 0;
}
