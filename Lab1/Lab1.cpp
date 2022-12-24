#include <iostream>
#include <fstream>

using namespace std;

/* 18. Составить программу перевода вещественного числа в форму
константы с плавающей  точкой  в строковом формате. Целая часть
мантиссы должна состоять из одной цифры. Исходное  число  имеет
тип double, а преобразуется к типу string (8). */


int main()
{
    double n = 0;
    fstream fin, fout;
    string pathIn, pathOut;

    cout << "Enter a filename for input file: ";
    cin >> pathIn;
    cout << "Enter a filename for output file: ";
    cin >> pathIn;

    fin.open(pathIn);
    fout.open(pathOut);

    if (fin.is_open() && fout.is_open())
    {
        cout << "You entered invalid name inpur or output files.";
    }
    else
    {
        cout << "Successful opening both files!" << endl;
        fin >> n;

        cout << "Your number is " << n << endl;
        fout << n << " - this is your number." << endl;
    }
    

    fin.close();
    fout.close();

    return 0;
}
