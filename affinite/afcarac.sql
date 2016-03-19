# PHP4u/admin MySQL-Dump
# http://www.multimania.lycos.fr/general/membre/php4u/admin/
#
# Basé sur PHPMyAdmin : http://phpwizard.net/phpMyAdmin/
# Base de données : teledev_db
# --------------------------------------------------------

#
# Structure de la table 'afcarac'
#

CREATE TABLE afcarac (
   categorie varchar(60) NOT NULL,
   caractere varchar(60) NOT NULL,
   x1 tinyint(4) DEFAULT '0' NOT NULL,
   KEY numero (categorie, caractere),
   KEY x1 (x1)
);
