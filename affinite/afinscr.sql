# PHP4u/admin MySQL-Dump
# http://www.multimania.lycos.fr/general/membre/php4u/admin/
#
# Basé sur PHPMyAdmin : http://phpwizard.net/phpMyAdmin/
# Base de données : teledev_db
# --------------------------------------------------------

#
# Structure de la table 'afinscr'
#

CREATE TABLE afinscr (
   numero int(11) NOT NULL auto_increment,
   pseudo varchar(30) NOT NULL,
   prenom varchar(30) NOT NULL,
   nom varchar(40) NOT NULL,
   adresse varchar(60) NOT NULL,
   telephone varchar(60) NOT NULL,
   email varchar(60) NOT NULL,
   web varchar(60) NOT NULL,
   photo varchar(60) NOT NULL,
   x1 tinyint(4) DEFAULT '0' NOT NULL,
   x2 tinyint(4) DEFAULT '0' NOT NULL,
   x3 tinyint(4) DEFAULT '0' NOT NULL,
   x4 tinyint(4) DEFAULT '0' NOT NULL,
   autre varchar(250) NOT NULL,
   motdepasse varchar(30) NOT NULL,
   PRIMARY KEY (numero),
   KEY numero (numero, pseudo, prenom, nom, adresse, telephone, email, web, photo, x1, x2, x3, x4),
   KEY autre (autre),
   KEY motdepasse (motdepasse)
);
