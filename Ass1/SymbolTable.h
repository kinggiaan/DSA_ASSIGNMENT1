#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"


/////////////Function Addition/////

bool checkCode(string str);
bool checkConstDigi(string str);
bool checkConstString(string str);
bool checkQuote(string str);
string deleteQuote(string str);

struct Symbol {
    string indentify;
    string type;
    int level;
    Symbol() : indentify(""), type(""), level(0) {}
    Symbol(string _inden, string _type,int global_level) : indentify(_inden), type(_type), level(global_level) {}
};

struct SymbolNode {
    Symbol data;
    SymbolNode* next;
    int redeclared;

    SymbolNode() : data(Symbol()), next(nullptr), redeclared(0) {}
    SymbolNode(Symbol _data, SymbolNode* _next, int _redeclare) : data(_data), next(_next), redeclared(_redeclare) {}


};
class SymbolTable
{
public:
    SymbolNode* head;
    
    SymbolTable();
    SymbolTable(SymbolNode* _head);
    ~SymbolTable() ;
    void run(string filename);
    bool checkSameBlockLevelDec(SymbolNode* node);
    
    int LOOKUPLargest(string _iden);
    
    void PRINT();
    void RPRINT();
    void REVERSE();
};
#endif


