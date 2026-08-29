# Arkanoid

![C++20](https://img.shields.io/badge/C%2B%2B-20-blue)
![Allegro 5](https://img.shields.io/badge/Allegro-5-orange)
![Licence MIT](https://img.shields.io/badge/Licence-MIT-green)

Jeu de casse-briques inspiré d'Arkanoid, développé en C++20 avec Allegro 5. Le joueur déplace une raquette afin de renvoyer la balle, détruire les briques et progresser à travers les niveaux.

Projet réalisé dans le cadre du cours **Langages de programmation 2 — INFO-F-202** à l'Université libre de Bruxelles, durant l'année académique 2024-2025.

## Aperçu

| Menu principal | Partie |
| --- | --- |
| ![Menu principal](assets/screenshots/start_menu.png) | ![Partie](assets/screenshots/game_level.png) |

| Écran de défaite | Écran de victoire |
| --- | --- |
| ![Défaite](assets/screenshots/lose_menu.png) | ![Victoire](assets/screenshots/win_game_menu.png) |

## Sommaire

- [Fonctionnalités](#fonctionnalités)
- [Prérequis](#prérequis)
- [Installation et compilation](#installation-et-compilation)
- [Lancement et commandes](#lancement-et-commandes)
- [Données et configuration](#données-et-configuration)
- [Organisation du projet](#organisation-du-projet)
- [Documentation et licence](#documentation-et-licence)

## Fonctionnalités

- Neuf niveaux chargés depuis les fichiers `assets/data/level_0.json` à `assets/data/level_8.json`.
- Score, meilleur score et trois vies initiales.
- Briques standard, briques nécessitant deux impacts et briques indestructibles.
- Bonus d'agrandissement de la raquette, d'attrape de la balle, de ralentissement et de vie supplémentaire.
- Déplacement au clavier ou à la souris, avec menus graphiques et effets sonores.
- Architecture Modèle–Vue–Contrôleur, répartie entre `model`, `view` et `controller`.

## Prérequis

- Un compilateur `g++` compatible C++20 ;
- GNU Make ;
- `pkg-config` ;
- Allegro 5 et les modules `allegro_primitives`, `allegro_font`, `allegro_image`, `allegro_ttf`, `allegro_audio` et `allegro_acodec`.

La dépendance JSON est incluse sous la forme de l'en-tête `libs/json.hpp`.

## Installation et compilation

Clonez le dépôt puis lancez la cible par défaut du `Makefile` :

```bash
git clone https://github.com/9Chrk/Arkanoid.git
cd Arkanoid
make
```

La compilation produit l'exécutable `Arkanoid` à la racine du dépôt. La commande `make clean` supprime cet exécutable et le répertoire `obj/`.

## Lancement et commandes

Depuis la racine du projet :

```bash
./Arkanoid
```

| Action | Commande |
| --- | --- |
| Déplacer la raquette à gauche | `←`, `A` ou `Q` |
| Déplacer la raquette à droite | `→`, `D` ou `P` |
| Déplacer la raquette | Mouvement horizontal de la souris |
| Lancer la balle | `Espace` |
| Réinitialiser la partie en cours | `R` |
| Accéder à un niveau | `0` à `8` |
| Quitter | `Échap` ou fermeture de la fenêtre |

## Données et configuration

Les paramètres de jeu et le meilleur score sont enregistrés dans `assets/data/settings.json`. Les niveaux sont définis dans `assets/data/level_*.json` et les ressources graphiques, sonores et typographiques se trouvent dans `assets/`.

Le jeu écrit le meilleur score dans `assets/data/settings.json` à la fin d'un niveau ; exécuter le programme peut donc modifier ce fichier suivi par Git.

## Organisation du projet

```text
.
├── assets/                 ressources, niveaux JSON et captures d'écran
├── docs/                   énoncé et rapport du projet
├── include/                interfaces C++
│   ├── controller/         contrôleur
│   ├── engine/             boucle de jeu
│   ├── model/              logique et entités
│   └── view/               affichage et entrées Allegro
├── libs/                   dépendance JSON incluse
├── src/                    implémentations C++
├── Makefile                règles de compilation
└── README.md
```

## Documentation et licence

- [Énoncé du projet](docs/enonce-project-2024.pdf)
- [Rapport](docs/rapport.pdf)
- [Licence MIT](LICENSE)

Le projet a été réalisé par Jawad Cherkaoui et Rayan Rabeh.
