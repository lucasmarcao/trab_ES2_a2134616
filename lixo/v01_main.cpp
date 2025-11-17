// main.cpp: controla execução, interpreta comandos e chama rotinas
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include "terminal.hpp"
#include "armazenar.hpp"
#include "TextProcessor.hpp"
#include "Index.hpp"
#include "Indexer.hpp"
#include "Serializer.hpp"
#include "QueryProcessor.hpp"

//  g++ -std=c++17 *.hpp main.cpp -o main.exe
// Para construir o índice a partir de arquivos em ./data/machado:
//  ./main.exe construir ./data/machado
// Para buscar, os termos "comendo" e "cachorro", execute:
//  ./main.exe buscar comendo cachorro
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
            std::cerr << "Caminho do diretório de documentos não informado.\n";
            return 1;
        }
        std::string dirPath = argv[2];
        // Constrói índice a partir dos arquivos no diretório
        Indexer::build(dirPath);
    }
    else if (command == "buscar")
    {
        // Carrega índice salvo em binário
        Index index;
        Serializer::deserialize("./data/gerados/index.dat", index);
        // Termos da query vindos dos argumentos (ex: "./main.exe buscar termo1 termo2")
        std::vector<std::string> terms;
        for (int i = 2; i < argc; i++)
        {
            terms.push_back(argv[i]);
        }
        // Executa busca interativa (faz a busca inicial e depois pergunta se continua)
        QueryProcessor::queryLoop(index, terms);
    }
    else
    {
        std::cerr << "Comando desconhecido: " << command << "\n";
        return 1;
    }
    return 0;
}
