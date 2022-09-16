#pragma once

#include <set>
#include <string>
#include <iostream>

using namespace std;

namespace SetHelper {
    /*
     * Prints set
     * @param inpSet - input set
     */
    template <typename T> void print(set<T> inpSet) {
        for (T it = inpSet.begin(); it < inpSet.end(); it++) {
            cout << it;
            if (it !=  inpSet.end() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }
}
