#include <iostream>
#include <fstream>
#include <string>
#include <vector>

/*
Общими требованиями к лабораторной работе являются:
1) вводить исходное дерево из файла в понятной для
пользователя форме, а не с клавиатуры;
2) по требованию прользователя показывать дерево на экране;
3) обрабатывать дерево в оперативной памяти, а не путем
многократного обращения к файлу;
4) обеспечить возможность многократных запросов без
повторного запуска программы.

21. В листьях И - ИЛИ дерева, соответствующего некоторому
множеству конструкций, заданы значения массы. Известно
максимально допустимое значение массы изделия. Требуется усечь
дерево так, чтобы дерево включало все элементы,
соответствующие допустимым значениям массы, но не содержало
"лишних" вершин. Конечное дерево выдать на экран в наглядном
виде (13).

*/


using namespace std;

struct tree
{
    string name; // имя вершины 
    char and_or_root = ' '; // тип вершины

    int level; // уровень вершины в дереве

    int maxWeight; // максимальный вес для И-ИЛИ вершины
    int minWeight; // минимальный вес для ИЛИ-вершины

    bool isChosen = true; // Выбрана ли вершина при усечении

    tree* ptrDad; // указатель на отца
    vector<tree*> sons; // указатель на сыновей
};

void init_tree(string path, tree*& root)
{
    int prevLevel = -1;
    ifstream fin;
    char ch;
    string data;

    tree* pinnacle, * ptr, * temp;
    temp = nullptr;

    fin.open(path);

    std::cout << "Файл " << path << " открыт." << endl;
    std::cout << "Начинаю инициализировать дерево из файла..." << endl;
    while (fin.get(ch)) // пока в потоке fin есть символы
    {
        int level = 0;

        pinnacle = new tree;

        while (ch == '.') // подсчёт уровня
        {
            level++;
            fin.get(ch);
        }
        pinnacle->level = level;
        pinnacle->minWeight = _I32_MAX;
        pinnacle->maxWeight = -_I32_MAX;
        while (ch != ' ') // пока не пробел: записываем имя в pinnacle->namе
        {
            pinnacle->name += ch;
            fin.get(ch);
        }
        while (ch != '\n') // Пока не конец строки: записываем число в строку, затем string to int иначе встречаем a || o
        {
            data += ch;
            fin.get(ch);
            if (ch == 'a')
            {
                pinnacle->and_or_root = 'a'; // И-вершина
            }
            else if (ch == 'o')
            {
                pinnacle->and_or_root = 'o'; // ИЛИ-вершина
            }
        }
        if (pinnacle->and_or_root == ' ') // листок
        {
            pinnacle->maxWeight = stoi(data);
            pinnacle->minWeight = pinnacle->maxWeight;
        }
        data = "";
        if (level == 0) // инициализация вершины
        {
            root = pinnacle;
            temp = root;
            prevLevel = 0;
            continue;
        }
        if (level > prevLevel) // спуск вниз дерева
        {
            temp->sons.push_back(pinnacle);
            pinnacle->ptrDad = temp;
        }
        else if (level == prevLevel) // без спуска в дерево
        {
            ptr = temp->ptrDad;
            ptr->sons.push_back(pinnacle);
            pinnacle->ptrDad = ptr;
        }
        else  // подъем вверх (случаи с вложенностью)
        {
            ptr = temp;
            for (int i = 0; i <= prevLevel - level; i++)
            {
                ptr = ptr->ptrDad;
            }
            ptr->sons.push_back(pinnacle);
            pinnacle->ptrDad = ptr;
        }
        prevLevel = level;
        temp = pinnacle;
    }
    fin.close();
    std::cout << "Файл " << path << " закрыт." << endl;
    if (root)
    {
        std::cout << "Дерево успешно инициализированно!" << endl;
    }
    else
    {
        cout << "Дерево пустое. Неккоректные входные данные, попробуйте ещё раз" << endl;
    }
}

void calclulate_weight(tree* root) // функция подсчета весов в дереве
{
    if (!root->sons.empty())
    {
        for (int i = 0; i < root->sons.size(); i++)
        {
            calclulate_weight(root->sons[i]);
        }
        //      cout << root->level << " " << root->name << " " << root->and_or_root << " " << endl;      // отладочный оператор

        if (root->and_or_root == 'o')
        {
            for (int j = 0; j < root->sons.size(); j++)
            {
                if (root->sons[j]->maxWeight > root->maxWeight)
                {
                    root->maxWeight = root->sons[j]->maxWeight;
                }
                if (root->sons[j]->minWeight < root->minWeight)
                {
                    root->minWeight = root->sons[j]->minWeight;
                }
            }
        }
        else if (root->and_or_root == 'a')
        {
            int s_min = 0;
            int s_max = 0;

            for (int k = 0; k < root->sons.size();k++)
            {
                s_min += root->sons[k]->minWeight;
                s_max += root->sons[k]->maxWeight;
            }
            root->minWeight = s_min;
            root->maxWeight = s_max;
        }
    }
}

