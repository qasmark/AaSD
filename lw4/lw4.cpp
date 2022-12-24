#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
/*
    Общими требованиями к лабораторной работе являются:
       1) вводить  граф из файла в понятной для пользователя форме
    (не в виде матрицы смежности, без дублирования информации и т.
    п.);
       2) обеспечить   возможность   многократных   запросов   без
    повторного запуска программы.

    22. На диске имеется база  данных  по  физическим  эффектам
    (ФЭ).  Каждый  ФЭ  записан в виде тройки компонент:  название,
    входная  физическая  величина  (вход),   выходная   физическая
    величина  (выход).  Имеется  не  более 50 различных физических
    величин. Требуется построить в основной памяти граф связи ФЭ и
    обеспечить  поиск  цепочек  ФЭ от одного до четырех звеньев по
    заданным входу и выходу по возрастанию количества звеньев(12).
*/

struct infoAboutfPhysicalQuantities // физическая величина для инициализации бд
{
    std::string name; // имя ФВ
    int number = 0; // порядковый номер == имя ФВ (взаимное однозначное соотвествие)
};

struct NamesOfEdge // хранение количества наименований ребра
{
    std::vector<std::string> List; // список физических величин
};

struct physicalEffectFromInputOutputQuantities // физический эффект для инициализации графа из ФВ
{
    std::string name; // имя ФЭ
    int inputPhysicalEffect = 0; // входная ФВ
    int outputPhysicalEffect = 0; // выходная ФВ
};

std::vector <infoAboutfPhysicalQuantities> initDB(std::string path) // инициализация словаря
{
    infoAboutfPhysicalQuantities node; // запись для словаря

    std::fstream fin;   // файловый поток
    std::string tempString; // строка для чтения файла построчно

    int key{}; // ключ словаря
    std::string value; // значение по ключу в словаре

    std::vector <infoAboutfPhysicalQuantities> dict; // возвращаемый словарь 

    fin.open(path);
    std::cout << "Файл " << path << " открыт." << std::endl;
    std::cout << "Начинаю инициализацию словаря..." << std::endl;

    while (!fin.eof()) // пока не конец файлика, читаем построчно
    {
        value = "";

        std::getline(fin, tempString);
        if (tempString == "") // если пустая строка, то выходим 
        {
            break;
        }
        for (char const& ch : tempString) // итерация по строке 
        {
            if (ch == ':')
            {
                key = std::stoi(value);
                value = "";
            }
            else
            {
                value += ch;
            }

        }
        node.name = value;
        node.number = key;

        dict.push_back(node);


    }
    fin.close();
    std::cout << "Файл " << path << " закрыт." << std::endl;

    std::cout << "Инициализация словаря успешно завершена!" << std::endl;
    std::cout << std::endl;

    return dict;
}


void printDB(std::vector <infoAboutfPhysicalQuantities> dict)
{
    for (int i = 0; i < dict.size(); i++)
    {
        std::cout << "Ключ: " << dict[i].number << " Значение: " << dict[i].name << std::endl;

    }
}

std::vector <physicalEffectFromInputOutputQuantities> initGraph(std::string path)
{
    std::vector <physicalEffectFromInputOutputQuantities> Edges; // коллекция ребер
    physicalEffectFromInputOutputQuantities edge; // ребро

    std::fstream fin; // файловый поток

    std::string digit1 = "", digit2 = "", digit3 = "", name = ""; // буферные строки для чисел
    char ch = ' '; // буферный символ для построчного чтения файла

    bool isFirstSemicolon = false; // для первого вхождения символа точки с запятой
    bool isSecondSemicolon = false; // для второго вхождения символа точки с запятой
    bool isThirdSemiclolon = false; // для третьего вхождения символа точки с запятой
    bool isComma = false; // для проверки вхождения запятой


    fin.open(path);
    std::cout << "Файл " << path << " открыт." << std::endl;
    std::cout << "Начинаю инициализацию графа..." << std::endl;

    while (fin.get(ch))
    {
        while (ch != ';' && !isFirstSemicolon) // запись имени
        {
            name += ch;
            fin.get(ch);
            if (ch == ';')
            {
                isFirstSemicolon = true;
                break;
            }

        }
        while (ch != ';' && !isSecondSemicolon) // запись входов
        {
            if (ch == ',')
            {
                isComma = true;
            }
            if (!isComma)
            {
                digit1 += ch;
            }
            else if (isComma)
            {
                if (ch == ',')
                {
                    fin.get(ch);
                }
                digit2 += ch;
            }
            fin.get(ch);
            if (ch == ';')
            {
                isSecondSemicolon = true;
                break;
            }
        }
        while (ch != ';' && !isThirdSemiclolon) // запись выхода
        {

            digit3 += ch;
            fin.get(ch);
            if (ch == ';')
            {
                isThirdSemiclolon = true;
            }
        }
        if (ch == '\n')
        {
            edge = { name, std::stoi(digit1), std::stoi(digit3) };
            Edges.push_back(edge);
            if (digit2 != "")
            {
                edge = { name, std::stoi(digit2), std::stoi(digit3) };
                Edges.push_back(edge);
            }
            // Обнуление переменных
            digit1 = "";
            digit2 = "";
            digit3 = "";
            name = "";
            isFirstSemicolon = false;
            isSecondSemicolon = false;
            isThirdSemiclolon = false;
            isComma = false;
            continue;
        }
    }
    fin.close();
    std::cout << "Файл " << path << " закрыт. " << std::endl;

    std::cout << "Инициализация графа успешно завершена!" << std::endl;
    std::cout << std::endl;

    return Edges;
}

