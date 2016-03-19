# PHP4u/admin MySQL-Dump
# http://www.multimania.fr/general/membre/php4u/admin/
#
# Basé sur PHPMyAdmin : http://phpwizard.net/phpMyAdmin/
# Base de données : teledev_db
# --------------------------------------------------------

#
# Structure de la table 'sites'
#

CREATE TABLE sites (
   numero int(11) NOT NULL auto_increment,
   site varchar(120) NOT NULL,
   texte text NOT NULL,
   motdepasse varchar(30) NOT NULL,
   KEY numero (numero, site),
   KEY motdepasse (motdepasse)
);
