%{
    #include "misc.h"
    #include "parser.h"
//    #define YYDEBUG 1

    extern int yylineno;
    std::map<string, ArgVal> macros;

    int yylex(void);
    void yyerror(const char*);
    void yywarn(const char*);

    void addMacro(const char* i, ArgVal val);

%}

%code requires { #include "parser.h" } /* shoves include into parser.hpp */
%union {
    int64_t          sint;
    char*         cstring;
    AccessMode       mode;
    ArgVal*        argval;
    Line*            line;
};

%token <sint> BIN OCT DEC HEX
%token <cstring> LABEL MACRO WORD

%type <sint> number
%type <cstring> lines
%type <mode> mode
%type <argval> arg_val
%type <line> line labels arguments argument

%start lines

%%

lines           : lines line                                {;}
                | line                                      {;}
                | /* empty */								{;} //the file has no code
                ;
line            : labels WORD arguments                     { free($2); }
                | WORD arguments                            { free($1); }
                | MACRO WORD                                { addMacro($1, ArgVal($2)); free($1); }
                | MACRO number                              { addMacro($1, ArgVal($2)); free($1); }
                ;
labels          : labels LABEL                              { free($2); }
                | LABEL                                     { free($1); }
                ;
arguments       : argument ',' argument                     {;}
                | argument                                  {;}
                ;
argument        : mode '[' arg_val ']'                      { delete $3; }
                | mode arg_val                              { delete $2; }
                ;
mode            : /* empty */                               { $$ = AccessMode::DIRECT;   }
                | '%'                                       { $$ = AccessMode::RELATIVE; }
                ;
arg_val         : number                                    { $$ = new ArgVal($1); }
                | WORD                                      { $$ = new ArgVal($1); }
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

void addMacro(const char* i, ArgVal val) {
    string ident(i);
    if(macros.find(ident) != macros.end())
        yywarn(("Redefinition of macro \"" + ident + "\"").c_str());
    macros[ident] = val;
}
