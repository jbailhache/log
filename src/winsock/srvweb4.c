
#include "windows.h"
#include "winsock.h"

#include <stdio.h>

#define BUFSIZE 1024

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

#define PACKET_SIZE 32

int send1 (SOCKET s, char FAR *data, int l, int opt)
{
int i, j, np, status;
	np = (l + PACKET_SIZE - 1) / PACKET_SIZE;
	for (i=0; i<np-1; i++)
	{
		status = send (s, data+i*PACKET_SIZE, PACKET_SIZE, opt);
		for (j=0; j<30000; j++);
	}
	status = send (s, data+(np-1)*PACKET_SIZE, l-(np-1)*PACKET_SIZE, opt);
	return status;
}
 
treat_get1 (char *buf, SOCKET s)
{
char filename[200];
int i;
FILE *f;
static char buf_out[BUFSIZE]; /*512*/
/* char buf_in[512]; */
int c;
int status, status1;

	for (i=0;
		 buf[i+5] && buf[i+5]!=13 && buf[i+5]!=10 && buf[i+5]!=' ';
		 i++)
		filename[i] = buf[i+5];
	filename[i] = 0;
	printf ("filename: <%s>\n", filename);
	f = fopen (filename, "r");
	if (f == NULL)
	{
		sprintf (buf_out, "Cannot open file <%s>\n", filename);
		status = send (s, (char FAR *)buf_out, strlen(buf_out), 0);
		printf ("%d bytes sent\n", status);
		printf (buf_out);
	}
	else
	{
		for (;;)
		{
			status = fread (buf_out, 1, sizeof(buf_out), f);
			printf ("%d bytes read from file\n", status);
			buf_out[status] = 0;
			printf ("<%s>\n", buf_out);
			if (status <= 0)
				break;
			status1 = send (s, (char FAR *)buf_out, status, 0);
			printf ("%d bytes sent\n", status1);
        }
    	fclose (f);
    }
}

get_filename (char *buf, char *filename)
{
int i, j, l;
	l = strlen (buf);
	for (j=0; j<l; j++)
	{
		if (buf[j] == '/')
        	break;
    }
	for (i=0;
		 buf[i+j+1] && buf[i+j+1]!=13 && buf[i+j+1]!=10 && buf[i+j+1]!=' ';
		 i++)
		filename[i] = buf[i+j+1];
	filename[i] = 0;
	printf ("filename: <%s>\n", filename);
}

send_file (char *filename, SOCKET s)
{
FILE *f;
static char buf_out[BUFSIZE]; /*512*/
/* char buf_in[512]; */
int c;
int status, status1;

	f = fopen (filename, "r");
	if (f == NULL)
	{
		sprintf (buf_out, "Cannot open file \"%s\"\n", filename);
		status = send (s, (char FAR *)buf_out, strlen(buf_out), 0);
		printf ("%d bytes sent\n", status);
		printf (buf_out);
	}
	else
	{
		for (;;)
		{
			status = fread (buf_out, 1, sizeof(buf_out), f);
			printf ("%d bytes read from file\n", status);
			buf_out[status] = 0;
			printf ("<%s>\n", buf_out);
			if (status <= 0)
				break;
			status1 = send (s, (char FAR *)buf_out, status, 0);
			printf ("%d bytes sent\n", status1);
        }
    	fclose (f);
    }

}

treat_get (char *buf, SOCKET s)
{
char filename[200];

	get_filename (buf, filename);
	send_file (filename, s);
}

get_input (char *buf, char *input)
{
int i, j, l;
	*input = 0;
	l = strlen (buf);
	for (i=0; i<l; i++)
	{
		if (buf[i] == '=')
		{
			for ( ; i>0 && buf[i]!=10; i--);
			i++;
			for (j=0; buf[i]!=13 && i<l; i++, j++)
            	input[j] = buf[i];
            input[j] = 0;
         	break;
		}
    }
}

treat_post (char *buf, SOCKET s)
{
char filename[200];
int i;
static char input[1000];
static char command[300];
FILE *f;

	get_filename (buf, filename);

	get_input (buf, input);
	printf ("Input : <%s>\n", input);

	f = fopen ("srvweb.in", "w");
	if (f == NULL)
	{
    	printf ("Error opening CGI input file\n");
		return;
	}
	fprintf (f, "%s", input);
	fclose (f);

	sprintf (command, "%s <srvweb.in >srvweb.out", filename);
	printf ("Command : <%s>\n", command);
	/* system (command); */

	send_file ("srvweb.out", s);

}


treat (char *buf, SOCKET s)
{
static char buf_out[BUFSIZE/*600*/];
int status;
int l, l1;
FILE *f;
	if (!strncmp (buf, "GET /", 5))
		treat_get (buf, s);
	else if (!strncmp (buf, "POST /", 6))
    	treat_post (buf, s);
	else
	{
		sprintf (buf_out, "Unknown request : (%s)\n", buf);
		/* sprintf (buf_out, "<HTML><HEAD><TITLE>Unknown request</TITLE></HEAD><BODY>Unknown request : (%s)\n</BODY></HTML>", buf); */
		printf (buf_out);
		f = fopen ("srvweb.log", "w");
		if (f != NULL)
		{
        	fprintf (f, "%s", buf_out);
        	fclose (f);
        }
		l1 = l = strlen(buf_out);
		/* if (l > 60)
			l = 60; */
		status = send (s, (char FAR *)buf_out, l, 0);
		printf ("%d/%d/%d bytes sent\n", status, l, l1);
		/* printf (buf_out); */
		status = WSAGetLastError ();
		printf ("Error %d\n", status);

	}
}

main ()
{
int x;
int status;
WORD version;
WSADATA WSAdata;
static char buf[BUFSIZE/*500*/];
LPHOSTENT host;
LPSTR adr;
SOCKET s, s1;
struct sockaddr FAR *addr;
/*struct sockaddr*/ SOCKADDR_IN sa, caller;
int port;
int len;

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

	/*
	printf ("Port ? ");
	gets (buf);
	port = atoi (buf);
	*/
	port = htons (80);

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
		/* return status; */
	}
	printf ("Bound\n");

	/* printf ("Listening...\n"); */
/*loop:*/
    printf ("Listen\n");
	status = listen (s, 6);
	if (status)
	{
		printf ("listen failed %d ", status);
		goto er;
	}
loop:
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
    if (status >= 0 && status < sizeof(buf))
		buf[status] = 0;
	else
		printf ("recv: bad count %d\n", status);

	printf ("Data received : <%s>\n", buf);

	/* strcpy (buf, "test server->client"); */
	/*
	status = send (s1, (char FAR *)buf, sizeof(buf), 0);
	printf ("%d bytes sent\n", status);
	*/
	treat (buf, s1);

	closesocket (s1);

	goto loop;

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
