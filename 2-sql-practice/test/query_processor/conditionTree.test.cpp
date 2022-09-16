#include "../../src/query_processor/QueryObject.cpp"
#include "../../src/query_processor/SelectObject.cpp"

#include "../../src/query_processor/condition_tree/NegatableCondition.cpp"
#include "../../src/query_processor/condition_tree/OrCondition.cpp"
#include "../../src/query_processor/condition_tree/AndCondition.cpp"
#include "../../src/query_processor/condition_tree/MultipleCondition.cpp"
#include "../../src/query_processor/condition_tree/BinaryCondition.cpp"
#include "../../src/query_processor/condition_tree/InCondition.cpp"
#include "../../src/query_processor/condition_tree/RelationCondition.cpp"
#include "../../src/query_processor/condition_tree/LikeCondition.cpp"

#include "../../src/query_processor/condition_tree/BaseOperand.cpp"
#include "../../src/query_processor/condition_tree/NumberSetOperand.cpp"
#include "../../src/query_processor/condition_tree/StringSetOperand.cpp"
#include "../../src/query_processor/condition_tree/NumberOperand.cpp"
#include "../../src/query_processor/condition_tree/StringOperand.cpp"
#include "../../src/query_processor/condition_tree/TableFieldOperand.cpp"

#include "../../src/shared/SetHelper.cpp"

#include <iostream>
#include <set>

using namespace std;

template<typename T> string rowToString(vector<T> object) {
    string message;
    message += "Row: {\n";
    for (int i = 0; i < object.size(); ++i) {
        message += "   " + object[i]->toString();
        if (i != object.size() - 1) {
            message += ",";
        }
        
        message += "\n";
    }
    
    message += "}\n\n";
    return message;
}

template<typename T> string fieldsToString(vector<T*> object) {
    string message;
    message += "Fields: {\n";
    for (int i = 0; i < object.size(); ++i) {
        message += "   " + object[i]->toString();
        if (i != object.size() - 1) {
            message += ",";
        }
        
        message += "\n";
    }

    message += "}\n\n";
    return message;
}

void testRelationCondition() {
    cout << "WHERE f2 = 'qwerty'" << endl << endl;

    vector<TableField*> fields;
    fields.push_back(new TableField("f1", DataTypeEnum::NUMBER));
    fields.push_back(new TableField("f2", DataTypeEnum::VARCHAR));
    fields.push_back(new TableField("f3", DataTypeEnum::NUMBER));

    vector<DataType*> row;
    row.push_back(new Number("1"));
    row.push_back(new Varchar("'qwerty'"));
    row.push_back(new Number("145.5"));
    
    cout << fieldsToString(fields);
    cout << rowToString(row);
    
    BaseOperand* fieldOperand = new TableFieldOperand("f2");
    BaseOperand* stringOperand = new StringOperand("qwerty");
    BaseCondition* condition = new RelationCondition(fieldOperand, stringOperand, "=");

    bool result = condition->calculate(fields, row);

    cout << condition->toString();
    cout << result << endl << endl;


    cout << "WHERE f3 = 12" << endl << endl;

    cout << fieldsToString(fields);
    cout << rowToString(row);

    delete fieldOperand;
    delete condition;

    fieldOperand = new TableFieldOperand("f3");
    BaseOperand* numberOperand = new NumberOperand(12);
    condition = new RelationCondition(fieldOperand, numberOperand, "=");

    result = condition->calculate(fields, row);

    cout << condition->toString();
    cout << result << endl << endl;


    cout << "WHERE f3 = 145.5" << endl << endl;

    cout << fieldsToString(fields);
    cout << rowToString(row);

    delete fieldOperand;
    delete condition;
    delete numberOperand;

    fieldOperand = new TableFieldOperand("f3");
    numberOperand = new NumberOperand(145.5);
    condition = new RelationCondition(fieldOperand, numberOperand, "=");

    result = condition->calculate(fields, row);

    cout << condition->toString();
    cout << result << endl << endl;


    cout << "WHERE f3 > 145" << endl << endl;

    cout << fieldsToString(fields);
    cout << rowToString(row);

    delete fieldOperand;
    delete condition;
    delete numberOperand;

    fieldOperand = new TableFieldOperand("f3");
    numberOperand = new NumberOperand(145);
    condition = new RelationCondition(fieldOperand, numberOperand, ">");

    result = condition->calculate(fields, row);

    cout << condition->toString();
    cout << result << endl << endl;


    cout << "WHERE f3 < 146" << endl << endl;

    cout << fieldsToString(fields);
    cout << rowToString(row);

    delete fieldOperand;
    delete condition;
    delete numberOperand;

    fieldOperand = new TableFieldOperand("f3");
    numberOperand = new NumberOperand(146);
    condition = new RelationCondition(fieldOperand, numberOperand, "<");

    result = condition->calculate(fields, row);

    cout << condition->toString();
    cout << result << endl << endl;
}

