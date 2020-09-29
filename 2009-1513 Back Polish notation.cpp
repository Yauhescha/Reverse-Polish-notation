#include <stdio.h>
#include <iostream>
#include <malloc.h>
#include <stack>
#include <set>
#include <string>
#include <vector>
#include <sstream>
using std::cout;
using std::cin;
using std::endl;
using namespace std;


class Stack {
    double* m_data;
    int m_size;
    int values_count;
public:
    Stack(int size) {
        m_size = size;
        m_data = (double*)malloc(m_size * sizeof(int));
        values_count = 0;
    };
    ~Stack() {
        free(m_data);
    };
    double pop() {
        if (values_count)
            return m_data[--values_count];
        else
            return 0;
    };
    void push(double a) {
        if (values_count >= m_size - 1) {
            m_size = 10 + 2 * m_size;
            m_data = (double*)realloc(m_data, m_size * sizeof(int));
        }
        m_data[values_count++] = a;
    };
    int empty() {
        return (values_count == 0);
    }
};

//метод решения
// на вход принимает строку в постфиксной форме (обязательно чтоб все элементы были через пробел, сделано так для выявления дробных чисел)
void solver(string str) {
    //определяем стек. На 10 элементов вполне хватит, если что, увеличится автоматически
    class Stack s(10);
    // переменные для лучшего отображения арифметики. a, b - операнды, с = значение операндов в зависимости от знака
    double  a, b, ans;
    //снова разбиваем по пробелу
    istringstream iss(str);
    //и по одному проверяем
    do
    {
        string subs;
        iss >> subs;

        //берем первый символ элемента для проверки
        char c = subs[0];
        double x;
        switch (c) {
        case '\n':
        case ' ': break;
        case '\0': break;
            //если знак равно - выводим результат
        case '=': ans = s.pop();
            cout << "Result: " + to_string(ans) << endl; 
            return;
            //если знаки уравнений - применяем их к двум ближайшим левым числам из стэка
        case '+':
            a = s.pop();
            b = s.pop();
            ans = a + b;
            s.push(ans);
            break;
        case '-':
            a = s.pop();
            b = s.pop();
            ans = -a + b;
            s.push(ans);
            break;
        case '*':
            a = s.pop();
            b = s.pop();
            ans = a * b;
            s.push(ans);
            break;
        case '/':
            a = s.pop();
            b = s.pop();
            ans = b / a;
            s.push(ans);
            break;
        default:
            //если это не знаки, значит числа или пустые строки
            //если пустота - выводим предупреждение
            if (subs == "")
                fprintf(stderr, "Can't read double\n");
            else {
                // если число - приводим к даблу и записываем в стек
                x = stod(subs);
                s.push(x);
            }
            break;
        }
        // и так пока не кончатся строки
    } while (iss);


}

//метод проверки приоритетности знака
int priority(string e) {
    int pri = 0;

    if (e == "*" || e == "/" || e == "%") {
        pri = 2;
    }
    else {
        if (e == "+" || e == "-") {
            pri = 1;
        }
    }
    return pri;
}
// метод получения последнего индекса символа
int getLastIndex(string& str, char ch) {
    for (int i = str.length() - 1; i >= 0; i--)
        if (str[i] == ch)
            return i;
    return -1;
}

// метод получения количества арифметических знаков в строке
int getMarkCount(string str) {
    int count = 0;
    for (int n = 0; n < str.size(); n++)
        if (str[n] == '+' || str[n] == '-' || str[n] == '*' || str[n] == '/')
            count++;
    return count;
}

// удаление пробелов слева строки
static inline void ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
        }));
}

// удаление лишних пробелов справа строки
static inline void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), s.end());
}

// удаление пробелов с начала и конца строки
static inline void trim(std::string& s) {
    ltrim(s);
    rtrim(s);
}




