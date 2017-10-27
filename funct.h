unsigned char *INITIALIZE(int n);

void FINALIZE(unsigned char *arena);

void DUMP(unsigned char *arena, int n);

unsigned char *ALLOC(unsigned char *arena, int n, int size);

void FILL(unsigned char *arena, int poz, int size, int value);

void FREE(unsigned char *arena, int index);

void SHOW(unsigned char *arena, int n, int value);

unsigned char *ALLOCALIGNED(unsigned char *arena, int n, int poz, int size);

unsigned char *REALLOC(unsigned char *arena, int n, int index, int size);

int VERIFY(unsigned char *arena, int n, int index);

void SHOW_MAP(unsigned char *arena, int n, int value);
