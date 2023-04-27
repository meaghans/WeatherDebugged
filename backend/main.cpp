#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <unordered_set>
#include "../backend/City.h"
#include <sstream>
#include <chrono>

using namespace std;

void quickSort(vector<City>& cities, int first, int last);
int partition(vector<City>& cities, int first, int last);
void swap(City& x, City& y);

void radixSort(vector<City>& cities, int extra1, int extra2);
void countingSort(vector<City>& cities, vector<int>& buckets, int place);
int getDigit(int num, int place);

void generateData();
void readData(vector<City>& cities);

int getMinTemp(vector<City>& cities, int i);
int getMaxTemp(vector<City>& cities, int i);
int getMinRain(vector<City>& cities, int i);
int getMaxRain(vector<City>& cities, int i);
int getMinHumid(vector<City>& cities, int i);
int getMaxHumid(vector<City>& cities, int i);

void setRanks(vector<City>& cities, vector<int>& input);

double sort(vector<City>& cities, vector<int>& input);

int main() {

    //generateData();

    vector<int> input = {44, 82, 16, 56, 40, 80};
    vector<City> cities;
    readData(cities);

    cout << sort(cities, input) << endl;

    for (int i = 0; i < cities.size(); i++) {
        cout << cities[i].name << "\'s rank: " << cities[i].rank << endl;
    }

    return 0;
}

// quicksort/partition algorithm pseudocode from Module 6: Sorting slides
void quickSort(vector<City>& cities, int first, int last) {
    if (first < last) {
        int pivotIndex = partition(cities, first, last);
        quickSort(cities, first, pivotIndex);
        quickSort(cities, pivotIndex + 1, last);
    }
}

int partition(vector<City>& cities, int first, int last) {
    int pivot = first;
    int up = first;
    int down = last;
    while (up < down) { // TODO: CAN'T HANDLE DUPLICATE VALUES
        while (cities[up].rank < cities[pivot].rank) up++;
        while (cities[down].rank > cities[pivot].rank) down--;
        if (cities[up].rank > cities[down].rank) {
            swap(cities[up], cities[down]);
        }
    }
    swap(cities[pivot], cities[down]);
    return down; // index of pivot
}

void swap(City& x, City& y) {
    City temp = x;
    x = y;
    y = temp;
}

// radix/counting sort pseudocode/algorithm idea from https://www.programiz.com/dsa/radix-sort
// and https://www.interviewkickstart.com/learn/radix-sort-algorithm
// params "extra1" and "extra2" are extraneous vars so that I can use function pointers in sort
void radixSort(vector<City>& cities, int extra1, int extra2) {
    vector<vector<int>> buckets(3, vector<int>(10, 0));

    // call counting sort for each digit (ones, tens, hundreds places)
    for (int i = 0; i < 3; i++) {
        countingSort(cities, buckets[i], i);
    }
}

void countingSort(vector<City>& cities, vector<int>& buckets, int place) {
    for (int i = 0; i < cities.size(); i++) {
        buckets[getDigit(cities[i].rank, place)]++; // update buckets with counts
    }
    for (int i = 1; i < 10; i++) {
        buckets[i] += buckets[i - 1]; // find cumulative sum
    }

    // sort data into new array and update old array to match
    vector<City> newCities(cities.size());
    for (int i = cities.size() - 1; i >= 0; i--) {
        newCities[--buckets[getDigit(cities[i].rank, place)]] = cities[i];
    }
    cities = newCities;
}

int getDigit(int num, int place) {
    return (num / (int) pow(10, place)) % 10;
}

