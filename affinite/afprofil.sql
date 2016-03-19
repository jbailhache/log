# PHP4u/admin MySQL-Dump
# http://www.multimania.lycos.fr/general/membre/php4u/admin/
#
# Basé sur PHPMyAdmin : http://phpwizard.net/phpMyAdmin/
# Base de données : teledev_db
# --------------------------------------------------------

#
# Structure de la table 'afprofil'
#

CREATE TABLE afprofil (
   numero int(11) NOT NULL auto_increment,
   profil varchar(60) NOT NULL,
   pseudo varchar(60) NOT NULL,
   action varchar(30) NOT NULL,
   x tinyint(4) DEFAULT '0' NOT NULL,
   KEY numero (numero, profil, pseudo, action, x)
);
