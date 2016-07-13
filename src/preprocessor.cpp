#include <regex>
#include <iostream>

#include <boost/algorithm/string.hpp>

#include "numbers.h"
#include "patterns.h"
#include "preprocessor.h"
#include "compiler_exception.h"

void Preprocessor::run(line_vec& lines) {
    removeComments(lines);
    removeWhiteSpace(lines);
    for(Line& i : lines) boost::to_upper(i.cur);

    //This error should never have to occur, but just in case someone tries to test the system...
    if(lines.size() > 0xffff) throw compiler_exception("Code too long", lines, 0xffff);

    str_map macros = getMacros(lines);
    replaceMacros(lines, macros);
    label_map labels = getLabels(lines);
    replaceLabels(lines, labels);
    formatNumbers(lines);
}

void Preprocessor::removeComments(line_vec &lines) {
    applyReplace(lines, Patterns::comment);
    printLines(lines);
}

void Preprocessor::removeWhiteSpace(line_vec& lines) {
    applyReplace(lines, Patterns::white_space, " ");

    //This is regrettably inefficient, but lookbehinds not supported
    applyReplace(lines, std::regex(", +"), ",");
    applyReplace(lines, std::regex(": +"), ":");

    applyReplace(lines, Patterns::eol_white_space);
    applyReplace(lines, Patterns::bol_white_space);
    for(auto i = lines.begin(); i != lines.end();) {
        if(i->cur.empty()) lines.erase(i);
        else ++i;
    }
    printLines(lines);
}

str_map Preprocessor::getMacros(line_vec& lines) {
    str_map macros;
    for(uint16_t cur_line = 0; cur_line < lines.size(); ++cur_line) {
        const Line& line = lines[cur_line];
        std::smatch match;

        //Attempt to match entire line to vague description, i.e. a line starting with a '#'
        if(!std::regex_match(line.cur, match, Patterns::macro_line)) continue;

        //We found line which should represent a macro; attempt to read...
        string tmp = match[1];
        if(!std::regex_match(tmp, match, Patterns::macro))
            throw compiler_exception("Invalid Macro", lines, cur_line);

        //It is valid formatting, now verify the name is unique
        if(macros.find(match[1]) != macros.end())
            throw identifier_exception("Non-unique Identifier", match[1], lines, cur_line);

        //All is good, add it to macro list
        macros[match[1]] = match[2];
        lines.erase(lines.begin() + cur_line--);
    }

    debugStrMap(macros);
    printLines(lines);
    return macros;
}

void Preprocessor::replaceMacros(line_vec& lines, const str_map& macros) {
    for(auto&& i : macros)
        applyReplace(lines, Patterns::findSting(i.first), i.second);
    printLines(lines);
}

label_map Preprocessor::getLabels(line_vec& lines) {
    label_map labels;
    for(uint16_t cur_line = 0; cur_line < lines.size(); ++cur_line) {
        Line& line = lines[cur_line];

        //Find a line containing a ':'
        bool found = false;
        for(std::sregex_iterator i = std::sregex_iterator(line.cur.begin(), line.cur.end(), Patterns::label);
                i != Patterns::iterator_end; ++i)
        {
            found = true;
            string label = (*i)[1];
            if(labels.find(label) != labels.end())
                throw identifier_exception("Non-unique Identifier", label, lines, cur_line);
            if(cur_line >= lines.size() - 1)
                throw compiler_exception("Label Refers to Nothing", lines, cur_line);
            labels[label] = cur_line;
        }

        if(!found) continue;

        string::iterator end_of_label = line.cur.begin();
        for(uint16_t x = line.cur.size() - 1; x >= 0; --x) {
            if(line.cur[x] == ':') {
                end_of_label += x;
                break;
            }
        }
        line.cur.erase(line.cur.begin(), end_of_label + 1);
        if(line.cur.empty())
            lines.erase(lines.begin() + cur_line);
    }

    debugLabelMap(labels);
    printLines(lines);
    return labels;
}

