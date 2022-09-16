#ifndef QUERY_PARSER_H
#define QUERY_PARSER_H

#include <vector>
#include <string>

#include "../query_processor/QueryObject.h"
#include "../query_processor/SelectObject.h"
#include "../query_processor/InsertObject.h"
#include "../query_processor/UpdateObject.h"
#include "../query_processor/DropObject.h"
#include "../query_processor/DeleteObject.h"
#include "../query_processor/CreateObject.h"

#include "../query_processor/condition_tree/OrCondition.h"
#include "../query_processor/condition_tree/AndCondition.h"
#include "../query_processor/condition_tree/RelationCondition.h"
#include "../query_processor/condition_tree/LikeCondition.h"
#include "../query_processor/condition_tree/InCondition.h"

#include "../query_processor/condition_tree/NumberSetOperand.h"
#include "../query_processor/condition_tree/StringSetOperand.h"
#include "../query_processor/condition_tree/NumberOperand.h"
#include "../query_processor/condition_tree/StringOperand.h"
#include "../query_processor/condition_tree/TableFieldOperand.h"

#include "../engine/TableField.h"

namespace QueryParser {
    QueryObject* parseQuery(string query);

    SelectObject* parseSelectQuery(vector<string> queryTokens);
    InsertObject* parseInsertQuery(vector<string> queryTokens);
    UpdateObject* parseUpdateQuery(vector<string> queryTokens);
    DeleteObject* parseDeleteQuery(vector<string> queryTokens);
    CreateObject* parseCreateQuery(vector<string> queryTokens);
    DropObject* parseDropQuery(vector<string> queryTokens);

    vector<DataType*> parseFieldValues(vector<string> queryTokens);
    vector<TableField*> parseFieldDescriptions(vector<string> queryTokens);
    TableField* parseTableField(vector<string> tableInfo);

    OrCondition* parseWhereClause(vector<string> queryTokens);
    OrCondition* parseLogicExpression(vector<string> queryTokens);
    AndCondition* parseLogicTerm(vector<string> queryTokens);
    BaseCondition* parseLogicFactor(vector<string> queryTokens);
    BinaryCondition* parseOperation(vector<string> queryTokens);
    RelationCondition* parseRelation(vector<string> queryTokens);
    LikeCondition* parseLikeOperation(vector<string> queryTokens);
    InCondition* parseSetOperation(vector<string> queryTokens);
}

#endif
