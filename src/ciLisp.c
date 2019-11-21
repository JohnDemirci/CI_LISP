#include "ciLisp.h"
#include "math.h"

#define DEFAULT_RET_VAL ((RET_VAL){INT_TYPE, NAN})

// john

void yyerror(char *s) {
    fprintf(stderr, "\nERROR: %s\n", s);
    // note stderr that normally defaults to stdout, but can be redirected: ./src 2> src.log
    // CLion will display stderr in a different color from stdin and stdout
}

// Array of string values for operations.
// Must be in sync with funcs in the OPER_TYPE enum in order for resolveFunc to work.
char *funcNames[] = {
        "neg",
        "abs",
        "exp",
        "sqrt",
        "add",
        "sub",
        "mult",
        "div",
        "remainder",
        "log",
        "pow",
        "max",
        "min",
        "exp2",
        "cbrt",
        "hypot",
        "read",
        "rand",
        "print",
        "equal",
        "less",
        "greater",
        ""
};

OPER_TYPE resolveFunc(char *funcName) {
    int i = 0;
    while (funcNames[i][0] != '\0') {
        if (strcmp(funcNames[i], funcName) == 0)
            return i;
        i++;
    }
    return CUSTOM_OPER;
}


AST_NODE *makeNewNode(AST_NODE_TYPE type) {
    AST_NODE *node = calloc(1, sizeof(AST_NODE));
    if (node == NULL) {
        yyerror("Memory allocation failed!");
        exit(42069);
    }
    node->type = type;
    return node;
}
// Called when an INT or DOUBLE token is encountered (see ciLisp.l and ciLisp.y).
// Creates an AST_NODE for the number.
// Sets the AST_NODE's type to number.
// Populates the value of the contained NUMBER_AST_NODE with the argument value.
// SEE: AST_NODE, NUM_AST_NODE, AST_NODE_TYPE.


AST_NODE *createSymbolNode(char *ident) {
    AST_NODE *node = makeNewNode(SYMBOL_NODE_TYPE);
    node->data.symbol.ident = ident;
    return node;
}



AST_NODE *createNumberNode(double value, NUM_TYPE type) {
    // TODO set the AST_NODE's type, assign values to contained NUM_AST_NODE
    AST_NODE *node = makeNewNode(NUM_NODE_TYPE);
    node->data.number.type = type;
    node->data.number.value = value;
    return node;
}

// Called when an f_expr is created (see ciLisp.y).
// Creates an AST_NODE for a function call.
// Sets the created AST_NODE's type to function.
// Populates the contained FUNC_AST_NODE with:
//      - An OPER_TYPE (the enum identifying the specific function being called)
//      - 2 AST_NODEs, the operands
// SEE: AST_NODE, FUNC_AST_NODE, AST_NODE_TYPE.
AST_NODE *createFunctionNode(char *funcName, AST_NODE *op1, AST_NODE *op2) {
    AST_NODE *node = makeNewNode(FUNC_NODE_TYPE);
    node->data.function.ident = funcName;
    node->data.function.oper = resolveFunc(funcName);
    node->data.function.op1 = op1;
    node->data.function.op1->parent = node;
    if (op2 != NULL) {
        node->data.function.op2 = op2;
        node->data.function.op2->parent = node;
    }
    return node;
}

// Called after execution is done on the base of the tree.
// (see the program production in ciLisp.y)
// Recursively frees the whole abstract syntax tree.
// You'll need to update and expand freeNode as the project develops.
void freeNode(AST_NODE *node) {
    if (!node)
        return;

    if (node->type == FUNC_NODE_TYPE) {
        // Recursive calls to free child nodes
        freeNode(node->data.function.op1);
        freeNode(node->data.function.op2);

        // Free up identifier string if necessary
        if (node->data.function.oper == CUSTOM_OPER) {
            free(node->data.function.ident);
        }
    }

    free(node);
}

// Evaluates an AST_NODE.
// returns a RET_VAL storing the the resulting value and type.
// You'll need to update and expand eval (and the more specific eval functions below)
// as the project develops.
RET_VAL eval(AST_NODE *node) {
    if (!node)
        return (RET_VAL) {INT_TYPE, NAN};

    RET_VAL result = {INT_TYPE, NAN}; // see NUM_AST_NODE, because RET_VAL is just an alternative name for it.

    // TODO complete the switch.
    // Make calls to other eval functions based on node type.
    // Use the results of those calls to populate result.
    switch (node->type) {
        case NUM_NODE_TYPE:
            // do something
            result = evalNumNode(&node->data.number);

            break;
        case FUNC_NODE_TYPE:
            // do something
            result = evalFuncNode(&node->data.function);
            break;
        case SYMBOL_NODE_TYPE:
            result = evalSymbolNode(node);
            break;
        default:
            yyerror("Invalid AST_NODE_TYPE, probably invalid writes somewhere!");
    }

    return result;
}


