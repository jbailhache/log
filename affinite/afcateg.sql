# PHP4u/admin MySQL-Dump
# http://www.multimania.lycos.fr/general/membre/php4u/admin/
#
# Basé sur PHPMyAdmin : http://phpwizard.net/phpMyAdmin/
# Base de données : teledev_db
# --------------------------------------------------------

#
# Structure de la table 'afcateg'
#

CREATE TABLE afcateg (
   numero int(11) NOT NULL auto_increment,
   categorie varchar(60) NOT NULL,
   x1 tinyint(4) DEFAULT '0' NOT NULL,
   position double DEFAULT '0' NOT NULL,
   KEY numero (numero, categorie, x1),
   KEY position (position)
);
