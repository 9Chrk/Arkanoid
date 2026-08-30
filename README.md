# Arkanoid

![C++20](https://img.shields.io/badge/C%2B%2B-20-blue)
![Allegro 5](https://img.shields.io/badge/Allegro-5-orange)
![Licence MIT](https://img.shields.io/badge/Licence-MIT-green)

Arkanoid est un jeu de casse-briques inspiré d’*Arkanoid*, développé en **C++20** avec la bibliothèque graphique **Allegro 5**. Le joueur dirige une raquette, renvoie la balle, détruit les briques et enchaîne les niveaux tout en conservant son score.

Le jeu propose des menus graphiques, des effets sonores, un affichage du score et du meilleur score, ainsi que des bonus déclenchés par certaines briques. Les paramètres de jeu et les niveaux sont fournis sous forme de fichiers JSON dans `assets/data/`.

Projet réalisé dans le cadre du cours **Langages de programmation 2 — INFO-F-202** à l’Université libre de Bruxelles, durant l’année académique 2024-2025.

---

<a id="captures-decran"></a>

## 📸 Captures d’écran

| Menu principal | Niveau en cours | Autre niveau |
| --- | --- | --- |
| ![Menu principal](assets/screenshots/start_menu.png) | ![Partie](assets/screenshots/game_level.png) | ![Partie sur un autre niveau](assets/screenshots/game_level_1.png) |

| Défaite | Niveau terminé | Partie terminée |
| --- | --- | --- |
| ![Défaite](assets/screenshots/lose_menu.png) | ![Victoire d’un niveau](assets/screenshots/win_level_menu.png) | ![Victoire finale](assets/screenshots/win_game_menu.png) |

---

<a id="sommaire"></a>

## 📖 Sommaire

- [Fonctionnalités](#fonctionnalites)
- [Prérequis](#prerequis)
- [Configuration](#configuration)
- [Installation](#installation)
- [Lancement](#lancement)
- [Utilisation](#utilisation)
- [Données et persistance](#donnees-et-persistance)
- [Architecture](#architecture)
- [Flux général](#flux-general)
- [Structure du projet](#structure-du-projet)
- [Tests](#tests)
- [Documentation](#documentation)
- [Problèmes fréquents](#problemes-frequents)
- [Licence](#licence)

---

<a id="fonctionnalites"></a>

## ✨ Fonctionnalités

- **Progression sur neuf niveaux** : le moteur détecte les fichiers `level_*.json` dans `assets/data/`, les charge dans l’ordre et conserve le score au passage au niveau suivant.
- **Casse-briques et score** : les briques attribuent des points ; les briques à deux impacts changent d’état après le premier choc, tandis que les briques dorées ne sont pas détruites.
- **Trois vies et meilleur score** : la raquette commence avec trois vies. Le meilleur score est lu puis mis à jour dans `assets/data/settings.json`.
- **Bonus jouables** : certaines briques libèrent un bonus qui agrandit temporairement la raquette, immobilise temporairement la balle sur la raquette, ralentit la balle ou ajoute une vie.
- **Contrôles clavier et souris** : la raquette se déplace horizontalement avec le clavier ou avec la souris ; une touche lance la balle et permet de redémarrer la partie.
- **Interface et audio Allegro** : les écrans de démarrage, victoire et défaite, le HUD, les bitmaps, les polices et les sons sont chargés depuis `assets/`.

---

<a id="prerequis"></a>

## 🧰 Prérequis

- Un compilateur `g++` compatible **C++20** ;
- GNU Make ;
- `pkg-config` ;
- Allegro 5 et les modules `allegro_primitives`, `allegro_font`, `allegro_image`, `allegro_ttf`, `allegro_audio` et `allegro_acodec`.

La bibliothèque JSON est incluse dans le dépôt sous la forme de l’en-tête `libs/json.hpp` : aucune installation séparée n’est prévue pour elle.

---

<a id="configuration"></a>

## ⚙️ Configuration

Le jeu ne requiert aucune variable d’environnement. Ses paramètres sont centralisés dans `assets/data/settings.json`, notamment la taille et la vitesse de la balle, les dimensions de la raquette, le nombre de vies, la géométrie des briques, les couleurs et les positions de l’interface.

Les fichiers `assets/data/level_0.json` à `assets/data/level_8.json` décrivent les grilles de briques. Chaque entrée associe une valeur de score à une éventuelle abréviation de bonus, par exemple `1|EX`.

---

<a id="installation"></a>

## 📦 Installation

Clonez le dépôt puis compilez la cible par défaut du `Makefile` :

```bash
git clone https://github.com/9Chrk/Arkanoid.git
cd Arkanoid
make
```

La compilation crée l’exécutable `Arkanoid` à la racine et les fichiers objets dans `obj/`. Pour nettoyer ces artefacts :

```bash
make clean
```

---

<a id="lancement"></a>

## ▶️ Lancement

Depuis la racine du projet, après compilation :

```bash
./Arkanoid
```

Le programme doit être lancé depuis cette racine : les niveaux et les ressources sont chargés avec des chemins relatifs vers `assets/`.

---

<a id="utilisation"></a>

## 🎮 Utilisation

Au menu principal, utilisez la souris pour choisir de jouer ou de quitter. Pendant une partie :

| Action | Commande |
| --- | --- |
| Déplacer la raquette à gauche | `←`, `A` ou `Q` |
| Déplacer la raquette à droite | `→`, `D` ou `P` |
| Déplacer la raquette | Mouvement horizontal de la souris |
| Lancer la balle | `Espace` |
| Réinitialiser la balle, le score et le meilleur score | `R` |
| Charger directement un niveau | `0` à `8` |
| Quitter | `Échap` ou fermeture de la fenêtre |

Après la victoire d’un niveau ou à la fin de la partie, le jeu attend une touche avant de poursuivre ou de fermer la fenêtre.

---

<a id="donnees-et-persistance"></a>

## 🗃️ Données et persistance

| Emplacement | Format | Rôle |
| --- | --- | --- |
| `assets/data/settings.json` | JSON | Paramètres de jeu, couleurs d’interface et meilleur score. |
| `assets/data/level_0.json` à `assets/data/level_8.json` | JSON | Disposition, score et bonus associés aux briques de chaque niveau. |
| `assets/images/`, `assets/fonts/`, `assets/sounds/` | PNG, TTF, WAV | Ressources chargées par la vue Allegro. |

À la fin d’une tentative de niveau, `GameModel::saveHighScore()` peut réécrire la clé `highscore` de `assets/data/settings.json`. La touche `R` peut aussi la remettre à zéro ; exécuter le jeu est donc susceptible de modifier ce fichier suivi par Git.

---

<a id="architecture"></a>

## 🧱 Architecture

Le point d’entrée `src/main.cpp` crée `Engine` et convertit toute exception non gérée en message sur la sortie d’erreur. `src/engine/Engine.cpp` initialise ensuite le modèle, la vue et le contrôleur, affiche le menu, puis pilote une boucle d’événements Allegro à 60 rafraîchissements par seconde.

Le projet suit une organisation **Modèle–Vue–Contrôleur**. `GameModel` dans `src/model/` porte l’état de la partie — balle, raquette, briques, bonus, score et meilleur score. `GameView` dans `src/view/` initialise Allegro, charge les ressources et effectue le rendu. `GameController` dans `src/controller/` traduit les actions reçues en modifications du modèle. `AllegroInputAdapter` isole la conversion des événements clavier, souris et minuteur en `InputAction`.

La logique de collision est centralisée dans `GameModel::checkCollisions()`. Elle échantillonne plusieurs points autour de la balle, cherche l’impact le plus proche le long de sa trajectoire parmi les briques actives, applique le rebond suivant le côté touché, puis met à jour le score, l’état de la brique et un éventuel bonus. La collision avec les bordures et la raquette est gérée par `Ball` ; l’angle de rebond dépend de la position d’impact sur la raquette.

Les fichiers JSON sont lus par les utilitaires de `src/core/utils.cpp`. Ils alimentent l’initialisation de `GameModel` et de `UIConfig`; seul le meilleur score est écrit en retour dans `settings.json`.

---

<a id="flux-general"></a>

## 🧬 Flux général

```text
src/main.cpp
  → Engine::run()
      → chargement des niveaux JSON et création Model / View / Controller
      → menu Allegro
      → événement Allegro
          → AllegroInputAdapter
          → GameController
          → GameModel (mouvement, collisions, bonus, score)
          → GameView (rendu et sons)
      → sauvegarde éventuelle du meilleur score
```

---

<a id="structure-du-projet"></a>

## 📂 Structure du projet

```text
Arkanoid/
├── assets/
│   ├── data/                 # Réglages JSON et neuf descriptions de niveaux
│   ├── fonts/                # Polices utilisées par l’interface
│   ├── images/               # Écrans, HUD et sprite de la raquette
│   ├── screenshots/          # Captures présentées dans ce README
│   └── sounds/               # Musiques et effets sonores WAV
├── docs/                     # Énoncé et rapport du projet
├── include/
│   ├── controller/           # Interface du contrôleur de jeu
│   ├── core/                 # Géométrie, constantes et utilitaires JSON
│   ├── engine/               # Orchestration de la boucle de jeu
│   ├── input/                # Actions indépendantes d’Allegro
│   ├── model/                # Entités, collisions et bonus
│   └── view/                 # Affichage, ressources et adaptation des entrées
├── libs/json.hpp             # Dépendance JSON embarquée
├── src/                      # Implémentations C++ correspondantes
├── Makefile                  # Compilation avec g++ et pkg-config
├── LICENSE                   # Licence MIT
└── README.md
```

Les en-têtes de `include/` définissent les contrats utilisés par les implémentations de `src/`. Les sous-dossiers `model/`, `view/` et `controller/` communiquent par l’intermédiaire de `GameModel`, `GameView` et `GameController`, tandis que `engine/` coordonne leur cycle de vie.

---

<a id="tests"></a>

## 🧪 Tests

Le `Makefile` ne définit pas de cible de test automatisé. Il fournit les cibles `all` (utilisée par `make`) et `clean`.

---

<a id="documentation"></a>

## 📄 Documentation

- [Énoncé du projet](docs/enonce-project-2024.pdf)
- [Rapport du projet](docs/rapport.pdf)

---

<a id="problemes-frequents"></a>

## ❗ Problèmes fréquents

### `pkg-config` ou Allegro introuvable lors de la compilation

Le `Makefile` interroge `pkg-config` pour obtenir les options de compilation et d’édition de liens d’Allegro 5 et de ses modules. Vérifiez que `pkg-config` ainsi que `allegro-5`, `allegro_primitives-5`, `allegro_font-5`, `allegro_image-5`, `allegro_ttf-5`, `allegro_audio-5` et `allegro_acodec-5` sont installés et visibles par `pkg-config`.

### Erreur de chargement des niveaux ou des ressources

L’exécutable utilise des chemins relatifs tels que `./assets/data` et `./assets/images`. Exécutez `./Arkanoid` depuis la racine du dépôt et conservez l’arborescence `assets/`.

### `No level files found.`

Cette erreur est levée si aucun fichier `level_*.json` n’est trouvé dans `assets/data/`. Vérifiez la présence des fichiers de niveau et le répertoire courant de lancement.

---

<a id="licence"></a>

## 📜 Licence

Ce projet est distribué sous licence [MIT](LICENSE).
