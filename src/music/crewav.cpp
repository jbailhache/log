// crewav.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <math.h>

FILE *f;

void memcpy (char *d, char *s, int n)
{
	int i;
	for (i=0; i<n; i++)
		d[i] = s[i];
}

void init ()
{
	char buf[1000];
	f = fopen ("sound.wav", "wb");
	if (f == NULL)
	{
		perror ("sound.wav");
		gets(buf);
		exit(0);
	}
}

void term ()
{
	fclose (f);
}

void save (char *s, int n)
{
	int i;
	for (i=0; i<n; i++)
		fputc (s[i], f);
}

struct wave_header
{
	int riff;
	int len_riff;
	int wave;
	int fmt;
	int len_fmt;
	short x;
	short n_chan;
	int samples_per_sec;
	int bytes_per_sec;
	short bytes_per_sample;
	short bit_per_sample;
	int data;
	int len_data;
};

float fn1 (float t)
{
	return sin (440 * t);
}

float T;

float fn (float t)
{
	float y;
	y = (T-t) * sin (200 * t) + t * sin (800 * t);
	y = y/T * (0.6 + 0.3 * sin(4*t));
	y *= 1.5;
	return y;
}

void crewav ()
{
	char buf[1000];
	struct wave_header header;
	int duree;
	int freq;
	int samples_per_sec;
	int nbytes;
	char x;
	int i;
	float y;

	duree = 10;
	freq = 440;
	samples_per_sec = 0x5622;
	nbytes = duree * samples_per_sec;

	T = duree * (2*3.1415926);

	init ();
	memcpy ((char *)&header.riff, "RIFF", 4);
	memcpy ((char *)&header.wave, "WAVE", 4);
	memcpy ((char *)&header.fmt, "fmt ", 4);
	memcpy ((char *)&header.data, "data", 4);

	header.len_data = nbytes;
	header.len_riff = nbytes + 36;
	header.len_fmt = 16;
	header.x = 1;
	header.n_chan = 1;
	header.samples_per_sec = samples_per_sec;
	header.bytes_per_sec = samples_per_sec;
	header.bytes_per_sample = 1;
	header.bit_per_sample = 8;

	save ((char *)&header, sizeof(header));

	for (i=0; i<nbytes; i++)
	{
		/*x = 0x80 + 0x7F * sin (i * freq / samples_per_sec); */
		y = fn (2*3.1415926*(float)i / (float)samples_per_sec);
		x = 0x80 + 0x7F * y;
		save (&x, 1);
	}

	x = 0;
	save (&x, 1);

	term();
}

int main(int argc, char* argv[])
{
	char buf[1000];
	printf("Hello World!\n");
	printf ("%d\n", sizeof(int));
	crewav();
	gets(buf);
	return 0;
}

