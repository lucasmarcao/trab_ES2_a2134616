// main.cpp
#include <iostream>
#include <string>
#include <vector>

#include "terminal.hpp"
#include "armazenar.hpp"
#include "TextProcessor.hpp"
#include "Index.hpp"
#include "Indexer.hpp"
#include "Serializer.hpp"
#include "QueryProcessor.hpp"

//  fazer pelo cmd, pelo bash nao funciona.
//  g++ -std=c++17 *.hpp main.cpp -o main.exe
// Para construir o índice a partir de arquivos em ./data/machado:
//  ./main.exe construir ./data/machado
//  ou -  main.exe construir data/machado
// Para buscar, os termos "comendo" e "cachorro", execute:
//  ./main.exe buscar comendo cachorro
//  ou -  main.exe buscar comendo cachorro
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Uso: " << argv[0] << " construir <diretorio> | buscar <termos...>\n";
        return 1;
    }

    std::string command = argv[1];

    if (command == "construir")
    {
        if (argc < 3)
        {
            std::cerr << "Erro: caminho do diretório não informado.\n";
            return 1;
        }
        std::string dirPath = argv[2];
        std::cout << "[main] Iniciando indexacao do diretorio: " << dirPath << "\n";
        Indexer::build(dirPath);
        std::cout << "[main] Indexacao concluida.\n";
    }
    else if (command == "buscar")
    {
        // Antes de buscar, tenta desserializar
        const std::string indexPath = "./data/gerados/index.dat";
        Index index;
        if (!Serializer::deserialize(indexPath, index))
        {
            std::cerr << "[main] Falha ao carregar índice. Rode 'construir' primeiro.\n";
            return 1;
        }
        // termos vindos de argv (após "buscar")
        std::vector<std::string> terms;
        for (int i = 2; i < argc; ++i)
            terms.push_back(argv[i]);

        QueryProcessor::queryLoop(index, terms);
    }
    else
    {
        std::cerr << "Comando desconhecido: " << command << "\n";
        std::cerr << "Uso: " << argv[0] << " construir <diretorio> | buscar <termos...>\n";
        return 1;
    }

    return 0;
}
