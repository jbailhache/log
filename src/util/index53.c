
/* Indexed files */

/*
#define VAX_C
*/
#define NEW
#define TRACE

#include <stdio.h>
#include <errno.h>

main () { test_index(); }

typedef int item;   /* record or offset of node */
/* typedef int key; */    /* record */
typedef int record; /* offset of record in data file */
typedef FILE *data_file;
typedef FILE *index_file;
#ifdef VAX_C
typedef int (*order) (void *r1, void *r2);
#else
typedef int (*order) ();
#endif

#define NODE_SIZE 4

#define NO_RECORD (-1)
#define NO_NODE (-1)
#define ERROR (-1)
#define ALLRIGHT 0

#define LT (-1)
#define EQ 0
#define GT 1

#ifdef VAX_C
typedef int (*rw_function) (void *, int, int, FILE *);
#else
typedef size_t (*rw_function) ();
#endif

FILE *_index, *_data;

struct index_node
{
    char level; /* 0 = data */
    char n_items, pos, align;
    item up, previous, next;
    int ext;
    item items[NODE_SIZE];
    /*key*/ item keys[NODE_SIZE];
};

struct index_header
{
    int nodesize;
    item root, first, last, current, free;
    int current_item;
    record current_record, free_record;
    int n_records, n_nodes;
    int record_size;
    int ext[6];
};

/* Structure of files :
 *   - data : records
 *   - index : header + nodes
 */

break_debug () { ; }

#if 0
#define GET_HEADER \
    status = fseek (index, SEEK_SET, 0); \
    if (status) \
        return errno; \
    status = fread (&header, sizeof(header), 1, index); \
    if (status != 1) \
        return errno;
#endif

#define test_status(x) {int status; status=x; if (status) \
{ \
        perror("error"); \
        return -1; \
} }

#define GET_HEADER \
        test_status (file_access(index,0,sizeof(header),&header,fread));

#define PUT_HEADER \
        test_status (file_access(index,0,sizeof(header),&header,fwrite);)

int file_access (FILE *f, int offset, int size, void *buf,
#ifdef VAX_C
        int (*function) (void *, int, int, FILE *)
#else
        /* size_t (*function) (void *, size_t, size_t, FILE *) */
        size_t (*function) ()
#endif
        )
{
int status;
        status = fseek (f, offset, SEEK_SET);
        if (status) return ERROR;
        status = (*function) (buf, size, 1, f);
        if (status != 1) return ERROR;
        status = fflush (f);
        return ALLRIGHT;
}

/* #define TEST_INDEX test_index_(); */

#define TEST_INDEX

test_index_ ()
{
struct index_header header;
FILE *index;
int offset;
int status;
        index = _index;
        offset = ftell(index);
        GET_HEADER
        if (header.root == 0x10000)
                printf ("Bad root\n");
        status = fseek (index, offset, SEEK_SET);
        if (status)
        {
            perror("fseek in test_index_");
            return errno;
        }
        return 0;
}

record get_current (index_file index)
{
struct index_header header;
int status;
struct index_node node;
        GET_HEADER
#ifdef TRACE
        printf ("get_current: header.current = %d = 0x%X (offset node)\n", header.current, header.current);
        printf ("get_current: header.current_item = %d\n", header.current_item);
#endif
        test_status (file_access (index, header.current, sizeof(node),
                        &node, fread));
#ifdef TRACE
        printf ("node.items[%d] = ", header.current_item);
        printf (" %d\n", node.items[header.current_item]);
#endif
        header.current_record = node.items[header.current_item];
#ifdef TRACE
        printf ("header.current_record = %d = 0x%X\n",
                header.current_record, header.current_record);
#endif
        PUT_HEADER
#ifdef TRACE
        printf ("get_current -> %d 0x%X\n", 
            header.current_record, header.current_record);
#endif
        return header.current_record;
}

int get_n_records (index_file index)
{
struct index_header header;
    GET_HEADER
    return header.n_records;
}

record get_first (index_file index)
{
struct index_header header;
int status;
        GET_HEADER
        if (header.n_records <= 0) return NO_RECORD;
        header.current = header.first;
        header.current_item = 0;
        /* test_status (get_current (index)); */
        PUT_HEADER
        get_current (index);
        /* PUT_HEADER */
        return header.current_record;
}

record get_last (index_file index)
{
struct index_header header;
int status;
struct index_node node;
item ofsrec;
        GET_HEADER
#ifdef TRACE
        printf ("last = %d = 0x%X\n", header.last, header.last);
#endif
        if (header.n_records == 0) 
        {
#ifdef TRACE
            printf ("get_last: empty file\n");
#endif
            return NO_RECORD;
        }
        header.current = header.last;
        test_status (file_access (index, header.last, sizeof(node), &node,
                        fread))
#ifdef TRACE
        printf ("n_items = %d\n", node.n_items);
#endif
        header.current_item = node.n_items-1;
        PUT_HEADER
        /* test_status (get_current (index)); */
        ofsrec = get_current (index);
        /* PUT_HEADER */
#ifdef TRACE
        printf ("get_last -> %d 0x%X\n", 
            ofsrec, ofsrec);
/*          header.current_record, header.current_record);*/
#endif
        return ofsrec; /* header.current_record;*/
}

