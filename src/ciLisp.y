%{
    #include "ciLisp.h"
%}

%union {
    double dval;
    char *sval;
    struct ast_node *astNode;
    struct symbol_table_node *symbolNode;
}

%token <sval> FUNC SYMBOL
%token <dval> INT_LITERAL DOUBLE_LITERAL
%token LPAREN RPAREN EOL QUIT LET INT DOUBLE

%type <astNode> s_expr_list s_expr f_expr number
%type <symbolNode> let_list let_elem let_section
%%

program:
    s_expr EOL
    {
        fprintf(stderr, "yacc: program ::= s_expr EOL\n");
        if ($1) {
            printRetVal(eval($1));
            freeNode($1);
        }
    };

s_expr:

    number
    {
        fprintf(stderr, "yacc: s_expr ::= number\n");
        $$ = $1;
    }
    | SYMBOL
    {
            fprintf(stderr, "yacc: s_expr ::= SYMBOL\n");
            $$ = createSymbolNode($1);
    }
    | LPAREN let_section s_expr RPAREN
    {
    	    fprintf(stderr, "yacc: s_expr ::= SYMBOL\n");
            $$ = linkSymbolNode($2,$3);
    }
    | f_expr
    {
    	fprintf(stderr, "yacc: s_expr ::= f_expr\n");
        $$ = $1;
    }
    | QUIT
    {
        fprintf(stderr, "yacc: s_expr ::= QUIT\n");
        exit(EXIT_SUCCESS);
    }
    | error
    {
        fprintf(stderr, "yacc: s_expr ::= error\n");
        yyerror("unexpected token");
        $$ = NULL;
    };

number:
    INT_LITERAL
    {
        fprintf(stderr, "yacc: number ::= INT\n");
        $$ = createNumberNode($1, INT_TYPE);
    }
    | DOUBLE_LITERAL
    {
        fprintf(stderr, "yacc: number ::= DOUBLE\n");
        $$ = createNumberNode($1, DOUBLE_TYPE);
    }
    | INT INT_LITERAL
    {
    	fprintf(stderr, "yacc: number ::= INT INT_LITERAL\n");
       	$$ = createNumberNode($2, INT_TYPE);
    }
    | DOUBLE DOUBLE_LITERAL
    {
    	fprintf(stderr, "yacc: number ::= DOUBLE DOUBLE_LITERAL\n");
    	$$ = createNumberNode($2, DOUBLE_TYPE);
    }
    | INT DOUBLE_LITERAL
    {
 	fprintf(stderr, "yacc: number ::= INT DOUBLE_LITERAL\n");
     	$$ = createNumberNode($2, INT_TYPE);
    }
    | DOUBLE INT_LITERAL
    {
    	fprintf(stderr, "yacc: number ::= DOUBLE INT_LITERAL\n");
	$$ = createNumberNode($2, DOUBLE_TYPE);
    };

f_expr:
    LPAREN FUNC s_expr_list RPAREN
    {
        fprintf(stderr, "yacc: s_expr ::= LPAREN FUNC s_expr_list RPAREN\n");
        $$ = createFunctionNode($2, $3);
    };

let_section:
    /* <empty> */ {
    	$$=NULL;
    }
    | LPAREN let_list RPAREN
    {
    	fprintf(stderr, "yacc: s_expr ::= LPAREN let_list RPAREN\n");
    	$$=$2;
    };
let_list:
    LET let_elem
    {
    	fprintf(stderr, "yacc: s_expr ::= let let_list");
        $$=$2;
    }
    | let_list let_elem
    {
    	fprintf(stderr, "yacc: s_expr ::= let_list let_elem");
    	$$=addToSymbolTable($1, $2);
    };
let_elem:
    LPAREN SYMBOL s_expr RPAREN
    {
    	fprintf(stderr, "yacc: s_expr ::= LPAREN symbol expr RPAREN\n");
    	$$=createSymbolTableNode($2,$3,NO_TYPE);
    }
    | LPAREN INT SYMBOL s_expr RPAREN
    {
    	fprintf(stderr, "yacc: s_expr ::= LPAREN symbol expr RPAREN\n");
        $$=createSymbolTableNode($3,$4,INT_TYPE);
    }
    | LPAREN DOUBLE SYMBOL s_expr RPAREN
    {
    	fprintf(stderr, "yacc: s_expr ::= LPAREN symbol expr RPAREN\n");
        $$=createSymbolTableNode($3,$4,DOUBLE_TYPE);
    };
s_expr_list:
    s_expr
    {
    	fprintf(stderr, "yacc: s_expr_list ::= s_expr s_expr_list\n");
    	$$=$1;
    }
    | /* <empty> */{
    	$$=NULL;
    }
    | s_expr s_expr_list
    {
    	fprintf(stderr, "yacc: s_expr_list ::= s_expr s_expr_list\n");
    	$$=addSexprToList($1,$2);
    }
%%