void testInCondition() {
    cout << "WHERE f1 IN (6, 10, 28)" << endl << endl;

    vector<TableField*> fields;
    fields.push_back(new TableField("f1", DataTypeEnum::NUMBER));
    fields.push_back(new TableField("f2", DataTypeEnum::VARCHAR));
    fields.push_back(new TableField("f3", DataTypeEnum::NUMBER));

    vector<DataType*> row;
    row.push_back(new Number("1"));
    row.push_back(new Varchar("'abcd'"));
    row.push_back(new Number("145.5"));
    
    cout << fieldsToString(fields);
    cout << rowToString(row);
    
    BaseOperand* fieldOperand = new TableFieldOperand("f1");

    set<long double> numberSetValue;
    numberSetValue.insert(10);
    numberSetValue.insert(6);
    numberSetValue.insert(28);

    BaseOperand* numberSetOperand = new NumberSetOperand(numberSetValue);
    InCondition* condition = new InCondition(fieldOperand, numberSetOperand);
        
    bool result = condition->calculate(fields, row);

    cout << condition->toString();
    cout << result << endl << endl;


    cout << "WHERE f1 NOT IN (6, 10, 28)" << endl << endl;
    
    condition->negate();
    result = condition->calculate(fields, row);

    cout << condition->toString();
    cout << result << endl << endl;


    cout << "WHERE f1 IN (6, 10, 28, 1)" << endl << endl;
    
    numberSetValue.insert(1);

    cout << fieldsToString(fields);
    cout << rowToString(row);
    
    delete(numberSetOperand);
    delete(condition);

    numberSetOperand = new NumberSetOperand(numberSetValue);
    condition = new InCondition(fieldOperand, numberSetOperand);
    
    result = condition->calculate(fields, row);

    cout << condition->toString();
    cout << result << endl << endl;


    cout << "WHERE f1 NOT IN (6, 10, 28, 1)" << endl << endl;
    
    condition->negate();
    result = condition->calculate(fields, row);

    cout << condition->toString();
    cout << result << endl << endl;


    cout << "WHERE f2 IN ('a', 'bc', 'qwer')" << endl << endl;

    cout << fieldsToString(fields);
    cout << rowToString(row);
    
    delete(fieldOperand);
    delete(numberSetOperand);
    delete(condition);

    set<string> stringSetValue;
    stringSetValue.insert("a");
    stringSetValue.insert("bc");
    stringSetValue.insert("qwer");

    StringSetOperand* stringSetOperand = new StringSetOperand(stringSetValue);
    fieldOperand = new TableFieldOperand("f2");
    condition = new InCondition(fieldOperand, stringSetOperand);
    
    result = condition->calculate(fields, row);

    cout << condition->toString();
    cout << result << endl << endl;

    cout << "WHERE f2 NOT IN ('a', 'bc', 'qwer')" << endl << endl;

    condition->negate();
    result = condition->calculate(fields, row);

    cout << condition->toString();
    cout << result << endl << endl;

    cout << "WHERE f2 IN ('a', 'bc', 'qwer', 'abcd')" << endl << endl;

    cout << fieldsToString(fields);
    cout << rowToString(row);
    
    delete(fieldOperand);
    delete(stringSetOperand);
    delete(condition);

    stringSetValue.insert("abcd");

    stringSetOperand = new StringSetOperand(stringSetValue);
    fieldOperand = new TableFieldOperand("f2");
    condition = new InCondition(fieldOperand, stringSetOperand);
    
    result = condition->calculate(fields, row);

    cout << condition->toString();
    cout << result << endl << endl;
}