record get_next (index_file index)
{
struct index_header header;
int status;
struct index_node node;
        GET_HEADER
        test_status (file_access (index, header.current, sizeof(node), &node,
                        fread));
        if (header.current_item < node.n_items-1)
        {
                header.current_item++;
                header.current_record = node.items[header.current_item];
                PUT_HEADER
                return header.current_record;
        }
        else
        {
                header.current_item = 0;
                if (node.next == NO_NODE) return /*ERROR*/ NO_RECORD;
                header.current = node.next;
                PUT_HEADER
#ifdef TRACE
                printf ("get_next: current = %d ", header.current);
                printf (" (%d)\n", header.current_item);
#endif
                return get_current(index);
        }
}

record get_previous (index_file index)
{
struct index_header header;
int status;
struct index_node node, node_p;
        GET_HEADER
        test_status (file_access (index, header.current, sizeof(node), &node,
                        fread));
        if (header.current_item > 0)
        {
                header.current_item--;
                header.current_record = node.items[header.current_item];
                PUT_HEADER
                return header.current_record;
        }
        else
        {
                if (node.previous == NO_NODE) return /*ERROR*/ NO_RECORD;
                header.current = node.previous;
                /* header.current_item = node.n_items-1; */
                test_status (file_access (index, node.previous,
                                sizeof(node_p), &node_p, fread));
                header.current_item = node_p.n_items - 1;
                PUT_HEADER
                return get_current(index);
        }
}

int get_record (index_file index, data_file data, /* record *rec, */ void *buf,
                        order ord, void *param, int flag_last)
{
int status;
struct index_header header;
struct index_node node;
item ofsnod;
int i;
char *buf_data;
int o, o1, o_up;

        GET_HEADER
#ifdef TRACE
        printf ("get_record: root = %d = 0x%X\n", header.root);
        TEST_INDEX
#endif
        ofsnod = header.root;
        buf_data = malloc (header.record_size);
loop:
#ifdef TRACE
        printf ("get_record loop: ofsnod = %d = 0x%X\n", ofsnod, ofsnod);
        TEST_INDEX
#endif
        test_status (file_access (index, ofsnod, sizeof(node), &node, fread));
#ifdef TRACE
        printf ("level %d\n", node.level);
        TEST_INDEX
#endif

/*      if (node.level == 0) */
                i = -1 /* 0 */;
/*
        else
                i = 0;
*/
        if (node.n_items == 0)
        {
            header.current = ofsnod;
            header.current_item = -1;
            header.current_record = NO_RECORD;
            PUT_HEADER
            return -2;
        }
        o = 1;
loop_item:
#if 0
        printf ("loop_item : i = %d\n", i);
#endif
        if (i < node.n_items-1)
        {
                o1 = o;
            if (flag_last > 0 || o != EQ)
            {
                test_status (file_access (data, node.keys[i+1],
                        header.record_size, buf_data, fread));
                /* o1 = o; */
		o = (*ord) (buf, buf_data
#ifdef NEW
					 , param
#endif
						);
/*
                if (LT != (o = (*ord)(buf,  buf_data)))
*/
                if (o == GT || ((flag_last || node.level == 0) && o == EQ))
                {
                        i++;
                        goto loop_item;
                }
            }
        }
        else
                o1 = o;

#ifdef TRACE
        printf ("i = %d, level = %d\n", i, node.level);
        TEST_INDEX
#endif
        if (node.level == 0)
        {
                if (/* flag_last && */ o1 == GT && node.next != NO_NODE &&
                        o_up == EQ)
                {
                        ofsnod = node.next;
                        goto loop;
                }
                header.current = ofsnod;
                header.current_item = i;
                if (i == -1)
                        header.current_record = NO_RECORD;
                else
                        header.current_record = node.items[i];
                PUT_HEADER
                return o1;
        }
        if (i == -1)
            ofsnod = node.items[0];
        else
            ofsnod = node.items[i];
        o_up = o;
        goto loop;
}

#define read_current(i,d,b) treat_current(i,d,b,fread)
#define write_current(i,d,b) treat_current(i,d,b,fwrite)

int treat_current (index_file index, data_file data, void *buf,
#ifdef VAX_C
        int (*function) (void *, int, int, data_file)
#else
        /* size_t (*function) (void *, size_t, size_t, data_file) */
        size_t (*function) ()
#endif
        )
{
int status;
struct index_header header;
        GET_HEADER
        return file_access (data, header.current_record, header.record_size,
                buf, function);
}

