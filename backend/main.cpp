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
void removeDuplicates(vector<City>& cities);

void radixSort(vector<City>& cities, int extra1, int extra2);
void countingSort(vector<City>& cities, vector<int>& buckets, int place);
int maxNumOfPlaces(vector<City>& cities);
int getDigit(int num, int place);

void readData(vector<City>& cities);

int getMinTemp(vector<City>& cities, int i);
int getMaxTemp(vector<City>& cities, int i);
int getMinRain(vector<City>& cities, int i);
int getMaxRain(vector<City>& cities, int i);
int getMinHumid(vector<City>& cities, int i);
int getMaxHumid(vector<City>& cities, int i);
int getTempTrend(vector<City>& cities, int i);
int getClimate(vector<City>& cities, int i);

void setRanks(vector<City>& cities, vector<int>& input);

double sort(vector<City>& cities, vector<int>& input);

void filterByCat(vector<City>& cities, int input, int cat);

int main(int argc, char* argv[]) {
    //generateData();

    // read in user data from cmd line
    vector<int> input(9);
    (string(argv[1]) == "quick") ? input[0] = 0 : input[0] = 1;
    for (int i = 2; i < argc - 2; i++) {
        if (string(argv[i]) == "NULL") // if the user leaves a category blank, use INT_MAX as a flag
            input[i - 1] = INT_MAX;
        else
            input[i - 1] = stoi(argv[i]);
    }
    (string(argv[8]) == "warmer") ? input[7] = 0 : input[7] = 1;
    if (string(argv[9]) == "tropical")
        input[8] = 0;
    else if (string(argv[9]) == "mild")
        input[8] = 1;
    else if (string(argv[9]) == "continental")
        input[8] = 2;
    else if (string(argv[9]) == "polar")
        input[8] = 3;
    else if (string(argv[9]) == "dry")
        input[8] = 4;

    vector<City> cities;
    readData(cities);
    cout << sort(cities, input) << endl;

    for (int i = 0; i < cities.size(); i++) {
        cout << cities[i].name << endl;
        cout << cities[i].lat << endl;
        cout << cities[i].lon << endl;
        cout << cities[i].min_temp << endl;
        cout << cities[i].max_temp << endl;
        cout << cities[i].min_rain << endl;
        cout << cities[i].max_rain << endl;
        cout << cities[i].min_humid << endl;
        cout << cities[i].max_humid << endl;
        cout << cities[i].temp_trend << endl;
        cout << cities[i].climate << endl;
        cout << i + 1 << endl; // relative rank
    }

    return 0;
}

// quicksort/partition algorithm pseudocode from Module 6: Sorting slides
void quickSort(vector<City>& cities, int first, int last) {
    if (first < last) {
        int pivotIndex = partition(cities, first, last);
        quickSort(cities, first, pivotIndex - 1);
        quickSort(cities, pivotIndex + 1, last);
    }
}

int partition(vector<City>& cities, int first, int last) {
    int pivot = first;
    int up = first;
    int down = last;
    while (up < down) {
        while (cities[up].rank < cities[pivot].rank) up++;
        while (cities[down].rank > cities[pivot].rank) down--;
        if (cities[up].rank > cities[down].rank && up < down) {
            swap(cities[up], cities[down]);
        }
        if (cities[up].rank == cities[down].rank && up < down) { // prevent infinite loop caused by duplicates
            up++;
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

// used as a workaround to the quicksort duplicate bug
void removeDuplicates(vector<City>& cities) {
    unordered_set<int> values;
    vector<City> newCities;
    for (int i = 0; i < cities.size(); i++) {
        if (values.count(cities[i].rank) == 0) {
            newCities.push_back(cities[i]);
            values.insert(cities[i].rank);
        }
    }
    cities = newCities;
}

// radix/counting sort pseudocode/algorithm idea from https://www.programiz.com/dsa/radix-sort
// and https://www.interviewkickstart.com/learn/radix-sort-algorithm
// params "extra1" and "extra2" are extraneous vars so that I can use function pointers in the "sort" method
void radixSort(vector<City>& cities, int extra1, int extra2) {
    int places = maxNumOfPlaces(cities);

    vector<vector<int> > buckets(places, vector<int>(10, 0));

    // call counting sort for each digit
    for (int i = 0; i < places; i++) {
        countingSort(cities, buckets[i], i);
    }
}

int maxNumOfPlaces(vector<City>& cities) {
    int max = 0;
    for (int i = 0; i < cities.size(); i++) {
        if (max < cities[i].rank) max = cities[i].rank;
    }
    int numOfPlaces = 0;
    while (max != 0) {
        numOfPlaces++;
        max /= 10;
    }
    return numOfPlaces;
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
int getTempTrend(vector<City>& cities, int i) {
    return cities[i].temp_trend;
}
int getClimate(vector<City>& cities, int i) {
    return cities[i].climate;
}

void setRanks(vector<City>& cities, vector<int>& input) {
    int(*categories[6])(vector<City>&, int);
    categories[0] = getMinTemp;
    categories[1] = getMaxTemp;
    categories[2] = getMinRain;
    categories[3] = getMaxRain;
    categories[4] = getMinHumid;
    categories[5] = getMaxHumid;

    for (int i = 1; i < 7; i++) {
        // if user did not input anything for a category, disregard it from the ranking
        if (input[i] == INT_MAX) continue;

        for (int j = 0; j < cities.size(); j++) {
            // add the city's rank for each category to its total rank
            // (where rank is |difference between the user input value and the city's actual value|)
            cities[j].rank += abs(categories[i - 1](cities, j) - input[i]);
        }
    }
}

// use of chrono library derived from https://www.geeksforgeeks.org/chrono-in-c/#
double sort(vector<City>& cities, vector<int>& input) {
    void (*sorts[2])(vector<City> &, int, int);
    sorts[0] = quickSort;
    sorts[1] = radixSort;

    setRanks(cities, input);
    
    chrono::time_point<chrono::system_clock> start, end;
    start = chrono::system_clock::now();

    sorts[input[0]](cities, 0, (int) cities.size() - 1); // calls quick or radix based on user input

    end = chrono::system_clock::now();
    chrono::duration<double> elapsed = end - start;

    // filter by temperature trend and climate (bring user preference to top of list)
    for (int i = 0; i < 2; i++) {
        if (input[7 + i] == INT_MAX) continue;
        filterByCat(cities, input[7 + i], i);
    }

    return elapsed.count();
}

void filterByCat(vector<City>& cities, int input, int cat) {
    int(*categories[2])(vector<City>&, int);
    categories[0] = getTempTrend;
    categories[1] = getClimate;

    vector<City> inputMatch, noMatch;
    int size = (int) cities.size();

    // split cities into a list of cities which match the user input and a list of cities that don't
    for (int i = 0; i < size; i++) {
        if (categories[cat](cities, i) == input)
            inputMatch.push_back(cities[i]);
        else
            noMatch.push_back(cities[i]);
    }

    // merge lists together
    for (int i = 0; i < cities.size(); i++) {
        if (i < inputMatch.size())
            cities[i] = inputMatch[i];
        else
            cities[i] = noMatch[i - inputMatch.size()];
    }
}