void Preprocessor::replaceLabels(line_vec& lines, const label_map& labels) {
    for(auto&& label : labels) {
        auto pattern = Patterns::findSting(label.first);

        for(uint16_t cur_line = 0; cur_line < lines.size(); ++cur_line) {
            Line& line = lines[cur_line];
            std::string replace = std::to_string((int)label.second - (int)cur_line);
            line.cur = std::regex_replace(line.cur, pattern, replace);
        }
    }
    printLines(lines);
}


void Preprocessor::formatNumbers(line_vec& lines) {
    enum { BIN, OCT, DEC, HEX, NONE } num_type;

    for(uint16_t cur_line = 0; cur_line < lines.size(); ++cur_line) {
        Line& line = lines[cur_line];

        //Find numbers
        for(std::sregex_iterator i = std::sregex_iterator(line.cur.begin(), line.cur.end(), Patterns::loose_number);
                i != Patterns::iterator_end; ++i)
        {
            auto& match = *i;

            //It is a decimal because it does not have 0X or BODH
            if(match[2].str().empty() && match[4].str().empty()) {
                //check if only 0-9
                std::smatch check;
                std::regex decimal("[0-9_]+");
                std::string num = match[3].str();
                if(!std::regex_match(num, check, decimal)) continue;
                //don't throw since it does not have any markings as a number, just continue

                //it is a (mostly) valid decimal, could just be underscores
                num_type = DEC;
            }

            //It is invalid (hex 0x but an O for ending)
            else if(!match[2].str().empty() && match[4].str() != "O")
                throw compiler_exception("Cannot interpret number: " + match.str(), lines, cur_line);

            //It is hex
            else if(!match[2].str().empty() || match[4].str() == "H")
                num_type = HEX;

            //It is Binary
            else if(match[4].str() == "B") {
                std::smatch check;
                std::regex non_dec("[A-F_]");
                std::regex bin("[01_]+");
                std::string num = match[3].str();
                if(std::regex_match(num, check, non_dec)) continue; //catches words
                if(!std::regex_match(num, check, bin))
                    throw compiler_exception("Invalid binary: " + match.str(), lines, cur_line);

                num_type = BIN;
            }

            //It is Octal
            else if(match[4].str() == "O") {
                std::smatch check;
                std::regex non_dec("[A-F_]");
                std::regex oct("[0-7_]+");
                std::string num = match[3].str();
                if(std::regex_match(num, check, non_dec)) continue; //catches words
                if(!std::regex_match(num, check, oct))
                    throw compiler_exception("Invalid octal: " + match.str(), lines, cur_line);

                num_type = OCT;
            }

            //It is Decimal
            else if(match[4].str() == "D") {
                std::smatch check;
                std::regex non_dec("[A-F_]+");
                std::regex dec("[0-9_]");
                std::string num = match[3].str();
                if(std::regex_match(num, check, non_dec)) continue; //catches words
                if(!std::regex_match(num, check, dec))
                    throw compiler_exception("Invalid Decimal: " + match.str(), lines, cur_line);


                num_type = DEC;
            }

            else { //This should never happen
                throw compiler_exception("Odd Number Error with Value: " + match.str() +
                                         ". Please file a bug report for this error.", lines, cur_line);
            }


            //Now we know what type it is, and have already moved on if it is not valid
            // While it is true that it may read in a value just to reformat it as it was
            // it is also cleaning it to put it in the same way, as HEX can be represented
            // a few ways

            //NOTE: Can't just convert to two's compliment as we don't know how large the
            // final data size is, so preserve the +/- if it is there
            uint64_t value = 0;
            switch(num_type) {
                case BIN: value = Numbers::readBin(match[3].str()); break;
                case OCT: value = Numbers::readOct(match[3].str()); break;
                case DEC: value = Numbers::readDec(match[3].str()); break;
                case HEX:
                    value = Numbers::readHex(
                            (match[4].str() != "H") ?
                            (match[3].str() + match[4].str()) :
                            (match[3].str())
                    );
                    break;
            }

            std::cout << "Number: " << value << " \"" <<   match[0] << "\"" << std::endl;
        }
    }
    printLines(lines);
}