record add_record (index_file index, void *buf, order ord, void *param)
{

}

#define header (*p_header)
item get_free_node (index_file index, struct index_header header)
{
item ofsnod_r;
int status;

            if (header.free == NO_NODE)
            {
                status = fseek (index, 0, SEEK_END);
                if (status) return ERROR;       
                ofsnod_r = ftell(index);
#ifdef TRACE
                printf ("get_free_node: ofsnod_r = %d = 0x%X\n",
                        ofsnod_r, ofsnod_r);
#endif
            }
            else
            {
                ofsnod_r = header.free;
                test_status (file_access (index, ofsnod_r , sizeof(header.free),
                                &header.free, fread));
                status = fseek (index, ofsnod_r, SEEK_SET);
                if (status) return ERROR;
            }
            return ofsnod_r;
}
#undef header

item add_item (index_file index, item ofsnod, int pos, item x, record key,
    int *nod, int *itm)
{
int status;
struct index_header header;
struct index_node node, node_r, node_d, node_u, node_next;
int i;
item ofsnod_r, ofsnod_u;
int itm1, nod1;

#ifdef TRACE
        printf ("add_item ofsnod=%d=0x%X pos=%d item=%d=0x%X key=%d=0x%X\n",
            ofsnod, ofsnod, pos, x, x, key, key);
        TEST_INDEX
#endif
        test_status (file_access (index, ofsnod, sizeof(node),
                        &node, fread));
#ifdef TRACE
        printf ("level = %d, %d items\n", node.level, node.n_items);
        TEST_INDEX
#endif
        if (node.n_items >= NODE_SIZE)
        {
            node_r.level = node.level;
            node.n_items = NODE_SIZE / 2;
            node_r.n_items = NODE_SIZE - node.n_items;
            node_r.up = node.up;
            node_r.previous = ofsnod;
            node_r.next = node.next;
            if (node.pos < NODE_SIZE - 1)
                node_r.pos = node.pos + 1;
            else
                node_r.pos = NODE_SIZE/2;

            GET_HEADER
            ofsnod_r = get_free_node (index, &header);
#if 0
            if (header.free == NO_NODE)
            {
                status = fseek (index, 0, SEEK_END);
                if (status) return ERROR;
                ofsnod_r = ftell(index);
            }
            else
            {
                ofsnod_r = header.free;
                test_status (file_access (index, ofsnod_r, sizeof(header.free),
                                &header.free, fread));
                status = fseek (index, ofsnod_r, SEEK_SET);
                if (status) return ERROR;
            }
#endif

            if (node.next == NO_NODE)
            {
                /* GET_HEADER */
                header.last = ofsnod_r;
                /* PUT_HEADER */
            }
            PUT_HEADER

#ifdef TRACE
            printf ("ofsnod_r before = %d = 0x%X\n", ofsnod_r, ofsnod_r);
            TEST_INDEX
#endif
            if (node.next != NO_NODE)
            {
#ifdef TRACE
                printf ("node.next = %d = 0x%X\n", node.next, node.next);
#endif
                test_status (file_access (index, node.next, sizeof(node_next),
                                &node_next, fread));
                node_next.previous = ofsnod_r;
                test_status (file_access (index, node.next, sizeof(node_next),
                                &node_next, fwrite));
            }
            for (i=0; i<node_r.n_items; i++)
            {
                node_r.items[i] = node.items[i+NODE_SIZE/2];
                if (node.level > 0)
                {
#ifdef TRACE
                    printf ("Reading node_d: node_r.items[%d] = %d = 0x%X\n",
                            i, node_r.items[i], node_r.items[i]);
#endif
                    test_status (file_access (index, node_r.items[i],
                        sizeof(node_d), &node_d, fread));
                    node_d.pos = i;
                    node_d.up = ofsnod_r;
#ifdef TRACE
                    printf ("Writing node_d: node_r.items[%d] = %d = 0x%X\n",
                            i, node_r.items[i], node_r.items[i]);
#endif
                    test_status (file_access (index, node_r.items[i],
                        sizeof(node_d), &node_d, fwrite));
                }
            }
/*
            for (i=0; i<node_r.n_items-1; i++)
                node_r.keys[i] = node.keys[i+NODE_SIZE/2];
*/
            for (i=0; i<node_r.n_items; i++)
                node_r.keys[i] = node.keys[i+NODE_SIZE/2];

            status = fseek (index, ofsnod_r, SEEK_SET);
            if (status) return ERROR;
#if 0
            GET_HEADER
            ofsnod_r = get_free_node (index, &header);
#ifdef TRACE
            printf ("ofsnod_r: get_free_node -> %d = 0x%X\n",
                    ofsnod_r, ofsnod_r);
#endif
            /* PUT_HEADER */
#if 0
            status = fseek (index, 0, SEEK_END);
            if (status)
            {
#ifdef TRACE
                perror ("fseek end index");
#endif
                return ERROR;
            }
            ofsnod_r = ftell(index);
#ifdef TRACE
            printf ("ftell -> %d = 0x%X\n", ofsnod_r, ofsnod_r);
#endif

#endif

#endif

#ifdef TRACE
            printf ("ofsnod_r after = %d = 0x%X\n", ofsnod_r, ofsnod_r);
            TEST_INDEX
#endif
            node.next = ofsnod_r;
            status = fwrite (&node_r, sizeof(node_r), 1, index);
            if (status != 1)
            {
#ifdef TRACE
                perror ("fwrite node_r");
#endif
                return ERROR;
            }
#ifdef TRACE
            printf ("Writing node, ofsnod = %d = 0x%X\n",
                    ofsnod, ofsnod);
#endif
            test_status (file_access (index, ofsnod, sizeof(node), &node,
                                        fwrite)); /* save node */
#ifdef TRACE
            printf ("node.up = %d = 0x%X\n", node.up, node.up);
            TEST_INDEX
#endif
            if (node.up != NO_NODE)
            {
#ifdef TRACE
                printf ("add_item recursive call pos=%d \
ofsnod_r=%d=0x%X key=%d=0x%X\n",
                    node.pos, ofsnod_r, ofsnod_r, node.keys[NODE_SIZE/2],
                    node.keys[NODE_SIZE/2]);
                TEST_INDEX
#endif
                add_item (index, node.up, node.pos, ofsnod_r,
                        node.keys[NODE_SIZE/2/*-1*/], &nod1, &itm1);

                /* node_r.pos = itm1; */
#ifdef TRACE
                printf ("nod1 = %d = 0x%X\n", nod1, nod1);
                printf ("itm1 = %d\n", itm1);
#endif
                node_r.up = nod1;
                node_r.pos = itm1;
                test_status (file_access (index, ofsnod_r, sizeof(node_r),
                                &node_r, fwrite));

#ifdef TRACE
                printf ("add_item recursive ret. pos=%d \
ofsnod_r=%d=0x%X key=%d=0x%X\n",
                    node.pos, ofsnod_r, ofsnod_r, node.keys[NODE_SIZE/2],
                    node.keys[NODE_SIZE/2]);
                TEST_INDEX
#endif

            }
            else
            {
#ifdef TRACE
                printf ("Root division\n");
                TEST_INDEX
#endif
                node_u.level = node.level + 1;
                node_u.n_items = 2;
                node_u.pos = 0;
                node_u.up = NO_NODE;
                node_u.previous = NO_NODE;
                node_u.next = NO_NODE;
                node_u.items[0] = ofsnod;
                node_u.items[1] = ofsnod_r;
                node_u.keys[0] = node.keys[0];
                node_u.keys[/*0*/1] = node.keys[NODE_SIZE/2/*-1*/];

/*              GET_HEADER
                ofsnod_u = get_free_node (index, &header);
                PUT_HEADER
*/
/* #if 0 */
                status = fseek (index, 0, SEEK_END);
                if (status) return ERROR;
                ofsnod_u = ftell (index);
/* #endif */

#ifdef TRACE
                printf ("ofsnod_u = %d = 0x%X\n", ofsnod_u, ofsnod_u);
                TEST_INDEX
#endif
                status = fwrite (&node_u, sizeof(node_u), 1, index);
                if (status != 1) return ERROR;
                node.up = node_r.up = ofsnod_u;
                node.pos = 0;
                node_r.pos = 1;
                test_status (file_access (index, ofsnod, sizeof(node), &node,
                                fwrite)); /* save up node */
                test_status (file_access (index, ofsnod_r, sizeof(node_r),
                                &node_r, fwrite));
                GET_HEADER
                header.root = ofsnod_u;
#ifdef TRACE
                printf ("New root = %d = 0x%X\n", header.root, header.root);
                TEST_INDEX
                break_debug();
#endif
                PUT_HEADER
            }

#ifdef TRACE
            printf ("pos=%d ofsnod=%d=0x%X ofsnod_r=%d=0x%X x=%d=0x%X \
key=%d=0x%X\nadd_item recursive call\n",
                    pos, ofsnod, ofsnod, ofsnod_r, ofsnod_r, x, x, key, key);
            TEST_INDEX
#endif
            if (pos < NODE_SIZE/2)
                add_item (index, ofsnod, pos, x, key, nod, itm);
            else
                add_item (index, ofsnod_r, pos-NODE_SIZE/2, x, key, nod, itm);
#ifdef TRACE
            printf ("add_item recursive return: *nod=%d=0x%X *itm=%d\n",
                *nod, *nod, *itm);
            TEST_INDEX
#endif

        }
        else
        {
/*
        for (i=pos+1; i<node.n_items; i++)
*/
        for (i=node.n_items-1; i>pos; i--)
        {
            node.items[i+1] = node.items[i];
            node.keys[i+1] = node.keys[i];

            if (node.level > 0)
            {
#ifdef TRACE
                    printf ("Reading node.items[%d] = %d = 0x%X\n",
                                i, node.items[i], node.items[i]);
                    TEST_INDEX
#endif
                    test_status (file_access (index, node.items[i],
                        sizeof(node_d), &node_d, fread));
                    node_d.pos = i+1;
                    /* node_d.up = ofsnod_r; */
#ifdef TRACE
                    printf ("Writing node.items[%d] = %d = 0x%X\n",
                                i, node.items[i], node.items[i]);
                    TEST_INDEX
#endif
                    test_status (file_access (index, node.items[i],
                        sizeof(node_d), &node_d, fwrite));

            }

        }
        node.n_items++;
        node.items[pos+1] = x;
        *itm = pos+1;
        *nod = ofsnod;
        /*if (pos >= 0)*/ node.keys[pos+1] = key;
        test_status (file_access (index, ofsnod, sizeof(node),
                        &node, fwrite));
        if (node.level > 0)
        {
#ifdef TRACE
                printf ("position of node %d (0x%X) in node %d (0x%X) is %d\n",
                        x, x, ofsnod, ofsnod, pos+1);
#endif
                test_status (file_access (index, x, sizeof(node_d),
                        &node_d, fread));
                node_d.pos = pos+1;
                test_status (file_access (index, x, sizeof(node_d),
                        &node_d, fwrite));
#ifdef TRACE
                printf ("node_d.pos = %d\n", node_d.pos);
#endif
        }
        if (pos == -1 && node.up != NO_NODE)
        {
                test_status (file_access (index, node.up,
                                sizeof(node_u), &node_u, fread));
                node_u.keys[node.pos] = x;
                test_status (file_access (index, node.up,
                                sizeof(node_u), &node_u, fwrite));
        }

        }
        return ALLRIGHT;
}