std::vector<std::vector<NamesOfEdge>> initAdjacencyMatrix(std::vector<physicalEffectFromInputOutputQuantities> ListPhys, std::vector<infoAboutfPhysicalQuantities> Dict)
{
    std::vector<std::vector<NamesOfEdge>> AdjacencyMatrix(Dict.size(), std::vector<NamesOfEdge>(Dict.size()));

    for (int i = 0; i < ListPhys.size(); i++)
    {
        AdjacencyMatrix[ListPhys[i].inputPhysicalEffect][ListPhys[i].outputPhysicalEffect].List.push_back(ListPhys[i].name);
    }
    return AdjacencyMatrix;
}

void printGraph(std::vector <physicalEffectFromInputOutputQuantities> Edges)
{
    for (int i = 0; i < Edges.size(); i++)
    {
        std::cout << Edges[i].name << std::endl << " Вход. узел: " << Edges[i].inputPhysicalEffect << " Выход. узел: " << Edges[i].outputPhysicalEffect << std::endl;
        std::cout << std::endl;
    }
}

void DeepFirstSearch(int NodeOfStart, int NodeOfEnd, std::string way, std::vector<std::string>& possibleWays, std::vector<int> tagged, std::vector<std::vector<NamesOfEdge>>& AdjacencyMatrix)
{
    tagged[NodeOfStart] = 1;

    for (int i = 0; i < AdjacencyMatrix.size(); i++)
    {
        if ((tagged[i] == 0) && (AdjacencyMatrix[NodeOfStart][i].List.size() != 0)) // ищу все пути в вершины, в которых есть путь
        {
            if (i == NodeOfEnd)
            {
                possibleWays.push_back(way + std::to_string(i));
                continue;
            }
            DeepFirstSearch(i, NodeOfEnd, way + std::to_string(i) + "->", possibleWays, tagged, AdjacencyMatrix);
        }
    }
}

int countSizeOfSequence(std::string sequence)
{
    char arrow = '>';
    int counter = 0;

    for (int i = 0; i < sequence.size(); i++)
    {
        if (sequence[i] == arrow)
        {
            counter++;
        }
        if (counter > 4)
        {
            return -1;
        }
    }
    return counter;
}

void printEdges(std::string way, int num, std::vector<physicalEffectFromInputOutputQuantities> Graph, std::vector <infoAboutfPhysicalQuantities> dict)
{
    std::string delimiter = "->";
    std::vector<int> numbers;

    size_t pos = 0;
    std::string token;

    while ((pos = way.find(delimiter)) != std::string::npos)
    {
        token = way.substr(0, pos);
        numbers.push_back(stoi(token));
        way.erase(0, pos + delimiter.length());
    }
    numbers.push_back(stoi(way));

    for (int j = 1; j <= num; ++j)
    {
        std::cout << j << " ребро: " << numbers[j - 1] << "->" << numbers[j] << std::endl;
        std::cout << " " << numbers[j - 1] << " - " << dict[numbers[j - 1]].name << " ";
        std::cout << " " << numbers[j] << " - " << dict[numbers[j]].name << std::endl;
        for (int k = 0; k < Graph.size(); k++)
        {
            if (Graph[k].inputPhysicalEffect == numbers[j - 1] && Graph[k].outputPhysicalEffect == numbers[j])
            {
                std::cout << "   " << Graph[k].name << std::endl;
            }
        }
    }
}

void printSequenceLessOrEqualFour(std::vector<std::string>& ways, std::vector<physicalEffectFromInputOutputQuantities> Graph, std::vector <infoAboutfPhysicalQuantities> dict)
{
    int times = 0;
    int counter = 0;
    int allTimes = 0;
    std::string separator = "->";
    short int pos = 0;
    std::string token;

    if (ways.size() == 0)
    {
        std::cout << "Последовательности не найдены." << std::endl;
    }
    else
    {
        std::cout << "Возможные последовательности:" << std::endl;
    }
    for (int i = 0; i < ways.size(); i++)
    {
        times = countSizeOfSequence(ways[i]);

        if (times == -1)
        {
            continue;
        }
        std::cout << ++counter << ". " << ways[i] << std::endl;
        std::cout << "_____________________________________________________________________________________________________________________" << std::endl;
        printEdges(ways[i], times, Graph, dict);
        std::cout << "_____________________________________________________________________________________________________________________" << std::endl;
        
    }
}

