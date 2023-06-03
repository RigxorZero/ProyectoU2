#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <limits>
#include <cctype>
#include <unordered_set>

// Función para validar una entrada numérica entera
int validarEntradaEntera() {
    int valor;
    std::string entrada;

    while (true) {
        std::cin >> entrada;

        // Verificar si la entrada contiene solo dígitos
        bool esNumero = true;
        for (char c : entrada) {
            if (!std::isdigit(c)) {
                esNumero = false;
                break;
            }
        }

        if (!esNumero) {
            std::cout << "Entrada inválida. Intente nuevamente: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            std::stringstream ss(entrada);
            if (!(ss >> valor)) {
                std::cout << "Entrada inválida. Intente nuevamente: ";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            } else {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
        }
    }

    return valor;
}


// Estructura base de guardianes
struct Guardian 
{
    std::string name;
    int powerLevel;
    std::string mainMaster;
    std::string village;
};
// Estructura base de aldeas
struct Village 
{
    std::string name;
    std::vector<std::string> connectedVillages;
    std::string master; // Maestro actual de la aldea
};

// Función para buscar el índice de un guardián en el vector 'guardians'
int guardianIndex(const std::vector<Guardian>& guardians, const std::string& guardianName) {
    for (int i = 0; i < guardians.size(); i++) {
        if (guardians[i].name == guardianName) {
            return i;
        }
    }
    return -1; // Si no se encuentra el guardián, se devuelve -1
}



int main() 
{
    std::vector<Guardian> guardians;
    std::unordered_map<std::string, Village> villages;


#pragma  region Archivos //Carga de archivos de guardianes y aldeas

    std::ifstream inputFileVillages("./Archivos/villages.txt");
    if (!inputFileVillages) {
        std::cout << "No se pudo abrir el archivo." << std::endl;
        return 1;
    }

    std::string line;
    std::getline(inputFileVillages, line); // Leer y descartar la primera línea de encabezado
    while (std::getline(inputFileVillages, line)) {
        std::istringstream iss(line);
        std::string villageName, connectedVillage;
        std::getline(iss, villageName, ',');
        std::getline(iss, connectedVillage);

        villages[villageName].name = villageName;
        villages[villageName].connectedVillages.push_back(connectedVillage);
    }

    inputFileVillages.close();

    std::ifstream inputFile("./Archivos/guardians.txt");
    if (!inputFile) 
    {
        std::cout << "No se pudo abrir el archivo." << std::endl;
        return 1;
    }

    std::string lineG;
    while (std::getline(inputFile, lineG)) 
    {
        Guardian guardian;

        std::istringstream iss(lineG);
        std::getline(iss, guardian.name, ',');
        iss >> guardian.powerLevel;
        iss.ignore();
        std::getline(iss, guardian.mainMaster, ',');
        std::getline(iss, guardian.village);

        guardians.push_back(guardian);

        // Actualizar el maestro de la aldea si el guardian tiene un powerLevel mayor
        if (villages.find(guardian.village) != villages.end()) {
            Village& village = villages[guardian.village];
            if (village.master.empty() || guardian.powerLevel > guardians[guardianIndex(guardians, village.master)].powerLevel) {
                village.master = guardian.name;
            }
        }
    }

    inputFile.close();

#pragma endregion Archivos    

    // Imprimir los guardianes
    for (const auto& guardian : guardians) 
    {
        std::cout << "Name: " << guardian.name << std::endl;
        std::cout << "Power Level: " << guardian.powerLevel << std::endl;
        std::cout << "Main Master: " << guardian.mainMaster << std::endl;
        std::cout << "Village: " << guardian.village << std::endl;
        std::cout << std::endl;
    }

    // Imprimir las aldeas y sus conexiones
    for (const auto& pair : villages) {
        const Village& village = pair.second;
        std::cout << "Village: " << village.name << std::endl;
        std::cout << "Connected Villages: ";
        for (const std::string& connectedVillage : village.connectedVillages) {
            std::cout << connectedVillage << " ";
        }
        std::cout << std::endl;
        std::cout << "Master: " << village.master << std::endl << std::endl;
    }

    int choice = 0;
    while (choice != 4) {
        std::cout << "----- Guardian's Journey -----" << std::endl;
        std::cout << "1. Crear un nuevo aprendiz" << std::endl;
        std::cout << "2. Seleccionar un aprendiz existente" << std::endl;
        std::cout << "3. Comenzar el viaje" << std::endl;
        std::cout << "4. Salir del juego" << std::endl;
        std::cout << "Ingrese su elección: ";
        choice = validarEntradaEntera();

        switch (choice) {
            case 1:
                // Lógica para crear un nuevo aprendiz
                break;
            case 2:
                // Lógica para seleccionar un aprendiz existente
                break;
            case 3:
                // Lógica para comenzar el viaje
                break;
            case 4:
                std::cout << "Gracias por jugar. ¡Hasta luego!" << std::endl;
                break;
            default:
                std::cout << "Elección inválida. Intente nuevamente." << std::endl;
                break;
        }

        std::cout << std::endl;
    }

    return 0;
}

