
#include "windows.h"
#include "winsock.h"

#define CONNECT

main ()
{
int x;
int status;
WORD version;
WSADATA WSAdata;
char buf[200];
LPHOSTENT host;
LPSTR adr;
SOCKET s, s1;
struct sockaddr FAR *addr;
/*struct sockaddr*/ SOCKADDR_IN sa, caller;
int port;
int len;

	printf ("Tapez un nombre : ");
    gets (buf);
	sscanf (buf, "%d", &x);
	printf ("Le carre du nombre est : %d\n", x*x);

	version = 0x101;
    status = WSAStartup (version, &WSAdata);
	if (status)
	{
		printf ("WSAStartup failed %d\n", status);
		return status;
    }

    printWSA (&WSAdata);

#if 0
    printf ("Host ? ");
	gets (buf);
	host = gethostbyname (buf);
	if (!host)
	{
		printf ("Could not get IP address\n");
	}
	else
	{
		adr = inet_ntoa (*(LPIN_ADDR)*(host->h_addr_list));
		printf ("address : <%s>\n", adr);
    }
#endif

	s = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET)
	{
		printf ("Cannot create socket\n");
	}
	printf ("Socket created\n");

	printf ("Port ? ");
	gets (buf);
	port = atoi (buf);

	sa.sin_family = AF_INET;
	sa.sin_port = port;


	buf[0] = 0;
	buf[1] = 0;
	buf[2] = 0;
	buf[3] = 0;

/*
	buf[0] = 1;
	buf[1] = 2;
	buf[2] = 3;
	buf[3] = 4;
*/
/*
	buf[0] = 194;
	buf[1] = 206;
	buf[2] = 12;
	buf[3] = 227;
*/
/*
	buf[0] = 204;
	buf[1] = 71;
	buf[2] = 177;
	buf[3] = 69;
*/
	memcpy (&sa.sin_addr, buf, 4);
	status = bind (s, (LPSOCKADDR)&sa, sizeof(sa));

	if (status)
	{
		printf ("bind failed %d ", status);
	er:
		status = WSAGetLastError ();
		printf ("%d\n", status);
		return status;
	}
	printf ("Bound\n");

#ifdef CONNECT
	/* printf ("Listening...\n"); */
	status = listen (s, 6);
	if (status)
	{
		printf ("listen failed %d ", status);
		goto er;
	}
    printf ("Listening...\n");
	/* printf ("call received\n"); */

    len = sizeof (caller);
	s1 = accept (s, (LPSOCKADDR)&caller, (int FAR *)&len);
	printf ("Call received\n");
	if (s1 == INVALID_SOCKET)
	{
		printf ("accept failed ");
		goto er;
	}
	printf ("Call accepted from %d.%d.%d.%d\n",
		caller.sin_addr.S_un.S_un_b.s_b1,
		caller.sin_addr.S_un.S_un_b.s_b2,
		caller.sin_addr.S_un.S_un_b.s_b3,
		caller.sin_addr.S_un.S_un_b.s_b4);

	status = recv (s1, (char FAR *)buf, sizeof(buf), 0);
#else
	status = recvfrom (s, (char FAR *)buf, sizeof(buf), 0,
		(LPSOCKADDR)&sa, sizeof(sa));
#endif
	printf ("Data received : <%s>\n", buf);

	strcpy (buf, "test server->client");
	status = send (s1, (char FAR *)buf, sizeof(buf), 0);
	printf ("%d bytes sent\n", status);

	closesocket (s1);

    closesocket (s);

	gets (buf);
	status = WSACleanup ();
	if (status)
	{
		printf ("WSACleanup failed %d\n");
		return status;
	}
	return 0;
}
