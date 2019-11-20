#include "ciLisp.h"
#include "math.h"

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

OPER_TYPE resolveFunc(char *funcName)
{
    int i = 0;
    while (funcNames[i][0] != '\0')
    {
        if (strcmp(funcNames[i], funcName) == 0)
            return i;
        i++;
    }
    return CUSTOM_OPER;
}

// Called when an INT or DOUBLE token is encountered (see ciLisp.l and ciLisp.y).
// Creates an AST_NODE for the number.
// Sets the AST_NODE's type to number.
// Populates the value of the contained NUMBER_AST_NODE with the argument value.
// SEE: AST_NODE, NUM_AST_NODE, AST_NODE_TYPE.


AST_NODE *createSymbolNode (char* ident) {
    AST_NODE *node;
    size_t nodeSize;
    nodeSize = sizeof(AST_NODE);
    if ((node = calloc(nodeSize, 1)) == NULL)
        yyerror("Memory allocation failed!");
    node->data.symbol.ident = (char *)malloc(sizeof(char *));
    node->type = SYMBOL_NODE_TYPE;
    strcpy(node->data.symbol.ident, ident);
    eval(node);
    return node;
}





AST_NODE *createNumberNode(double value, NUM_TYPE type)
{
    AST_NODE *node;
    size_t nodeSize;

    // allocate space for the fixed sie and the variable part (union)
    nodeSize = sizeof(AST_NODE);
    if ((node = calloc(nodeSize, 1)) == NULL)
        yyerror("Memory allocation failed!");

    // TODO set the AST_NODE's type, assign values to contained NUM_AST_NODE

    node->type = NUM_NODE_TYPE;

    node->data.number.type = type;
    node->data.number.value = value;
    eval(node);
    return node;
}

// Called when an f_expr is created (see ciLisp.y).
// Creates an AST_NODE for a function call.
// Sets the created AST_NODE's type to function.
// Populates the contained FUNC_AST_NODE with:
//      - An OPER_TYPE (the enum identifying the specific function being called)
//      - 2 AST_NODEs, the operands
// SEE: AST_NODE, FUNC_AST_NODE, AST_NODE_TYPE.
AST_NODE *createFunctionNode(char *funcName, AST_NODE *op1, AST_NODE *op2)
{
    AST_NODE *node;
    size_t nodeSize;

    // allocate space (or error)
    nodeSize = sizeof(AST_NODE);
    if ((node = calloc(nodeSize, 1)) == NULL)
        yyerror("Memory allocation failed!");

    // TODO set the AST_NODE's type, populate contained FUNC_AST_NODE
    // NOTE: you do not need to populate the "ident" field unless the function is type CUSTOM_OPER.
    // When you do have a CUSTOM_OPER, you do NOT need to allocate and strcpy here.
    // The funcName will be a string identifier for which space should be allocated in the tokenizer.
    // For CUSTOM_OPER functions, you should simply assign the "ident" pointer to the passed in funcName.
    // For functions other than CUSTOM_OPER, you should free the funcName after you're assigned the OPER_TYPE.

    // funcName = (char*)malloc((strlen(funcName)+1)* sizeof(char));
   /* node->data.function.op1 = ()malloc(sizeof(char *));
    node->data.function.op2 = malloc(sizeof(char *));*/

    node->type = FUNC_NODE_TYPE;
    // funcname will be a func
    int position;
    int comparison;
    size_t NumberOfElements = sizeof(funcNames)/sizeof(funcNames[0]);
    int size = (int)NumberOfElements;
    for (position = 0; position < size; position++ ) {

        comparison = strcmp(funcNames[position], funcName);
        if (comparison == 0) {
            node->data.function.oper = position;
            node->data.function.op1 = op1;
            node->data.function.op2 = op2;

            if (node->data.function.op1 != NULL ) {
                node->data.function.op1->parent = node;
            }
            if (node->data.function.op2 != NULL) {
                node->data.function.op2->parent = node;
            }
            free(funcName);
            eval(node);
            // evalFuncNode(&node->data.function);
            break;
        }
    }
    if (position == CUSTOM_OPER) {
        node->data.function.oper = position;
        node->data.function.op1 = op1;
        node->data.function.op2 = op2;
        node->data.function.ident = funcName;
        free(funcName);
        eval(node);
    }
    return node;
}

