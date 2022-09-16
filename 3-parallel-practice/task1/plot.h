#include <fstream>
#include <iostream>

using namespace std;

void writeToCSV(string fileName, int mode, float seconds) {
    ofstream file(fileName, ios::app);

    file << mode << "\t" << seconds << "\n";

    file.close();
}