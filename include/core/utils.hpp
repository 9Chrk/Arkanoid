#pragma once
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ranges>
#include <vector>
#include <utility>
#include <cstdio>
#include <memory>
#include <limits>
#include <algorithm>
#include <unordered_map>

#include "json.hpp"
#include "Rectangle.hpp"
#include "constants.hpp"

using json = nlohmann::json;


/* ############## Initialisation des ressources ############## */

/// @brief Lève une erreur si l'initialisation échoue.
/// @param test Résultat de l'initialisation.
/// @param description Nom de la ressource.
/// @throws std::runtime_error si test est faux.
void must_init(bool test, const std::string& description);

/// @brief Variante pointeur de must_init.
/// @param test Pointeur à contrôler.
/// @param description Nom de la ressource.
/// @throws std::runtime_error si test est nul.
void must_init(const void* test, const std::string& description);


/* ############## Fonctions JSON ############## */

/// @brief Charge un fichier JSON.
/// @param fileName Chemin vers le fichier.
/// @return Données JSON du fichier.
/// @throws std::runtime_error si le fichier est inaccessible.
json  openJsonFile(const std::string& fileName);

/// @brief Lit une clé dans un fichier JSON.
/// @param fileName Chemin vers le fichier.
/// @param key Clé recherchée.
/// @return Valeur associée à la clé.
/// @throws std::invalid_argument si la clé est absente.
json  readJsonFile(const std::string& fileName, const std::string& key);

/// @brief Écrit une clé dans un fichier JSON existant.
/// @param fileName Chemin vers le fichier.
/// @param key Clé à modifier.
/// @param value Valeur à sauvegarder.
/// @throws std::runtime_error si l'écriture échoue.
void writeJsonFile(const std::string& fileName, const std::string& key, const json& value);

/// @brief Écrit des données JSON dans un fichier.
/// @param fileName Chemin vers le fichier.
/// @param data Données complètes.
/// @throws std::runtime_error si l'écriture échoue.
void writeJsonFile(const std::string& fileName, const json& data);

/// @brief Charge la configuration du jeu.
/// @return Données de configuration.
json loadSettings();


/* ############## Commandes système ############## */

/// @brief Liste les fichiers de niveaux.
/// @param dir Répertoire à explorer.
/// @return Noms triés des fichiers.
std::vector<std::string> listLevels(const std::filesystem::path& dir);
