# Data Base Management System

## Project structure

`src/engine` - engine component  
`src/query_parser` - parser component  
`src/query_processor` - query execution component  
`src/socket` - socket library  
`src/shared` - helper library for work with std::string/std::vector/std::set  

## Implemented SQL

### SQL query

- \<SQL-query> ::= \<SELECT-query> | \<INSERT-query> | \<UPDATE-query> | \<DELETE-query> | \<CREATE-query> | \<DROP-query>  

### Query types

- \<SELECT-query> ::= SELECT \<fields> FROM \<table name> [\<WHERE-clause>]  
- \<INSERT-query> ::= INSERT INTO \<table name> (\<field value> { , \<field value> })  
- \<UPDATE-query> ::= UPDATE \<table name> SET \<field name> = \<expression> [\<WHERE-clause>]  
- \<DELETE-query> ::= DELETE FROM \<table name> \<WHERE-clause>  
- \<CREATE-query> ::= CREATE TABLE \<table name> ( \<field descriptions list> )  
- \<DROP-query> ::= DROP TABLE \<table name>  

### WHERE clause

- \<WHERE-clause> ::= WHERE (\<boolean expression> | ALL)  
- \<boolean expression> ::= \<boolean term> { OR \<boolean term> }  
- \<boolean term> ::= \<boolean factor> { AND \<boolean factor> }  
- \<boolean factor> ::= ( NOT \<boolean factor> ) | ((\<boolean expression>)) | \<operation>  
- \<operation> ::= \<equation> | \<like-expression> | \<in-expression>  
- \<like-expression> ::= \<field name> [ NOT ] LIKE \<string>  
- \<in-expression> ::= \<expression> [ NOT ] IN ( \<constants list> )  
- \<equation> ::= \<string-equation> | \<number-equation>  
- \<string-equation> ::= \<string-expression> \<comparation> \<string-expression>  
- \<number-equation> ::= \<number-expression> \<comparation> \<number-expression>  
- \<comparation> ::= = | > | \< | >= | \<= | !=  

### Common

- \<fields> ::= \<field name> { , \<field name> } | *  
- \<table name> ::= \<name>  
- \<field name> ::= \<name>  
- \<field value> ::= \<string> | \<number>  
- \<name> ::= \<name> [ \<char> | \<digit> | _ ] | \<char>  
- \<field value> ::= \<string> | \<number>  
- \<string> ::= '\<symbol> { \<symbol> }'  
- \<symbol> ::= \<any symbol but '>  
- \<field descriptions list> ::= \<field description> { , \<field description> }  
- \<field description> ::= \<field name> \<field type>  
- \<field type> ::= VARCHAR ( \<unsigned> ) | NUMBER  
- \<expression> ::= \<string-expression> | \<number-expression>  
- \<string-expression> ::= \<field name> | \<string>  
- \<number-expression> ::= \<field name> | \<number>  
- \<number> ::= \<digit without 0>\<digits sequence>[ .[ \<digits sequence> ] \<digit without 0> ]  
- \<digits sequence> ::= \<digit> [ \<digits sequence> ]  

## Components

### Sockets library

- `BaseSocket` - Abstract base class  
- `ServerSocket`  
- `ClientSocket`  
- `SocketException`  

### Engine

- `DataType` \<- `Number`, `Varcher` - supported types  
- `Table`, `TableField`, `TableRow` - table structure classes  
- `TableIO` - interface for low-level file I/O    
- `Engine` - namespace providing interface for engine interaction    
- `EngineException`, `EngineStatusEnum`  

### Query parser

- `LexicParser`  
- `QueryParse`  
- `QueryParserException`, `QueryStatusEnum`  

### Query executor

- `condition_tree/*` - WHERE-clause processing  
- `QueryObject` \<- `SelectObject`, `InsertObject`, ... - query internal representation  
- `QueryResult` \<- `SelectQueryResult`, `InsertQueryResult`, ... - query result representation    

## Before running

- `src/engine/Config.h` change `STORAGE_LOCATION` to yours  

## Compilation

- Build - `make main`  
- Test - `make test`  

## Run

- Run server - `./out/main/server.out`  
- Run client - `./out/main/client.out`  