// точка входа
int main() {
    cout << "Input expression" << endl;
    // стек для хранения выражений
    stack<string> charStack;
    // массив входных данных (каждый элемент массива это число или знак)
    string input[100];
    // массив выходных данных
    string output[100];
    //переменная ждя хранения знаков
    string n1;
    //для хранения текущего входного значения
    string* o;
    // цельная строка, получаемая с консоли
    string inConsole;
    //собственно, получаем всю строку с пробелами
    getline(cin, inConsole);
    //будем обращаться к массиву чрез о
    o = &output[0];
    //разделяем входную строку по пробелу
    istringstream iss(inConsole);
    //получаем количество пробелов в входной строке
    int allSpases = count(inConsole.begin(), inConsole.end(), ' ');
    //проходим массив разделенных данных, ориентируясь на пробел
    int j = 0;
    for (int i = 0; i < allSpases + 1; i++)
    {
        string subs;
        iss >> subs;
        //если стока ничего не содержит - пропускаем
        if (subs == "") { continue; }
        //если же в строке число или знак - заносим в стек
        input[j] = subs;
        j++;
    }


    // переменная на текущий индекс элемента входных данных
    int n = 0;
    // парсим данные, пока не дойдем до знака равно
    while (input[n] != "=") {
        //смотрим, является ли элемент double. так как в языке нет явной функции, пытаемся привести к double. если ошибка - значит не число и идем дальше. Если число - отправляем в выходные данные
        try
        {
            double temp = stod(input[n]);
            *o = input[n];
            n++;
            o++;
        }
        catch (const std::exception&)
        {
            //не число, проверяем дальше
        }
        //фиксируем открытие скобки в стеке знаков
        if (input[n] == "(") {
            charStack.push(input[n]);
            n++;
        }
        //если закрыли скобку, выкидываем все знаки
        if (input[n] == ")") {
            n1 = charStack.top();
            charStack.pop();
            // причем выкидываем знаки до первой открывающейся собки
            while (n1 != "(") {
                *o = n1;
                o++;
                n1 = charStack.top();
                charStack.pop();
            }
            n++;
        }
        //если попался какой-то знак препинания
        if (input[n] == "+" || input[n] == "-" || input[n] == "*" || input[n] == "/") {
            //проверяем, если ли еще какие знаки в стеке
            if (charStack.empty()) {
                //если нет, добавляем наш
                charStack.push(input[n]);
            }
            // если же знаки есть, смотрим какой
            else {
                n1 = charStack.top();
                charStack.pop();
                //и в зависимости от приоритета ставим их в выходные данные
                while (priority(n1) >= priority(input[n])) {
                    *o = n1;
                    o++;
                    // не забываем проверку на пустоту
                    if (charStack.empty()) break;
                    n1 = charStack.top();
                    charStack.pop();
                }
                charStack.push(n1);
                charStack.push(input[n]);
            }
            n++;
        }
    }
    //когда знаки кончились и дошли до =, выталкиваем все что осталось в выходные данные
    while (!charStack.empty()) {
        *o = charStack.top();
        o++;
        charStack.pop();
    }
    *o = '\0';
    // выходная строка, уже в постфиксной форме
    string out;
    // проходим массив выходных данных и записываем одной строкой
    for (int i = 0; i < allSpases; i++)
        out += output[i] + " ";
    //обрезаем пробелы с концов строки
    trim(out);
    //проверка на мало ли что
    //получаем количество арифметических знаков в самой первой введенной строке
    // и то же самое делоем с получившейся нашей строкой
    int realCount = getMarkCount(inConsole);
    int newCount = getMarkCount(out);
    // ели вдруг число знаков не совпадает, убираем неправильные
    while (realCount < newCount) {
        out = out.substr(0, out.size() - 3);
        newCount--;
    }
    //выводим на консоль
    cout << out << endl;
    // отправляем строку в постфиксной форме на решение
    solver(out + " =");
    // повторяем все действия
    main();
    return 0;
}



