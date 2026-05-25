#include <clocale>
#include <string>
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace std;
int pokol = 0;                                                  // глобальные переменные для поколения и клеток
int kklet = 0;

void clear_console()                                            //очистка консоли
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int initmas(string* mas[], int icopy, int jcopy, int rows, int cols, string* mert) {         // проверка массива на клетки

    int kolich = 0;

    for (int i = icopy - 1; i <= icopy + 1; i++) {
        if (i < 0 || i >= rows) { continue; }

        for (int j = jcopy - 1; j <= jcopy + 1; j++) {
            if (j < 0 || j >= cols) { continue; }
            if (icopy == i && jcopy == j) { continue; }
            if (mas[i][j] != *mert) { kolich++; }
        }
    }
    return kolich;
}

void nachalmas(string* mas[], int rows, int cols) {
    for (int i = 0; i < rows; i++) {                                            //вывод стартового массива на консоль
        for (int j = 0; j < cols; j++) {
            cout << mas[i][j];
        }
        cout << endl;
       }
    pokol++;
    cout << "Номер поколения: " << pokol << "    Живых клеток: " << kklet << endl;
}

int initvselennoi(string* mas[], string* mascopy[], int rows, int cols, string* jiv, string* mert)
{
    if (pokol == 0) {nachalmas(mas, rows, cols);
    if (kklet == 0) {                                            // проверям массив на мертвые клетки с начальным
        cout << "Все клетки мертвы. Конец игры." << endl; return 0;}
    }
    Sleep(1000);                                                 //очистка экрана и задержка на 1 секунду
    clear_console();
    kklet = 0;
    pokol++;
    
    for (int i = 0; i < rows; i++) {                                   //проверка массива по соседним клеткам на жив/мертв с условиями
        for (int j = 0; j < cols; j++) {
            int kolich = initmas(mas, i, j, rows, cols, mert);
            if (mas[i][j] == *mert && kolich == 3) { mascopy[i][j] = *jiv; }
            if (mas[i][j] == *jiv) {
                if (kolich < 2 || kolich > 3) { mascopy[i][j] = *mert; }
            }
        }
    }
    for (int i = 0; i < rows; i++) {                                            //считаем колличество живых клеток и выводим массив
        for (int j = 0; j < cols; j++) {
            cout << mascopy[i][j];
            if (mascopy[i][j] == *jiv) {
                kklet++;
            }
        }
        cout << endl;
    }
    
    cout << "Номер поколения: " << pokol << "    Живых клеток: " << kklet << endl;
    if (kklet == 0) {                                                       // проверям массив на все ли клетки мертвы
        cout << "Все клетки мертвы. Конец игры." << endl; return 0;
    }
    
    int sovpad = rows * cols;
    int rezult = 0;
    for (int i = 0; i < rows; i++) {                                        //проверяем массивы на совпадения всех клеток    
        for (int j = 0; j < cols; j++) {
            if(mascopy[i][j] == mas[i][j]) {
                rezult++;
            }
        }
    }
        
    if (rezult == sovpad) {
        cout << "Стабильная конфигурация. Конец игры." << endl; return 0;}
    else {
        for (int i = 0; i < rows; i++) {                     // Если не одинаковые перезаписываем массив в копию и прогоняем функцию еще раз
            for (int j = 0; j < cols; j++) {
                mas[i][j] = mascopy[i][j];
            }
        }
        initvselennoi(mas, mascopy, rows, cols, jiv, mert);
    }
}

void udaleniemas(string* mas[], string* mascopy[], int d)                         //Удаление массивов
{
    for (int i = 0; i < d; i++) {
        delete[] mas[i];
    }
    delete[] mas;
    for (int i = 0; i < d; i++) {
        delete[] mascopy[i];
    }
    delete[] mascopy;
}

int main() {
    setlocale(LC_CTYPE, "ru_RU.UTF-8");
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    
    string jiv = " *", mert = " -";                                 // живая и мертвая клетка
    string str, jkstr, jkstolb;                            // переменные для загрузки из файла где живая клетка jkstr(живая клетка строка) jkstolb(живая клетка столбец)
    ifstream fin("in.txt");
    if (!fin.is_open()) {
        cout << "File \"in.txt\" has not been found!" << endl;
        return 0;
    }
    
        fin >> str;
        int rows = stoi(str);
        fin >> str;
        int cols = stoi(str);
        
    string** mas = new string * [rows, cols];
    for (int i = 0; i < rows; i++) {
        mas[i] = new string[cols]();
    }

    for (int i = 0; i < rows; i++) {                      //заполнение всего массива мертвыми клетками
        for (int j = 0; j < cols; j++) {
            mas[i][j] = mert;
        }
    }
    while (!fin.eof()) {                                  //заполнение живыми клетками из файлы
        fin >> jkstr;
        fin >> jkstolb;
        mas[stoi(jkstr)][stoi(jkstolb)] = jiv;
        kklet++;
    }
    fin.close();
    
    string** mascopy = new string * [rows, cols];          //создание копии массива 
    for (int i = 0; i < rows; i++) {
        mascopy[i] = new string[cols]();
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mascopy[i][j] = mas[i][j];
        }
    }
    
    initvselennoi(mas, mascopy, rows, cols, &jiv, &mert);
    udaleniemas(mas, mascopy, rows);
    return EXIT_SUCCESS;
}
