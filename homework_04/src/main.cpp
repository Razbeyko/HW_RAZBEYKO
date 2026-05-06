#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>

struct point
{
    float timestamp_ms;
    float fl_ticks;
    float fr_ticks;
    float bl_ticks;
    float br_ticks;
    point();
    point(float _timestamp_ms, float _fl_ticks, float _fr_ticks, float _bl_ticks, float _br_ticks)
    {
        timestamp_ms = _timestamp_ms;
        fl_ticks = _fl_ticks;
        fr_ticks = _fr_ticks;
        bl_ticks = _bl_ticks;
        br_ticks = _br_ticks;
    }
};
struct wayData
{
    std::vector<point> points;
};
struct coord
{
    float x;
    float y;
    float theta;
};

void readFromFileInput(std::vector<point>& _points)
{
    //std::ifstream file("combined.txt");
    //std::ifstream file("straight.txt");
    std::ifstream file("turn.txt");
    std::string line;
    if (file.is_open()) 
    {
        while(getline(file, line))
        {
            std::string subLines[5];
            int num_subLine = 0;
            for(int i = 0; i < line.length(); i++)
            {
                if(line[i] == ' ')
                {
                    num_subLine++;
                    continue;
                }
                subLines[num_subLine] += line[i];
            }
            _points.push_back(point(std::stof(subLines[0]), 
                                    std::stof(subLines[1]), 
                                    std::stof(subLines[2]), 
                                    std::stof(subLines[3]), 
                                    std::stof(subLines[4])));
        }
        file.close(); // Закриття файлу
    }
    else
    {
        std::cout << "Помилка відкриття файлу";
    }

    return;
}

int main(int argc, char** argv) {
    // The program expects exactly one argument: a path to telemetry samples.
    if (argc != 2) {
        std::cerr << "usage: ugv_odometry <input_path>\n";
        return 1;
    }

// TODO: implement wheel odometry for a 4-wheel differential-drive UGV.
//
// Model parameters:
//   ticks_per_revolution = 1024
//   wheel_radius_m       = 0.3
//   wheelbase_m          = 1.0
//
// Input: a text file with 5 whitespace-separated values per line:
//         timestamp_ms fl_ticks fr_ticks bl_ticks br_ticks
// Output: a table on stdout, starting from the second sample:
//         timestamp_ms x y theta

    wayData wayPointsOfRobot;
    readFromFileInput(wayPointsOfRobot.points);

    // Model parameters:
    float ticks_per_revolution = 1024.f;
    float wheel_radius_m       = 0.300f;
    float wheelbase_m          = 1.000f;
    float pi                   = 3.14159f;

    coord dron;
    dron.x = 0.f;
    dron.y = 0.f;
    dron.theta = 0.f;
    std::ofstream outF("outputFile.txt"); // Открытие для записи

    for(int i = 1; i < wayPointsOfRobot.points.size(); i++)
    {
        //На кожному кроцi (для i-го рядка, i >= 1):
        //Крок 1. Delta iмпульсiв по кожному колесу:
        float d_fl = wayPointsOfRobot.points.at(i).fl_ticks - wayPointsOfRobot.points.at(i-1).fl_ticks;
        float d_fr = wayPointsOfRobot.points.at(i).fr_ticks - wayPointsOfRobot.points.at(i-1).fr_ticks;
        float d_bl = wayPointsOfRobot.points.at(i).bl_ticks - wayPointsOfRobot.points.at(i-1).bl_ticks;
        float d_br = d_bl = wayPointsOfRobot.points.at(i).br_ticks - wayPointsOfRobot.points.at(i-1).br_ticks;
        //Крок 2. Усереднити борти (передне i заднє колесо одного боку обертаються синхронно):
        float d_left  = (d_fl + d_bl) / 2.0f;
        float d_right = (d_fr + d_br) / 2.0f;
        //Крок 3. Перевести iмпульси у метри:
        float distance_per_tick = 2.0f * pi * wheel_radius_m / (float)ticks_per_revolution;
        float dL = d_left  * distance_per_tick;
        float dR = d_right * distance_per_tick;
        //Крок 4. Скiльки пройшов центр робота i на скiльки повернувся:
        float d      = (dL + dR) / 2.0f;              // пройдена вiдстань центру
        float dtheta = (dR - dL) / wheelbase_m;       // змiна орiєнтацiї
        //Крок 5. Оновити позицiю (midpoint integration - усереднений напрямок на кроцi):
        dron.x     += d * cosf(dron.theta + dtheta / 2.0f);
        dron.y     += d * sinf(dron.theta + dtheta / 2.0f);
        dron.theta += dtheta;
        //Початковi значення: x = 0, y = 0, theta = 0.
        if (outF.is_open()) {
            outF << wayPointsOfRobot.points.at(i).timestamp_ms  << " " << dron.x << " " << dron.y << " " << dron.theta << std::endl;
        }
    }
    outF.close(); // Закрываем файл
    return 0;
}