add_current (index_file index, item rec)
{
int status;
struct index_header header;
struct index_node node;
int i, n;
int status_add;
        GET_HEADER
        status_add = add_item (index, header.current, header.current_item, rec, rec, &n, &i);
        GET_HEADER
        header.current = n;
        /* header.current_item++; */
        header.current_item = i;
        header.current_record = rec;
        header.n_records ++;
        PUT_HEADER
        if (status_add == ALLRIGHT) return rec;
        return status_add;      
}

#if 0
add_current (index_file index, item rec)
{
int status;
struct index_header header;
struct index_node node;
int i;
        GET_HEADER
        test_status (file_access (index, header.current, sizeof(node),
                        &node, fread));
        if (node.n_items >= NODE_SIZE)
        {

        }
        for (i=header.current_item+1; i<node.n_items; i++)
        {
            node.items[i+1] = node.items[i];
            node.keys[i+1] = node.keys[i];
        }
        node.n_items++;
        node.items[i+1] = rec;
        node.keys[i+1] = rec;
        test_status (file_access (index, header.current, sizeof(node),
                        &node, fwrite));
        header.current_item++;
        header.current_record = rec;
        PUT_HEADER
}
#endif


record write_record (index_file index, data_file data, void *buf, order ord,
                        void *param)
{

}

