#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <limits>
#include <cctype>
#include <unordered_set>
#include <algorithm>

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

// Estructura nodo guardianes
struct GuardianNode
{
    Guardian guardian;
    std::vector<GuardianNode*> apprentices;
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

// Función para crear a un aprendiz
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
            break; // Salir del loop
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
            int villageIndex = selectVillage(villages);
            if (villageIndex >= 0 && villageIndex < villages.size())
            {
                auto it = villages.begin();
                std::advance(it, villageIndex);
                newGuardian.village = it->second.name;
            }
            else
            {
                std::cout << "Índice de aldea inválido. No se asignará aldea al aprendiz." << std::endl;
            }

            trainee = newGuardian;
            guardians.push_back(trainee);
            
            exitLoop = true;
        }
    } while (!exitLoop);
}

// Obtiene la lista de guardianes disponibles
void getAvailableGuardians(GuardianNode* node, std::vector<Guardian>& availableGuardians)
{
    if (node != nullptr)
    {
        // Agregar el guardián actual al vector si no es Stormheart
        if (node->guardian.name != "Stormheart")
        {
            availableGuardians.push_back(node->guardian);
        }

        // Recorrer los aprendices del nodo actual de forma recursiva
        for (GuardianNode* apprentice : node->apprentices)
        {
            getAvailableGuardians(apprentice, availableGuardians);
        }
    }
}

// Actualiza la relación maestro aprendiz
void updateHierarchy(GuardianNode* node, const std::string& oldMaster, const std::string& newMaster)
{
    if (node != nullptr)
    {
        // Actualizar el nombre del maestro del nodo si coincide con el maestro anterior
        if (node->guardian.name == oldMaster)
        {
            node->guardian.name = newMaster;
        }

        // Recorrer los aprendices del nodo actual de forma recursiva
        for (GuardianNode* apprentice : node->apprentices)
        {
            updateHierarchy(apprentice, oldMaster, newMaster);
        }
    }
}

// Seleccionar guardian de la lista inicial
void selectGuardian(GuardianNode* root, Guardian& trainee, std::vector<Guardian>& guardians)
{
    std::vector<Guardian> availableGuardians;

    // Obtener los guardianes disponibles (excepto Stormheart) del árbol de jerarquía
    getAvailableGuardians(root, availableGuardians);

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

        // Actualizar la jerarquía de niveles del árbol
        updateHierarchy(root, trainee.name, "Liz");
    } 
    else 
    {
        std::cout << "Número de guardián inválido. Intente nuevamente." << std::endl;
    }
}

// Buscar nodo
GuardianNode* findNode(GuardianNode* currentNode, const std::string& guardianName)
{
    if (currentNode->guardian.name == guardianName)
    {
        return currentNode;
    }
    else
    {
        for (GuardianNode* apprentice : currentNode->apprentices)
        {
            GuardianNode* foundNode = findNode(apprentice, guardianName);
            if (foundNode != nullptr)
            {
                return foundNode;
            }
        }
    }

    return nullptr;
}

// Crea la relación
void buildHierarchy(std::vector<Guardian>& guardians, GuardianNode* root)
{
    for (const Guardian& guardian : guardians)
    {
        if (guardian.mainMaster.empty())
        {
            // Si no tiene maestro, es un nodo raíz
            GuardianNode* node = new GuardianNode{guardian};
            root->apprentices.push_back(node);
        }
        else
        {
            // Buscar el maestro correspondiente en el árbol
            GuardianNode* masterNode = findNode(root, guardian.mainMaster);
            if (masterNode != nullptr)
            {
                // Crear un nuevo nodo para el aprendiz
                GuardianNode* apprenticeNode = new GuardianNode{guardian};

                // Establecer la relación maestro-aprendiz
                masterNode->apprentices.push_back(apprenticeNode);
            }
        }
    }
}

// Imprime la jerarquía
void printHierarchy(const GuardianNode* node, int indent = 0)
{
    if (node != nullptr)
    {
        std::cout << std::string(indent, ' ') << "- " << node->guardian.name << std::endl;

        for (const GuardianNode* apprentice : node->apprentices)
        {
            printHierarchy(apprentice, indent + 4);
        }
    }
}

