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
#include <functional>

using namespace std;

// Función para validar una entrada numérica entera
int validarEntradaEntera() 
{
    int valor;
    string entrada;

    while (true) 
    {
        cin >> entrada;

        // Verificar si la entrada contiene solo dígitos
        bool esNumero = true;
        for (char c : entrada) 
        {
            if (!isdigit(c)) 
            {
                esNumero = false;
                break;
            }
        }

        if (!esNumero) 
        {
            cout << "Entrada inválida. Intente nuevamente: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else 
        {
            stringstream ss(entrada);
            if (!(ss >> valor)) 
            {
                cout << "Entrada inválida. Intente nuevamente: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else 
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
        }
    }
    return valor;
}

// Estructura base de guardianes
struct Guardian 
{
    string name;
    int powerLevel;
    string mainMaster;
    string village;
};

// Estructura nodo guardianes
struct GuardianNode
{
    Guardian guardian;
    vector<GuardianNode*> apprentices;
};

// Estructura base de aldeas
struct Village 
{
    string name;
    vector<string> connectedVillages;
    string master; // Maestro actual de la aldea
    vector<Guardian> localApprentices;
};

// Función para buscar el índice de un guardián en el vector 'guardians'
int guardianIndex(const vector<Guardian>& guardians, const string& guardianName) 
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
int findGuardianByName(const vector<Guardian>& guardians, const string& name) 
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
int selectVillage(const unordered_map<string, Village>& villages) 
{
    cout << "Aldeas disponibles:" << endl;
    int index = 1;
    for (const auto& pair : villages) 
    {
        cout << index << ". " << pair.second.name << endl;
        index++;
    }

    int selectedVillageIndex = -1;

    cout << "Ingrese el número de la aldea del nuevo aprendiz: ";
    selectedVillageIndex = validarEntradaEntera();


    return selectedVillageIndex - 1; // Adjust index to match vector indexing
}

// Función para crear a un aprendiz
void createGuardian(vector<Guardian>& guardians, const unordered_map<string, Village>& villages, Guardian& trainee)
{
    string newGuardianName;
    bool exitLoop = false;

    do 
    {
        cout << "Ingrese el nombre del nuevo aprendiz (o '2' para seleccionar uno existente): ";
        getline(cin, newGuardianName);

        if (newGuardianName == "2") 
        {
            break; // Salir del loop
        }

        int existingGuardianIndex = findGuardianByName(guardians, newGuardianName);

        if (existingGuardianIndex != -1) 
        {
            cout << "El nombre ingresado ya existe. Por favor, ingrese un nombre diferente." << endl;
            cout << "Puede seleccionar el guardián existente en la opción 2." << endl;
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
                advance(it, villageIndex);
                newGuardian.village = it->second.name;
            }
            else
            {
                cout << "Índice de aldea inválido. No se asignará aldea al aprendiz." << endl;
            }

            trainee = newGuardian;
            guardians.push_back(trainee);
            
            exitLoop = true;
        }
    } while (!exitLoop);
}

// Obtiene la lista de guardianes disponibles
void getAvailableGuardians(GuardianNode* node, vector<Guardian>& availableGuardians)
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
void updateHierarchy(GuardianNode* node, const string& oldMaster, const string& newMaster)
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
void selectGuardian(GuardianNode* root, Guardian& trainee, vector<Guardian>& guardians, unordered_map<string, Village>& villages)
{
    vector<Guardian> availableGuardians;

    // Obtener los guardianes disponibles (excepto Stormheart) del árbol de jerarquía
    getAvailableGuardians(root, availableGuardians);

    cout << "Guardianes disponibles:" << endl;
    for (int i = 0; i < availableGuardians.size(); i++) 
    {
        cout << i + 1 << ". " << availableGuardians[i].name << endl;
    }

    cout << "Ingrese el número del guardián que desea seleccionar: ";
    int selectedGuardianIndex = validarEntradaEntera();

    if (selectedGuardianIndex >= 1 && selectedGuardianIndex <= availableGuardians.size()) 
    {
        trainee = availableGuardians[selectedGuardianIndex - 1];
        trainee.powerLevel = 50; // Establecer el powerLevel como 50
        cout << "Se ha seleccionado el guardián: " << trainee.name << endl;

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

            // Obtener el nombre de la aldea a la que pertenecía el guardián seleccionado
            string villageName = guardians[originalGuardianIndex].village;

            // Verificar si la aldea existe en el mapa de aldeas
            if (villages.find(villageName) != villages.end()) 
            {
                // Obtener una referencia a la aldea correspondiente
                Village& village = villages[villageName];

                // Buscar al guardián seleccionado en la lista de aprendices de la aldea
                for (Guardian& apprentice : village.localApprentices) 
                {
                    if (apprentice.name == trainee.name) 
                    {
                        // Actualizar el nombre del guardián en la lista de aprendices
                        apprentice.name = "Liz";
                        break;
                    }
                }
                
                // Verificar si el guardián seleccionado es el maestro de la aldea
                if (village.master == trainee.name) 
                {
                    // Actualizar el nombre del maestro en la aldea
                    village.master = "Liz";
                }
            }

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
        cout << "Número de guardián inválido. Intente nuevamente." << endl;
    }
}


// Buscar nodo
GuardianNode* findNode(GuardianNode* currentNode, const string& guardianName)
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
void buildHierarchy(vector<Guardian>& guardians, GuardianNode* root)
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
        cout << string(indent, ' ') << "- " << node->guardian.name << " (" << node->guardian.village << ")"<< endl;

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

void travel(const string& origin, const string& destination, const unordered_map<string, Village>& villages) 
{
    cout << "Viaje solicitado desde " << origin << " a " << destination << endl;

    if (villages.count(origin) > 0 && villages.count(destination) > 0) 
    {
        const Village& originVillage = villages.at(origin);
        const Village& destinationVillage = villages.at(destination);

        // Verificar si las aldeas están conectadas
        if (find(originVillage.connectedVillages.begin(), originVillage.connectedVillages.end(), destination) != originVillage.connectedVillages.end()) 
        {
            // Realizar el viaje
            cout << "Viaje exitoso desde " << origin << " a " << destination << endl;

            // Aquí puedes implementar la lógica adicional para el viaje, como el entrenamiento aleatorio del aprendiz.
        } 
        else 
        {
            cout << "Las aldeas no están conectadas. No se puede realizar el viaje." << endl;
        }
    } 
    else 
    {
        cout << "Aldea de origen o aldea de destino no válidas." << endl;
    }
}


// Menú de viaje
void travelMenu(unordered_map<string, Village>& villages, Guardian& trainee, GuardianNode* root)
{
    cout << "----- Viaje del Aprendiz -----" << endl;
    string origin = trainee.village;
    // Mostrar el maestro y sus aprendices
    Village& oVillage = villages.at(origin);
    GuardianNode* masterNode = findNode(root, oVillage.master);
     cout << masterNode->guardian.name << endl;

    int choice = 0;
    while (choice != 3)
    {
        cout << trainee.name << " - " << trainee.powerLevel << endl;
        cout << "Aldea actual: " << origin << endl << endl;
        
        cout << "1. Viajar a otra aldea" << endl;
        cout << "2. Ver lista de adversarios para entrenar" << endl;
        cout << "3. Volver al menú principal" << endl;
        cout << "Ingrese su elección: ";
        choice = validarEntradaEntera();

        switch (choice)
        {
        case 1:
            if (villages.count(origin) > 0)
            {
                const Village& originVillage = villages.at(origin);

                cout << "Aldeas a las que se puede viajar desde " << origin << ":" << endl;
                for (const string& connectedVillage : originVillage.connectedVillages)
                {
                    cout << "- " << connectedVillage << endl;
                }

                if (originVillage.connectedVillages.empty())
                {
                    cout << "No hay aldeas a las que se pueda viajar desde " << origin << endl;
                    break; // Salir del switch
                }
                else
                {
                    string destination;
                    do
                    {
                        cout << "Ingrese el nombre de la aldea de destino ('0' para volver al menú): ";
                        getline(cin, destination);

                        if (destination == "0")
                        {
                            break; // Salir del do-while y del switch
                        }

                        if (villages.count(destination) > 0)
                        {
                            travel(origin, destination, villages);
                            trainee.powerLevel+=1;
                            origin = destination; // Actualizar la aldea de origen para futuros viajes

                            // Actualizar la referencia a la aldea actual
                            oVillage = villages.at(origin);
                            masterNode = findNode(root, oVillage.master);
                            break; // Salir del do-while
                        }
                        else
                        {
                            cout << "Aldea de destino no válida. Intente nuevamente." << endl;
                        }
                    } while (true);
                }
            }
            else
            {
                cout << "Aldea de origen no válida." << endl;
                break; // Salir del switch
            }

            break;
        case 2:
            // Lista de adversarios para entrenar en la aldea actual
            cout << "Lista de adversarios para entrenar en " << origin << ":" << endl;
            
            if (masterNode != nullptr)
            {
                cout << "Maestro: " << masterNode->guardian.name << " - " << masterNode->guardian.powerLevel << endl;
                cout << "Aprendices:" << endl;
                for (Guardian apprentice : oVillage.localApprentices) {
                    if (apprentice.name != oVillage.master) { // Excluir al maestro de la lista
                        cout << "- " << apprentice.name << " - " << apprentice.powerLevel << endl;
                    }
                }
            }
            else
            {
                cout << "No se encontró al maestro y sus aprendices en la jerarquía." << endl;
                break;
            }
            // ...
            break;
        case 3:
            cout << "Volviendo al menú principal." << endl;
            break;
        default:
            cout << "Elección inválida. Intente nuevamente." << endl;
            break;
        }
        cout << endl;
    }
}

// Función comparadora para ordenar guardianes por powerLevel de forma descendente
bool compareGuardiansByPowerLevel(const Guardian& a, const Guardian& b)
{
    return a.powerLevel > b.powerLevel;
}




int main() 
{
    GuardianNode* root = new GuardianNode{}; // Crear el nodo raíz vacío
    vector<Guardian> guardians;
    unordered_map<string, Village> villages;
    bool selectTrainee = false;
    Guardian trainee;// Variable para almacenar al aprendiz

#pragma  region Archivos //Carga de archivos de guardianes y aldeas

    ifstream inputFileVillages("./Archivos/villages.txt");
    if (!inputFileVillages) {
        cout << "No se pudo abrir el archivo." << endl;
        return 1;
    }

    string line;
    getline(inputFileVillages, line); // Leer y descartar la primera línea de encabezado
    while (getline(inputFileVillages, line)) 
    {
        istringstream iss(line);
        string villageName, connectedVillage;
        getline(iss, villageName, ',');
        getline(iss, connectedVillage);

        villages[villageName].name = villageName;
        villages[villageName].connectedVillages.push_back(connectedVillage);
    }

    inputFileVillages.close();

    ifstream inputFile("./Archivos/guardians.txt");
    if (!inputFile) 
    {
        cout << "No se pudo abrir el archivo." << endl;
        return 1;
    }

    string lineG;
    while (getline(inputFile, lineG)) 
    {
        Guardian guardian;

        istringstream iss(lineG);
        getline(iss, guardian.name, ',');
        iss >> guardian.powerLevel;
        iss.ignore();
        getline(iss, guardian.mainMaster, ',');
        getline(iss, guardian.village);

        guardians.push_back(guardian); 

        // Actualizar el maestro de la aldea si el guardian tiene un powerLevel mayor
        if (villages.find(guardian.village) != villages.end()) 
        {
            Village& village = villages[guardian.village];
            if (village.master.empty() || guardian.powerLevel > guardians[guardianIndex(guardians, village.master)].powerLevel) {
                village.master = guardian.name;
            }
            
            village.localApprentices.push_back(guardian); // Agregar el aprendiz a la lista

            // Ordenar a los aprendices según su powerLevel
            sort(village.localApprentices.begin(), village.localApprentices.end(), compareGuardiansByPowerLevel);
        }
    }

    inputFile.close();

#pragma endregion Archivos    

    buildHierarchy(guardians, root);

    int choice = 0;
    while (choice != 4) {
        cout << "----- Guardian's Journey -----" << endl;
        if(selectTrainee){cout << trainee.name << endl;}
        cout << "1. Crear un nuevo aprendiz" << endl;
        cout << "2. Seleccionar un guardian existente" << endl;
        cout << "3. Comenzar el viaje" << endl;
        cout << "4. Salir del juego" << endl;
        cout << "Ingrese su elección: ";
        choice = validarEntradaEntera();

        switch (choice) {
            case 1:
                if (selectTrainee) 
                {
                    cout << "Ya se seleccionó un guardián. Complete la aventura para crear uno nuevo." << endl;
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
                    cout << "Ya se seleccionó un guardián. Complete la aventura para crear uno nuevo." << endl;
                    break; // No se puede seleccionar otro
                }
                selectGuardian(root, trainee, guardians, villages);
                if(!trainee.name.empty())
                {
                    selectTrainee = true;
                }
                break;
            case 3:
                if (villages.empty()) 
                {
                    cout << "No hay aldeas cargadas. Cargue aldeas desde el archivo antes de realizar un viaje." << endl;
                    break;
                }
                if (trainee.name.empty()) 
                {
                    cout << "No se ha seleccionado un guardián. Seleccione un guardián antes de realizar un viaje." << endl;
                    break;
                }
                travelMenu(villages, trainee, root);
                break;
            case 4:
                cout << "Gracias por jugar. ¡Hasta luego!" << endl;
                deleteTree(root);
                break;
            default:
                cout << "Elección inválida. Intente nuevamente." << endl;
                break;
        }

        cout << endl;
    }

    return 0;
}