delete_item (index_file index, item ofsnod, int pos)
{
int status;
struct index_header header;
struct index_node node, node_u, node_d;
int i;
        test_status (file_access (index, ofsnod, sizeof(node),
                                        &node, fread));
        if (node.n_items <= 1)
        {
                if (node.up != NO_NODE)
                {
                        delete_item (index, node.up, node.pos); 
                        GET_HEADER
                        test_status (file_access (index, ofsnod, 
                                sizeof(header.free), &header.free, fwrite));
                        header.free = ofsnod;
                        PUT_HEADER                      
                }
                else
                        new_index (index);
                return ALLRIGHT;
        }
        node.n_items--;
        for (i=pos; i<node.n_items; i++)
        {
            node.items[i] = node.items[i+1];
            node.keys[i] = node.keys[i+1];
            
            if (node.level > 0)
            {
#ifdef TRACE
                    printf ("Reading node.items[%d] = %d = 0x%X\n",
                                i, node.items[i], node.items[i]);
                    TEST_INDEX
#endif
                    test_status (file_access (index, node.items[i],
                        sizeof(node_d), &node_d, fread));
                    node_d.pos = i;
                    /* node_d.up = ofsnod_r; */
#ifdef TRACE
                    printf ("Writing node.items[%d] = %d = 0x%X\n",
                                i, node.items[i], node.items[i]);
                    TEST_INDEX
#endif
                    test_status (file_access (index, node.items[i],
                        sizeof(node_d), &node_d, fwrite));

            }

        }
        test_status (file_access (index, ofsnod, sizeof(node),
                                        &node, fwrite));
        /* get_current (index); */
        if (pos == 0 && node.up != NO_NODE)
        {
                test_status (file_access (index, node.up, sizeof(node_u),
                                        &node_u, fread));
#ifdef TRACE
                printf ("node.keys[0] = %d = 0x%X\n",
                        node.keys[0], node.keys[0]);
                printf ("offset node up = %d = 0x%X\n", node.up, node.up);
#endif
                node_u.keys[node.pos] = node.keys[0];
                test_status (file_access (index, node.up, sizeof(node_u),
                                        &node_u, fwrite));
        }
        return ALLRIGHT;
}

