%{
    #include "misc.h"
    #include "parser.h"
//    #define YYDEBUG 1

    extern int yylineno;
    std::map<string, ArgVal> macros;

    int yylex(void);
    void yyerror(const char*);
    void yywarn(const char*);

    /**
     * Adds a macro to the gloabl hashmap.
     *
     * @param i the identifier
     * @param val the substitution the macro represents
     */
    void addMacro(const char* i, ArgVal val);

    /**
     * Adds the label to an exisitng set of labels, or creates a new set.
     *
     * @param i the identifier of the label
     * @param labels an existing set of labels or nullptr
     * @note copies the cstring into a std::string
     * @returns the set of labels
     */
    Labels* addLabel(const char* i, Labels* labels = nullptr);

%}

%code requires { #include "parser.h" } /* shoves include into parser.hpp */
%union {
    int64_t          sint;
    char*         cstring;
    AccessMode       mode;
    ArgVal*        argval;
    Argument*    argument;
    Labels*        labels;
    Line*            line;
};

%token <sint> BIN OCT DEC HEX
%token <cstring> LABEL MACRO WORD

%type <sint> number
%type <cstring> lines
%type <mode> mode
%type <argval> arg_val
%type <argument> argument
%type <labels> labels;
%type <line> line arguments

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
labels          : labels LABEL                              { $$ = addLabel($2, $1); free($2); }
                | LABEL                                     { $$ = addLabel($1);     free($1); }
                ;
arguments       : argument ',' argument                     { delete $1; delete $3; }
                | argument                                  { delete $1;}
                ;
argument        : mode '[' arg_val ']'                      { $$ = new Argument($1, $3, true);  }
                | mode arg_val                              { $$ = new Argument($1, $2, false); }
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

Labels* addLabel(const char* i, Labels* labels) {
    if(labels == nullptr) labels = new Labels();
    labels->insert(string(i));
    return labels;
}
