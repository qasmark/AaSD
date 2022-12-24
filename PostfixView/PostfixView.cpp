#include <iostream>
#include <cmath>
#include <fstream>
#include <map>

using namespace std;

/*  23. В строке текстового файла  задано  выражение  из  целых
чисел и операций '+', '-', '*', '/', '^'.  Порядок  вычислений
определяется приоритетом операций и круглыми скобками. Возможен
одноместный минус в начале  выражения  или  после  открывающей
скобки. Преобразовать выражение в постфиксную  форму (алгоритм
Дейкстры) и вычислить его значение. Показать этапы  выполнения
(11).
*/

template <typename T>

struct NodeStack
{
    T item;
    NodeStack<T>* next;
};

template <typename T>

class StackList
{
private:
    NodeStack<T>* pTop; // указатель на вершину стека

public:
    StackList() { pTop = nullptr; }

    StackList(const StackList& SL)
    {
        NodeStack<T>* p; // дополнительные указатели
        NodeStack<T>* p2;
        NodeStack<T>* p3;

        // init
        pTop = nullptr;
        p3 = nullptr;

        p = SL.pTop; // указатель p движется по списку SL.pTop->...
        while (p != nullptr)
        {
            try {
                // попытка выделения памяти
                p2 = new NodeStack<T>;
            }
            catch (bad_alloc e)
            {
                // если память не выделена, то выход
                cout << e.what() << endl;
                return;
            }
            p2->item = p->item;
            p2->next = nullptr;

            if (pTop == nullptr) // создать очередь
            {
                pTop = p2;
                p3 = p2;
            }
            else
            {
                p3->next = p2;
                p3 = p3->next;
            }

            // перейти на следующий элемент
            p = p->next;
        }
    }

    // поместить элемент на вершину стека
    void Push(T item)
    {
        NodeStack<T>* p;

        try {
            p = new NodeStack<T>; // попытка выделить память
        }
        catch (bad_alloc e)
        {
            // если память не выделилась, то выход
            cout << e.what() << endl;
            return;
        }
        p->item = item;
        p->next = pTop; // p указывает на 1-й элемент

        pTop = p;
    }


    // очищает стек - удаляет все элементы из стека
    void Empty()
    {
        NodeStack<T>* p;
        NodeStack<T>* p2;

        p = pTop;

        while (p != nullptr)
        {
            p2 = p; 
            p = p->next; 
            delete p2;
        }
        pTop = nullptr; // поправить вершину стека
    }



    // деструктор
    ~StackList()
    {
        Empty();
    }

    T Pop() {
        // проверка, пуст ли стек?
        if (pTop == nullptr)
            return 0;

        NodeStack<T>* p2; // дополнительный указатель
        T item;
        item = pTop->item;

        p2 = pTop;
        pTop = pTop->next;

        delete p2;

        // вернуть item
        return item;
    }

    // метод, позволяющий получить элемент без его вынесения
    T Get() {   
        if (pTop == nullptr)  // проверка, пуст ли стек?
            return 0;
        else
            return pTop->item; 

    }
    bool isEmpty() { // пуст ли стек?
        if (pTop == nullptr)
            return true;
        else
            return false;
    }
};

char unamyMinus(string s, int i) {
    if (i == 0)
        return '~';
    else if (s[i - 1] == '+' || s[i - 1] == '*' || s[i - 1] == '/' || s[i - 1] == '^' || s[i - 1] == '(')
        return '~';
    else
        return '-';
}
int priority(char operation) {

    switch (operation) {

    case '^':
        return 5;
    case '*': case '/':
        return 4;
    case '~':
        return 3;
    case '+': case '-':
        return 2;
    case '(':
        return 0;
    case ')':
        return 1;
    }
}

