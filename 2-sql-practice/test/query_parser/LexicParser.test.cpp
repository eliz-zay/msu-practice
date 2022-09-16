#include "../../src/query_parser/QueryParserException.cpp"
#include "../../src/query_parser/LexicParser.cpp"


#include <string>
#include <set>

using namespace std;

void testIsString() {
    try {
        bool result;
        
        string validString = "'lama'";
        string invalidString;
        
        result = LexicParser::isString(validString);
        assert(result == 1);
        cout << "isString(" << validString << ") = " << result << endl;
        
        invalidString = "''";
        result = LexicParser::isString(invalidString);
        assert(result == 0);
        cout << "isString(" << invalidString << ") = " << result << endl;
        
        
        invalidString = "'dog";
        result = LexicParser::isString(invalidString);
        assert(result == 0);
        cout << "isString(" << invalidString << ") = " << result << endl;
        
        invalidString = "dog'";
        result = LexicParser::isString(invalidString);
        assert(result == 0);
        cout << "isString(" << invalidString << ") = " << result << endl;
        
        invalidString = "123";
        result = LexicParser::isString(invalidString);
        assert(result == 0);
        cout << "isString(" << invalidString << ") = " << result << endl;
        
        cout << endl;
    } catch (exception &ex) {
        cout << ex.what();
        assert(false);
    }
    
}

void testIsNumber() {
    try {
        bool result;
        
        string validNumber = "86.7";
        string invalidNumber;
        
        result = LexicParser::isNumber(validNumber);
        assert(result == 1);
        cout << "isNumber(" << validNumber << ") = " << result << endl;
        
        validNumber = "0";
        result = LexicParser::isNumber(validNumber);
        assert(result == 1);
        cout << "isNumber(" << validNumber << ") = " << result << endl;
        
        invalidNumber = "15.8qqq";
        result = LexicParser::isNumber(invalidNumber);
        assert(result == 0);
        cout << "isNumber(" << invalidNumber << ") = " << result << endl;
        
        invalidNumber = "'15.8'";
        result = LexicParser::isNumber(invalidNumber);
        assert(result == 0);
        cout << "isNumber(" << invalidNumber << ") = " << result << endl;
        
        invalidNumber = "-186";
        result = LexicParser::isNumber(invalidNumber);
        assert(result == 0);
        cout << "isNumber(" << invalidNumber << ") = " << result << endl;
        
        cout << endl;
    } catch (exception &ex) {
        cout << ex.what();
        assert(false);
    }
}

void testIsIdentifier() {
    try {
        bool result;
        
        string validIdentifier = "f10";
        string invalidIdentifier;
        
        result = LexicParser::isIdentifier(validIdentifier);
        assert(result == 1);
        cout << "isIdentifier(" << validIdentifier << ") = " << result << endl;
        
        validIdentifier = "c_plus_plus";
        result = LexicParser::isIdentifier(validIdentifier);
        assert(result == 1);
        cout << "isIdentifier(" << validIdentifier << ") = " << result << endl;
        
        invalidIdentifier = "0qwe";
        result = LexicParser::isIdentifier(invalidIdentifier);
        assert(result == 0);
        cout << "isIdentifier(" << invalidIdentifier << ") = " << result << endl;
        
        invalidIdentifier = "'f1'";
        result = LexicParser::isIdentifier(invalidIdentifier);
        assert(result == 0);
        cout << "isIdentifier(" << invalidIdentifier << ") = " << result << endl;
        
        invalidIdentifier = "";
        result = LexicParser::isIdentifier(invalidIdentifier);
        assert(result == 0);
        cout << "isIdentifier(" << invalidIdentifier << ") = " << result << endl;
        
        cout << endl;
    } catch (exception &ex) {
        cout << ex.what();
        assert(false);
    }
}

void testIsSet() {
    try {
        bool result;
        
        string validSet = "0,42,85.6";
        string invalidSet;
        
        result = LexicParser::isSet(validSet);
        assert(result == 1);
        cout << "isSet(" << validSet << ") = " << result << endl;
        
        validSet = "'cat','dog','lama'";
        result = LexicParser::isSet(validSet);
        assert(result == 1);
        cout << "isSet(" << validSet << ") = " << result << endl;
        
        validSet = "20";
        result = LexicParser::isSet(validSet);
        assert(result == 1);
        cout << "isSet(" << validSet << ") = " << result << endl;
        
        invalidSet = "20,'horse'";
        result = LexicParser::isSet(invalidSet);
        assert(result == 0);
        cout << "isSet(" << invalidSet << ") = " << result << endl;
        
        invalidSet = "'cat',12";
        result = LexicParser::isSet(invalidSet);
        assert(result == 0);
        cout << "isSet(" << invalidSet << ") = " << result << endl;
        
        invalidSet = "";
        result = LexicParser::isSet(invalidSet);
        assert(result == 0);
        cout << "isSet(" << invalidSet << ") = " << result << endl;
        
        cout << endl;
    } catch (exception &ex) {
        cout << ex.what();
        assert(false);
    }
}

void testSplitToStringSet() {
    try {
        string stringSet = "'ghi','abc','zkl','def'";
        set<string> resultSet;
        set<string> validationSet;
        
        validationSet.insert("abc");
        validationSet.insert("def");
        validationSet.insert("ghi");
        validationSet.insert("zkl");
        
        resultSet = LexicParser::splitToStringSet(stringSet, ',');
        assert(resultSet == validationSet);
        cout << "splitToStringSet(" << stringSet << ") = ";
        for (string it: resultSet) {
            cout << it << " ";
        }
        
        cout << endl;
    } catch (exception &ex) {
        cout << ex.what();
        assert(false);
    }
}

void testSplitToNumberSet() {
    try {
        string numberSet = "10,20,35,49";
        set<long double> resultSet;
        set<long double> validationSet;
        
        validationSet.insert(10);
        validationSet.insert(20);
        validationSet.insert(35);
        validationSet.insert(49);
        
        resultSet = LexicParser::splitToNumberSet(numberSet, ',');
        assert(resultSet == validationSet);
        cout << "splitToStringSet(" << numberSet << ") = ";
        for (long double it: resultSet) {
            cout << it << " ";
        }
        
        cout << endl;
    } catch (exception &ex) {
        cout << ex.what();
        assert(false);
    }
}

int main() {
    testIsString();
    testIsNumber();
    testIsIdentifier();
    testIsSet();
    testSplitToStringSet();
    testSplitToNumberSet();
}
