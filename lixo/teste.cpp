#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include "lib1.hpp"

using namespace std;

//  g++ -std=c++17 *.hpp main.cpp -o main.exe
//  ./main.exe
int main(int argc, char const *argv[])
{
  cout << "\n\n";
  // comeco

  cout << " unordered_map " << endl;

  /*
    modelo de indice invertido do trabalho:...
    {
      "comendo": ["doc1.txt", "doc2.txt"],
      "gato": ["doc1.txt"],
      "está": ["doc1.txt", "doc2.txt"],
      "telhado": ["doc1.txt"],
      "cachorro": ["doc2.txt"],
      "quintal": ["doc2.txt"]
    }
  */

  unordered_map<string, int> idade;

  // variavel do trabalho.
  unordered_map<string, vector<string>> indice_invertido;

  // Esse tabalho vai precissar usar classes e ordenação.
  // salvar o índice invertido em um arquivo binário (index.dat)

  idade.insert({"Alice", 30});
  idade.insert({"Bob", 25});
  idade.insert({"Lucas", ronaldo});

  // listando o unordered map de um jeito mais entendivel possivel.
  for (const auto &[chave, valor] : idade)
  {
    std::cout << " " << chave << ": " << valor << "\n";
  }

  // fim
  cout << "\n\n";
  return 0;
}