void testLikeCondition() {
    cout << "WHERE f2 LIKE '%ab'" << endl << endl;

    vector<TableField*> fields;
    fields.push_back(new TableField("f1", DataTypeEnum::NUMBER));
    fields.push_back(new TableField("f2", DataTypeEnum::VARCHAR));
    fields.push_back(new TableField("f3", DataTypeEnum::NUMBER));

    vector<DataType*> row;
    row.push_back(new Number("1"));
    row.push_back(new Varchar("'abc'"));
    row.push_back(new Number("145.5"));
    
    cout << fieldsToString(fields);
    cout << rowToString(row);
    
    BaseOperand* fieldOperand = new TableFieldOperand("f2");
    BaseOperand* stringOperand = new StringOperand("%ab");
    BaseCondition* condition = new LikeCondition(fieldOperand, stringOperand);

    bool result = condition->calculate(fields, row);

    cout << condition->toString();
    cout << result << endl << endl;


    cout << "WHERE f2 LIKE '%bc'" << endl << endl;

    delete(stringOperand);
    delete(condition);

    stringOperand = new StringOperand("%bc");
    condition = new LikeCondition(fieldOperand, stringOperand);

    result = condition->calculate(fields, row);

    cout << condition->toString();
    cout << result << endl << endl;

    cout << "WHERE f2 LIKE '%abc'" << endl << endl;

    delete(stringOperand);
    delete(condition);

    stringOperand = new StringOperand("%abc");
    condition = new LikeCondition(fieldOperand, stringOperand);

    result = condition->calculate(fields, row);

    cout << condition->toString();
    cout << result << endl << endl;


    cout << "WHERE f2 LIKE '_bc'" << endl << endl;

    delete(stringOperand);
    delete(condition);

    stringOperand = new StringOperand("_bc");
    condition = new LikeCondition(fieldOperand, stringOperand);

    result = condition->calculate(fields, row);

    cout << condition->toString();
    cout << result << endl << endl;


    cout << "WHERE f2 LIKE '_abc'" << endl << endl;

    delete(stringOperand);
    delete(condition);

    stringOperand = new StringOperand("_abc");
    condition = new LikeCondition(fieldOperand, stringOperand);

    result = condition->calculate(fields, row);

    cout << condition->toString();
    cout << result << endl << endl;


    cout << "WHERE f2 LIKE '_c'" << endl << endl;

    delete(stringOperand);
    delete(condition);

    stringOperand = new StringOperand("_c");
    condition = new LikeCondition(fieldOperand, stringOperand);

    result = condition->calculate(fields, row);

    cout << condition->toString();
    cout << result << endl << endl;


    cout << "WHERE f2 LIKE 'a[a-f]c'" << endl << endl;

    delete(stringOperand);
    delete(condition);

    stringOperand = new StringOperand("a[a-f]c");
    condition = new LikeCondition(fieldOperand, stringOperand);

    result = condition->calculate(fields, row);

    cout << condition->toString();
    cout << result << endl << endl;


    cout << "WHERE f2 LIKE 'a[c-f]c'" << endl << endl;

    delete(stringOperand);
    delete(condition);

    stringOperand = new StringOperand("a[c-f]c");
    condition = new LikeCondition(fieldOperand, stringOperand);

    result = condition->calculate(fields, row);

    cout << condition->toString();
    cout << result << endl << endl;


    cout << "WHERE f2 LIKE 'a[^c-f]c'" << endl << endl;

    delete(stringOperand);
    delete(condition);

    stringOperand = new StringOperand("a[^c-f]c");
    condition = new LikeCondition(fieldOperand, stringOperand);

    result = condition->calculate(fields, row);

    cout << condition->toString();
    cout << result << endl << endl;
}

void testAndCondition() {
    vector<TableField*> fields;
    fields.push_back(new TableField("f1", DataTypeEnum::NUMBER));
    fields.push_back(new TableField("f2", DataTypeEnum::VARCHAR));
    fields.push_back(new TableField("f3", DataTypeEnum::NUMBER));

    vector<DataType*> row;
    row.push_back(new Number("3"));
    row.push_back(new Varchar("'abc'"));
    row.push_back(new Number("145.5"));

    cout << fieldsToString(fields);
    cout << rowToString(row);


    cout << "WHERE f1 = 5 AND f2 LIKE '%ab'" << endl << endl;

    BaseOperand* fieldOperand1 = new TableFieldOperand("f1");
    BaseOperand* numberOperand = new NumberOperand(5);
    BaseCondition* relation = new RelationCondition(fieldOperand1, numberOperand, "=");

    BaseOperand* fieldOperand2 = new TableFieldOperand("f2");
    BaseOperand* stringOperand = new StringOperand("%ab");
    BaseCondition* likeCondition = new LikeCondition(fieldOperand2, stringOperand);

    AndCondition* andCondition = new AndCondition();
    andCondition->addOperand(relation);
    andCondition->addOperand(likeCondition);

    bool result = andCondition->calculate(fields, row);

    cout << andCondition->toString();
    cout << result << endl << endl;


    cout << "WHERE f1 = 5 AND f2 LIKE '%bc'" << endl << endl;

    numberOperand = new NumberOperand(5);
    relation = new RelationCondition(fieldOperand1, numberOperand, "=");

    stringOperand = new StringOperand("%bc");
    likeCondition = new LikeCondition(fieldOperand2, stringOperand);

    andCondition = new AndCondition();
    andCondition->addOperand(relation);
    andCondition->addOperand(likeCondition);

    result = andCondition->calculate(fields, row);

    cout << andCondition->toString();
    cout << result << endl << endl;


    cout << "WHERE f1 = 3 AND f2 LIKE '%ab'" << endl << endl;

    numberOperand = new NumberOperand(3);
    relation = new RelationCondition(fieldOperand1, numberOperand, "=");

    stringOperand = new StringOperand("%ab");
    likeCondition = new LikeCondition(fieldOperand2, stringOperand);

    andCondition = new AndCondition();
    andCondition->addOperand(relation);
    andCondition->addOperand(likeCondition);

    result = andCondition->calculate(fields, row);

    cout << andCondition->toString();
    cout << result << endl << endl;


    cout << "WHERE f1 = 3 AND f2 LIKE '%bc'" << endl << endl;

    numberOperand = new NumberOperand(3);
    relation = new RelationCondition(fieldOperand1, numberOperand, "=");

    stringOperand = new StringOperand("%bc");
    likeCondition = new LikeCondition(fieldOperand2, stringOperand);

    andCondition = new AndCondition();
    andCondition->addOperand(relation);
    andCondition->addOperand(likeCondition);

    result = andCondition->calculate(fields, row);

    cout << andCondition->toString();
    cout << result << endl << endl;


    cout << "WHERE NOT (f1 = 3 AND f2 LIKE '%bc')" << endl << endl;

    andCondition->negate();

    result = andCondition->calculate(fields, row);

    cout << andCondition->toString();
    cout << result << endl << endl;
}

