%{
    #include "ciLisp.h"
%}

%union {
    double dval;
    char *sval;
    struct ast_node *astNode;
    struct symbol_table_node *symNode;
}

%token <sval> FUNC SYMBOL
%token <dval> INT_LITTERAL DOUBLE_LITTERAL
%token LPAREN RPAREN EOL QUIT LET INT DOUBLE

%type <astNode> s_expr f_expr number symbol
%type <symNode> let_elem let_section let_list

%%

program:
    s_expr EOL {
        fprintf(stderr, "yacc: program ::= s_expr EOL\n");
        if ($1) {
            printRetVal(eval($1));
            freeNode($1);
        }
    };

s_expr:
    number {
        fprintf(stderr, "yacc: s_expr ::= number\n");
        $$ = $1;
    }
    | symbol {
    	fprintf(stderr, "yacc: s_expr ::= symbol\n");
    	$$ = $1;
    }
    | f_expr {
    	fprintf(stderr, "yacc: s_expr ::= f_expr\n");
        $$ = $1;
    }
    | QUIT {
        fprintf(stderr, "yacc: s_expr ::= QUIT\n");
        exit(EXIT_SUCCESS);
    }
    | error {
        fprintf(stderr, "yacc: s_expr ::= error\n");
        yyerror("unexpected token");
        $$ = NULL;

    }
    | LPAREN let_section s_expr RPAREN {
        fprintf(stderr, "yacc: s_expr ::= LPAREN let_section s_expr RPAREN \n");
        $$ = linkSymbolNode($2, $3);
    };

number:
    INT_LITTERAL {
     fprintf(stderr, "yacc: number ::= INT\n");
        $$ = createNumberNode($1, NO_TYPE);
    }
    | INT INT_LITTERAL {
        fprintf(stderr, "yacc: number ::= INT_LITTERAL\n");
        $$ = createNumberNode($2, INT_TYPE);
    }
    | DOUBLE INT_LITTERAL {
        fprintf(stderr, "yacc: number ::= INT_LITTERAL\n");
        $$ = createNumberNode($2, INT_TYPE);
    }
    | DOUBLE_LITTERAL {
        fprintf(stderr, "yacc: number ::= DOUBLE\n");
        $$ = createNumberNode($1, NO_TYPE);
    }
    | INT DOUBLE_LITTERAL {
        fprintf(stderr, "yacc: number ::= DOUBLE\n");
        $$ = createNumberNode($2, DOUBLE_TYPE);
    }
    | DOUBLE DOUBLE_LITTERAL {
         fprintf(stderr, "yacc: number ::= DOUBLE\n");
         $$ = createNumberNode($2, DOUBLE_TYPE);
     };

f_expr:
    LPAREN FUNC s_expr RPAREN {
        fprintf(stderr, "yacc: f_expr ::= LPAREN FUNC expr RPAREN\n");
        $$ = createFunctionNode($2, $3, NULL);
    }
    | LPAREN FUNC s_expr s_expr RPAREN {
        fprintf(stderr, "yacc: f_expr ::= LPAREN FUNC expr expr RPAREN\n");
        $$ = createFunctionNode($2, $3, $4);
    };

let_elem:
    LPAREN SYMBOL s_expr RPAREN {
    	fprintf(stderr, "yacc: let_elem ::= LPAREN SYMBOL s_expr RPAREN\n");
    	$$ = createSymbolTableNode($2, $3);
    }
    | LPAREN INT SYMBOL s_expr RPAREN {
        fprintf(stderr, "yacc: let_elem ::= LPAREN SYMBOL s_expr RPAREN\n");
        $$ = createSymbolTableNode($3, $4);
     }
    | LPAREN DOUBLE SYMBOL s_expr RPAREN {
        fprintf(stderr, "yacc: let_elem ::= LPAREN SYMBOL s_expr RPAREN\n");
        $$ = createSymbolTableNode($3, $4);
    };

let_list:
    LET let_elem {
    	fprintf(stderr, "yacc: let_list ::= LET let_elem\n");
        $$ = $2;
    }
    | let_list  let_elem  {
    	fprintf(stderr, "yacc: let_list ::= let_list  let_elem\n");
    	$$ = addToSymbolTable($1, $2);
    };

let_section:
    /* <empty> */ {
    	fprintf(stderr, "yacc: let_section ::= <empty>\n");
    	$$ = NULL;
    }
    | LPAREN let_list RPAREN {
    	fprintf(stderr, "yacc: let_section ::= LPAREN let_list RPAREN\n");
    	$$ = $2;
    };

symbol:
     SYMBOL {
     	fprintf(stderr, "yacc: symbol ::= SYMBOL\n");
     	$$ = createSymbolNode($1);
     };

%%

