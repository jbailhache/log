
typedef unsigned char uchar;
typedef unsigned int uint;

int opt, count, rounds;

#define GENOME_SIZE 9

#define G_SEA 1
#define G_NET 2
#define G_LAND 4
#define G_FENCE 8
#define G_SEASIDE 0x10
#define G_DESTROY 0x20
#define G_CARNIVORE 0x40
#define G_COMESTIBLE 0x80

struct MAP
{
	int greble, genome, food, tera;
};

struct MEN
{
	int x, y, wood, boat;
};