// Called after execution is done on the base of the tree.
// (see the program production in ciLisp.y)
// Recursively frees the whole abstract syntax tree.
// You'll need to update and expand freeNode as the project develops.
void freeNode(AST_NODE *node)
{
    if (!node)
        return;

    if (node->type == FUNC_NODE_TYPE)
    {
        // Recursive calls to free child nodes
        freeNode(node->data.function.op1);
        freeNode(node->data.function.op2);

        // Free up identifier string if necessary
        if (node->data.function.oper == CUSTOM_OPER)
        {
            free(node->data.function.ident);
        }
    }

    free(node);
}

// Evaluates an AST_NODE.
// returns a RET_VAL storing the the resulting value and type.
// You'll need to update and expand eval (and the more specific eval functions below)
// as the project develops.
RET_VAL eval(AST_NODE *node)
{
    if (!node)
        return (RET_VAL){INT_TYPE, NAN};

    RET_VAL result = {INT_TYPE, NAN}; // see NUM_AST_NODE, because RET_VAL is just an alternative name for it.

    // TODO complete the switch.
    // Make calls to other eval functions based on node type.
    // Use the results of those calls to populate result.
    switch (node->type)
    {
        case NUM_NODE_TYPE:
            // do something
            result = evalNumNode(&node->data.number);

            break;
        case FUNC_NODE_TYPE:
            // do something
            result = evalFuncNode(&node->data.function);
            break;
        case SYMBOL_NODE_TYPE:
            result = evalSyymbolNode(&node->data.symbol);
        default:
            yyerror("Invalid AST_NODE_TYPE, probably invalid writes somewhere!");
    }

    return result;
}


RET_VAL evalSyymbolNode (SYMBOL_AST_NODE *symbol) {

}

SYMBOL_TABLE_NODE *findSymbol (char* ident, AST_NODE s_expr) {
    SYMBOL_TABLE_NODE *node;
    node = s_expr.symbolTable;
    while (node != NULL) {
        if (strcmp(ident, node->ident) == 0) {
            return  node;
        }
        node = node->next;
    }
    SYMBOL_TABLE_NODE *item;

    item = findSymbol(ident, *(s_expr.parent));
    return item;
}


RET_VAL negHelper(double op1, RET_VAL result) {
    result.value = -1 * op1;
    return result;
}


RET_VAL absHelper (double op1, RET_VAL result)
{
    result.value = fabs(op1);
    return result;
}

RET_VAL expHelper (double op1, RET_VAL result)
{
    if (result.type == INT_TYPE) {
        result.value = round(exp(op1));
        return result;
    }
    result.value = exp(op1);
    return result;
}

RET_VAL sqrtHelper (double op1, RET_VAL result)
{
    if (result.type == INT_TYPE) {
        result.value = round(sqrt(op1));
        return result;
    }
    result.value = sqrt(op1);
    return result;
}

RET_VAL  addHelper (double op1, double op2, RET_VAL result) {
    result.value = op1 + op2;
    return result;
}

RET_VAL  subHelper (double op1, double op2, RET_VAL result) {
    result.value = op1 - op2;
    return result;
}

RET_VAL  multHelper (double op1, double op2, RET_VAL result) {
    result.value = op1 * op2;
    return result;
}
RET_VAL  remainderHelper (double op1, double op2, RET_VAL result) {
    if (result.type == INT_TYPE) {
        result.value = round(remainder(op1,op2));
        return result;
    }
    result.value = remainder(op1,op2);
    return result;
}

RET_VAL  divHelper (double op1, double op2, RET_VAL result) {

    if (result.type == INT_TYPE) {
        result.value = round(op1/op2);
        return result;
    }
    result.value = op1 / op2;
    return result;
}

RET_VAL logHelper (double op1, RET_VAL result)
{
    if (result.type == INT_TYPE) {
        result.value = round(log(op1));
        return result;
    }
    result.value = log(op1);
    return result;
}

RET_VAL  powerHelper (double op1, double op2, RET_VAL result) {
    result.value = pow(op1,op2);
    return result;
}

RET_VAL  maxHelper (double op1, double op2, RET_VAL result) {
    result.value = fmax(op1,op2);
    return result;
}

RET_VAL  minHelper (double op1, double op2, RET_VAL result) {
    result.value = fmin(op1,op2);
    return result;
}

RET_VAL exp2Helper (double op1, RET_VAL result)
{
    if (result.type == INT_TYPE) {
        result.value = round(exp(op1));
        return result;
    }
    result.value = exp(op1);
    return result;
}

RET_VAL cbrtHelper (double op1, RET_VAL result)
{
    if (result.type == INT_TYPE) {
        result.value = round(cbrt(op1));
        return result;
    }
    result.value = exp(op1);
    return result;
}

