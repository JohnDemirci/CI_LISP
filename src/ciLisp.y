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
%token <dval> INT_LITERAL DOUBLE_LITERAL
%token LPAREN RPAREN EOL QUIT LET INT DOUBLE

%type <astNode> s_expr f_expr number symbol s_expr_list
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

s_expr_list:
	{
		printf(stderr, "yacc: s_expr_list ::=  EMPTY\n");
		$$ = NULL;
	}
	| s_expr s_expr_list {
		printf(stderr, "yacc: s_expr_list ::= S_EXPR S_EXPR_LIST\n");
		$$ = addSexprToList($1,$2);
	}
	| s_expr {
		printf(stderr, "yacc: s_expr_list ::= S_EXPRT \n");
		$$ = $1;
	};

number:
    INT_LITERAL {
     fprintf(stderr, "yacc: number ::= INT_LITERAL\n");
        $$ = createNumberNode($1, INT_TYPE);
    }
    | INT INT_LITERAL {
        fprintf(stderr, "yacc: number ::= INT INT_LITERAL\n");
        $$ = createNumberNode($2, INT_TYPE);
    }
    | DOUBLE INT_LITERAL {
        fprintf(stderr, "yacc: number ::= DOUBLE INT_LITERAL\n");
        $$ = createNumberNode($2, DOUBLE_TYPE);
    }
    | DOUBLE_LITERAL {
        fprintf(stderr, "yacc: number ::= DOUBLE_LITERAL\n");
        $$ = createNumberNode($1, DOUBLE_TYPE);
    }
    | INT DOUBLE_LITERAL {
        fprintf(stderr, "yacc: number ::= INT DOUBLE_LITERAL\n");
        $$ = createNumberNode($2, INT_TYPE);
    }
    | DOUBLE DOUBLE_LITERAL {
         fprintf(stderr, "yacc: number ::= DOUBLE DOUBLE_LITERAL\n");
         $$ = createNumberNode($2, DOUBLE_TYPE);
     };
	    
f_expr:
    LPAREN FUNC s_expr_list LPAREN {
        fprintf(stderr, "yacc: f_expr ::= LPAREN FUNC s_expr_list LPAREN \n");
           $$ =  createFunctionNode($2, $3);
    };


let_elem:
    LPAREN SYMBOL s_expr RPAREN {
    	fprintf(stderr, "yacc: let_elem ::= LPAREN SYMBOL s_expr RPAREN\n");
    	$$ = createSymbolTableNode($2, $3, NO_TYPE);
    }
    | LPAREN INT SYMBOL s_expr RPAREN {
        fprintf(stderr, "yacc: let_elem ::= LPAREN INT SYMBOL s_expr RPAREN\n");
        $$ = createSymbolTableNode($3, $4, INT_TYPE);
     }
    | LPAREN DOUBLE SYMBOL s_expr RPAREN {
        fprintf(stderr, "yacc: let_elem ::= LPAREN DOUBLE SYMBOL s_expr RPAREN\n");
        $$ = createSymbolTableNode($3, $4, DOUBLE_TYPE);
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

