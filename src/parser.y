%{
    #include <stdio.h>
    #include <string>

//    #define YYDEBUG 1

    int yylex(void);
    extern int yylineno;
    void yyerror(const char*);
    void yywarn(const char*);
    int yywrap();
    int main();

%}

%union {
    int64_t          sint;
    char*         cstring;
};

%token <sint> BIN OCT DEC HEX
%token <cstring> LABEL MACRO WORD

%type <sint> number
%type <cstring> lines line labels arguments argument mode arg_val

%start lines

%%

lines           : lines line                                {;}
                | line                                      {;}
                | /* empty */								{;} //the file has no code
                ;
line            : labels WORD arguments                     { free($2); }
                | WORD arguments                            { free($1); }
                | MACRO WORD                                { free($1); free($2); }
                | MACRO number                              { free($1); }
                ;
labels          : labels LABEL                              { free($2); }
                | LABEL                                     { free($1); }
                ;
arguments       : argument ',' argument                     {;}
                | argument                                  {;}
                ;
argument        : mode '[' arg_val ']'                      {;}
                | mode arg_val                              {;}
                ;
mode            : /* empty */                               {;}
                | '%'                                       {;}
                ;
arg_val         : number                                    {;}
                | WORD                                      { free($1); }
                ;
number          : BIN                                       { $$ = $1; }
                | OCT                                       { $$ = $1; }
                | DEC                                       { $$ = $1; }
                | HEX                                       { $$ = $1; }
                ;

%%

void yywarn(const char* str) {
    fprintf(stdout, "Warning: %s, on line: %d\n", str, yylineno);
}

void yyerror(const char* str) {
    fprintf(stderr, "Error: %s, on line: %d\n", str, yylineno);
}