RET_VAL evalSymbolNode(AST_NODE *node)
{
    if (!node) {
        return DEFAULT_RET_VAL;
    }
    SYMBOL_TABLE_NODE *john = findSymbol(node->data.symbol.ident, node);
    if (!john) {
        yyerror("Unexpected Identifier");
        return DEFAULT_RET_VAL;
    }
    return eval(john->val);
}

SYMBOL_TABLE_NODE *findSymbol(char *ident, AST_NODE *s_expr) {
    //Did the father step out for some cigarettes and never return?
    if (s_expr == NULL)
        return NULL;
    //Grab the symbol table for this scope
    SYMBOL_TABLE_NODE *node = s_expr->symbolTable;
    //Iterate till at end of scope list or found our boy
    while (node != NULL && strcmp(ident, node->ident) != 0) {
        node = node->next;
    }
    //Did we not find our boy?
    if (node == NULL) {
        //Ask his dad
        node = findSymbol(ident, s_expr->parent);
    }
    //return our boy (possibly)
    return node;
}


RET_VAL checker (RET_VAL op1, RET_VAL op2, RET_VAL result) {
    if (op1.type == DOUBLE_TYPE || op2.type == DOUBLE_TYPE) {
        result.type = DOUBLE_TYPE;
        return result;
    } else {
        result.type = INT_TYPE;
        return result;
    }
}

RET_VAL checkerWithOneOperan (RET_VAL op1, RET_VAL result) {
    if (op1.type == DOUBLE_TYPE ) {
        result.type = DOUBLE_TYPE;
        return result;
    } else {
        result.type = INT_TYPE;
        return result;
    }
}



RET_VAL negHelper(RET_VAL op1, RET_VAL result) {
    result.value = -1 * op1.value;
    result.type = op1.type;
    return result;
}


RET_VAL absHelper(RET_VAL op1, RET_VAL result) {
    result.value = fabs(op1.value);
    result.type = op1.type;
    return result;
}

RET_VAL expHelper(RET_VAL op1, RET_VAL result) {
    if (result.type == INT_TYPE) {
        result.value = round(exp(op1.value));
        return result;
    }
    result.value = exp(op1.value);
    return result;
}

RET_VAL sqrtHelper(RET_VAL op1, RET_VAL result) {
    if (result.type == INT_TYPE) {
        result.value = round(sqrt(op1.value));
        return result;
    }
    result.value = sqrt(op1.value);
    return result;
}

RET_VAL addHelper(RET_VAL op1, RET_VAL op2, RET_VAL result) {
    result.value = op1.value + op2.value;
    if (op1.type == DOUBLE_TYPE || op2.type == DOUBLE_TYPE) {
        result.type = DOUBLE_TYPE;
    }
    return result;
}

RET_VAL subHelper(RET_VAL op1, RET_VAL op2, RET_VAL result) {
    result.value = op1.value - op2.value;
    result = checker(op1,op2,result);
    return result;
}

RET_VAL multHelper(RET_VAL op1, RET_VAL op2, RET_VAL result) {
    result.value = op1.value * op2.value;
    result = checker(op1,op2,result);
    return result;
}

RET_VAL remainderHelper(RET_VAL op1, RET_VAL op2, RET_VAL result) {
    result = checker(op1,op2,result);
    if (result.type == INT_TYPE) {
        result.value = round(remainder(op1.value, op2.value));
        return result;
    }
    result.value = remainder(op1.value, op2.value);
    return result;
}

RET_VAL divHelper(RET_VAL op1, RET_VAL op2, RET_VAL result) {
    result = checker(op1,op2,result);
    if (result.type == INT_TYPE) {
        result.value = round(op1.value / op2.value);
        return result;
    }
    result.value = op1.value / op2.value;
    return result;
}

RET_VAL logHelper(RET_VAL op1, RET_VAL result) {
    result = checkerWithOneOperan(op1,result);
    if (result.type == INT_TYPE) {
        result.value = round(log(op1.value));
        return result;
    }
    result.value = log(op1.value);
    return result;
}

RET_VAL powerHelper(RET_VAL op1, RET_VAL op2, RET_VAL result) {
    result = checker(op1,op2,result);
    result.value = pow(op1.value, op2.value);
    return result;
}

RET_VAL maxHelper(RET_VAL op1, RET_VAL op2, RET_VAL result) {
    result = checker(op1,op2,result);
    result.value = fmax(op1.value, op2.value);
    return result;
}

RET_VAL minHelper(RET_VAL op1, RET_VAL op2, RET_VAL result) {
    result = checker(op1,op2,result);
    result.value = fmin(op1.value, op2.value);
    return result;
}

RET_VAL exp2Helper(RET_VAL op1, RET_VAL result) {
    result = checkerWithOneOperan(op1,result);
    if (result.type == INT_TYPE) {
        result.value = round(exp(op1.value));
        return result;
    }
    result.value = exp(op1.value);
    return result;
}

RET_VAL cbrtHelper(RET_VAL op1, RET_VAL result) {
    result = checkerWithOneOperan(op1,result);
    if (result.type == INT_TYPE) {
        result.value = round(cbrt(op1.value));
        return result;
    }
    result.value = exp(op1.value);
    return result;
}

