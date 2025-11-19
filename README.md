# 🔎 Mecanismo de Busca (Índice Invertido) — Projeto C++ (STL only)

**Resumo:**
Aplicação de linha de comando em C++ (apenas STL) que indexa arquivos `.txt` e permite buscas por palavras-chave usando um **índice invertido**. Indexação → serialização (`./data/gerados/index.dat`). Busca → desserialização e resultados no terminal + `./data/gerados/saida_busca_atual.txt`.

---

## 📌 Conteúdo deste repositório

-   `main.cpp` — orquestra execução (construir / buscar).
-   `libs/terminal.hpp` — utilitários de I/O no terminal.
-   `libs/armazenar.hpp` — leitura de arquivos, listagem recursiva `.txt`, criação de `./data/gerados/`, carregamento de `stopwords.txt`.
-   `libs/TextProcessor.hpp` — limpeza, normalização e tokenização.
-   `libs/Index.hpp` — estrutura do índice invertido e mapeamento id ↔ arquivo.
-   `libs/Indexer.hpp` — logic para varrer arquivos e popular o índice.
-   `libs/Serializer.hpp` — serialização/desserialização binária manual.
-   `libs/QueryProcessor.hpp` — processamento de consultas (AND entre termos) e escrita de `saida_busca_atual.txt`.
-   `data/` — dados (ex.: `machado/`, `stopwords.txt`, `gerados/`).

    -   `./data/gerados/index.dat` — arquivo binário com o índice.
    -   `./data/gerados/saida_busca_atual.txt` — arquivo gerado a cada busca.

-   `README.md` — este arquivo.
-   `Makefile` (opcional) — conveniência para compilar.
-   pasta `lixo/`, `libs/`, `imgs/` etc. — recursos auxiliares.

---

## 🎯 Objetivo do projeto

Construir um mini-mecanismo de busca que:

1. **Indexação**: lê uma coleção de `.txt`, normaliza (lowercase / remove pontuação), remove stopwords (em `./data/stopwords.txt`), cria índice invertido (palavra → lista de IDs de documento) e grava em `./data/gerados/index.dat` (binário).
2. **Busca**: carregue `index.dat` e permita buscas por 1 ou múltiplos termos (AND), mostrando resultado no terminal e salvando em `./data/gerados/saida_busca_atual.txt`.

---

## 🛠️ Como compilar

> Requer: g++ com suporte a C++17 (Ubuntu 20.04+).

```bash
# comando simples
g++ -std=c++17 libs/*.hpp main.cpp -o main.exe

# ou, se preferir via Makefile (se houver):
make
```

Se houver erros, copie a primeira(s) linhas do erro e cole aqui — eu te ajudo a corrigir.

---

## 🚀 Como executar

### Construir índice (indexação)

```bash
./main.exe construir ./data/machado
```

-   Faz varredura **recursiva** em `./data/machado` e processa apenas arquivos `.txt`.
-   Gera `./data/gerados/index.dat`.
-   Mensagens de progresso aparecem no terminal (`[Indexer] Arquivos .txt encontrados: N`, etc).

### Buscar

```bash
./main.exe buscar comendo cachorro
```

-   Carrega `./data/gerados/index.dat` (se não existir, pede para rodar `construir`).
-   Faz busca AND entre termos (`comendo` **E** `cachorro`).
-   Resultados são impressos no terminal e gravados em `./data/gerados/saida_busca_atual.txt`.
-   Após a busca, o programa pergunta se deseja realizar outra busca (loop interativo).

---

## 🧭 Estrutura do índice (visão geral)

-   **indexMap** : `unordered_map<string, vector<int>>` — palavra → vetor ordenado de docIDs.
-   **idToDoc** : `vector<string>` — posição (id-1) → caminho completo do arquivo.
-   **docToId** : `unordered_map<string,int>` — caminho → id.
-   Observação: IDs são inteiros (1..N) — economizam espaço na serialização.

---

## 💾 Formato de serialização (alto nível)

Arquivo binário (`index.dat`) contém, em ordem:

1. comprimento do `rootDirectory` (int) + bytes do `rootDirectory`
2. número de documentos (int) e, para cada documento: comprimento do nome (int) + bytes do nome
3. número de termos (int) e, para cada termo: comprimento do termo (int) + bytes do termo + número de postings (int) + lista de docIDs (ints)

> A leitura correspondente reconstrói `idToDoc`, `docToId` e `indexMap`.

---

## 🧪 Testes sugeridos & verificação rápida

1. Confirme que `./data/gerados/` existe após executar `construir`.

    ```bash
    ls -la ./data/gerados
    ```

    Deve listar `index.dat` (e possivelmente `saida_busca_atual.txt`).

