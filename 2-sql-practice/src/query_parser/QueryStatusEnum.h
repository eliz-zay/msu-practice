#ifndef QUERY_STATUS_ENUM
#define QUERY_STATUS_ENUM

enum QueryStatusEnum {
    MissingSemicolon,
    UnknownCommand,
    WrongSelectSyntax,
    WrongInsertSyntax,
    WrongUpdateSyntax,
    WrongDeleteSyntax,
    WrongCreateSyntax,
    WrongDropSyntax,
    WrongWhereClauseSyntax,
    WrongBracesPlacement,
    InvalidStringOperand,
    WrongTableFieldName
};

#endif