RET_VAL  hypotHelper (double op1, double op2, RET_VAL result) {
    if (result.type == INT_TYPE) {
        result.value = round(hypot(op1,op2));
        return result;
    }
    result.value = hypot(op1,op2);
    return result;
}



// returns a pointer to the NUM_AST_NODE (aka RET_VAL) referenced by node.
// DOES NOT allocate space for a new RET_VAL.
RET_VAL evalNumNode(NUM_AST_NODE *numNode)
{
    if (!numNode)
        return (RET_VAL){INT_TYPE, NAN};
    RET_VAL result = {INT_TYPE, NAN};
    // TODO populate result with the values stored in the node.
    // SEE: AST_NODE, AST_NODE_TYPE, NUM_AST_NODE
    result.value = numNode->value;
    result.type = numNode->type;
    return result;
}






RET_VAL evalFuncNode(FUNC_AST_NODE *funcNode)
{
    if (!funcNode)
        return (RET_VAL){INT_TYPE, NAN};
    RET_VAL result = {INT_TYPE, NAN};
    // TODO populate result with the result of running the function on its operands.
    // SEE: AST_NODE, AST_NODE_TYPE, FUNC_AST_NODE
    result.type = funcNode->op1->data.number.type;
    double op1 = funcNode->op1->data.number.value;
    double op2 = 0; // initial
    if  (funcNode->op2 != NULL) {
        op2 = funcNode->op2->data.number.value;
    }
    switch (funcNode->oper) {
        case NEG_OPER:
            // do something
            result = negHelper(op1,result);
            break;
        case ABS_OPER:
            result = absHelper(op1, result);
            break;
        case EXP_OPER:
            result = expHelper(op1,result);
            // result.value = exp(op1);
            break;
        case SQRT_OPER:
            result = sqrtHelper(op1, result);
            break;
        case ADD_OPER:
            result = addHelper(op1, op2, result);
            break;
        case SUB_OPER:
            result = subHelper(op1,op2,result);
            break;
        case MULT_OPER:
            result = multHelper(op1,op2,result);
            break;
        case DIV_OPER:
            result = divHelper(op1,op2,result);
            break;
        case REMAINDER_OPER:
            result = remainderHelper(op1,op2,result);
            break;
        case LOG_OPER:
            result = logHelper(op1,result);
            break;
        case POW_OPER:
            result = powerHelper(op1,op2,result);
            break;
        case MAX_OPER:
            result = maxHelper(op1,op2,result);
            break;
        case MIN_OPER:
            result = minHelper(op1,op2,result);
            break;
        case EXP2_OPER:
            result = exp2Helper(op1,result);
            break;
        case CBRT_OPER:
            result = cbrtHelper(op1,result);
            break;
        case HYPOT_OPER:
            result = hypotHelper(op1,op2,result);
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
void printRetVal(RET_VAL val)
{
    // TODO print the type and value of the value passed in.
    switch (val.type) {
        case DOUBLE_TYPE:
            // do something
            printf("\ntype: Double");
            printf("\nvalue: %lf", val.value);
            break;
        case INT_TYPE:
            printf("\ntype: Int");
            printf("\nvalue: %d", (int)val.value);
            // do something
            break;
    }
}

// createSymbolNode





AST_NODE *createSymbolTableNode (char* ident, AST_NODE *s_expr) {
    AST_NODE *node;
    size_t nodeSize;
    // allocate space for the fixed sie and the variable part (union)
    nodeSize = sizeof(AST_NODE);

    if ((node = calloc(nodeSize, 1)) == NULL)
        yyerror("Memory allocation failed!");
    node->type =SYMBOL_NODE_TYPE;
    node->symbolTable->ident = (char *) malloc(sizeof(char*));
    strcpy(node->symbolTable->ident, ident);
    node->symbolTable->val = s_expr->symbolTable->val;
    node->symbolTable->next = NULL;
    return node;

 /*   s_expr->symbolTable->ident = (char *)malloc(sizeof(char*));
    s_expr->type = SYMBOL_NODE_TYPE;
    strcpy(s_expr->symbolTable->ident, ident);
    s_expr->symbolTable->next = NULL;
    return s_expr;*/
}




SYMBOL_TABLE_NODE *addToSymbolTable (SYMBOL_TABLE_NODE *list, SYMBOL_TABLE_NODE *item) {
    if (item == NULL) {
        item = list;
        item->next = NULL;
    }
    item->next = list;
    return item;
}


AST_NODE *linkSymbolNode (SYMBOL_TABLE_NODE* symbTable, AST_NODE *s_expr) {
    s_expr->symbolTable = symbTable;
    return  s_expr;
}