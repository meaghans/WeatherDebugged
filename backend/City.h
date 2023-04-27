#pragma once
#include <string>
using namespace std;

struct City {
    string name;
    int lat, lon;
    int min_temp, max_temp;
    int min_rain, max_rain;
    int min_humid, max_humid;
    int temp_trend;
    int climate;
    int rank;

    City() : name("Omelas"), lat(0), lon(0), min_temp(0), max_temp(0), min_rain(0),
        max_rain(0), min_humid(0), max_humid(0), temp_trend(0), climate(0), rank(0) {};
    
    City(string n, int la, int lo, int tmin, int tmax, int rmin, int rmax, int hmin, int hmax, int tt, int c) :
            name(n), lat(la), lon(lo), min_temp(tmin), max_temp(tmax), min_rain(rmin), max_rain(rmax), min_humid(hmin),
            max_humid(hmax), temp_trend(tt), climate(c), rank(0) {};
};
