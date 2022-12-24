#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <set>
#include <ctime>

/*
       21. Имеется N костей игры домино. На каждой кости имеется 2
    числа (каждое от 0 до 6). Кости могут повторяться. Требуется написать
    программу, которая будет определять максимальное число, составленное
    из цифр на костях цепочки, составленной по правилам домино из
    имеющихся костей. Время счета до 1 с.
       Ввод. Первая строка входного файла содержит целое число N –
    количество костей (2 <= N <= 15). Следующие N строк содержат 2
    целых числа X и Y (0 <= X, Y <= 6), разделенные пробелом.
       Вывод. В выходной файл необходимо вывести максимальное целое
    число, цифры которого соответствуют значению костей в цепочке(12).

*/
using namespace std;

class Domino
{
public:
    int p1 = 0;
    int p2 = 0;
    Domino() {
        p1 = -1;
        p2 = -1;
    }
    Domino(int p1_, int p2_) {
        p1 = p1_;
        p2 = p2_;
    }
    bool cmp(int p1_, int p2_) {
        if (((p1 == p1_) && (p2 == p2_)) || ((p1 == p2_) && (p2 == p1_)))
            return true;
        return false;
    }
};

typedef map<int, vector<Domino*> > LINK_TYPE;
typedef vector<Domino*> DOMINOS_TYPE;
typedef set<Domino*> MARKED_TYPE;

static LINK_TYPE links;
static DOMINOS_TYPE dominos;

string lex_cmp(const string& s1, const string& s2) {
    if (s1.size() > s2.size()) {
        return s1;
    }
    if (s2.size() > s1.size()) {
        return s2;
    }

    for (int i = 0; i < s1.size(); ++i) {
        if (s1[i] > s2[i]) {
            return s1;
        }
        if (s2[i] > s1[i]) {
            return s2;
        }
    }

    return s1;
}

void build_links() {
    for (int i = 0; i < dominos.size(); ++i) {
        if (dominos[i]->p1 != dominos[i]->p2) {
            if (links.find(dominos[i]->p1) == links.end()) {
                links[dominos[i]->p1] = vector<Domino*>();
            }
            links[dominos[i]->p1].push_back(dominos[i]);

            if (links.find(dominos[i]->p2) == links.end()) {
                links[dominos[i]->p2] = vector<Domino*>();
            }
            links[dominos[i]->p2].push_back(dominos[i]);
        }
        else {
            if (links.find(dominos[i]->p1) == links.end()) {
                links[dominos[i]->p1] = vector<Domino*>();
            }
            links[dominos[i]->p1].push_back(dominos[i]);
        }
    }
}

class Ref {
public:
    int key;
    int count;
    Ref() {
        key = -1;
        count = -1;
    }
    Ref(int key_, int count_) {
        key = key_;
        count = count_;
    }
    friend bool operator< (const Ref& left, const Ref& right);
};

bool operator< (const Ref& left, const Ref& right) {
    return left.key < right.key;
}

Ref get_ref(int key, MARKED_TYPE& marked) {
    DOMINOS_TYPE d = links[key];
    int cnt = 0;
    for (int i = 0; i < d.size(); ++i) {
        Domino* curr = d[i];
        if (marked.count(curr) == 0) {
            ++cnt;
        }
    }
    Ref new_ref(key, cnt);
    // new_ref.key = key;
    // new_ref.count = cnt;
    return new_ref;
}

vector<Ref> get_links(int key, MARKED_TYPE& marked) {
    if (links.find(key) == links.end()) {
        return vector<Ref>();
    }

    DOMINOS_TYPE d = links[key];
    set<Ref> ks;
    int selfref_cnt = 0;

    int p1, p2;

    for (Domino* curr_domino : d) {
        if (marked.count(curr_domino) != 0)
            continue;

        p1 = curr_domino->p1;
        p2 = curr_domino->p2;

        if (p1 != key) {
            auto c_ref = get_ref(p1, marked);
            ks.insert(c_ref);
        }
        if (p2 != key) {
            auto c_ref = get_ref(p2, marked);
            ks.insert(c_ref);
        }
        if ((p1 == key) && (p2 == key)) ++selfref_cnt;
    }

    if (selfref_cnt != 0) {
        Ref c_ref;
        c_ref.key = key;
        c_ref.count = selfref_cnt;
        ks.insert(c_ref);
    }
    return vector<Ref>(ks.begin(), ks.end());
}

class Node {
public:
    int key;
    vector<Node*> child;
    Node() {
        key = -1;
    }
    Node(int key_) {
        key = key_;
    }
};

class TopTrace {
public:
    string trace_res;
    TopTrace() {
        trace_res = "";
    }

    void update(const string& trace_res_) {
        trace_res = lex_cmp(trace_res_, trace_res);
    }
};

static TopTrace tracer;

void builder(Node* root, MARKED_TYPE marked, vector<Ref> refs, int stack_pos = 0, string trace_res = "") {
    if (refs.size() == 0) {
        tracer.update(trace_res);
    }

    for (Ref& ref : refs) {
        Node* c_node = new Node(ref.key);
        root->child.push_back(c_node);

        MARKED_TYPE new_marked = marked;

        for (Domino* K : dominos) {
            if (K->cmp(ref.key, root->key) && (marked.count(K) == 0)) {
                new_marked.insert(K);
                break;
            }
        }

        string trace_pt = to_string(root->key) + to_string(ref.key);
        builder(c_node, new_marked, get_links(ref.key, new_marked), stack_pos + 1, trace_res + trace_pt);
    }
}

void builder_prepare() {
    for (auto const& K : links) {
        int key = K.first;
        Node* root = new Node(key);
        MARKED_TYPE marked;
        vector<Ref> refs = get_links(key, marked);
        builder(root, marked, refs);
    }
}


DOMINOS_TYPE readDominos(string inputFile)
{
    std::ifstream input(inputFile);
    int n = 0;
    input >> n;

    DOMINOS_TYPE ret(n);
    for (int i = 0; i < n; ++i) {
        auto d = new Domino();
        input >> d->p1 >> d->p2;
        ret[i] = d;
    }
    return ret;
}

void saveResult(const std::string& result, string outputFile)
{
    std::ofstream output(outputFile);
    output << result;
}


int main()
{
    //clock_t timeSt = clock();
    fstream check;
    string inputFile;
    string outputFile;

    //string inputFile = "input.txt";
    //string outputFile = "output.txt";

    setlocale(LC_ALL, "ru");
    cout << "Введите имя input-файла: ";
    cin >> inputFile;
    check.open(inputFile);
    if (!check.is_open())
    {
        cout << "Файл " << inputFile << " не открывается. По умолчанию используется input.txt" << endl;
        inputFile = "input.txt";
    }
    check.close();
    cout << "Введите имя output-файла: ";
    cin >> outputFile;
    check.open(outputFile);
    if (!check.is_open())
    {
        cout << "Файл " << outputFile << " не открывается. По умолчанию используется output.txt" << endl;
        outputFile = "output.txt";
    }
    check.close();
    clock_t timeSt = clock();

    dominos = readDominos(inputFile);
    build_links();
    tracer = TopTrace();

    builder_prepare();

    cout << tracer.trace_res << endl;
    saveResult(tracer.trace_res, outputFile);
    cout << (clock() - timeSt) / (double)CLOCKS_PER_SEC << endl;
    return 0;
}