string convertToPostfix(string s) {

    StackList<char> StackList;
    string postfix;
    int index;

    for (int i = 0; i <= s.length(); i++) {
        index = i;

        if (s[i] >= 'a' && s[i] <= 'z' || s[i] >= 'A' && s[i] <= 'Z') { // Все операнды в output
            postfix = postfix + s[i];

            cout << "Встретился операнд. Выводим в выходную строку: " << s[i] << endl;
        }
        else if (s[i] == '-') {
            if (StackList.isEmpty() && postfix.size() != 0) {
                StackList.Push(s[i]);
            }
            else if (StackList.isEmpty() || StackList.Get() == '(') {
                StackList.Push('~');
            }
            else if (StackList.Get() == '~') {
                postfix = postfix + StackList.Get();
                StackList.Pop();
                StackList.Push(s[i]);
            }
            else
                StackList.Push('-');
        }
        else if ((s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/') && StackList.Get() == '~') {
            postfix = postfix + StackList.Get();
            StackList.Pop();
            StackList.Push(s[i]);
        }
        else if (StackList.isEmpty() && s[i] != ')') {  // Открывающаяся скобка всегда записывается в стек
            cout << "Встретилась открывающаяся скобка или операция, при этом стек пустой. Записываем в стек: " << s[i] << endl;

            StackList.Push(s[i]);
        }
        else if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/' || s[i] == '^' || s[i] == ')') {
            cout << "Встретилась операция или закрывающая скобка: " << s[i] << endl;
            if (s[i] == '^' && StackList.Get() == '^') {
                cout << "Встретилась операция возведения в степень, и в стеке тоже операция возведения в степень. Добавляю в стек\n";
                StackList.Push(s[i]);
            }
            else if (priority(StackList.Get()) < priority(s[i])) {
                StackList.Push(s[i]);

                cout << "Так как приоритет операции на входе больше, чем в стеке, записываем её в стек: " << s[i] << endl;
            }
            else if (s[i] == ')') {
                cout << "Встретилась закрывающаяся скобка, выводим все из стека до открывающейся скобки." << endl;

                while (StackList.Get() != '(') {
                    if (StackList.Get() != '(' && StackList.Get() != ')')
                        postfix = postfix + StackList.Get();

                    StackList.Pop();
                }
                // StackList.Pop(); // Попаем открывающуюся скобку 
            }
            else {
                while (priority(StackList.Get()) >= priority(s[i])) {

                    cout << "Приоритет операции на вершине стека больше или равен приоритету операции на входе. " << endl;
                    if (StackList.Get() != '(' && StackList.Get() != ')')
                        postfix = postfix + StackList.Get();

                    StackList.Pop();
                }
                StackList.Push(s[i]);

                cout << "Записываем в стек:" << s[i] << endl;
            }
        }
        else if (s[i] == '(')
            StackList.Push(s[i]);
    }
    while (!StackList.isEmpty()) {
        if (StackList.Get() != '(' && StackList.Get() != ')')
            postfix = postfix + StackList.Get();

        StackList.Pop();
    }

    return postfix;
}

double calc(double num1, double num2, char ch) {
    switch (ch)
    {
    case '+':
        return num2 + num1;
    case '-':
        return num2 - num1;
    case '*':
        return num2 * num1;
    case '/':
        return num2 / num1;
    case '^':
        return pow(num2, num1);
    }
}

double result(string s) {
    StackList<double> StackList;
    map <char, int> listOperands;
    double res, op1, op2;

    for (int i = 0; i <= s.length(); i++) {
        if (s[i] >= 'a' && s[i] <= 'z' || s[i] >= 'A' && s[i] <= 'Z') {
            cout << "Встретился операнд " << s[i] << ". ";

            if (listOperands.find(s[i]) != listOperands.end()) {
                cout << "Его значение уже введено" << endl;
                res = listOperands[s[i]];
            }
            else {
                cout << "Введите значение операнда: ";
                cin >> res;
                listOperands.insert(make_pair(s[i], res));
            }


            listOperands[s[i]] == res;

            StackList.Push(res);
        }
        else if (s[i] == '~') {
            cout << "Встретился унарный минус. Считаю\n";

            op1 = StackList.Get();
            StackList.Pop();
            
            cout << "Сейчас происходит действие -1 * " << op1 << endl;
            op1 = -1 * op1;
            StackList.Push(op1);
        }
        else if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/' || s[i] == '^') {
            cout << "Встретилась операция: " << s[i] << " Считаю." << endl;

            op1 = StackList.Get();
            StackList.Pop();
            op2 = StackList.Get();
            StackList.Pop();

            cout << "Сейчас происходит действие " << op2 << " " << s[i] << " " << op1 << endl;

            if (s[i] == '/' && op1 == 0) {
                cout << "Вы ввели число, при котором выражение не имеет смысла. Прерываю выполнение" << endl;

                break;
            }            

            res = calc(op1, op2, s[i]);

            StackList.Push(res);
        }
    }
    res = StackList.Get();
    StackList.Pop();

    return res;
}

