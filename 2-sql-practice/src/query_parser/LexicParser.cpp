#pragma once

#include <string>
#include <sstream>
#include <ctype.h>
#include <set>

#include "./QueryParserException.h"
#include "../shared/StringHelper.cpp"

using namespace std;

namespace LexicParser {
    /*
     * Checks if input string is a <string>
     * @param inpString - input string
     * @returns true if string is a <string>, false if not
     */
    bool isString(string &inpString) {
        if (inpString[0] == '\'' && inpString.find('\'', 1) == inpString.size() - 1 && inpString.size() > 2)
            return true;
        return false;
    }

    /*
     * Checks if input string is a number
     * @param inpString - input string
     * @returns true if string is a number, false if not
     */
    bool isNumber(string &inpString) {
        string::const_iterator iter = inpString.begin();
        while (iter != inpString.end() && (isdigit(*iter) || *iter == '.'))
            ++iter;
        return !inpString.empty() && iter == inpString.end();
    }

    /*
     * Checks if input string is a data type
     * @param inpString - input string
     * @returns true if string is a data type, false if not
     */
    bool isDataType(string &inpString) {
        return isString(inpString) || isNumber(inpString);
    }

    /*
     * Checks if input string is a table field
     * @param inpString - input string
     * @returns true if string is a field, false if not
     */
    bool isIdentifier(string &inpString) {
        if (!inpString.size() || !isalpha(inpString[0]))
            return false;
        
        for (int i = 1; i < inpString.size(); ++i) {
            if (!isalpha(inpString[i]) && !isdigit(inpString[i]) && (inpString[i] != '_'))
                return false;
        }
        
        return true;
    }

    /*
     * Checks if string is a set of elements of one type (without brackets)
     * @param inpString - input string
     * @return set if string is a set, empty set if not
     */
    bool isSet(string &inpString) {
        int occurrencesOfComa = std::count(inpString.begin(), inpString.end(), ',');
        vector<string> inpVector = StringHelper::splitToVector(inpString, ',');
        
        if (inpVector.size() != occurrencesOfComa + 1)
            return false;
        
        bool isElementString = isString(inpVector[0]);
        bool isElementNumber = isNumber(inpVector[0]);

        if (!isElementString && !isElementNumber)
            return false;

        for (int i = 1; i < inpVector.size(); ++i) {
            if (
                isString(inpVector[i]) != isElementString ||
                isNumber(inpVector[i]) != isElementNumber
            ) {
                return false;
            }
        }

        return true;
    }

    string parseStringLexem(string stringLexem) {
        if (
            stringLexem[0] != '\'' ||
            stringLexem.find('\'', 1) != stringLexem.size() - 1 ||
            stringLexem.size() < 3
        ) {
            throw QueryParserException(QueryStatusEnum::InvalidStringOperand);
        }

        return stringLexem.substr(1, stringLexem.size() - 2);
    }

    /*
     * Splits string into set of strings by char delimeter
     * @param stringLexem - input string
     * @param delimeter - char delimeneter
     * @returns set of elements splitted by delimeter
     */
    set<string> splitToStringSet(string stringLexem, char delimeter) {
        set<string> outpSet;
        istringstream stringStream(stringLexem);
        string token;
        
        while (getline(stringStream, token, delimeter)) {
            outpSet.insert(LexicParser::parseStringLexem(token));
        }
        
        return outpSet;
    }

    /*
     * Splits string into set of long double by char delimeter
     * @param stringLexem - input string
     * @param delimeter - char delimeneter
     * @returns set of elements splitted by delimeter
     */
    set<long double> splitToNumberSet(string stringLexem, char delimeter) {
        set<long double> outpSet;
        istringstream stringStream(stringLexem);
        string token;
        
        while (getline(stringStream, token, delimeter)) {
            outpSet.insert(stold(token));
        }
        
        return outpSet;
    }

    bool isInt(const string& inpString) {
        for (int i = 0; i < inpString.size(); i++) {
            if (!isdigit(inpString[i])) {
                return false;
            }
        }
        return true;
    }
}
