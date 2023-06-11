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
#include <random>

using namespace std;

// Función para generar un número aleatorio entre 1 y 20 (dado de 20 caras)
int rollDice() 
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 20);
    return dis(gen);
}

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
    bool visited;
    bool completed = false;

    Village() {} // Constructor predeterminado sin argumentos

    Village(const string& villageName) // Constructor que acepta un nombre de aldea
    {
        name = villageName;
    }
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
    for (int i = 1; i < availableGuardians.size(); i++) 
    {
        cout << i << ". " << availableGuardians[i].name << endl;
    }

    cout << "Ingrese el número del guardián que desea seleccionar: ";
    int selectedGuardianIndex = validarEntradaEntera();

    if (selectedGuardianIndex >= 1 && selectedGuardianIndex <= availableGuardians.size()) 
    {
        trainee = availableGuardians[selectedGuardianIndex];
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
        if (!node->guardian.name.empty()) 
        {
            cout << string(indent, ' ') << "- " << node->guardian.name << " (" << node->guardian.village << ")" << endl;
        }

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

bool training(Guardian& trainee, const Guardian& adversary, int title)
{
    cout << "El entrenamiento será de " << trainee.name << " VS " << adversary.name << endl;
    const int powerMax = 100;
    const int rangeLowerLimit = -5; // Modificar el límite inferior del rango
    const int rangeUpperLimit = 5;  // Modificar el límite superior del rango

    // Calcula la diferencia de poder entre el personaje y el adversario
    int powerDifference = adversary.powerLevel - trainee.powerLevel;

    // Calcula la dificultad modificada para el entrenamiento
    float adjustedDifference = static_cast<float>(powerDifference) / powerMax * (rangeUpperLimit - rangeLowerLimit);
    int difficulty;
    if (title == 1) {
        difficulty = 10 + static_cast<int>(adjustedDifference) + (adversary.powerLevel == powerMax ? 5 : 0) - (powerMax - trainee.powerLevel) / 10;
    } else {
        difficulty = 5 + static_cast<int>(adjustedDifference) + (adversary.powerLevel == powerMax ? 5 : 0) - (powerMax - trainee.powerLevel) / 10;
    }

    // Genera el resultado del dado
    int diceResult = rollDice(); // Reemplaza con tu función de generación de números aleatorios

    // Compara el resultado del dado con la dificultad para determinar el éxito o fracaso
    bool isSuccess = (diceResult > difficulty);

    // Imprime el resultado
    cout << "Resultado del dado: " << diceResult << endl;
    cout << "Dificultad del entrenamiento: " << difficulty << endl;
    return isSuccess;
}

void travel(const string& origin, const string& destination, const unordered_map<string, Village>& villages) 
{

    if (villages.count(origin) > 0 && villages.count(destination) > 0) 
    {
        const Village& originVillage = villages.at(origin);
        const Village& destinationVillage = villages.at(destination);

        // Verificar si las aldeas están conectadas
        if (find(originVillage.connectedVillages.begin(), originVillage.connectedVillages.end(), destination) != originVillage.connectedVillages.end()) 
        {
            // Realizar el viaje
            cout << "Viaje exitoso desde " << origin << " a " << destination << endl;
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

void travelMenu(unordered_map<string, Village>& villages, Guardian& trainee, GuardianNode* root)
{
    
    string origin = trainee.village;
    Village* oVillagePtr = &villages.at(origin);
    oVillagePtr->visited = true;
    GuardianNode* masterNode = findNode(root, oVillagePtr->master);
    int villagesPoints = 0; // Inicializar con 0 puntos
    int choice = 0;
    bool resultTraining;
    vector<string> adversariesFaced; // Lista temporal para almacenar los adversarios ya enfrentados
    bool condition =  true;

    cout << "----- Viaje del Aprendiz -----" << endl;
    while (choice != 9)
    {
        if (villagesPoints == 4 || adversariesFaced.size() == oVillagePtr->localApprentices.size() + 1 /*Maestro incluido*/)
        {
            cout << "Actualmente no puedes reunir más puntos en esta aldea, se recomienda viajar a una nueva " << endl;
            oVillagePtr->completed = true;
        }
        
        cout << trainee.name << " - " << trainee.powerLevel << endl;
        cout << "Aldea actual: " << origin << endl << endl;
        cout << "1. Viajar a otra aldea" << endl;
        cout << "2. Ver lista de adversarios para entrenar" << endl;
        cout << "Ingrese su elección: ";
        choice = validarEntradaEntera();
        switch (choice)
        {
        case 1:
            if (villages.count(origin) > 0)
            {   
                Village& originVillage = *oVillagePtr;
                cout << "originVillage: " << originVillage.name << endl;
                cout << "Aldeas a las que se puede viajar desde " << originVillage.name << ":" << endl;
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
                            trainee.powerLevel += 1;
                            villagesPoints = 1; // Reiniciar villagesPoints a 1 (se obtiene 1 punto por viajar)
                            origin = destination; // Actualizar la aldea de origen para futuros viajes
                            oVillagePtr = &villages.at(origin); // Actualizar el puntero oVillagePtr
                            oVillagePtr->visited = true;
                            masterNode = findNode(root, oVillagePtr->master);
                            adversariesFaced.clear();
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
            if (oVillagePtr->completed == true)
            {
                cout << "Ya haz alcanzado la máxima puntuación en " << oVillagePtr->name << ". Intenta visitar otra aldea" << endl;
                break;
            }

            if (oVillagePtr->name == "Tesla")
            {
                if(trainee.powerLevel <= 90)
                {
                    cout << "No tienes el poder suficiente, aún no estas preparado para este desafio" << endl;
                    condition = false;
                    break;
                }
                for(const auto& pair : villages)
                {
                    const Village& village = pair.second;
                    if (!village.completed && trainee.powerLevel <= 90)
                    {
                        cout << "No has visitado todas las aldeas, aún no estas preparado para este desafio" << endl;
                        condition = false;
                        break;
                    }    
                }
            }
            
            if (!condition && oVillagePtr->name == "Tesla")
            {
                break; // Detener el case 2 si no se cumple la condición
            }
            
            // Lista de adversarios para entrenar en la aldea actual
            cout << "Lista de adversarios para entrenar en " << origin << ":" << endl;

            if (masterNode != nullptr)
            {
                cout << "1. Maestro: " << masterNode->guardian.name << " - " << masterNode->guardian.powerLevel << endl;
                cout << "Aprendices:" << endl;

                // Buscar el aprendiz con el puntaje más bajo
                Guardian lowestScoringApprentice;
                bool firstApprentice = true;
                // Enumerar a los aprendices
                int apprenticeNumber = 2;
                for (const Guardian& apprentice : oVillagePtr->localApprentices)
                {
                    if (apprentice.name != oVillagePtr->master) // Excluir al maestro de la lista
                    {
                        cout << apprenticeNumber << ". " << apprentice.name << " - " << apprentice.powerLevel << endl;
                        apprenticeNumber++;
                        if (firstApprentice || apprentice.powerLevel < lowestScoringApprentice.powerLevel)
                        {
                            lowestScoringApprentice = apprentice;
                            firstApprentice = false;
                        }
                    }
                }
                // Etiquetar al último aprendiz como recomendado
                if (!firstApprentice)
                {
                    cout << "Recomendado: " << lowestScoringApprentice.name << " - " << lowestScoringApprentice.powerLevel << endl;
                }

                int choiceAd = -10;
                while (choiceAd < 0 || choiceAd >= apprenticeNumber)
                {
                    cout << "Seleccione el número del adversario contra el que desea pelear ('0' para volver al menú): ";
                    choiceAd = validarEntradaEntera();
                }
                
                if(choiceAd == 0)
                {
                    break;
                }
                else if (choiceAd == 1)
                {

                    // El usuario eligió pelear contra el maestro
                    // Verificar si el maestro ya fue enfrentado previamente
                    if (find(adversariesFaced.begin(), adversariesFaced.end(), masterNode->guardian.name) != adversariesFaced.end())
                    {
                        cout << "Ya has enfrentado al maestro anteriormente." << endl;
                        break;
                    }

                    bool resultTraining = training(trainee, masterNode->guardian, 1);
                    if (resultTraining)
                    {
                        
                        if (villagesPoints <= 2)
                        {   
                            villagesPoints = min(villagesPoints + 2, 4); // Incrementar villagesPoints (máximo 4)
                            trainee.powerLevel += 2;
                            adversariesFaced.push_back(masterNode->guardian.name); // Agregar el maestro a la lista de adversarios enfrentados
                        }
                        else if (villagesPoints > 2 && villagesPoints < 4)
                        {
                            villagesPoints = min(villagesPoints + 1, 4); // Incrementar villagesPoints (máximo 4)
                            trainee.powerLevel += 1;
                            adversariesFaced.push_back(masterNode->guardian.name); // Agregar el maestro a la lista de adversarios enfrentados
                        }
                    } else
                    {
                        cout << "Entrenamiento no superado" << endl;
                    }
                }
                else
                {
                    // El usuario eligió pelear contra un aprendiz
                    const Guardian& selectedApprentice = oVillagePtr->localApprentices[choiceAd - 2];
                    // Verificar si el aprendiz ya fue enfrentado previamente
                    if (find(adversariesFaced.begin(), adversariesFaced.end(), selectedApprentice.name) != adversariesFaced.end())
                    {
                        cout << "Ya has enfrentado a este aprendiz anteriormente." << endl;
                        break;
                    }

                    resultTraining = training(trainee, selectedApprentice, 2);
                    if (resultTraining)
                    {
                        if (villagesPoints < 4)
                        {
                            villagesPoints = min(villagesPoints + 1, 4); // Incrementar villagesPoints (máximo 4)
                            trainee.powerLevel += 1;
                            adversariesFaced.push_back(selectedApprentice.name); // Agregar el aprendiz a la lista de adversarios enfrentados
                        } 
                    } else
                    {
                        cout << "Entrenamiento no superado" << endl;
                    }      
                }
            }
            else
            {
                cout << "No se encontró al maestro y sus aprendices en la jerarquía." << endl;
                break;
            }    
            break;
        default:
            cout << "Elección inválida. Intente nuevamente." << endl;
            break;
        }
        cout << "Puntos ganados en la aldea actual: " << villagesPoints << endl;
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

         // Verificar si las aldeas ya existen en el mapa
        if (villages.count(villageName) == 0) {
            villages[villageName] = Village(villageName);
        }
        if (villages.count(connectedVillage) == 0) {
            villages[connectedVillage] = Village(connectedVillage);
        }

        // Establecer conexión desde villageName a connectedVillage
        villages[villageName].connectedVillages.push_back(connectedVillage);

        // Establecer conexión desde connectedVillage a villageName
        villages[connectedVillage].connectedVillages.push_back(villageName);
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

    // Verificar el powerLevel del guardián
    if (guardian.powerLevel <= 100) 
    {
            guardians.push_back(guardian);

            // Actualizar el maestro de la aldea si el guardian tiene un powerLevel mayor
            if (villages.find(guardian.village) != villages.end()) 
            {
                Village& village = villages[guardian.village];
                if (village.master.empty() || guardian.powerLevel > guardians[guardianIndex(guardians, village.master)].powerLevel) 
                {
                    village.master = guardian.name;
                }
                
                village.localApprentices.push_back(guardian); // Agregar el aprendiz a la lista

                // Ordenar a los aprendices según su powerLevel
                sort(village.localApprentices.begin(), village.localApprentices.end(), compareGuardiansByPowerLevel);
            }
        }
        else 
        {
            cout << "Guardián con nivel de poder superior a 100: " << guardian.name << " - Nivel de poder corregido a 100." << endl;
            guardian.powerLevel = 100; // Corregir el nivel de poder a 100
            guardians.push_back(guardian);
        }
    }



    inputFile.close();

    for (auto& entry : villages) {
        Village& village = entry.second;
        string& master = village.master;
        vector<Guardian>& apprentices = village.localApprentices;

        // Verificar si la aldea es "Tesla"
        if (village.name == "Tesla") {
            continue; // Saltar la validación para la aldea "Tesla"
        }

        // Verificar si el maestro está presente en la lista de aprendices
        auto it = find_if(apprentices.begin(), apprentices.end(), [&](const Guardian& guardian) {
            return guardian.name == master;
        });

        // Si el maestro está en la lista, eliminarlo
        if (it != apprentices.end()) {
            apprentices.erase(it);
        }

        // Verificar si no hay aprendices restantes en la lista después de eliminar al maestro
        if (apprentices.empty()) {
            cout << "Error: La aldea " << village.name << " no tiene aprendices." << endl;
            return 1;
        }
    }




#pragma endregion Archivos    

    buildHierarchy(guardians, root);
    printHierarchy(root);

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