// Borra el arbol
void deleteTree(GuardianNode* node)
{
    if (node != nullptr)
    {
        for (GuardianNode* apprentice : node->apprentices)
        {
            deleteTree(apprentice);
        }
        delete node;
    }
}

void travel(const std::string& origin, const std::string& destination, const std::unordered_map<std::string, Village>& villages) 
{
    if (villages.count(origin) > 0 && villages.count(destination) > 0) 
    {
        const Village& originVillage = villages.at(origin);
        const Village& destinationVillage = villages.at(destination);

        // Verificar si las aldeas están conectadas
        if (std::find(originVillage.connectedVillages.begin(), originVillage.connectedVillages.end(), destination) != originVillage.connectedVillages.end()) 
        {
            // Realizar el viaje
            std::cout << "Viaje exitoso desde " << origin << " a " << destination << std::endl;

            // Aquí puedes implementar la lógica adicional para el viaje, como el entrenamiento aleatorio del aprendiz.
        } 
        else 
        {
            std::cout << "Las aldeas no están conectadas. No se puede realizar el viaje." << std::endl;
        }
    } 
    else 
    {
        std::cout << "Aldea de origen o aldea de destino no válidas." << std::endl;
    }
}

// Menú de viaje
void travelMenu(const std::unordered_map<std::string, Village>& villages, const Guardian& trainee) 
{
    std::cout << "----- Viaje del Aprendiz -----" << std::endl;
    std::cout << "Aldea actual: " << trainee.village << std::endl;
    std::string origin = trainee.village;

    while (true) 
    {
        if (villages.count(origin) > 0) 
        {
            const Village& originVillage = villages.at(origin);

            std::cout << "Aldeas a las que se puede viajar desde " << origin << ":" << std::endl;
            for (const std::string& connectedVillage : originVillage.connectedVillages) 
            {
                std::cout << "- " << connectedVillage << std::endl;
            }

            if (originVillage.connectedVillages.empty()) 
            {
                std::cout << "No hay aldeas a las que se pueda viajar desde " << origin << std::endl;
                break; // Salir del ciclo while
            } 
            else 
            {
                std::cout << "Ingrese el nombre de la aldea de destino ('0' para salir): ";
                std::string destination;
                std::getline(std::cin, destination);

                if (destination == "0") 
                {
                    break; // Salir del ciclo while
                }

                if (villages.count(destination) > 0) 
                {
                    travel(origin, destination, villages);
                    origin = destination; // Actualizar la aldea de origen para futuros viajes
                } 
                else 
                {
                    std::cout << "Aldea de destino no válida." << std::endl;
                }
            }
        } 
        else 
        {
            std::cout << "Aldea de origen no válida." << std::endl;
            break; // Salir del ciclo while
        }
    }
}



int main() 
{
    GuardianNode* root = new GuardianNode{}; // Crear el nodo raíz vacío
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
    while (std::getline(inputFileVillages, line)) 
    {
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

    buildHierarchy(guardians, root);

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
                selectGuardian(root, trainee, guardians);
                if(!trainee.name.empty())
                {
                    selectTrainee = true;
                }
                break;
            case 3:
                if (villages.empty()) 
                {
                    std::cout << "No hay aldeas cargadas. Cargue aldeas desde el archivo antes de realizar un viaje." << std::endl;
                    break;
                }
                if (trainee.name.empty()) 
                {
                    std::cout << "No se ha seleccionado un guardián. Seleccione un guardián antes de realizar un viaje." << std::endl;
                    break;
                }
                travelMenu(villages, trainee);
                break;
            case 4:
                std::cout << "Gracias por jugar. ¡Hasta luego!" << std::endl;
                deleteTree(root);
                break;
            default:
                std::cout << "Elección inválida. Intente nuevamente." << std::endl;
                break;
        }

        std::cout << std::endl;
    }

    return 0;
}

