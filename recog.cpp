#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

bool alpha(char c) {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9');
}

pair<string, string> start_state(char c) {
    if (c == '<') {
        return {"CHECK", ""};
    }
    return {"INVALID", ""};
}

pair<string, string> check_state(char c, string curr_tag) {
    if (c == '/') {
        curr_tag += c;
        return {"TAG_CLOSE", curr_tag};
    } else if (alpha(c)) {
        curr_tag += c;
        return {"TAG_NAME", curr_tag};
    }
    return {"INVALID", curr_tag};
}

pair<string, string> tag_name_state(char c, string curr_tag) {
    if (alpha(c)) {
        curr_tag += c;
        return {"TAG_NAME", curr_tag};
    } else if (c == '>') {
        return {"END", curr_tag};
    }
    return {"INVALID", curr_tag};
}

pair<string, string> tag_close_state(char c, string curr_tag) {
    if (alpha(c)) {
        curr_tag += c;
        return {"TAG_CLOSE", curr_tag};
    } else if (c == '>') {
        return {"END", curr_tag};
    }
    return {"INVALID", curr_tag};
}

pair<string, string> end_state(char c, string curr_tag) {
    return {"INVALID", curr_tag};
}

string recog(const string& token) {
    string curr_state = "START";
    string curr_tag = "";
    vector<string> acc_tags = {"html", "head", "body", "title", "h1", "p"};

    for (char c : token) {
        if (curr_state == "START") {
            auto [new_state, new_tag] = start_state(c);
            curr_state = new_state;
            curr_tag = new_tag;
        } else if (curr_state == "CHECK") {
            auto [new_state, new_tag] = check_state(c, curr_tag);
            curr_state = new_state;
            curr_tag = new_tag;
        } else if (curr_state == "TAG_NAME") {
            auto [new_state, new_tag] = tag_name_state(c, curr_tag);
            curr_state = new_state;
            curr_tag = new_tag;
        } else if (curr_state == "TAG_CLOSE") {
            auto [new_state, new_tag] = tag_close_state(c, curr_tag);
            curr_state = new_state;
            curr_tag = new_tag;
        } else if (curr_state == "END") {
            auto [new_state, new_tag] = end_state(c, curr_tag);
            curr_state = new_state;
            curr_tag = new_tag;
        } else if (curr_state == "INVALID") {
            break;
        }
    }

    if (curr_state == "END" && (find(acc_tags.begin(), acc_tags.end(), curr_tag) != acc_tags.end() ||
        (curr_tag.size() > 1 && curr_tag[0] == '/' && find(acc_tags.begin(), acc_tags.end(), curr_tag.substr(1)) != acc_tags.end()))) {
        return "ACCEPTED";
    }

    return "REJECTED";
}

int main() {
    cout << "Masukkan token HTML (ketik 'exit' untuk keluar):" << endl;
    string token;

    while (true) {
        cout << "Token: ";
        getline(cin, token);
        string tag = token;
        transform(tag.begin(), tag.end(), tag.begin(), ::tolower); // Convert to lowercase

        if (tag == "exit") {
            break;
        }

        string result = recog(tag);
        cout << "Token: " << token << ", Result: " << result << endl;
    }

    return 0;
}
