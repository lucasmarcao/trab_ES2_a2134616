#ifndef ARMAZENAR_HPP
#define ARMAZENAR_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <filesystem>

class Armazenar
{
public:
    static void ensureDirectory(const std::string &dir)
    {
        try
        {
            std::filesystem::create_directories(dir);
        }
        catch (const std::exception &e)
        {
            std::cerr << "[Armazenar] Erro criando diretorio '" << dir << "': " << e.what() << std::endl;
        }
    }

    // Lista arquivos .txt recursivamente no diretório
    static std::vector<std::string> listTextFilesRecursive(const std::string &dirPath)
    {
        std::vector<std::string> files;
        try
        {
            if (!std::filesystem::exists(dirPath))
            {
                std::cerr << "[Armazenar] Diretorio nao existe: " << dirPath << std::endl;
                return files;
            }
            for (auto const &entry : std::filesystem::recursive_directory_iterator(dirPath))
            {
                if (entry.is_regular_file())
                {
                    auto p = entry.path();
                    if (p.has_extension())
                    {
                        std::string ext = p.extension().string();
                        for (auto &c : ext)
                            c = std::tolower((unsigned char)c);
                        if (ext == ".txt")
                        {
                            files.push_back(p.string());
                        }
                    }
                }
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "[Armazenar] Erro ao listar arquivos: " << e.what() << std::endl;
        }
        return files;
    }

    static std::unordered_set<std::string> loadStopwords(const std::string &filepath)
    {
        std::unordered_set<std::string> stop;
        std::ifstream in(filepath);
        if (!in)
        {
            std::cerr << "[Armazenar] Aviso: stopwords nao encontradas em: " << filepath << std::endl;
            return stop;
        }
        std::string w;
        while (in >> w)
        {
            // normalize: lowercase
            for (auto &c : w)
                c = std::tolower((unsigned char)c);
            stop.insert(w);
        }
        return stop;
    }

    static std::string loadText(const std::string &filepath)
    {
        std::ifstream in(filepath);
        if (!in)
        {
            std::cerr << "[Armazenar] Erro abrindo arquivo: " << filepath << std::endl;
            return "";
        }
        std::string line, all;
        while (std::getline(in, line))
        {
            all += line;
            all += ' ';
        }
        return all;
    }
};

#endif