delete_current (index_file index)
{
struct index_header header;
int status_delete;
        GET_HEADER
        status_delete = delete_item (index, header.current, 
                                        header.current_item);
        header.n_records --;
        PUT_HEADER
        /* get_current (index); */
        return status_delete;

}

#if 0
delete_current (index_file index)
{
int status;
struct index_header header;
struct index_node node, node_u;
int i;
        GET_HEADER
        test_status (file_access (index, header.current, sizeof(node),
                                        &node, fread));
        node.n_items--;
        for (i=header.current_item; i<node.n_items; i++)
        {
                node.items[i] = node.items[i+1];
                node.keys[i] = node.keys[i+1];
        }
        test_status (file_access (index, header.current, sizeof(node),
                                        &node, fwrite));
        get_current (index);    
        if (header.current_item == 0)
        {
                test_status (file_access (index, node.up, sizeof(node_u),
                                        &node_u, fread));
#ifdef TRACE
                printf ("node.keys[0] = %d = 0x%X\n", 
                        node.keys[0], node.keys[0]);
                printf ("offset node up = %d = 0x%X\n", node.up, node.up);
#endif
                node_u.keys[node.pos] = node.keys[0];
                test_status (file_access (index, node.up, sizeof(node_u),
                                        &node_u, fwrite));
        }
}
#endif

int new_index (index_file index, int size)
{
int status;
struct index_header header;
struct index_node node;
        status = fseek (index, 0, SEEK_SET);
        if (status) return ERROR;
        header.nodesize = NODE_SIZE;
        header.root = header.first = header.last = header.current
                = sizeof(header);
        header.free = NO_NODE;
        header.current_item = -1;
        header.current_record = NO_RECORD;
        header.free_record = NO_RECORD;
        header.n_records = 0;
        header.n_nodes = 1;
        header.record_size = size;
        PUT_HEADER
        node.level = 0;
        node.n_items = 0;
        node.previous = NO_NODE;
        node.next = NO_NODE;
        node.up = NO_NODE;
        status = fwrite (&node, sizeof(node), 1, index);
#ifdef TRACE
        printf ("status fwrite = %d\n", status);
#endif
        if (status != 1) return ERROR;
        status = fflush (index);
        return ALLRIGHT;
}

create_index (index_file index, data_file data, order ord, void *param)
{

}

struct rec_f
{
    int numero;
    char nom[50];
};

int add (FILE *index, FILE *data, void *rec, int size, order ord1, void *param)
{
int status, ofs;
struct index_header header;
    get_record (index, data, rec, ord1, param, 1);
    GET_HEADER
    if (header.free_record == NO_RECORD)
    {
        status = fseek (data, 0, SEEK_END);
        if (status)
        {
                perror ("fseek");
                return errno;
        }
        ofs = ftell (data);
    }
    else
    {
        ofs = header.free_record;
        status = fseek (data, ofs, SEEK_SET);
        if (status)
        {
                perror ("fseek");
                return errno;
        }
        status = fread (&header.free_record, sizeof(header.free_record), 1,
                        data);
        PUT_HEADER
        if (status != 1)
        {       
                perror ("fread");
                return errno;
        }
        status = fseek (data, ofs, SEEK_SET);
        if (status)
        {
                perror ("fseek");
                return errno;
        }
    }
    status = fwrite (rec, size, 1, data);
    if (status != 1)
    {
        perror("fwrite");
        return errno;
    }
    return add_current (index, ofs);
}

int add_index (FILE *index, FILE *data, void *rec, int size, order ord1, 
                void *param, int ofs)
{
int status;
struct index_header header;
    get_record (index, data, rec, ord1, param, 1);
    return add_current (index, ofs);
}

int del (FILE *index, FILE *data)
{
int status;
struct index_header header;
struct rec_f rec;

        GET_HEADER
        status = fseek (data, header.current_record, SEEK_SET);
        if (status)
        {
                perror ("fseek");
                return errno;
        }
        status = fwrite (&header.free_record, sizeof(header.free_record),
                         1, data);
        header.free_record = header.current_record;
        /*
        rec_f.numero = 0;
        test_status (file_access (data, header.record, sizeof(rec),
                                        &rec, fwrite));
        */
        PUT_HEADER
        return delete_current(index);
        /* PUT_HEADER */
}