void delete_tree(tree*& root);

void count_weighted_tree(tree* root, int weight) // функция подсчета весов дерева, доделать
{
    //if (root->and_or_root == 'a')
    //{
    //    tree* temp_pinnacle;
    //    int sum, maxim = root->sons[0]->maxWeight;
    //    for (int i = 0; i < root->sons.size(); i++)
    //    {
    //        sum += root->sons[i]->minWeight;
    //        if (maxim < root->sons[i]->maxWeight)
    //        {
    //            sum -= root->sons[i]->minWeight;
    //            maxim = root->sons[i]->maxWeight;
    //            temp_pinnacle = root->sons[i];
    //        }

    //    }
    //}
    //if (!root->sons.empty())
    //{
    //    for (int i = 0; i < root->sons.size(); i++)
    //    {
    //        count_weighted_tree(root->sons[i], weight);
    //    }
    //}
    //else
    //{
    //    if (root->maxWeight > weight && root->and_or_root == ' ')
    //    {
    //        root->isChosen = false;
    //    }
    //}

    int temp_weight;
    for (int i = 0; i < root->sons.size();i++) // инициализируем сумму всех сыновей
    {
        if (root->sons[i]->and_or_root == 'o' && root->sons[i]->minWeight > weight)
        {
            root->sons[i]->isChosen = false;
        }
        else if (root->sons[i]->and_or_root == 'o')
        {
            if (root->and_or_root == 'a')
            {
                temp_weight = weight - root->minWeight + root->sons[i]->minWeight;
            }
            else if (root->and_or_root == 'o')
            {
                temp_weight = weight;
            }
            count_weighted_tree(root->sons[i], temp_weight);
        }
        else if (root->sons[i]->and_or_root == ' ' && root->sons[i]->maxWeight > weight)
        {
            root->sons[i]->isChosen = false;
        }
        else if (root->sons[i]->and_or_root == 'a' && root->sons[i]->minWeight > weight)
        {
            root->sons[i]->isChosen = false;
        }
    }
}

void print_tree(tree* root)  // печать дерева рекурсивно
{
    fstream fout;
    string data = "";
    if (root && root->isChosen)
    {
        for (int i = 0; i < root->level; i++)
        {
            data = data + ".";
        }
        data = data + root->name;
        if (root->and_or_root == ' ')
        {
 //           data = data + " [" + to_string(root->minWeight) + ';' + to_string(root->maxWeight) + ']';   // отладочный оператор
              data = data + " " + to_string(root->minWeight);           
        }
        else if (root->and_or_root == 'a' || root->and_or_root == 'o')
        {
 //           data = data + " " + root->and_or_root + " [" + to_string(root->minWeight) + ';' + to_string(root->maxWeight) + ']';  // отладочный оператор
              data = data + " " + root->and_or_root;                    
        }
        cout << data << endl;
        for (int i = 0; i < root->sons.size(); i++)
        {
            print_tree(root->sons[i]);
        }
    }
}

void delete_tree(tree*& root)  // очистка памяти рекурсивно
{
    for (int i = 0; i < root->sons.size(); i++)
    {
        delete_tree(root->sons[i]);
    }
    delete root;
    root = nullptr;
}

void help();

void info();

void help_file();

