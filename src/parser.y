%{
    #include "misc.h"
    #include "parser.h"
    #include "line.h"

//    #define YYDEBUG 1

    extern int yylineno;
    std::map<string, ArgVal*> macros;
    std::vector<Line*> lines;

    int yylex(void);
    void yyerror(const char*);
    void yywarn(const char*);

    /**
     * Adds a macro to the gloabl hashmap.
     *
     * @param i the identifier
     * @param val the substitution the macro represents
     */
    void addMacro(const char* i, ArgVal* val);

    /**
     * Checks for a string and applys the macro if needed.
     * I do not believe there are any cases where a number must be forced, so
     * for right now at least it only has forceword, which would be for an opcode
     * for instance, which will never be a number, however arguments can be either.
     *
     * @note It will ususally not alter the input
     * @note Deallocates the identifier string if needed
     *
     * @param i identifier string, the word that is being checked for replacement
     * @param forceword if true it will throw an error if it replaces to a number
     * @return A cstring after applying a macro if relevent
     */
    ArgVal* applyMacro(string* i, bool forceword = false);

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
    int64_t                 sint;
    char*                   cstring;
    AccessMode              mode;
    ArgVal*                 argval;
    Argument*               argument;
    Labels*                 labels;
    Line*                   line;
};

%token <sint> BIN OCT DEC HEX
%token <cstring> LABEL MACRO WORD

%type <sint> number
%type <mode> mode
%type <argval> arg_val
%type <argument> argument
%type <labels> labels;
%type <line> line arguments
%type <cstring> lines

%start lines

%%

lines           : lines line                            {
                                                            if($2 != nullptr) {
                                                                lines.push_back($2);
                                                            }
                                                        }
                | line                                  {
                                                            if($1 != nullptr) {
                                                                lines.push_back($1);
                                                            }
                                                        }
                | /* empty */							{ /* the file has no code */; }
                ;
line            : labels WORD arguments                 {
                                                            $$ = $3;
                                                            ArgVal* t = applyMacro(new std::string($2), true);
                                                            free($2);
                                                            $$->setOPCode(t);
                                                            delete t;
                                                            $$->labels = $1;
                                                        }
                | WORD arguments                        {
                                                            $$ = $2;
                                                            ArgVal* t = applyMacro(new std::string($1), true);
                                                            free($1);
                                                            $$->setOPCode(t);
                                                            delete t;
                                                        }
                | MACRO WORD                            {
                                                            $$ = nullptr;
                                                            // allow macros to replace each other
                                                            addMacro($1, applyMacro(new std::string($2)));
                                                            free($1); free($2);
                                                        }
                | MACRO number                          { $$ = nullptr; addMacro($1, new ArgVal($2)); free($1); }
                ;
labels          : labels LABEL                          { $$ = addLabel($2, $1); free($2); }
                | LABEL                                 { $$ = addLabel($1);     free($1); }
                ;
arguments       : argument ',' argument                 { $$ = new Line(yylineno, nullptr, $1, $3); }
                | argument                              { $$ = new Line(yylineno, nullptr, $1, nullptr); }
                ;
argument        : mode '[' arg_val ']'                  { $$ = new Argument($1, $3, true);  }
                | mode arg_val                          { $$ = new Argument($1, $2, false); }
                ;
mode            : /* empty */                           { $$ = AccessMode::DIRECT;   }
                | '%'                                   { $$ = AccessMode::RELATIVE; }
                ;
arg_val         : number                                { $$ = new ArgVal($1); }
                | WORD                                  { $$ = applyMacro(new std::string($1)); free($1); }
                ;
number          : BIN                                   { $$ = $1; }
                | OCT                                   { $$ = $1; }
                | DEC                                   { $$ = $1; }
                | HEX                                   { $$ = $1; }
                ;

%%

void yywarn(const char* str) {
    fprintf(stdout, "Warning: %s, on line: %d\n", str, yylineno);
}

void yyerror(const char* str) {
    fprintf(stderr, "Error: %s, on line: %d\n", str, yylineno);
}

void addMacro(const char* i, ArgVal* val) {
    string ident(i);
    if(macros.find(ident) != macros.end())
        yywarn(("Redefinition of macro \"" + ident + "\"").c_str());
    macros[ident] = val;
}

ArgVal* applyMacro(string* i, bool forceword) {
    ArgVal* t = nullptr;
    try {
        t = macros.at(*i);
    } catch(std::out_of_range& e) {
        return new ArgVal(i);
    }

    delete i; //Already know it is not the origional
    if(forceword && t->isNum())
        yyerror("Macro replacement to number of string only type");
    return new ArgVal(*t);
}

Labels* addLabel(const char* i, Labels* labels) {
    if(labels == nullptr) labels = new Labels();
    labels->insert(string(i));
    return labels;
}
