# PHP4u/admin MySQL-Dump
# http://www.multimania.lycos.fr/general/membre/php4u/admin/
#
# Basé sur PHPMyAdmin : http://phpwizard.net/phpMyAdmin/
# Base de données : teledev_db
# --------------------------------------------------------

#
# Structure de la table 'affinite'
#

CREATE TABLE affinite (
   numero int(11) NOT NULL auto_increment,
   type char(1) NOT NULL,
   but varchar(60) NOT NULL,
   nom varchar(60) NOT NULL,
   pseudo varchar(60) NOT NULL,
   categorie varchar(60) NOT NULL,
   caractere varchar(60) NOT NULL,
   valeur float DEFAULT '0' NOT NULL,
   action varchar(30) NOT NULL,
   profil varchar(60) NOT NULL,
   reponse varchar(60) NOT NULL,
   KEY numero (numero, type, but, nom, pseudo, categorie, caractere, valeur),
   KEY action (action),
   KEY profil (profil),
   KEY reponse (reponse)
);