2. Se `./main.exe construir ./data/machado` não imprime nada:

    - Verifique que você rodou o binário correto (use `./main.exe` no diretório do projeto).
    - Rode `./main.exe` sem argumentos para ver a mensagem de uso.
    - Execute `strace ./main.exe construir ./data/machado` para diagnóstico avançado (Linux).

3. Se a serialização falhar: mensagens de erro começam com `[Serializer]` no terminal.

4. Teste com um diretório pequeno (ex.: criar 2 arquivos `.txt` com frases simples) para confirmar fluxo.

---

## 🧰 Dicas de depuração (se “não acontece nada”)

-   Confirme se você compilou os arquivos atualizados:

    ```bash
    date -r main.cpp && g++ -std=c++17 libs/*.hpp main.cpp -o main.exe && ./main.exe construir ./data/machado
    ```

-   Verifique permissões de escrita em `./data/gerados/`:

    ```bash
    touch ./data/gerados/teste && echo "ok" > ./data/gerados/teste
    ```

-   Verifique se `stopwords.txt` está no local correto (`./data/stopwords.txt`) e legível.
-   Rodando `./main.exe buscar ...` sem `index.dat` deveria imprimir erro sugerindo rodar `construir`.

---

## ✅ Boas práticas e observações

-   Todos os `.hpp` contêm definições **inline** das classes (conforme solicitado).
-   A aplicação usa apenas a **STL** (`unordered_map`, `vector`, `set_intersection`, `filesystem`, etc).
-   A interseção das listas é feita manualmente (STL algorithms), e os postings são mantidos ordenados.
-   Ao modificar os textos (adicionar/editar arquivos), re-execute `./main.exe construir <dir>` para atualizar o índice.
-   O índice armazena caminhos completos; se você mover arquivos, reindexe.

---

## 🔍 Possíveis melhorias futuras

-   Normalização avançada (remoção de acentuação / stemmer).
-   Busca com OR / frase exata / ranking por TF-IDF.
-   Compressão do arquivo `index.dat` (varint / delta-encoding).
-   UI TUI (ncurses) ou web front-end.
-   Paralelização da indexação (threads) para grande volume.

---

## 🧾 Exemplos de uso (workflow)

1. Indexar:

    ```bash
    ./main.exe construir ./data/machado
    # saída esperada:
    # [Indexer] Arquivos .txt encontrados: 30
    # [Indexer] Total processados: 30
    # [Indexer] Índice salvo em: ./data/gerados/index.dat
    ```

2. Buscar:

    ```bash
    ./main.exe buscar comendo cachorro
    # saída esperada:
    # Documentos encontrados:
    #  - ./data/machado/conto/exemplo1.txt
    #  - ./data/machado/romance/exemplo2.txt
    ```

---

## 📁 Arquivos úteis a incluir no repositório

-   `alunos.txt` — nomes dos autores / integrantes.
-   `LEIAME.txt` — instruções curtas para submissão (Moodle).
-   `Makefile` — target `all`, `clean`, `run`.
-   `.gitignore` — excluir `data/gerados/*` se o index não deve ser versionado:

    ```
    /data/gerados/*
    *.exe
    *.o
    ```

-   `tests/` — exemplos `.txt` e scripts de teste.

---

## 🧑‍🤝‍🧑 Colaboradores

-   **Você** — autor principal do projeto (adicione seu nome em `alunos.txt`).
-   Se o trabalho for em dupla, adicione ambos os nomes em `alunos.txt`.

---

## 📜 Licença

Distribua sob **MIT License** (ou outra de sua preferência). Exemplo rápido no repo (`LICENSE`).

---

## ❓ FAQ rápido

**Q:** O que fazer se `index.dat` não é criado?
**A:** Veja logs no terminal; verifique permissões e se `./data/gerados/` existe; recompile; coloque `std::cout` de debug extra se necessário.

**Q:** Como faço para indexar outra pasta?
**A:** `./main.exe construir ./outro/diretorio`.

**Q:** Posso indexar PDFs?
**A:** Não diretamente; extraia texto dos PDFs para `.txt` antes (p.ex. `pdftotext`).

---

## ✨ Toque final (bons comandos) (foco win11)

-   Compilar:

    ```bash
    g++ -std=c++17 ./libs/*.hpp main.cpp -o main.exe
    ```

-   Rodar index:

    ```bash
    ./main.exe construir ./data/machado
    ```

-   Rodar busca:

    ```bash
    ./main.exe buscar termo1 termo2
    ```

---

<br>
