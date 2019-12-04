#ifndef __cilisp_h_
#define __cilisp_h_
// Umit Demirci
// CS232
// Project: CILisp
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "ciLispParser.h"

int yyparse(void);

int yylex(void);

void yyerror(char *);

// Enum of all operators.
// must be in sync with funcs in resolveFunc()
typedef enum oper {
    NEG_OPER, // 0
    ABS_OPER,
    EXP_OPER,
    SQRT_OPER,
    ADD_OPER,
    SUB_OPER,
    MULT_OPER,
    DIV_OPER,
    REMAINDER_OPER,
    LOG_OPER,
    POW_OPER,
    MAX_OPER,
    MIN_OPER,
    EXP2_OPER,
    CBRT_OPER,
    HYPOT_OPER,
    READ_OPER,
    RAND_OPER,
    PRINT_OPER,
    EQUAL_OPER,
    LESS_OPER,
    GREATER_OPER,
    CUSTOM_OPER =255
} OPER_TYPE;

OPER_TYPE resolveFunc(char *);

// Types of Abstract Syntax Tree nodes.
// Initially, there are only numbers and functions.
// You will expand this enum as you build the project.
typedef enum {
    NUM_NODE_TYPE,
    FUNC_NODE_TYPE,
    SYMBOL_NODE_TYPE
} AST_NODE_TYPE;

// Types of numeric values
typedef enum { NO_TYPE, INT_TYPE, DOUBLE_TYPE } NUM_TYPE;


// Node to store a number.
typedef struct {
    NUM_TYPE type;
    double value;
} NUM_AST_NODE;

// Values returned by eval function will be numbers with a type.
// They have the same structure as a NUM_AST_NODE.
// The line below allows us to give this struct another name for readability.
typedef NUM_AST_NODE RET_VAL;

// Node to store a function call with its inputs
typedef struct {
    char *name;
    struct ast_node *opList;
} FUNC_AST_NODE;

typedef struct symbol_ast_node {
    char *ident;
} SYMBOL_AST_NODE;

typedef struct symbol_table_node {
    NUM_TYPE val_type;
    char *ident;
    struct ast_node *val;
    struct symbol_table_node *next;
} SYMBOL_TABLE_NODE;

// Generic Abstract Syntax Tree node. Stores the type of node,
// and reference to the corresponding specific node (initially a number or function call).
typedef struct ast_node {
    AST_NODE_TYPE type;
    SYMBOL_TABLE_NODE *symbolTable;
    struct ast_node *parent;
    union {
        NUM_AST_NODE number;
        FUNC_AST_NODE function;
        SYMBOL_AST_NODE symbol;
    } data;
    struct ast_node *next;
} AST_NODE;


AST_NODE *addSexprToList (AST_NODE *item, AST_NODE *list);
AST_NODE *createNumberNode(double value, NUM_TYPE type);
AST_NODE *createSymbolNode (char* ident);
AST_NODE *createFunctionNode(char *funcName, AST_NODE *op1, AST_NODE *op2);
SYMBOL_TABLE_NODE *createSymbolTableNode (char* ident, AST_NODE *s_expr, NUM_TYPE type);

RET_VAL eval(AST_NODE *node);
RET_VAL evalNumNode(NUM_AST_NODE *numNode);
RET_VAL evalFuncNode(FUNC_AST_NODE *funcNode);
RET_VAL evalSymbolNode (AST_NODE *node);
RET_VAL checker (RET_VAL op1, RET_VAL op2, RET_VAL result);
RET_VAL checkerWithOneOperan (RET_VAL op1, RET_VAL result);

        void printRetVal(RET_VAL val);
AST_NODE *linkSymbolNode (SYMBOL_TABLE_NODE* symbTable, AST_NODE *s_expr);
SYMBOL_TABLE_NODE *addToSymbolTable (SYMBOL_TABLE_NODE *list, SYMBOL_TABLE_NODE *item);
SYMBOL_TABLE_NODE *findSymbol (char* ident, AST_NODE *s_expr);

void freeNode(AST_NODE *node);


#endif
