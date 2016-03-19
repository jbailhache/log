 
// D'après G. Saporta, Probabilités, analyse de données et statistiques, ed. Technip

variables = ["PAO", "PAA", "VIO", "VIA", "POT", "LEC", "RAI", "PLP"]
individus = ["AGRI", "SAAG", "PRIN", "CSUP", "CMOY", "EMPL", "OUVR", "INAC", "XXXX"]
  
X = [ 	167	1	163	23	41	8	6	6	;
	162	2	141	12	40	12	4	15	;
	119	6	69	56	39	5	13	41	;
	87	11	63	111	27	3	18	39	;
	103	5	68	77	32	4	11	30	;
	111	4	72	66	34	6	10	28	;
	130	3	76	52	43	7	7	16	;
	138	7	117	74	53	8	12	20	;
	120	8	110	40	45	9	10	26	]

[valpro, u, c, r] = acp (X, variables, individus)

 
 