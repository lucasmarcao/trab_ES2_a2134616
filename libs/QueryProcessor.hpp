// QueryProcessor.hpp
#ifndef QUERYPROCESSOR_HPP
#define QUERYPROCESSOR_HPP

#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>

#include "terminal.hpp"
#include "Index.hpp"
#include "TextProcessor.hpp"

class QueryProcessor
{
public:
    static void queryLoop(Index &index, std::vector<std::string> initialTerms)
    {
        bool first = true;
        std::vector<std::string> terms = initialTerms;

        while (true)
        {
            if (!first)
            {
                std::string line = Terminal::prompt("Digite termos para busca (ou ENTER para sair): ");
                if (line.empty())
                    break;
                // split line
                terms.clear();
                std::istringstream iss(line);
                std::string t;
                while (iss >> t)
                    terms.push_back(t);
            }
            first = false;
            if (terms.empty())
                break;

            // normalize query terms
            std::vector<std::string> qterms;
            for (auto &t : terms)
            {
                std::string clean = TextProcessor::cleanTerm(t);
                if (!clean.empty())
                    qterms.push_back(clean);
            }
            if (qterms.empty())
            {
                Terminal::print("Termos invalidos apos limpeza. Tente novamente.");
                if (!Terminal::askYesNo("Deseja tentar outra busca?"))
                    break;
                continue;
            }

            std::vector<int> result;
            bool firstTerm = true;
            for (const auto &term : qterms)
            {
                const std::vector<int> &p = index.getPostings(term);
                if (firstTerm)
                {
                    result = p;
                    firstTerm = false;
                }
                else
                {
                    std::vector<int> tmp;
                    std::set_intersection(result.begin(), result.end(), p.begin(), p.end(), std::back_inserter(tmp));
                    result.swap(tmp);
                }
                if (result.empty())
                    break;
            }

            // salva em ./data/gerados/saida_busca_atual.txt (sobrescreve)
            const std::string outPath = "./data/gerados/saida_busca_atual.txt";
            std::ofstream out(outPath);
            if (!out)
            {
                std::cerr << "[QueryProcessor] Nao foi possivel abrir saida_busca_atual.txt para escrita.\n";
            }

            if (result.empty())
            {
                Terminal::print("Nenhum documento encontrado para a consulta.");
                if (out)
                    out << "Nenhum documento encontrado.\n";
            }
            else
            {
                Terminal::print("Documentos encontrados:");
                for (int id : result)
                {
                    std::string name = index.getDocName(id);
                    Terminal::print(" - " + name);
                    if (out)
                        out << name << "\n";
                }
            }
            if (out)
                out.close();

            if (!Terminal::askYesNo("Deseja fazer outra busca?"))
                break;
        }
    }
};

#endif // QUERYPROCESSOR_HPP