void help() {
    cout << "Реализация алгоритма перевода инфиксного выражения в постфиксную запись и подсчет результата выражения.\n";

    cout << "Ввод 0 - предложить свое имя файла с готовыми данными. \n";

    cout << "Ввод 1, или 2, или 3 - вызвать готовый тест из файла test1.txt, test2.txt, test3.txt соотвественно. \n";

    cout << "Ввод 4 - ввести выражение со строки. \n";

    cout << "Ввод 5 - завершить работу программы. \n";

    cout << "Ввод help - показать команды для выполнения\n";

}

int main()
{
    setlocale(LC_ALL, "ru");

    string input, output, tempString, request, path;
    fstream fin;
    char ch;
    double answer;
    bool isFinish = false;


    help();
    cin >> request;
 
    while (!isFinish) {
        if (request == "5")
            break;
        else if (request == "4") {
            input = "";

            cout << "Введите выражение: ";

            cin >> input;

            for (int i = 0; i <= input.length(); i++) {
                if (input[i] != ' ')
                    tempString = tempString + input[i];
            }

            output = convertToPostfix(tempString);

            cout << "Выражение в постфиксной форме: " << output << endl;

            answer = result(output);

            cout << "Результат: " << answer << endl;

            cout << "Введите запрос: ";
            cin >> request;
        }
        else if (request == "1") {
            input = "";

            fin.open("test1.txt");

            while (fin.get(ch)) {
                if (ch != ' ') {
                    input = input + ch;
                }
            }
            output = convertToPostfix(input);

            cout << "Выражение в постфиксной форме: " << output << endl;

            answer = result(output);

            cout << "Результат: " << answer << endl;

            fin.close();

            cout << "Введите запрос: ";
            cin >> request;
        }
        else if (request == "2") {
            input = "";

            fin.open("test2.txt");

            while (fin.get(ch)) {
                if (ch != ' ') {
                    input = input + ch;
                }
            }
            output = convertToPostfix(input);

            cout << "Выражение в постфиксной форме: " << output << endl;

            answer = result(output);

            cout << "Результат: " << answer << endl;

            fin.close();

            cout << "Введите запрос: ";
            cin >> request;
        }
        else if (request == "3") {
            input = "";

            fin.open("test3.txt");

            while (fin.get(ch)) {
                if (ch != ' ') {
                    input = input + ch;
                }
            }
            output = convertToPostfix(input);

            cout << "Выражение в постфиксной форме: " << output << endl;

            answer = result(output);

            cout << "Результат: " << answer << endl;

            fin.close();

            cout << "Введите запрос: ";
            cin >> request;
        }
        else if (request == "0") {
            input = "";

            cout << "Введите путь к файлу: ";

            cin >> path;

            fin.open(path);

            if (!fin.is_open()) {
                cout << "Файл не удалось открыть, попробуйте снова.";
            }
            else {
                while (fin.get(ch)) {
                    if (ch != ' ') {
                        input = input + ch;
                    }
                }
                output = convertToPostfix(input);

                cout << "Выражение в постфиксной форме: " << output << endl;

                answer = result(output);

                cout << "Результат: " << answer << endl;

                fin.close();

                cout << "Введите запрос: ";
                cin >> request;
            }
        }
        else if (request == "help") {
            help();

            cout << "Введите запрос: ";
            cin >> request;
        
        }
        else
            cout << "Команда не обнаружена. Попробуйте ещё раз" << endl;

            cout << "Введите запрос: ";
            cin >> request;
        
    }
    
	return 0;
}



