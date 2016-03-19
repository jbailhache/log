
#include "windows.h"
#include "winsock.h"

char bigbuf[5000];

printWSA (WSADATA *WSAdata)
{
	printf ("WSA data :\n");
	printf ("\tversion %x\n", WSAdata->wVersion);
	printf ("\thighest %x\n", WSAdata->wHighVersion);
	printf ("\tdescription <%s>\n", WSAdata->szDescription);
	printf ("\tstatus <%s>\n", WSAdata->szSystemStatus);
	printf ("\tmax sockets %d\n", WSAdata->iMaxSockets);
	printf ("\tmax datagram size %d\n", WSAdata->iMaxUdpDg);
}

main ()
{
int x;
int status;
WORD version;
WSADATA WSAdata;
char buf[200 /*5000*/];
LPHOSTENT host;
LPSTR adr;
SOCKET s;
struct sockaddr FAR *addr;
/*struct sockaddr*/ SOCKADDR_IN sa;
int port;
LPPROTOENT proto;
int i;
char filename[120];
char buf1[80];

	version = 0x101;
    status = WSAStartup (version, &WSAdata);
	if (status)
	{
		printf ("WSAStartup failed %d\n", status);
		return status;
    }

    printWSA (&WSAdata);
 
	s = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET)
	{
		printf ("Cannot create socket\n");
	}
	printf ("Socket created\n");

	printf ("Port [80] ? ");
	gets (buf);
    if (*buf)
		port = htons (atoi (buf));
	else
		port = htons (80);

	sa.sin_family = AF_INET;
	sa.sin_port = port;
    /*
 	buf[0] = 194;
	buf[1] = 2;
	buf[2] = 222;
    buf[3] = 12;
	*/
	printf ("Server address : \n");
	for (i=0; i<4; i++)
	{
		printf ("\tpart %d ? ", i+1);
		gets (buf1);
        buf[i] = atoi (buf1);
    }
 	memcpy (&sa.sin_addr, buf, 4);

	status = connect (s, (LPSOCKADDR)&sa, sizeof(sa));

	if (status)
	{
		printf ("Connect failed %d ", status);
		status = WSAGetLastError ();
		printf ("%d\n", status);
		return status;
	}
	printf ("Connected\n");

	/*
	printf ("Data to send ? ");
	gets (buf);

	i = strlen (buf);
	buf[i] = 13;
	buf[i+1] = 10;
	buf[i+2] = 0;

	*/

	printf ("File to receive ? ");
	gets (filename);
	sprintf (buf, "GET /%s\r\n", filename);

	status = send (s, (char FAR *)buf, /*sizeof(buf)*/ strlen(buf), 0);
 
	printf ("%d bytes sent\n", status);
	printf ("Data sent : <%s>\n", buf);

    printf ("Data received : \n");
loop:
	status = recv (s, (char FAR *)bigbuf, sizeof(bigbuf), 0);
    if (status > 0 && status < sizeof(bigbuf))
	{
		bigbuf[status] = 0;
		/* printf ("Data received : <%s>\n", bigbuf); */
		printf ("%s", bigbuf);
		goto loop;
	}
	else
	{
		/* printf ("recv failed %d\n", status); */
		printf ("\nDone.\n");
	}

    /*
    printf ("Data received : \n");
	for (i=0; i<200; i++)
	{
		status = recv (s, (char FAR *)buf, 1, 0);
        printf ("%c", buf[0]);
	}
	*/
    closesocket (s);

	/* gets (buf); */
	status = WSACleanup ();
	if (status)
	{
		printf ("WSACleanup failed %d\n");
		return status;
	}
	return 0;
}
