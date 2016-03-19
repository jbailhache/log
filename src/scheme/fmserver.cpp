
#include "stdafx.h"

#include <afxdb.h>

#include <winsock.h>

#include "RecordsetClients.h"
#include "RecordsetRec.h"

struct fmparam
{
	int port;
	int taille_blocs;
	char base[100];
	char table[100];
	char login[100];
	char pass[100];
	char credits[100];
	char bloques[100];
};

void init_param (struct fmparam *param)
{
	param->port = 6020;
	param->taille_blocs = 512;
	strcpy (param->base, "droits");
	strcpy (param->table, "clients");
	strcpy (param->login, "login");
	strcpy (param->pass, "pass");
	strcpy (param->credits, "credits");
	strcpy (param->bloques, "bloques");

}

int status;

void input (SOCKET s, char *buf)
{
	char c;
	/*int status;*/
	int i;

	i = 0;
	for (;;)
	{
		status = recv (s, &c, 1, 0);
		if (status == SOCKET_ERROR || c == 10 || c == 13)
		{
			buf[i] = 0;
			break;
		}
		else
			buf[i++] = c;
	}

}

void output (SOCKET s, char *buf)
{
int status;
	status = send (s, buf, strlen(buf), 0);
}

void lire_fichier (char *dir, char *file, char *buf)
{
	FILE *f;
	char path[200];
	sprintf (path, "%s\\%s", dir, file);
	f = fopen (path, "r");
	fgets (buf, 100, f);
	if (buf[strlen(buf)-1] == 10 ||buf[strlen(buf)-1] == 13)
		buf[strlen(buf)-1]=0;
	fclose(f);
}

void fmserver (void)
{
	struct fmparam param[1];

	// MessageBox (NULL, "Hello", "Message", MB_OK);

	init_param(param);
	
	/* test

	CDatabase db;
	db.OpenEx (_T("DSN=droits"), 0);

	CRecordsetClients rs;

	rs.Open (AFX_DB_USE_DEFAULT_TYPE, "SELECT * FROM clients", 0);
	rs.MoveFirst();
	for (;;)
	{
		if (rs.IsEOF())
			break;
		MessageBox (NULL, rs.m_login, "Login", MB_OK);
		rs.MoveNext();
	}

	rs.Close();

	*/

	WSADATA wsda;
	SOCKET sListen, sClient;
	struct hostent * hp;
	char buf[10000];
	int len;
	SOCKADDR_IN addr, remote_addr;
	int ret;
	int iAddrLen;

	WSAStartup (MAKEWORD(1,1), &wsda);

	hp = gethostbyname ("localhost");

	/*sListen = socket (hp->h_addrtype, SOCK_STREAM, 0);*/

	sListen = socket (AF_INET, SOCK_STREAM, 0 /*IPPROTO_IP*/);
	if (sListen == SOCKET_ERROR)
	{
		MessageBox (NULL, "Error creating socket", "Error", MB_OK);
		exit(0);
	}

	memset (&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(6030);
	addr.sin_addr.s_addr = htonl (INADDR_ANY);

	ret = bind (sListen, (struct sockaddr *)&addr, sizeof(addr));
	if (ret == SOCKET_ERROR)
	{
		MessageBox (NULL, "Error bind", "Error", MB_OK);
		exit(0);
	}

	ret = listen (sListen, 10);
	if (ret == SOCKET_ERROR)
	{
		sprintf (buf, "Error listen: %d", WSAGetLastError());		
		MessageBox (NULL, buf, "Error listen", MB_OK);
		exit(0);
	}

	for (;;)
	{

	iAddrLen = sizeof(remote_addr);

	sClient = accept (sListen, (struct sockaddr *) &remote_addr, &iAddrLen);
	if (sClient == SOCKET_ERROR)
	{
		MessageBox (NULL, "Error accept", "Error", MB_OK);
		exit(0);
	}

	// MessageBox (NULL, "Accepted", "server", MB_OK);

	/*
	ret = recv (sClient, buf, sizeof(buf), 0);
	if (ret == SOCKET_ERROR)
	{
		MessageBox (NULL, "Error recv", "Error", MB_OK);
		exit(0);
	}
	buf[ret] = 0;

	MessageBox (NULL, buf, "Received", MB_OK);
	*/

	/*
	input (sClient, buf);
	MessageBox (NULL, buf, "received", MB_OK);
	*/

	char dirfax[100];
	char basedroits[100];
	char basefax[100];
	char faxdir[100];
	char language[100];
	char login[100];
	char pass[100];

	strcpy (basedroits, "droits");
	strcpy (basefax, "AT05");
	strcpy (faxdir, "L:\\AT05");


	for (;;)
	{
		input (sClient, buf);
		if (status == SOCKET_ERROR)
			break;

		// MessageBox (NULL, buf, "commande", MB_OK);
	
		if (!strcmp (buf, "BASEFAX"))
		{
			input (sClient, dirfax);
			input (sClient, language);
			
			if (*dirfax)
			{
				lire_fichier (dirfax, "baserights.txt", basedroits);
				lire_fichier (dirfax, "basefax.txt", basefax);
				lire_fichier (dirfax, "faxdir.txt", faxdir);
			}

			output (sClient, "ID\tIdentifiant\t30\n");
			output (sClient, "CNX\tDate\t20\n");
			output (sClient, "TIME\tDurée\t5\n");
			output (sClient, "PGENB\tPages\t5\n");
			output (sClient, "HGN\tStatus\t5\n");
			output (sClient, "FILE\tFichier\t15\n");
			output (sClient, "\n");
		 
		}
		else if (!strcmp (buf, "USER"))
		{
			input (sClient, login);
		}
		else if (!strcmp (buf, "PASS"))
		{
			input (sClient, pass);
			output (sClient, "LOGIN\n1\n");
		}
		
		else if (!strcmp (buf, "LISTREC"))
		{
			
			char param[100];
			char strsince[100];
			input (sClient, param);
			input (sClient, strsince);

			CDatabase db;
			char dsn[100];
			sprintf (dsn, "DSN=%s", basefax);
			db.OpenEx (_T(dsn), 0);

			CRecordsetRec rs;

			rs.Open (AFX_DB_USE_DEFAULT_TYPE, "SELECT * FROM REC", 0);
			rs.MoveFirst();

			// MessageBox (NULL, "Reading base", "trace", MB_OK);

			for (;;)
			{
				if (rs.IsEOF())
					break;
				/*MessageBox (NULL, rs.m_ID, "id", MB_OK);*/
				sprintf (buf, 
 "<MESSAGE><BASE>FAXBT</BASE><ID>%s</ID><CNX>%s</CNX><TIME>%lf</TIME><PGENB>%lf</PGENB><HGN>%lf</HGN><FILE>%s</FILE></MESSAGE>\n",
								LPCSTR(rs.m_ID), 
								LPCSTR(rs.m_CNX), 
								rs.m_TIME, 
								rs.m_PGENB, 
								rs.m_HGN,
								LPCSTR(rs.m_FILE));
				/*MessageBox (NULL, buf, "Message", MB_OK);*/
				output (sClient, buf);
				rs.MoveNext();
			}
			output (sClient, "\n");

			rs.Close();



		}
		else if (!strcmp (buf, "END"))
		{
			// MessageBox (NULL, buf, "END", MB_OK);
			break;
		}
		else
		{
			// MessageBox (NULL, buf, "autre commande", MB_OK);
		}


	}

	closesocket (sClient);

	}
/*
	for (;;)
	{
		input (commande);
		MessageBox (NULL, commande, "Commande", MB_OK);

	}
*/

}
