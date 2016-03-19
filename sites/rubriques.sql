# PHP4u/admin MySQL-Dump
# http://www.multimania.fr/general/membre/php4u/admin/
#
# Basé sur PHPMyAdmin : http://phpwizard.net/phpMyAdmin/
# Base de données : teledev_db
# --------------------------------------------------------

#
# Structure de la table 'rubriques'
#

CREATE TABLE rubriques (
   numero int(11) NOT NULL auto_increment,
   site varchar(120) NOT NULL,
   rubrique varchar(120) NOT NULL,
   position float DEFAULT '0' NOT NULL,
   KEY numero (numero, site, rubrique, position)
);