access_data (FILE *index, FILE *data, void *rec, int size,
                rw_function function)
{
int status;
struct index_header header;
    GET_HEADER
#ifdef TRACE
    printf ("access_data: current_record = %d = 0x%X\n",
                header.current_record, header.current_record);
#endif
    status = fseek (data, header.current_record, SEEK_SET);
    if (status)
    {
        perror ("fseek");
        return errno;
    }
    status = (*function) (rec, size, 1, data);
    if (status != 1)
    {
        perror ("fread");
        return errno;
    }
    return ALLRIGHT;
}

int get_data (FILE *index, FILE *data, void *rec, int size)
{
        return access_data (index, data, rec, size, fread);
}

int put_data (FILE *index, FILE *data, void *rec, int size)
{
        return access_data (index, data, rec, size, fwrite);
}

/*
get_data (FILE *index, FILE *data, void *rec, int size)
{
int status;
struct index_header header;
    GET_HEADER
    status = fseek (data, header.current_record, SEEK_SET);
    if (status)
    {
        perror ("fseek");
        return errno;
    }
    status = fread (rec, size, 1, data);
    if (status != 1)
    {
        perror ("fread");
        return errno;
    }
    return ALLRIGHT;
}
*/

int get (FILE *index, FILE *data, void *rec, int size, order ord1,
	void *param
#ifdef NEW
		    , int flag_last
#endif
			)
{
int status, status_get;
struct index_header header;
/*
struct rec_f rec1;
    memcpy (&rec1, rec, sizeof(rec1));
*/
    status_get = get_record (index, data, rec, ord1, 0,
#ifdef NEW
							flag_last
#else
							0
#endif
								);
    if (status_get < -1) return status_get;
    GET_HEADER
    if (header.current_record == NO_RECORD) return status_get;
    status = fseek (data, header.current_record, SEEK_SET);
    if (status)
    {
        perror ("fseek");
        return -3;
    }
    status = fread (rec, size, 1, data);
    if (status != 1)
    {
        perror ("fread");
        return -4;
    }
    return status_get;
/*    return ALLRIGHT; */
}

int first (FILE *index, FILE *data, void *rec, int size)
{
int ofsrec;
        ofsrec = get_first (index);
        if (ofsrec != NO_RECORD) 
            get_data (index, data, rec, size);
        return ofsrec;
}

int last (FILE *index, FILE *data, void *rec, int size)
{
int ofsrec;
        ofsrec = get_last (index);
#ifdef TRACE
        printf ("last: get_last -> %d 0x%X\n", ofsrec, ofsrec);
#endif
        if (ofsrec != NO_RECORD)
        {
#ifdef TRACE
            printf ("getting data\n");
#endif
            get_data (index, data, rec, size);
        }
#ifdef TRACE
        printf ("last -> %d 0x%X\n", ofsrec, ofsrec);
#endif
        return ofsrec;
}

int next (FILE *index, FILE *data, void *rec, int size)
{
int ofsrec;
        ofsrec = get_next (index);
        if (ofsrec != NO_RECORD)
            get_data (index, data, rec, size);
        return ofsrec;
}

int previous (FILE *index, FILE *data, void *rec, int size)
{
int ofsrec;
        ofsrec = get_previous (index);
        if (ofsrec != NO_RECORD)
            get_data (index, data, rec, size);
        return ofsrec;
}

int ord1 (struct rec_f *r1, struct rec_f *r2
#ifdef NEW
						, void *param
#endif
								)
{
    if (r1->numero > r2->numero) return GT;
    if (r1->numero < r2->numero) return LT;
    return EQ;
}

print_margin (int level)
{
int i;
    for (i=0; i<level; i++)
        printf ("    ");
}

print_node (FILE *index, FILE *data, int ofsnod, int level)
{
struct index_node node;
int i;
struct rec_f rec;
    print_margin(level);
    printf ("Node %d 0x%X - ", ofsnod, ofsnod);
    test_status (file_access (index, ofsnod, sizeof(node), &node, fread));
    printf ("up %d 0x%X - pos %d - previous %d 0x%X - next %d 0x%X\n",
                node.up, node.up, node.pos, node.previous, node.previous,
                node.next, node.next);
    for (i=0; i<node.n_items; i++)
    {
        print_margin(level);
        /* printf ("%d\n", node.keys[i]); */
        test_status (file_access (data, node.keys[i], sizeof(rec), &rec,
                                    fread)); /* get key */
        printf ("  %d\n", rec.numero);
        if (node.level == 0)
        {
            print_margin(level);
            test_status (file_access (data, node.items[i], sizeof(rec), &rec,
                                        fread)); /* get record */
            printf ("    %d <%s> (%d 0x%X)\n", rec.numero, rec.nom,
                        node.items[i], node.items[i]);
        }
        else
        {
            print_node (index, data, node.items[i], level+1);
        }
    }
}

