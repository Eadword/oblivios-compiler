namespace Patterns { // Eventually this could read from a config file
    const std::regex comment(";.*");

    /// Recognizes entire line macro
    const std::regex macro_line("#(.*)\\n");
    /// Matches sub parts of the macro
    const std::regex macro("([A-Z_a-z]\\w*)[ \\t\\v]+(.*)");

    /// The ending iterator
    const std::sregex_iterator iterator_end = std::sregex_iterator();
}