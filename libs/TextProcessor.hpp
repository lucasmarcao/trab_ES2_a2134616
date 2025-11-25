#ifndef TEXTPROCESSOR_HPP
#define TEXTPROCESSOR_HPP

#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include <algorithm>

class TextProcessor
{
public:
    // Remove pontuacao (ASCII) e converte para minusculas
    static std::string cleanString(const std::string &s)
    {
        std::string out;
        out.reserve(s.size());
        for (unsigned char c : s)
        {
            if (std::ispunct(c))
            {
                out += ' ';
            }
            else
            {
                out += std::tolower(c);
            }
        }
        return out;
    }

    // Tokeniza string limpa em palavras por whitespace
    static std::vector<std::string> tokenize(const std::string &s)
    {
        std::vector<std::string> tokens;
        std::istringstream iss(cleanString(s));
        std::string w;
        while (iss >> w)
            tokens.push_back(w);
        return tokens;
    }

    // Limpeza rápida para termos de consulta individuais (remove pontuacao e lowercase)
    static std::string cleanTerm(const std::string &t)
    {
        std::string res;
        for (unsigned char c : t)
        {
            if (!std::ispunct(c))
                res += std::tolower(c);
        }
        return res;
    }
};

#endif