print_index (FILE *index, FILE *data)
{
struct index_header header;
    GET_HEADER
    printf ("First %d %x%X - last %d 0x%X - current %d 0x%X - %d records",
                header.first, header.first, header.last, header.last,
                header.current, header.current, header.n_records);
    printf (" (%d)\n", header.current_item & 0xFF);
    print_node (index, data, header.root, 0);
}

test_index ()
{
FILE *data, *index;
struct rec_f rec;
int status;
char command[50];
char buffer[50];

    printf ("Initialize ? ");
    gets (buffer);
    if (*buffer == 'y')
    {
#ifdef VAX_C
    data = fopen ("f.dat", "w", "ctx=bin");
#else
    data = fopen ("f.dat","wb");
#endif
    if (data == NULL)
    {
        perror ("f.dat");;
        return errno;
    }

#ifdef VAX_C
    index = fopen ("f.idx", "w", "ctx=bin");;
#else
    index = fopen ("f.idx", "wb");
#endif
    if (index == NULL)
    {
        perror ("f.idx");
        return errno;
    }

    fclose (data);
    fclose (index);
    }

#ifdef VAX_C
    data = fopen ("f.dat", "r+", "ctx=bin");
#else
    data = fopen ("f.dat", "r+b");
#endif
    if (data == NULL)
    {
        perror ("f.dat");;
        return errno;
    }

#ifdef VAX_C
    index = fopen ("f.idx", "r+", "ctx=bin");;
#else
    index = fopen ("f.idx", "r+b");
#endif
    if (index == NULL)
    {
        perror ("f.idx");
        return errno;
    }

    _index = index;
    _data = data;

    if (*buffer == 'y')
        new_index (index, sizeof(rec));

    for (;;)
    {
        printf ("add get print quit ? ");
        gets (command);
        switch (command[0])
        {
                case 'a':
                        printf ("Numero ? ");
                        gets (buffer);
                        sscanf (buffer, "%d", &(rec.numero));
                        printf ("Nom ? ");
                        gets (rec.nom);
                        add (index, data, &rec, sizeof(rec), ord1, 0);
                        break;
                case 'g':
                        printf ("Numero ? ");
                        gets (buffer);
                        sscanf (buffer, "%d", &(rec.numero));
			status = get (index, data, &rec, sizeof(rec),
					ord1, 0
#ifdef NEW
						, 0
#endif
							);
                        printf ("Nom = <%s>, status = %d\n", rec.nom, status);
                        break;
                case 'm':
                        printf ("Numero ? ");
                        gets (buffer);
                        sscanf (buffer, "%d", &(rec.numero));
			get (index, data, &rec, sizeof(rec), ord1, 0
#ifdef NEW
								     , 0
#endif
									);
                        printf ("Nom = <%s> -> ", rec.nom);
                        gets (rec.nom);
                        put_data (index, data, &rec, sizeof(rec));
                        break;
                case 'd':
                        del (index, data);
                        break;
                case '1':
                        status = first (index, data, &rec, sizeof(rec));
                print_rec:
                        printf ("%d <%s> status = %d\n",
                            rec.numero, rec.nom, status);
                        break;
                case '$':
                        status = last (index, data, &rec, sizeof(rec));
#ifdef TRACE
                        printf ("main: last -> %d 0x%X\n", status, status);
#endif
                        goto print_rec;
                case '>':
                        status = next (index, data, &rec, sizeof(rec));
                        goto print_rec;
                case '<':
                        status = previous (index, data, &rec, sizeof(rec));
                        goto print_rec;
                case '=':
                        get_data (index, data, &rec, sizeof(rec));
                        goto print_rec;
                case 'p':
                        print_index (index, data);
                        break;
                case 'q':
                        return;
                default:
                        printf ("Unknown command\n");
        }
    }

/*
    get_record (index, data, &rec, ord1, 0);
    ofs = ftell (data);
    status = fwrite (&rec, sizeof(rec), 1, data);
    if (status != 1)
    {
        perror("fwrite");
        return errno;
    }
    add_current (index, ofs);
*/
#if 0
    rec.numero = 15;
    strcpy (rec.nom, "Quinze");
    add (index, data, &rec, sizeof(rec), ord1, 0);

    rec.numero = 10;
    strcpy (rec.nom, "Dix");
    add (index, data, &rec, sizeof(rec), ord1, 0);
    
    rec.numero = 12;
    strcpy (rec.nom, "Douze");
    add (index, data, &rec, sizeof(rec), ord1, 0);
    
    rec.numero = 20;
    strcpy (rec.nom, "Vingt");
    add (index, data, &rec, sizeof(rec), ord1, 0);

    rec.numero = 18;
    strcpy (rec.nom, "Dix-huit");
    add (index, data, &rec, sizeof(rec), ord1, 0);

    rec.numero = 12;
/*
    get_record (index, data, &rec, ord1);
*/
    get (index, data, &rec, sizeof(rec), ord1, 0);
    printf ("%s\n", rec.nom);
#endif
}


