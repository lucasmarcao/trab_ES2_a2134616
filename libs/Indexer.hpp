// Indexer.hpp
#ifndef INDEXER_HPP
#define INDEXER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>

#include "armazenar.hpp"
#include "TextProcessor.hpp"
#include "Index.hpp"
#include "Serializer.hpp"

class Indexer
{
public:
    static void build(const std::string &dirPath)
    {
        // carrega stopwords
        std::unordered_set<std::string> stop = Armazenar::loadStopwords("./data/stopwords.txt");

        // garante dir de saída
        Armazenar::ensureDirectory("./data/gerados");

        // busca arquivos .txt recursivamente
        std::vector<std::string> files = Armazenar::listTextFilesRecursive(dirPath);
        std::cout << "[Indexer] Arquivos .txt encontrados: " << files.size() << "\n";
        if (files.empty())
        {
            std::cerr << "[Indexer] Nenhum arquivo .txt encontrado em: " << dirPath << std::endl;
        }

        Index index;
        index.setRootDirectory(dirPath);

        int processed = 0;
        for (const auto &f : files)
        {
            std::string content = Armazenar::loadText(f);
            if (content.empty())
            {
                std::cerr << "[Indexer] Aviso: arquivo vazio ou nao pode ser lido: " << f << std::endl;
                continue;
            }
            int docId = index.addDocument(f);
            std::vector<std::string> tokens = TextProcessor::tokenize(content);
            for (const auto &tok : tokens)
            {
                if (tok.empty())
                    continue;
                if (stop.find(tok) != stop.end())
                    continue;
                index.addWord(tok, docId);
            }
            ++processed;
            if (processed % 50 == 0)
                std::cout << "[Indexer] Processados " << processed << " arquivos...\n";
        }

        std::cout << "[Indexer] Total processados: " << processed << "\n";
        // salva índice
        const std::string outPath = "./data/gerados/index.dat";
        if (!Serializer::serialize(outPath, index))
        {
            std::cerr << "[Indexer] Erro ao serializar índice.\n";
        }
        else
        {
            std::cout << "[Indexer] Índice salvo em: " << outPath << "\n";
        }
    }
};

#endif // INDEXER_HPP
