# Makefile para projeto: Índice Invertido (C++17, STL only)
# -------------------------------------------------------
# Uso rápido:
#   make          -> compila (default)
#   make run ARGS="construir ./data/machado"   -> executa binário com argumentos
#   make index     -> compila e executa ./main.exe construir ./data/machado
#   make search SEARCH="comendo cachorro" -> compila e executa ./main.exe buscar <termos>
#   make clean     -> remove binário e arquivos gerados (index e saida)
#   make rebuild   -> clean + all
#   make help      -> imprime ajuda
#
# Ajuste variáveis abaixo se necessário.

CXX      := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra
TARGET   := main.exe
SRCS     := main.cpp
HEADERS  := $(wildcard *.hpp)

# Diretórios / arquivos usados pelo programa
DATA_DIR    := ./data/gerados
INDEX_DIR   := ./data/machado
INDEX_FILE  := $(DATA_DIR)/index.dat
OUT_SEARCH  := $(DATA_DIR)/saida_busca_atual.txt

.PHONY: all help run index search clean rebuild dirs

all: $(TARGET)

# compiling using headers + main.cpp (headers are inline)
$(TARGET): $(SRCS) $(HEADERS)
	@echo "[make] Compilando $(TARGET) ..."
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)
	@echo "[make] OK: $(TARGET) criado."

# executa o binário com argumentos arbitrários (use ARGS variable)
# exemplo: make run ARGS="construir ./data/machado"
run: $(TARGET)
	@if [ -z "$(ARGS)" ]; then \
		echo "[make run] Aviso: variavel ARGS vazia. Ex: make run ARGS=\"construir ./data/machado\""; \
		exit 1; \
	fi
	./$(TARGET) $(ARGS)

# cria diretório de saída (garante que ./data/gerados exista)
dirs:
	@mkdir -p $(DATA_DIR)
	@echo "[make] Garantido: $(DATA_DIR) existe."

# alvo rápido para indexar o diretório padrão (INDEX_DIR)
index: $(TARGET) dirs
	@echo "[make] Indexando: $(INDEX_DIR) -> $(INDEX_FILE)"
	./$(TARGET) construir $(INDEX_DIR)

# alvo para busca interativa / com termos passados
# uso: make search SEARCH="comendo cachorro"
search: $(TARGET)
	@if [ -z "$(SEARCH)" ]; then \
		echo "Uso: make search SEARCH=\"termo1 termo2\""; exit 1; \
	fi
	@echo "[make] Buscando: $(SEARCH)"
	./$(TARGET) buscar $(SEARCH)

# limpa binário e arquivos gerados (index + saida)
clean:
	@echo "[make] Limpando artefatos..."
	@rm -f $(TARGET)
	@rm -f $(INDEX_FILE)
	@rm -f $(OUT_SEARCH)
	@echo "[make] Done."

rebuild: clean all

# ajuda / documentação rápida
help:
	@echo "Makefile targets:"
	@echo "  make         -> compila o projeto (default)"
	@echo "  make run ARGS=\"...\" -> executa o binario com ARGS"
	@echo "       Ex: make run ARGS=\"construir ./data/machado\""
	@echo "  make index   -> compila e roda 'construir ./data/machado'"
	@echo "  make search SEARCH=\"termo1 termo2\" -> compila e roda busca"
	@echo "  make clean   -> remove binario e arquivos gerados"
	@echo "  make rebuild -> clean + all"
	@echo "Notas:"
	@echo "  - Ajuste INDEX_DIR e DATA_DIR no topo do Makefile se necessário."
	@echo "  - Arquivos gerados: $(INDEX_FILE) e $(OUT_SEARCH)"

