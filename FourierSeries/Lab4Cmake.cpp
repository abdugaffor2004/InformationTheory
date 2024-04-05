    // Lab4Cmake.cpp: определяет точку входа для приложения.
//

#include "Lab4Cmake.h"
#include <muParser.h>
#include <iostream> 
#include <cmath> 
#include <vector> 


using namespace mu;
using namespace std;


#define M_PI 3.1415926535 
//#define YELLOW_TEXT "\033[1;33m" 
//#define RED_TEXT "\033[1;31m" 
//#define RESET_COLOR "\033[0m" 



int main()
{
    setlocale(LC_ALL, "russian");

    try
    {
        double value;
        Parser p;
        string funk;
        int N;
        double h;

        cout << "Введите функцию: ";

        getline(cin, funk); //Вводить самому 
        //funk = "10+2*cos(6*pi*x)*cos(8*pi*x)+6*sin(12*pi*x)*cos(6*pi*x)";//Вводить в коде 

        p.DefineVar("x", &value);
        p.DefineConst("pi", M_PI);
        p.SetExpr(funk);

        cout << "Введите N: ";
        cin >> N;
        cout << "Введите h: ";
        cin >> h;
        cout << "Введите t0: ";
        double t0;
        cin >> t0;

        double xs = 0;

        // Вывод заголовка 
        cout << "-------------------------------------------" << endl;
        cout << "Разложение в ряд Фурье для функции: " << funk << endl;
        cout << "N = " << N << ", h = " << h << endl;
        cout << "-------------------------------------------" << endl;




        for (int k = 1; k <= N / 2 - 1; k++)
        {
            double s1 = 0, s2 = 0;
            xs = 0;
            int j = 1;

            for (int n = 1; n <= N; n++)
            {
                value = t0 + j * h;
                double x = p.Eval();

                s1 += cos(2.0 * M_PI * static_cast<double>(k) * static_cast<double>(n) / static_cast<double>(N)) * x;
                s2 += sin(2.0 * M_PI * static_cast<double>(k) * static_cast<double>(n) / static_cast<double>(N)) * x;
                xs += t0 + x;
                j += 1;

            }
            double w = static_cast<double>(k) / (static_cast<double>(N) * h);




            // Вывод коэффициентов A_k и B_k с цветом 
            cout << "\nt = " << t0 + k * h << "\t\t" << "A_" << k << " = " << 2.0 / static_cast<double>(N) * s1
                << "\t\t" << "B_" << k << " = " << 2.0 / static_cast<double>(N) * s2
                << "\t\t" << "W_" << k << " = " << w << endl;;

        }

        // Вывод коэффициента A_0 
        cout << "A_0 = " << 1.0 / static_cast<double>(N) * xs << endl;

        // Завершение вывода 
        cout << "-------------------------------------------" << endl;
        int asd;
        cin >> asd;
    }
    catch (Parser::exception_type& e)
    {
        std::cerr << e.GetMsg() << std::endl;
    }
    system("pause");
    return 0;

}