void info()
{
    std::cout << "______________________________________________________info_____________________________________________________________" << std::endl;
    std::cout << "22. На диске имеется база  данных  по  физическим  эффектам (ФЭ).Каждый  ФЭ  записан в виде тройки компонент: название," << std::endl;
    std::cout << "входная  физическая  величина(вход), выходная   физическая величина(выход). Имеется  не  более 50 различных физических" << std::endl;
    std::cout << "величин. Требуется построить в основной памяти граф связи ФЭ и обеспечить  поиск  цепочек от одного до четырех звеньев" << std::endl;
    std::cout << "по заданным входу и выходу по возрастанию количества звеньев(12)." << std::endl;
    std::cout << "_______________________________________________________________________________________________________________________" << std::endl;
}

void help()
{
    std::cout << "______________________________________________________help_____________________________________________________________" << std::endl;
    std::cout << "0, или 1, или stop - завершить работу программы" << std::endl;
    std::cout << "2 или start - начать или продолжить работу программы" << std::endl;
    std::cout << "3 - показать словарь физических величин" << std::endl;
    std::cout << "4 - показать граф физических эффектов" << std::endl;
    std::cout << "info - показать информацию о программе" << std::endl;
    std::cout << "help - показать все команды" << std::endl;
    std::cout << "_______________________________________________________________________________________________________________________" << std::endl;
}

int main()
{
    std::vector <infoAboutfPhysicalQuantities> dct;
    std::vector <physicalEffectFromInputOutputQuantities> Graph;
    std::vector<std::vector<NamesOfEdge>> AdjacencyMatrix;
    std::vector<std::string> ways;

    std::string way;

    std::string request = " ";
    bool status = true;
    bool isInit = false;

    const std::string dirQuantities = "P_NAME.TXT";
    const std::string dirEdges = "PHYS.TXT";

    int inputPhysicalQuantities;
    int outputPhysicalQuantities;

    std::fstream fin;

    setlocale(LC_ALL, "ru");

    info();
    help();

    fin.open(dirQuantities);
    if (!fin.is_open())
    {
        std::cout << "Невозможно открыть файл P_NAME.TXT" << std::endl;
        return 0;

    }
    else
    {
        fin.close();
    }

    fin.open(dirEdges);
    if (!fin.is_open())
    {
        std::cout << "Невозможно открыть файл PHYS.TXT" << std::endl;
        return 0;

    }
    else
    {
        fin.close();
    }

    while (status)
    {
        std::cout << "Введите команду: ";
        std::cin >> request;

        if (request == "1" || request == "stop" || request == "0")
        {
            status = false;
            std::cout << "Работа программы завершена.\n";
        }
        else if (request == "2" || request == "start")
        {
            if (!isInit) // инициализация всех переменных
            {
                dct = initDB(dirQuantities);
                Graph = initGraph(dirEdges);
                AdjacencyMatrix = initAdjacencyMatrix(Graph, dct);
                isInit = true;
            }
            //printDB(dct);  // отладочный оператор
            //printGraph(Graph); // отладочный оператор

            std::cout << "Введите номер входной физической величины: ";
            std::cin >> inputPhysicalQuantities;
            if (inputPhysicalQuantities < 0 || inputPhysicalQuantities > 59)
            {
                std::cout << "Входная физическая величина находится в промежутке [1;58]. Попробуйте ещё раз." << std::endl;
                continue;
            }

            std::cout << "Введите номер выходной физической величины: ";
            std::cin >> outputPhysicalQuantities;
            if (outputPhysicalQuantities < 0 || outputPhysicalQuantities > 59)
            {
                std::cout << "Выходная физическая величина находится в промежутке [1;58]. Попробуйте ещё раз." << std::endl;
                continue;
            }

            way = std::to_string(inputPhysicalQuantities) + "->";

            std::vector<int> tagged(AdjacencyMatrix.size(), 0);


            DeepFirstSearch(inputPhysicalQuantities, outputPhysicalQuantities, way, ways, tagged, AdjacencyMatrix);
            
            if (ways.size() != 0)
            {
                std::sort(ways.begin(), ways.end(), []
                (const std::string& first, const std::string& second) {
                        return first.size() < second.size();
                    });
            }

            printSequenceLessOrEqualFour(ways, Graph, dct);

            ways.clear();
        }
        else if (request == "3")
        {
            if (isInit)
            {
                printDB(dct);
            }
            else
            {
                std::cout << "Словарь ещё не инициализирован." << std::endl;
            }
        }
        else if (request == "4")
        {
            if (isInit)
            {
                printGraph(Graph);
            }
            else
            {
                std::cout << "Граф ещё не инициализирован." << std::endl;
            }
        }
        else if (request == "info")
        {
            info();
        }
        else if (request == "help")
        {
            help();
        }
        else
        {
            std::cout << "Неизвестная команда. Попробуйте снова." << std::endl;
        }
    }

    return 0;
}