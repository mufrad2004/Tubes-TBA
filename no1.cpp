#include <iostream>
#include <string>

using namespace std;

typedef struct elmHTML *adrHTML;
struct elmHTML {
    char info;
    adrHTML next;   
    adrHTML prev;
};

struct listHTML {
    adrHTML first;
    adrHTML last;
};

string tagYangDigunakan[6] = {"html" , "head", "body" , "title","h1" , "div"};

string toLowercase(string input) {
    string result = input;
    for (char& c : result) {
        c = tolower(c);
    }
    return result;
}


bool cekTagYangDigunakan(string tag) {
    for (int i = 0; i < 6; i++) {
        if (tag == tagYangDigunakan[i]) {
            return true;
        }
    }
    return false;
}


void buatList(listHTML &l) {
    l.first = NULL;
    l.last = NULL;
}


void insertTagHTML(listHTML &l, string input){
    adrHTML p,q;
    for (char isi : input){
        p = new elmHTML;
        p->info = isi;
        if (l.first == NULL){
            l.first = p;
            l.last = p;
        }else {
            q = l.last;
            q->next = p;
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



bool cekTagHTML(listHTML l){
    adrHTML p;
    string tag = "";
    if (l.first->info != '<' || l.last->info != '>'){
        return false;
    }else {
        p = l.first->next;
        if(p->info != '/'){
            tag += p->info;
            while(p->next != l.last){
                p = p->next;
                tag+= p->info;
            }
        }else {
            p = p->next;
            while(p != l.last){
                tag+= p->info;
                p = p->next;
            }
            p = p->next;

        }
    }
    p = l.first ->next;
    return cekTagYangDigunakan(tag);
}

void hapusList(listHTML &l){
    adrHTML p = l.first;
    while( p!= NULL){
        adrHTML temp = p;
        p = p->next;
        delete temp;
    }
    l.first = NULL;
    l.first = NULL;
}

int main(){
    listHTML l;
    string input, lowCase;
    buatList(l);
    input = "123910ifjodkvn";

    while (input != "exit"){
        cout << "Tag : ";
        cin >> input;
        lowCase = toLowercase(input);
        insertTagHTML(l,lowCase);

        if(cekTagHTML(l)){
            cout << lowCase << " : " << "Accepted" << endl;
        }else {
            cout << lowCase << " : " << "Rejected" << endl;
        }

        hapusList(l);
    }
    

    return 0;
}

