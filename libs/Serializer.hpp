// Serializer.hpp
#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Index.hpp"

class Serializer
{
public:
    // retorna true se ok
    static bool serialize(const std::string &filepath, const Index &index)
    {
        std::ofstream out(filepath, std::ios::binary);
        if (!out)
        {
            std::cerr << "[Serializer] Nao pode abrir para escrita: " << filepath << std::endl;
            return false;
        }
        // salva rootDirectory (tamanho + bytes)
        std::string root = index.getRootDirectory();
        int rootLen = (int)root.size();
        out.write(reinterpret_cast<const char *>(&rootLen), sizeof(rootLen));
        if (rootLen > 0)
            out.write(root.c_str(), rootLen);

        // salva lista de documentos
        const auto &idToDoc = index.getIdToDoc();
        int docCount = (int)idToDoc.size();
        out.write(reinterpret_cast<const char *>(&docCount), sizeof(docCount));
        for (const auto &name : idToDoc)
        {
            int l = (int)name.size();
            out.write(reinterpret_cast<const char *>(&l), sizeof(l));
            if (l > 0)
                out.write(name.c_str(), l);
        }

        // salva índice (numero de termos)
        const auto &idx = index.getIndexMap();
        int termCount = (int)idx.size();
        out.write(reinterpret_cast<const char *>(&termCount), sizeof(termCount));
        for (const auto &kv : idx)
        {
            const std::string &term = kv.first;
            const std::vector<int> &postings = kv.second;
            int tlen = (int)term.size();
            out.write(reinterpret_cast<const char *>(&tlen), sizeof(tlen));
            if (tlen > 0)
                out.write(term.c_str(), tlen);
            int psize = (int)postings.size();
            out.write(reinterpret_cast<const char *>(&psize), sizeof(psize));
            for (int id : postings)
            {
                out.write(reinterpret_cast<const char *>(&id), sizeof(id));
            }
        }
        out.close();
        return true;
    }

    static bool deserialize(const std::string &filepath, Index &index)
    {
        std::ifstream in(filepath, std::ios::binary);
        if (!in)
        {
            std::cerr << "[Serializer] Nao pode abrir para leitura: " << filepath << std::endl;
            return false;
        }
        index.clear();

        // read root dir
        int rootLen = 0;
        in.read(reinterpret_cast<char *>(&rootLen), sizeof(rootLen));
        if (!in)
        {
            std::cerr << "[Serializer] Erro lendo rootLen\n";
            return false;
        }
        if (rootLen > 0)
        {
            std::string root(rootLen, ' ');
            in.read(&root[0], rootLen);
            index.setRootDirectory(root);
        }

        // read docs
        int docCount = 0;
        in.read(reinterpret_cast<char *>(&docCount), sizeof(docCount));
        if (!in)
        {
            std::cerr << "[Serializer] Erro lendo docCount\n";
            return false;
        }
        for (int i = 1; i <= docCount; ++i)
        {
            int l = 0;
            in.read(reinterpret_cast<char *>(&l), sizeof(l));
            if (!in)
            {
                std::cerr << "[Serializer] Erro lendo doc name len\n";
                return false;
            }
            std::string name(l, ' ');
            if (l > 0)
                in.read(&name[0], l);
            index.addDocMapping(name, i);
        }

        int termCount = 0;
        in.read(reinterpret_cast<char *>(&termCount), sizeof(termCount));
        if (!in)
        {
            std::cerr << "[Serializer] Erro lendo termCount\n";
            return false;
        }
        for (int t = 0; t < termCount; ++t)
        {
            int tlen = 0;
            in.read(reinterpret_cast<char *>(&tlen), sizeof(tlen));
            if (!in)
            {
                std::cerr << "[Serializer] Erro lendo tlen\n";
                return false;
            }
            std::string term(tlen, ' ');
            if (tlen > 0)
                in.read(&term[0], tlen);
            int psize = 0;
            in.read(reinterpret_cast<char *>(&psize), sizeof(psize));
            if (!in)
            {
                std::cerr << "[Serializer] Erro lendo psize\n";
                return false;
            }
            for (int i = 0; i < psize; ++i)
            {
                int id = 0;
                in.read(reinterpret_cast<char *>(&id), sizeof(id));
                index.addWord(term, id);
            }
        }

        in.close();
        return true;
    }
};

#endif // SERIALIZER_HPP
