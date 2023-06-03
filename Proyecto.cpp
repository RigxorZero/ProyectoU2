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
int validarEntradaEntera() 
{
    int valor;
    std::string entrada;

    while (true) 
    {
        std::cin >> entrada;

        // Verificar si la entrada contiene solo dígitos
        bool esNumero = true;
        for (char c : entrada) 
        {
            if (!std::isdigit(c)) 
            {
                esNumero = false;
                break;
            }
        }

        if (!esNumero) 
        {
            std::cout << "Entrada inválida. Intente nuevamente: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else 
        {
            std::stringstream ss(entrada);
            if (!(ss >> valor)) 
            {
                std::cout << "Entrada inválida. Intente nuevamente: ";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            } else 
            {
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
int guardianIndex(const std::vector<Guardian>& guardians, const std::string& guardianName) 
{
    for (int i = 0; i < guardians.size(); i++) 
    {
        if (guardians[i].name == guardianName) 
        {
            return i;
        }
    }
    return -1; // Si no se encuentra el guardián, se devuelve -1
}

// Función para encontrar un guardián por nombre en el vector de guardianes
int findGuardianByName(const std::vector<Guardian>& guardians, const std::string& name) 
{
    for (int i = 0; i < guardians.size(); i++) 
    {
        if (guardians[i].name == name) 
        {
            return i;
        }
    }
    return -1;
}

// Función para mostrar las aldeas disponibles y seleccionar la aldea para el nuevo aprendiz
int selectVillage(const std::unordered_map<std::string, Village>& villages) 
{
    std::cout << "Aldeas disponibles:" << std::endl;
    int index = 1;
    for (const auto& pair : villages) 
    {
        std::cout << index << ". " << pair.second.name << std::endl;
        index++;
    }

    int selectedVillageIndex = -1;

    std::cout << "Ingrese el número de la aldea del nuevo aprendiz: ";
    selectedVillageIndex = validarEntradaEntera();


    return selectedVillageIndex - 1; // Adjust index to match vector indexing
}


void createGuardian(std::vector<Guardian>& guardians, const std::unordered_map<std::string, Village>& villages, Guardian& trainee)
{
    std::string newGuardianName;
    bool exitLoop = false;

    do 
    {
        std::cout << "Ingrese el nombre del nuevo aprendiz (o '2' para seleccionar uno existente): ";
        std::getline(std::cin, newGuardianName);

        if (newGuardianName == "2") 
        {
            break; // Exit the loop and go to option 2
        }

        int existingGuardianIndex = findGuardianByName(guardians, newGuardianName);

        if (existingGuardianIndex != -1) 
        {
            std::cout << "El nombre ingresado ya existe. Por favor, ingrese un nombre diferente." << std::endl;
            std::cout << "Puede seleccionar el guardián existente en la opción 2." << std::endl;
        }
        else 
        {
            // Crear aprendiz
            Guardian newGuardian;
            newGuardian.name = newGuardianName;
            newGuardian.powerLevel = 50;

            // Seleccionar la aldea
            selectVillage(villages);

            guardians.push_back(newGuardian);
            trainee = newGuardian;
            exitLoop = true;
        }
    } while (!exitLoop);
}


void selectGuardian(std::vector<Guardian>& guardians, std::unordered_map<std::string, Village>& villages, Guardian& trainee)
{
    std::vector<Guardian> availableGuardians;

    // Agregar los guardianes disponibles (excepto Stormheart) al vector temporal
    for (const Guardian& guardian : guardians) 
    {
        if (guardian.name != "Stormheart") 
        {
            availableGuardians.push_back(guardian);
        }
    }

    std::cout << "Guardianes disponibles:" << std::endl;
    for (int i = 0; i < availableGuardians.size(); i++) 
    {
        std::cout << i + 1 << ". " << availableGuardians[i].name << std::endl;
    }

    std::cout << "Ingrese el número del guardián que desea seleccionar: ";
    int selectedGuardianIndex = validarEntradaEntera();

    if (selectedGuardianIndex >= 1 && selectedGuardianIndex <= availableGuardians.size()) 
    {
        trainee = availableGuardians[selectedGuardianIndex - 1];
        trainee.powerLevel = 50; // Establecer el powerLevel como 50
        std::cout << "Se ha seleccionado el guardián: " << trainee.name << std::endl;

        // Obtener el índice del guardián seleccionado en el vector original
        int originalGuardianIndex = -1;
        for (int i = 0; i < guardians.size(); i++) 
        {
            if (guardians[i].name == trainee.name) 
            {
                originalGuardianIndex = i;
                break;
            }
        }

        if (originalGuardianIndex != -1) 
        {
            // Modificar el nombre del guardián en la lista original por "Liz"
            guardians[originalGuardianIndex].name = "Liz"; 

            // Actualizar el nombre del maestro en los discípulos
            for (Guardian& disciple : guardians) 
            {
                // Verificar si el discípulo tiene al maestro anterior
                if (disciple.mainMaster == trainee.name) 
                {
                    // Establecer "Liz" como el nuevo maestro del discípulo
                    disciple.mainMaster = "Liz";
                }
            }
        }
        
    } 
    else 
    {
        std::cout << "Número de guardián inválido. Intente nuevamente." << std::endl;
    }
}



int main() 
{
    std::vector<Guardian> guardians;
    std::unordered_map<std::string, Village> villages;
    bool selectTrainee = false;
    Guardian trainee;// Variable para almacenar al aprendiz

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
        if (villages.find(guardian.village) != villages.end()) 
        {
            Village& village = villages[guardian.village];
            if (village.master.empty() || guardian.powerLevel > guardians[guardianIndex(guardians, village.master)].powerLevel) {
                village.master = guardian.name;
            }
        }
    }

    

    inputFile.close();

#pragma endregion Archivos    

    // Imprimir los guardianes
    std::cout << "----- Guardianes -----" << std::endl;
    for (const Guardian& guardian : guardians) 
    {
        std::cout << "Nombre: " << guardian.name << std::endl;
        std::cout << "Nivel de poder: " << guardian.powerLevel << std::endl;
        std::cout << "Maestro principal: " << guardian.mainMaster << std::endl;
        std::cout << "Aldea: " << guardian.village << std::endl;
        std::cout << std::endl;
    }

    // Imprimir las aldeas y sus conexiones
    std::cout << "----- Aldeas -----" << std::endl;
    for (const auto& pair : villages) 
    {
        const Village& village = pair.second;
        std::cout << "Nombre: " << village.name << std::endl;
        std::cout << "Aldeas conectadas: ";
        for (const std::string& connectedVillage : village.connectedVillages) 
        {
            std::cout << connectedVillage << " ";
        }
        std::cout << std::endl << std::endl;
    }

    int choice = 0;
    while (choice != 4) {
        std::cout << "----- Guardian's Journey -----" << std::endl;
        if(selectTrainee){std::cout << trainee.name << std::endl;}
        std::cout << "1. Crear un nuevo aprendiz" << std::endl;
        std::cout << "2. Seleccionar un guardian existente" << std::endl;
        std::cout << "3. Comenzar el viaje" << std::endl;
        std::cout << "4. Salir del juego" << std::endl;
        std::cout << "Ingrese su elección: ";
        choice = validarEntradaEntera();

        switch (choice) {
            case 1:
                if (selectTrainee) 
                {
                    std::cout << "Ya se seleccionó un guardián. Complete la aventura para crear uno nuevo." << std::endl;
                    break; // No se puede crear otro
                }
                createGuardian(guardians, villages, trainee);
                if(!trainee.name.empty())
                {
                    selectTrainee = true;
                }
                break;
            case 2:
                if (selectTrainee) 
                {
                    std::cout << "Ya se seleccionó un guardián. Complete la aventura para crear uno nuevo." << std::endl;
                    break; // No se puede seleccionar otro
                }
                selectGuardian(guardians, villages, trainee);
                if(!trainee.name.empty())
                {
                    selectTrainee = true;
                }

                // Imprimir los guardianes
                std::cout << "----- Guardianes -----" << std::endl;
                for (const Guardian& guardian : guardians) 
                {
                    std::cout << "Nombre: " << guardian.name << std::endl;
                    std::cout << "Nivel de poder: " << guardian.powerLevel << std::endl;
                    std::cout << "Maestro principal: " << guardian.mainMaster << std::endl;
                    std::cout << "Aldea: " << guardian.village << std::endl;
                    std::cout << std::endl;
                }
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