void testOrCondition() {
    vector<TableField*> fields;
    fields.push_back(new TableField("f1", DataTypeEnum::NUMBER));
    fields.push_back(new TableField("f2", DataTypeEnum::VARCHAR));
    fields.push_back(new TableField("f3", DataTypeEnum::NUMBER));

    vector<DataType*> row;
    row.push_back(new Number("3"));
    row.push_back(new Varchar("'abc'"));
    row.push_back(new Number("145.5"));

    cout << fieldsToString(fields);
    cout << rowToString(row);


    cout << "WHERE f1 = 5 OR f2 LIKE '%ab'" << endl << endl;

    BaseOperand* fieldOperand1 = new TableFieldOperand("f1");
    BaseOperand* numberOperand = new NumberOperand(5);
    BaseCondition* relation = new RelationCondition(fieldOperand1, numberOperand, "=");

    BaseOperand* fieldOperand2 = new TableFieldOperand("f2");
    BaseOperand* stringOperand = new StringOperand("%ab");
    BaseCondition* likeCondition = new LikeCondition(fieldOperand2, stringOperand);

    OrCondition* orCondition = new OrCondition();
    orCondition->addOperand(relation);
    orCondition->addOperand(likeCondition);

    bool result = orCondition->calculate(fields, row);

    cout << orCondition->toString();
    cout << result << endl << endl;


    cout << "WHERE f1 = 5 OR f2 LIKE '%bc'" << endl << endl;

    numberOperand = new NumberOperand(5);
    relation = new RelationCondition(fieldOperand1, numberOperand, "=");

    stringOperand = new StringOperand("%bc");
    likeCondition = new LikeCondition(fieldOperand2, stringOperand);

    orCondition = new OrCondition();
    orCondition->addOperand(relation);
    orCondition->addOperand(likeCondition);

    result = orCondition->calculate(fields, row);

    cout << orCondition->toString();
    cout << result << endl << endl;


    cout << "WHERE f1 = 3 OR f2 LIKE '%ab'" << endl << endl;

    numberOperand = new NumberOperand(3);
    relation = new RelationCondition(fieldOperand1, numberOperand, "=");

    stringOperand = new StringOperand("%ab");
    likeCondition = new LikeCondition(fieldOperand2, stringOperand);

    orCondition = new OrCondition();
    orCondition->addOperand(relation);
    orCondition->addOperand(likeCondition);

    result = orCondition->calculate(fields, row);

    cout << orCondition->toString();
    cout << result << endl << endl;


    cout << "WHERE f1 = 3 OR f2 LIKE '%bc'" << endl << endl;

    numberOperand = new NumberOperand(3);
    relation = new RelationCondition(fieldOperand1, numberOperand, "=");

    stringOperand = new StringOperand("%bc");
    likeCondition = new LikeCondition(fieldOperand2, stringOperand);

    orCondition = new OrCondition();
    orCondition->addOperand(relation);
    orCondition->addOperand(likeCondition);

    result = orCondition->calculate(fields, row);

    cout << orCondition->toString();
    cout << result << endl << endl;


    cout << "WHERE NOT (f1 = 3 OR f2 LIKE '%bc')" << endl << endl;

    orCondition->negate();

    result = orCondition->calculate(fields, row);

    cout << orCondition->toString();
    cout << result << endl << endl;
}

int main() {
    testRelationCondition();
    testInCondition();
    testLikeCondition();

    testAndCondition();
    testOrCondition();
}
