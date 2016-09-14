%{
    #include <algorithm>
    #include <cstdio>
    #include <string>

    #include "parser.hpp"
    #include "misc.h"
    extern YYSTYPE yylval;

	void yyerror(const char*);
    void yywarn(const char*);
	int64_t readValue(char* text, uint8_t base = 10);
%}

%option caseless
%option yylineno
%option noyywrap

word          [A-Z_][A-Z0-9_]*
hex           [0-9A-F][0-9A-F_]*

%%

;.*                                 ; /* comment */
[\[\],%]                            return *yytext;


-?[0-1][0-1_]*b {
    yylval.sint = readValue(yytext, 2);
    return BIN;
}

-?[0-7][0-7_]*o {
    yylval.sint = readValue(yytext, 8);
    return OCT;
}

-?[0-9][0-9_]*d? {
    yylval.sint = readValue(yytext);
    return DEC;
}

-?0x{hex}h? {
    yylval.sint = readValue(yytext, 16);
    return HEX;
}

-?(0x)?{hex}h {
    yylval.sint = readValue(yytext, 16);
    return HEX;
}

#{word} {
    yylval.cstring = strdup(yytext + 1); //don't include the octothrope
    return MACRO;

    //Code for multipart macros; not messing with them for now
    //because that would mess with the input stream for flex and
    //probably have to be done in advance of comming back through
    /*char* first = strtok(yytext, " \t");
    char* second = strtok(NULL, " \t");

    string identifier(first + 1);
    string value(second);
    if(macros.find(identifier) != macros.end())
        yywarn(("Redefinition of macro \'" + identifier + "\'").c_str());
    macros[identifier] = second;*/
}

{word}: {
    //the strtok will terminate the work cutting off the colon at the end
    yylval.cstring = strdup(strtok(yytext, ":"));
    return LABEL;
}

{word}/[^:] { //prevent interpreting a word as a label
    yylval.cstring = strdup(yytext);
    return WORD;
}


\n                                  ;
[ \t]+                              ;
.                                   yyerror((string("Unexpected character: \'" + *yytext) + '\'').c_str());

%%

/**
 * Read a value/number from a cstring with a defined
 * base and cleaning the thing of other characters.
 */
int64_t readValue(char* text, uint8_t base) {
    bool neg = (*text == '-');
    string val(text);
    val.erase(
        std::remove_if(
            val.begin(),
            val.end(),
            [](const char& c) { return !isdigit(c); }
        ),
        val.end()
    );

    long ret = stoull(val, nullptr, base);
    return neg ? -1 * ret : ret;
}
