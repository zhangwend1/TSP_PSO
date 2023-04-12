#include "swarm.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <ctime>
#include <vector>
#include<Python.h>

using namespace std;



//void print_velocity(Velocity v, std::string name);
//void print_position(Position p, std::string name);


template<class T>
string arr_to_string_list(T* arr, int N) {
    string s = "[";
    for (int i = 0; i < N; ++i) {
        s += to_string(arr[i]);
        if (i != N - 1) s += ",";
    }
    s += "]";
    return s;
}

template<class T, class V = int>
void plot(T* x, int N1, V* y = NULL, bool equal = false) {
    PyRun_SimpleString("import matplotlib.pyplot as plt");
    if (equal) {
        PyRun_SimpleString("plt.axis(\"equal\")");
    }

    string cmd = "plt.plot(";
    string s1 = arr_to_string_list(x, N1);
    if (y != NULL) {
        string s2 = arr_to_string_list(y, N1);
        cmd += (s1 + "," + s2 + ")");
        PyRun_SimpleString(cmd.c_str());
    }
    else {
        cmd += (s1 + ")");
        PyRun_SimpleString(cmd.c_str());
    }
    PyRun_SimpleString("plt.show()");
}

void pythonInitial() {
    Py_Initialize(); /*初始化python解释器,告诉编译器要用的python编译器*/
    string path = ".";
    string chdir_cmd = string("sys.path.append(\"") + path + "\")";
    const char* cstr_cmd = chdir_cmd.c_str();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString(cstr_cmd);
}






int main() {
    
        srand(time(0));//uncertain
        double result[100];
        std::string filename = "C:\\Users\\ZhangNaida\\source\\repos\\pso_tsp\\pso_tsp\\tests\\eil51.tsp";
        int particle_count = 100;
        double self_trust = 0.8;
        double past_trust = 0.1;
        double global_trust = 0.1;

        std::cout << "(" << self_trust << "，" << past_trust << "，" << global_trust << ") :" ;

        for (int i = 0; i < 100; i++)
        {

            clock_t start, end;
            start = clock();


            double val;
            //srand(1);



            Swarm s(particle_count, self_trust, past_trust, global_trust);


            s.read_graph_definition(filename);

            val = s.solve();
            double* coordinate_x = new double[s.best_position.nodes.size() + 1];
            double* coordinate_y = new double[s.best_position.nodes.size() + 1];
            coordinate_x[s.best_position.nodes.size()] = s.best_position.nodes[0].x;
            coordinate_y[s.best_position.nodes.size()] = s.best_position.nodes[0].y;

            result[i] = val;

            //cout << "The final solution is:" << val << endl;
            //cout << " " << endl;
            //cout << "-------------------------------------------" << endl;
            //cout << "Iteration end" << endl;
            //cout << "Best position:" << endl;

            //for (int i = 0; i < s.best_position.nodes.size(); i++)
            //{
            //    coordinate_x[i] = s.best_position.nodes[i].x;
            //    coordinate_y[i] = s.best_position.nodes[i].y;
            //    cout << "(" << s.best_position.nodes[i].x << "," << s.best_position.nodes[i].y << ")" << " ";
            //}
            //cout << " " << endl;

            end = clock();
            float running_time;
            running_time = ((float)end - (float)start) / CLK_TCK;
            //cout << "运行时长为：" << running_time << "s" << endl;

            /* pythonInitial();
             plot(coordinate_x, s.best_position.nodes.size()+1, coordinate_y);

             Py_Finalize();


             delete[] coordinate_x;
             delete[] coordinate_y;*/


             /* pythonInitial();
              double a[15] = { 10,20,30,40,50,60,70,80,90,100,150,200,500,1000,2000 };
              double b[15] = {464.49,456.35,453.6,451.52,453.14,454.25,449.70,450.72,449.73,446.24,446.60,446.92,443.68,440.31,442.55};
              plot(a, 15, b);

              Py_Finalize();*/
        }

        double s = 0, aver;
        double max = result[0];
        double min = result[0];
        for (int k = 0; k < 100; k++)        //查找数组中的重大和最小值，并累加
        {
            std::cout << result[k] << " ";
            if (max < result[k])
                max = result[k];
            if (min > result[k])
                min = result[k];
            s += result[k];
        }
        aver = (s - max - min) / (100 - 2);
        std::cout << "平均值为：" << aver << " ";



    return 0;
}

//void print_position(Position p, std::string name) {
//    std::cout << name << " is:" << std::endl;
//    std::cout << "  (" << p.nodes[0].index;
//    for (int i = 1; i < p.nodes.size(); i++) {
//        std::cout << ", " << p.nodes[i].index;
//    }
//    std::cout << ")" << std::endl;
//}
//
//void print_velocity(Velocity v, std::string name) {
//    std::cout << name << " has:" << std::endl;
//    for (int i = 0; i < v.size; i++) {
//        std::cout << "   (" << v.from[i] << ", " << v.to[i] << ")" << std::endl;
//    }
//}

