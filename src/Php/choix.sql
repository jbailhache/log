# phpMyAdmin MySQL-Dump
# version 2.2.0rc4
# http://phpwizard.net/phpMyAdmin/
# http://phpmyadmin.sourceforge.net/ (download page)
#
# Serveur: localhost
# Généré le : May 4, 2002, 1:02 am
# Version du serveur: 3.23.40
# Version de PHP: 4.0.6
# Base de données: `teledev`
# --------------------------------------------------------

#
# Structure de la table `choix`
#

CREATE TABLE choix (
  numero int(11) NOT NULL auto_increment,
  site varchar(120) NOT NULL default '',
  objet varchar(120) NOT NULL default '',
  ident varchar(120) NOT NULL default '',
  points double NOT NULL default '0',
  valeur text NOT NULL,
  PRIMARY KEY  (numero)
) TYPE=MyISAM;

#
# Contenu de la table `choix`
#

INSERT INTO choix VALUES (1,'exchoix','titre','titre1','100','Premier titre');
INSERT INTO choix VALUES (2,'exchoix','titre','titre2','140','Deuxième titre');
INSERT INTO choix VALUES (3,'exchoix','titre','titre3','120','Troisième titre');

    
