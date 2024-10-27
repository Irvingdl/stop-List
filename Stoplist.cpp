#include <iostream>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include <chrono>
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

// Cargar stop list desde archivo
std::unordered_set<std::string> loadStopList(const std::string& stopListFile) {
    std::unordered_set<std::string> stopList;
    std::ifstream file(stopListFile);
    if (file.is_open()) {
        std::string word;
        while (file >> word) {
            stopList.insert(word);
        }
        file.close();
    } else {
        std::cerr << "Error: No se pudo abrir el archivo de stop list." << std::endl;
    }
    return stopList;
}

// Refinar diccionario eliminando palabras de stop list, longitud 1 y baja frecuencia
void refineDictionary(std::unordered_map<std::string, int>& dictionary,
                      const std::unordered_set<std::string>& stopList,
                      int minFrequency) {
    for (auto it = dictionary.begin(); it != dictionary.end(); ) {
        const std::string& token = it->first;
        int frequency = it->second;
        
        if (stopList.count(token) > 0 || token.size() == 1 || frequency < minFrequency) {
            it = dictionary.erase(it); // Eliminar token si está en el stop list, es de longitud 1 o tiene baja frecuencia
        } else {
            ++it;
        }
    }
}

int main() {
    // Cargar el archivo de stop list
    std::string stopListFile = "stop_list.txt"; // Asegúrate de que el nombre coincide con el archivo cargado
    std::unordered_set<std::string> stopList = loadStopList(stopListFile);

    // Diccionario de tokens y frecuencia (para ejemplo)
    std::unordered_map<std::string, int> dictionary = {
        {"example", 10}, {"the", 50}, {"a", 5}, {"token", 2}, {"word", 1}
    };
    
    // Definir frecuencia mínima para considerar un token en el diccionario
    int minFrequency = 3;

    // Refinar diccionario con el stop list
    refineDictionary(dictionary, stopList, minFrequency);

    // Guardar diccionario refinado
    std::ofstream refinedDictionary("refined_dictionary.txt");
    for (const auto& entry : dictionary) {
        refinedDictionary << entry.first << " " << entry.second << "\n";
    }
    refinedDictionary.close();

    std::cout << "Diccionario refinado generado en refined_dictionary.txt" << std::endl;

    return 0;
}