int main()
{
    setlocale(LC_ALL, "ru");
    string request;
    string path;

    ifstream fin;

    setlocale(LC_ALL, "ru");

    info();

    help();

    help_file();

    while (true) {
        int weight;
        tree* root = nullptr;

        cout << "Введите комманду: ";
        cin >> request;

        if (request == "stop" || request == "0")
        {
            break;
        }
        if (request >= "1" && request <= "6" && size(request) == 1)
        {
            path = "test" + request + ".txt";
            //          cout << "Имя файла: " << path << endl;
            fin.open(path);
            if (!fin.is_open())
            {
                cout << "Неверное имя файла или файл невозможно открыть. Попробуйте снова" << endl;
            }
            else
            {
                init_tree(path, root);
                // make it
                cout << "Реализую подсчёт веса дерева..." << endl;
                calclulate_weight(root); // Считать если дерево пустое, переделать ч-з if(!root) думаю

                cout << "Ваше дерево: " << endl;
                print_tree(root);

                cout << "Чтобы усечь дерево, введите число от " << root->minWeight << " до " << root->maxWeight << " включительно." << endl;
                cout << "Введите значение массы для усечения: ";
                cin >> weight;
                if (weight < root->minWeight)
                {
                    cout << "Введено значение массы минимально допустимой. Дерево будет полностью усечено." << endl;
                    cout << "Удаляю дерево..." << endl;
                    delete_tree(root);

                    cout << "Дерево было удалено из оперативной памяти" << endl;
                }
                else if (weight > root->maxWeight)
                {
                    cout << "Введите значение массы больше максимальной. Дерево не будет усечено." << endl;
                    cout << "Удаляю дерево..." << endl;
                    delete_tree(root);

                    cout << "Дерево было удалено из оперативной памяти" << endl;
                }
                else
                {
                    cout << "Начинаю усечение дерева..." << endl;

                    count_weighted_tree(root, weight);

                    cout << "Усечение завершено. Ваше дерево: " << endl;
                    print_tree(root);

                    cout << "Удаляю дерево..." << endl;
                    delete_tree(root);

                    cout << "Дерево было удалено из оперативной памяти" << endl;
                }
            }
        }
        else if (request == "7" && size(request) == 1)
        {
            cout << "Введите имя файла: ";
            cin >> path;
            fin.open(path);
            if (!fin.is_open())
            {
                cout << "Неверное имя файла или файл невозможно открыть. Попробуйте снова" << endl;
            }
            else
            {
                fin.close();
                init_tree(path, root);

                cout << "Реализую подсчёт веса дерева..." << endl;
                calclulate_weight(root);

                cout << "Ваше дерево: " << endl;
                print_tree(root);

                cout << "Чтобы усечь дерево, введите число от " << root->minWeight << " до " << root->maxWeight << " включительно." << endl;
                cout << "Введите значение массы для усечения: ";
                cin >> weight;
                if (weight < root->minWeight)
                {
                    cout << "Введено значение массы минимально допустимой. Дерево будет полностью усечено." << endl;
                    cout << "Удаляю дерево..." << endl;
                    delete_tree(root);

                    cout << "Дерево было удалено из оперативной памяти" << endl;
                }
                else if (weight > root->maxWeight)
                {
                    cout << "Введите значение массы больше максимальной. Дерево не будет усечено." << endl;
                    cout << "Удаляю дерево..." << endl;
                    delete_tree(root);

                    cout << "Дерево было удалено из оперативной памяти" << endl;
                }
                else
                {
                    cout << "Начинаю усечение дерева..." << endl;

                    count_weighted_tree(root, weight);

                    cout << "Усечение завершено. Ваше дерево: " << endl;
                    print_tree(root);

                    cout << "Удаляю дерево..." << endl;
                    delete_tree(root);

                    cout << "Дерево было удалено из оперативной памяти" << endl;
                }
            }
        }
        else if (request == "help")
        {
            help();
        }
        else if (request == "info")
        {
            info();
        }
        else if (request == "reading")
        {
            help_file();
        }
        else
        {
            cout << "Неизвестная команда. Попробуйте ещё раз" << endl;
        }
    }
    return 0;
}

void info()
{
    cout << "--------------------------------------------------info------------------------------------------------------------" << endl;
    cout << "Программа для обработки И-ИЛИ дерева. В листьях дерева содержатся значения масс, узлы могут быть OR или AND." << endl;
    cout << "На вход поступает число, предварительно выводится максимальное и минимальное возможное значение, ";
    cout << "которое считывается из файла." << endl;
    cout << "Выход за пределы этого диапазона ведет к тому, что дерево либо останется прежним (введенное число больше максимума)," << endl;
    cout << "либо дерево уничтожится полностью (введенное число меньше минимума)." << endl;
    cout << "------------------------------------------------------------------------------------------------------------------" << endl;
}

void help()
{
    cout << "--------------------------------------------------help------------------------------------------------------------" << endl;
    cout << "1 .. 6 - провести готовые тесты с файлами test1.txt, ..., test6.txt. " << endl;
    cout << "7 - предложить свое имя файла в формате .txt" << endl;
    cout << "8 - дерево вывести  на экран,  9 - записать дерево в файл и удалить" << endl;
    cout << "stop или 0 - завершить работу программы" << endl;
    cout << "help - вызвать подсказку" << endl;
    cout << "info - получить информацию о программе" << endl;
    cout << "reading - получить информацию о входных данных" << endl;
    cout << "------------------------------------------------------------------------------------------------------------------" << endl;
}

void help_file()
{
    cout << "-----------------------------------------------reading------------------------------------------------------------" << endl;
    cout << "Не забывайте, что порядок оформления должен быть такой: ...A 25 или ..B o, где  o - or, а a - and" << endl;
    cout << "Вершина без точки является начальным корнем. Пример: A o или Велосипед a" << endl;
    cout << "Корни имеют область значений {o, a}, а листья - все натуральные числа в пределах integer" << endl;
    cout << "Название вершин может быть последовательностью символов без пробела" << endl;
    cout << "------------------------------------------------------------------------------------------------------------------" << endl;
}