RET_VAL hypotHelper(RET_VAL op1, RET_VAL op2, RET_VAL result) {
    result = checker(op1,op2,result);
    if (result.type == INT_TYPE) {
        result.value = round(hypot(op1.value, op2.value));
        return result;
    }
    result.value = hypot(op1.value, op2.value);
    return result;
}


// returns a pointer to the NUM_AST_NODE (aka RET_VAL) referenced by node.
// DOES NOT allocate space for a new RET_VAL.
RET_VAL evalNumNode(NUM_AST_NODE *numNode) {
    if (!numNode)
        return (RET_VAL) {INT_TYPE, NAN};
    RET_VAL result = {INT_TYPE, NAN};
    // TODO populate result with the values stored in the node.
    // SEE: AST_NODE, AST_NODE_TYPE, NUM_AST_NODE
    result.value = numNode->value;
    result.type = numNode->type;
    return result;
}

RET_VAL valuate () {
}




RET_VAL evalFuncNode(FUNC_AST_NODE *funcNode) {
    RET_VAL result = DEFAULT_RET_VAL;
    if (!funcNode) {
        return result;
    }
    // TODO populate result with the result of running the function on its operands.
    // SEE: AST_NODE, AST_NODE_TYPE, FUNC_AST_NODE
    RET_VAL op1rv = eval(funcNode->op1);
    RET_VAL op2rv = DEFAULT_RET_VAL;
    if (funcNode->op2 != NULL) {
        op2rv = eval(funcNode->op2);
    }
    /*double op1 = op1rv.value;
    double op2 = op2rv.value;
     // result.type = funcNode->op1->data.number.type;
     result = eval(funcNode->op1);*/

    // result.type = (int)resolveFunc(funcNode->ident);

    switch (funcNode->oper) {
        case NEG_OPER:
            // do something
            result = negHelper(op1rv, result);
            break;
        case ABS_OPER:
            result = absHelper(op1rv, result);
            break;
        case EXP_OPER:
            result = expHelper(op1rv, result);
            // result.value = exp(op1);
            break;
        case SQRT_OPER:
            result = sqrtHelper(op1rv, result);
            break;
        case ADD_OPER:
            result = addHelper(op1rv, op2rv, result);
            break;
        case SUB_OPER:
            result = subHelper(op1rv, op2rv, result);
            break;
        case MULT_OPER:
            result = multHelper(op1rv, op2rv, result);
            break;
        case DIV_OPER:
            result = divHelper(op1rv, op2rv, result);
            break;
        case REMAINDER_OPER:
            result = remainderHelper(op1rv, op2rv, result);
            break;
        case LOG_OPER:
            result = logHelper(op1rv, result);
            break;
        case POW_OPER:
            result = powerHelper(op1rv, op2rv, result);
            break;
        case MAX_OPER:
            result = maxHelper(op1rv, op2rv, result);
            break;
        case MIN_OPER:
            result = minHelper(op1rv, op2rv, result);
            break;
        case EXP2_OPER:
            result = exp2Helper(op1rv, result);
            break;
        case CBRT_OPER:
            result = cbrtHelper(op1rv, result);
            break;
        case HYPOT_OPER:
            result = hypotHelper(op1rv, op2rv, result);
            break;
        case READ_OPER:
            break;
        case RAND_OPER:
            break;
        case PRINT_OPER:
            break;
        case EQUAL_OPER:
            break;
        case LESS_OPER:
            break;
        case GREATER_OPER:
            break;
        case CUSTOM_OPER:
            break;
        default:
            printf("oof");
    }
    return result;
}

// prints the type and value of a RET_VAL
void printRetVal(RET_VAL val) {
    // TODO print the type and value of the value passed in.
    switch (val.type) {
        case DOUBLE_TYPE:
            // do something
            printf("\ntype: Double");
            printf("\nvalue: %lf", val.value);
            break;
        case INT_TYPE:
            printf("\ntype: Int");
            // int v = (int)val.value;

            printf("\nvalue: %d", (int) val.value);
            // do something
            break;
    }
    // printf("%lf", val.value);
}

// createSymbolNode


SYMBOL_TABLE_NODE * makeNewSymbol()
{
    SYMBOL_TABLE_NODE * node = calloc(1, sizeof(SYMBOL_TABLE_NODE));
    if (node == NULL) {
        yyerror("Memory allocation failed!");
        exit(420420);
    }
    return node;
}


SYMBOL_TABLE_NODE *createSymbolTableNode(char *ident, AST_NODE *s_expr) {
    SYMBOL_TABLE_NODE *node = makeNewSymbol();
    node->ident = ident;
    node->val = s_expr;
    return node;
}


SYMBOL_TABLE_NODE *addToSymbolTable(SYMBOL_TABLE_NODE *list, SYMBOL_TABLE_NODE *item) {
    if (item == NULL) {
        item = list;
    }
    item->next = list;
    return item;
}


AST_NODE *linkSymbolNode(SYMBOL_TABLE_NODE *symbTable, AST_NODE *s_expr) {
    s_expr->symbolTable = symbTable;
    return s_expr;
}
