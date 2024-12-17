# LAB2_tftp-client
# Client TFTP

## Objectif
Développer un client TFTP capable de :
- Télécharger un fichier depuis un serveur.
- Téléverser un fichier sur un serveur.

## Fonctionnalités
1. Lecture des arguments en ligne de commande.
2. Résolution de l'adresse avec `getaddrinfo`.
3. Création de sockets UDP.
4. Téléchargement de fichiers (`gettftp`).
5. Téléversement de fichiers (`puttftp`).
6. Gestion de l'option `blocksize` (RFC2348).
7. Gestion des pertes de paquets et retransmissions.

## Commandes
- **Télécharger un fichier :**
   ```bash
   ./gettftp <adresse_du_serveur> <nom_du_fichier>
