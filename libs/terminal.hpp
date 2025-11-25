#ifndef TERMINAL_HPP
#define TERMINAL_HPP

#include <iostream>
#include <string>

class Terminal
{
public:
    static void print(const std::string &s)
    {
        std::cout << s << std::endl;
    }
    static void info(const std::string &s)
    {
        std::cout << "[info] " << s << std::endl;
    }
    static void warn(const std::string &s)
    {
        std::cerr << "[warn] " << s << std::endl;
    }
    static std::string prompt(const std::string &s)
    {
        std::cout << s;
        std::string line;
        std::getline(std::cin, line);
        return line;
    }
    static bool askYesNo(const std::string &s)
    {
        while (true)
        {
            std::cout << s << " (s/n): ";
            std::string r;
            std::getline(std::cin, r);
            if (r == "s" || r == "S")
                return true;
            if (r == "n" || r == "N")
                return false;
            std::cout << "Resposta inválida. Digite 's' ou 'n'.\n";
        }
    }
};

#endif
