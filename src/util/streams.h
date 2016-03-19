
struct param_function
{
	int (*f) ();
	void *p;
};

typedef struct param_function *pfunc;

