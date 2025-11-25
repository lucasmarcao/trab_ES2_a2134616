#ifndef INDEX_HPP
#define INDEX_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

class Index
{
private:
    std::unordered_map<std::string, std::vector<int>> indexMap;
    std::vector<std::string> idToDoc;             // id 1..N --> nome
    std::unordered_map<std::string, int> docToId; // nome --> id
    std::string rootDirectory;                    // diretório raiz indexado

public:
    void setRootDirectory(const std::string &root) { rootDirectory = root; }
    std::string getRootDirectory() const { return rootDirectory; }

    int addDocument(const std::string &docName)
    {
        auto it = docToId.find(docName);
        if (it != docToId.end())
            return it->second;
        int id = (int)idToDoc.size() + 1;
        idToDoc.push_back(docName);
        docToId[docName] = id;
        return id;
    }

    void addWord(const std::string &word, int docId)
    {
        auto &vec = indexMap[word];
        if (vec.empty() || vec.back() != docId)
            vec.push_back(docId);
    }

    const std::vector<int> &getPostings(const std::string &word) const
    {
        static const std::vector<int> empty;
        auto it = indexMap.find(word);
        if (it == indexMap.end())
            return empty;
        return it->second;
    }

    std::string getDocName(int id) const
    {
        if (id >= 1 && id <= (int)idToDoc.size())
            return idToDoc[id - 1];
        return std::string();
    }

    const std::unordered_map<std::string, std::vector<int>> &getIndexMap() const { return indexMap; }
    const std::vector<std::string> &getIdToDoc() const { return idToDoc; }

    void clear()
    {
        indexMap.clear();
        idToDoc.clear();
        docToId.clear();
        rootDirectory.clear();
    }

    // usado durante desserializacao para reconstruir mapeamentos
    void addDocMapping(const std::string &name, int id)
    {
        if ((int)idToDoc.size() < id)
            idToDoc.resize(id);
        idToDoc[id - 1] = name;
        docToId[name] = id;
    }
};

#endif
