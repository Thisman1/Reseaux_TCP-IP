# Reseaux_TCP-IP
**Ce projet a pour but de pouvoir programmer des applications basées sur le modèle client/serveur 
utilisant le protocole TCP/IP qui est la brique de base d’Internet.**   
Un protocole en réseaux est un ensemble de règles nécessaires pour la communication entre 
deux machines (ordinateurs, objet en IoT, téléphone, etc.). Pour envoyer un message entre deux 
machines, plusieurs fonctionnalités sont nécessaires et donc cet envoi est divisé en étapes 
comme appellent « couches » pour faciliter leur implémentation et pour avoir une architecture 
modulaire: de la couche 7 (couche de l’application) à la couche 1 (couche physique responsable 
de la transmission du flux binaire sur le médium). L’objet de chaque couche est d’offrir des 
services spécifiques aux couches plus hautes. De plus, la couche N d’une machine gère la 
conversation avec la couche N d’une autre machine en utilisant un protocole de niveau N. L’objet 
de ce projet est de ce focaliser sur la couche 4 (la couche transport) responsable d’assurer la 
division du message en « morceaux » appelés segments et de gérer la transmission de ces 
segments entre la source et le destinataire.   

Pour cela, on a besoin de la notion de port (les numéros de ports servent à différencier les 
conversations sur une même machine) et d’adresse IP. En effet, les segments sont eux mêmes 
divisés en morceaux plus petits appelés paquets et acheminés de manière indépendante jusqu’à 
la destination et pour atteindre leur destination ils ont besoin qu’on affecte aux machines 
communicantes une adresse réseaux: l’adresse IP.   

Le détail du transfert effectif des données entre deux applications est spécifié par le protocole de 
de la couche transport, mais comment et quand les applications interagissent entre elles sont 
laissés à la charge du programmeur. Quand le protocole utilisé est de la famille TCP/IP, la 
méthode phare pour réaliser cela se base sur l’architecture client/serveur. En fait, TCP/IP ne 
fournit aucun mécanisme permettant l’exécution automatique d’un programme à l’arrivée d’un 
message. Par conséquent, dans une communication point à point, l’une des applications prend 
l’initiative de la communication (serveur) de la part de l’autre application (client).   
Pour cela, la première phase du projet est de créer dans le langage C des sockets (avec la 
libraires #include <sys/socket.h>): client et serveur. Dans la deuxième phase, on créera des 
applications orientées connexion et sans connexion.
