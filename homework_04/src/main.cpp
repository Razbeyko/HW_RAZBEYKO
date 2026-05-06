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

    return 0;
}