void generateData() {
    const std::string endings[] = {"ville", "town", "berg", "polis", " City"};
    const int num_endings = sizeof(endings) / sizeof(endings[0]);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> len_dist(6, 13);
    uniform_int_distribution<> ending_dist(0, num_endings - 1);
    unordered_set<string> generated;

    ofstream data("data.csv");
    const int num_strings = 10;
    int i = 0;
    data << "City,Lat,Long,Min_Temp,Max_Temp,Min_Rain,Max_Rain,Min_Humid,Max_Humid,Temp_Trend,Climate" << endl;
    while (i < num_strings) {
        // generate city name (code from chatgpt)
        int len = len_dist(gen);
        int ending_index = ending_dist(gen);
        string ending = endings[ending_index];
        int prefix_len = len - ending.length();
        uniform_int_distribution<> char_dist('a', 'z');
        string prefix;
        for (int j = 0; j < prefix_len; j++) {
            prefix += static_cast<char>(char_dist(gen));
        }
        string str = prefix + ending;
        str[0] = toupper(str[0]);

        // regenerate a name if we have a duplicate
        if (generated.count(str) == 1) continue;

        // generate lat/long
        uniform_int_distribution<> lat_range(0, 100); // TODO: ASK LYSANDRA REASONABLE COORDS FOR HER MAP
        int lat = lat_range(gen);
        uniform_int_distribution<> lon_range(0, 100);
        int lon = lon_range(gen);

        // generate min/max temp, rainfall, humidity
        uniform_int_distribution<> min_temp_range(0, 80);
        int min_temp = min_temp_range(gen);
        uniform_int_distribution<> max_temp_range(min_temp + 5, min_temp + 30);
        int max_temp = max_temp_range(gen);
        uniform_int_distribution<> min_rain_range(0, 50); // TODO: WHAT'S REASONABLE PRECIPITATION?
        int min_rain = min_rain_range(gen);
        uniform_int_distribution<> max_rain_range(min_rain + 5, min_rain + 20);
        int max_rain = max_rain_range(gen);
        uniform_int_distribution<> min_humid_range(0, 55);
        int min_humid = min_humid_range(gen);
        uniform_int_distribution<> max_humid_range(min_humid + 5, 100);
        int max_humid = max_humid_range(gen);

        // generate trend/climate information
        uniform_int_distribution<> temp_trend_range(0, 1);
        int temp_trend = temp_trend_range(gen);
        uniform_int_distribution<> climate_range(0, 4);
        int climate = climate_range(gen);

        data << str << "," << lat << "," << lon << "," <<
             min_temp << "," << max_temp << "," << min_rain << "," <<
             max_rain << "," << min_humid << "," << max_humid << "," <<
             temp_trend << "," << climate << endl;
        generated.insert(str);
        i++;
    }
    data.close();
}

void readData(vector<City>& cities) {
    string str;
    ifstream data("data.csv");
    getline(data, str);

    while (getline(data, str)) {
        istringstream line(str);
        string curr;
        string name;
        int lat, lon, min_temp, max_temp, min_rain, max_rain, min_humid, max_humid, temp_trend, climate;
        for (int i = 0; i < 11; i++) {
            getline(line, curr, ',');
            switch (i) {
                case 0:
                    name = curr;
                    break;
                case 1:
                    lat = stoi(curr);
                    break;
                case 2:
                    lon = stoi(curr);
                    break;
                case 3:
                    min_temp = stoi(curr);
                    break;
                case 4:
                    max_temp = stoi(curr);
                    break;
                case 5:
                    min_rain = stoi(curr);
                    break;
                case 6:
                    max_rain = stoi(curr);
                    break;
                case 7:
                    min_humid = stoi(curr);
                    break;
                case 8:
                    max_humid = stoi(curr);
                    break;
                case 9:
                    temp_trend = stoi(curr);
                    break;
                case 10:
                    climate = stoi(curr);
                    break;
            }
        }
        cities.push_back(City(name, lat, lon, min_temp, max_temp, min_rain, max_rain,
                              min_humid, max_humid, temp_trend, climate));
    }
}

int getMinTemp(vector<City>& cities, int i) {
    return cities[i].min_temp;
}
int getMaxTemp(vector<City>& cities, int i) {
    return cities[i].max_temp;
}
int getMinRain(vector<City>& cities, int i) {
    return cities[i].min_rain;
}
int getMaxRain(vector<City>& cities, int i) {
    return cities[i].max_rain;
}
int getMinHumid(vector<City>& cities, int i) {
    return cities[i].min_humid;
}
int getMaxHumid(vector<City>& cities, int i) {
    return cities[i].max_humid;
}

void setRanks(vector<City>& cities, vector<int>& input) {
    int(*categories[6])(vector<City>&, int);
    categories[0] = getMinTemp;
    categories[1] = getMaxTemp;
    categories[2] = getMinRain;
    categories[3] = getMaxRain;
    categories[4] = getMinHumid;
    categories[5] = getMaxHumid;

    // TODO: ADD FUNCTIONALITY IN CASE USER INPUT NOTHING FOR THAT CATEGORY?
    for (int i = 0; i < cities.size(); i++) {
        // add the city's rank for each category to its total rank
        // (where rank is |difference between the user input value and the city's actual value|)
        for (int j = 0; j < 6; j++) {
            cities[i].rank += abs(categories[j](cities, i) - input[j]);
        }
    }
}

// use of chrono library derived from https://www.geeksforgeeks.org/chrono-in-c/#
double sort(vector<City>& cities, vector<int>& input) {
    void(*sorts[2])(vector<City>&, int, int);
    sorts[0] = quickSort;
    sorts[1] = radixSort;

    chrono::time_point<chrono::system_clock> start, end;
    start = chrono::system_clock::now();

    setRanks(cities, input);
    sorts[1](cities, 0, (int) cities.size() - 1); // calls quick or radix based on user input

    end = chrono::system_clock::now();
    chrono::duration<double> elapsed = end - start;

    return elapsed.count();
}
