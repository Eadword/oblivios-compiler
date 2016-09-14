%{
    #include <algorithm>
    #include <cstdio>
    #include <string>

    #include "parser.hpp"
    extern YYSTYPE yylval;

	void yyerror(const char*);
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
    yylval.cstring = strdup(yytext);
    return MACRO;
}

{word}: {
    yylval.cstring = strdup(yytext);
    return LABEL;
}

{word} {
    yylval.cstring = strdup(yytext);
    return WORD;
}


\n                                  ;
[ \t]+                              ;
.                                   yyerror("Invalid character");

%%

/**
 * Read a value/number from a cstring with a defined
 * base and cleaning the thing of other characters.
 */
int64_t readValue(char* text, uint8_t base) {
    bool neg = (*text == '-');
    std::string val(text);
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
