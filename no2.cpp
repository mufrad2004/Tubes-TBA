#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <regex>
using namespace std;
// =================================================================
// Untuk baca file html
string readFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }

    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    return content;
}

// Fungsi untuk mengekstrak tag HTML dari string menggunakan regex
vector<string> extractTags(const string& htmlContent) {
    vector<string> tags;
    regex tagRegex(R"(<[^>]+>)");
    auto tags_begin = sregex_iterator(htmlContent.begin(), htmlContent.end(), tagRegex);
    auto tags_end = sregex_iterator();

    for (sregex_iterator i = tags_begin; i != tags_end; ++i) {
        smatch match = *i;
        tags.push_back(match.str());
    }

    return tags;
}
// =================================================================

string extractContent(const string& tag) {
    size_t start = tag.find('<');
    size_t end = tag.find('>');
    
    if (start != string::npos && end != string::npos && start < end) {
        return tag.substr(start + 1, end - start - 1);
    } else {
        return ""; 
    }
}


string extractTagName(const string& tag) {
    // Pastikan tag dimulai dengan "/"
    if (tag.size() > 1 && tag[0] == '/') {
        return tag.substr(1); // Ambil substring setelah "/"
    } else {
        return ""; // Jika tidak dimulai dengan "/", kembalikan tag itu sendiri
    }
}

vector<string> wajibBener1 = {"<html>" ,"</html>"};
vector<string> wajibBener2 = {"<html>","<head>","</body>","</html>"};


void showAllArray(vector<string> tags){
    for (int i = 0 ; i< tags.size(); ++i){
        cout << tags[i] << endl;
    }
}


typedef struct elmHTML *adrHTML;
struct elmHTML {
    string info;
    adrHTML next;   
    adrHTML prev;
};

struct listHTML {
    adrHTML first;
    adrHTML last;
};

void buatList(listHTML &l) {
    l.first = NULL;
    l.last = NULL;
}


void insertTagHTML(listHTML &l , vector<string> tags){
    adrHTML p,q;
    for (string isi : tags){
        p = new elmHTML;
        p->info = isi;
        if (l.first == NULL){
            l.first = p;
            l.last = p;
        }else {
            q = l.last;
            q->next  = p;
            p->prev = q;
            l.last = p;
        }
    }
}

void printAllList(listHTML l){
    adrHTML p ;
    p = l.first;
    while (p != NULL){
        cout << p->info <<endl;
        p = p->next;
    }
}

int hitungElmList(listHTML l){
    adrHTML p;
    int hasil = 0;
    p = l.first;
    while (p != NULL){
        hasil++;
        p = p->next;
    }
    return hasil;
}

bool cekTagTutup(listHTML l , string tag){
    adrHTML p,q;
    p = l.first;

    while (p != NULL){
        if (p->info == tag){
            break;
        }
        p = p->next;
    }
    if (p == NULL){
        return false;
    }

    if(p->info[0] == '/'){
        return false;
    }else {
        q = p->next;
        while(q != NULL){
            if (extractTagName(extractContent(q->info)) == extractContent(p->info)){
                return true;
            }
            q = q->next;
        }
        return false;
    }
    return false;
}

bool cekStrukturHTML(listHTML l) {
    adrHTML p = l.first;
    adrHTML r = l.last;
    int i = 1;

    // Memeriksa kondisi wajib jika jumlah elemen adalah 2
    if (hitungElmList(l) == 2) {
        if ((p->info == wajibBener1[0]) && (r->info == wajibBener1[1])) {
            return true;
        } else {    
            return false;
        }
    } else {
        // Memeriksa kondisi wajib jika jumlah elemen lebih dari 2
        if ((p->info == wajibBener2[0]) && (r->info == wajibBener2[3]) && 
            (p->next->info == wajibBener2[1]) && (r->prev->info == wajibBener2[2])) {
            adrHTML q = p->next->next; // Mulai setelah </head>
            while (q != r->prev) { // Berhenti sebelum </body>
                if (q->info[1] != '/' && q->info != "<body>") {
                    if (!cekTagTutup(l, q->info)) {
                        return false;
                    }
                }
                q = q->next;
            }
            return true;
        } else {
            return false;
        }
    }
}


int main(int argc, char* argv[]) {
    // =================================================================
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }
    string filename = argv[1];
    string htmlContent = readFile(filename);
    vector<string> tags = extractTags(htmlContent);
    // =================================================================
    listHTML l;
    cout << endl << endl;
    buatList(l);
    insertTagHTML(l,tags);
    printAllList(l);
    cout << endl<<endl;


    if(cekStrukturHTML(l)){
        cout << "Accepted" <<endl;
    }else {
        cout << "Rejected" <<endl;
    }

    cout << endl;

    return 0;
}

