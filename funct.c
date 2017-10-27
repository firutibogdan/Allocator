#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//Functia care initializeaza arena, punand 0 peste tot
unsigned char *INITIALIZE(int n) {
	int i;
	unsigned char *arena;
	arena = (unsigned char *) calloc(n, sizeof(unsigned char));
	return arena;
}

//Functia care elibereaza memoria
void FINALIZE(unsigned char *arena) {
	free(arena);
}

//Functia care afiseaza octetii
void DUMP(unsigned char *arena, int n) {
	int i = 0, j, mid;
	//Afisez cate 16 octeti pe rand
	while(i + 16 <= n) {
		printf("%08X\t", i);
		j = i; 
		i = i + 16;
		mid = j + 7;
		for(j ; j < i; j++) {
			printf("%02X ", arena[j]);
			if(j == mid) {
				printf(" ");
			}
		}
		printf("\n");
	}
	//La sfarsit e posibil sa mai am octeti de afisat (daca n % 16 != 0)
	if(i < n) {
		printf("%08X\t", i);
		mid = i + 7;
		for(i ; i < n; i++) {
			printf("%02X ", arena[i]); 
			if(i == mid) {
				printf(" ");
			}
		}	
	}
	printf("\n");
	printf("%08X\n", n);
}

//Functia care aloca memorie
unsigned char *ALLOC(unsigned char *arena, int n, int size) {
	int start, poz, back, backend, forward, i;
	unsigned char *p;
	//Verific daca nu cumva memoria mea e libera
	if(arena[0] == 0 && arena[1] == 0 && arena[2] == 0 && arena[3] == 0) {
		if(size + 12 <= n - 4) {
			start = 4;
			p = arena + 4;
			for(i = 0; i < 8; i++) {
				p[i] = 0;
			}
			*((int *)arena) = start;
			*((int *)(arena + 12)) = size + 12;	
			//Returnez p + 12 pentru a sari peste cei 12 octeti de gestiune
			return p + 12;
		} else {
			return NULL;
		}
	//Daca memoria nu e libera, parcurg din zona in zona
	} else {
		//back este inceputul zonei anterioare
		back = 0;
		//backend este sfarsitul zonei anterioare
		backend = 4;
		p = arena;
		//forward este inceputul zonei la care ma aflu
		forward = *((int *)p);
		while(forward != 0) {
			//Daca gasesc loc intre 2 zone de memorie consecutive
			if(forward - backend >= size + 12) {
				p = arena + backend;
				*((int *)p) = *((int *)(arena + back));
				*((int *)(p + 4)) = back;
				*((int *)(p + 8)) = size + 12;	
				*((int *)(arena + back)) = backend;
				*((int *)(arena + forward + 4)) = backend;				
				return p + 12;
			}
			//Trec la urmatoarea zona
			back = forward;
			p = arena + forward;
			forward = *((int *)p);
			backend = back + *((int *)(p + 8));
		}
		/*Daca am ajuns la sfarsitul zonelor ocupate, e posibil sa am loc
		sa aloc noua zona pe ultimii octeti ramasi liberi din memorie */
		if(n - backend >= size + 12) {
			p = arena + backend;
			*(int *)p = forward;
			*((int *)(p + 4)) = back;                        
                        *((int *)(p + 8)) = size + 12;
			*((int *)(arena + back)) = backend;                        
			return p + 12;
		}
	}
	return NULL; 
}

//Functia care pune valori
void FILL(unsigned char *arena, int poz, int size, int value) {
	int i;
	for(i = 0; i < size; i++) {
		*(arena + poz +i) = value & 255;
	}
}

//Functia care elibereaza memoria
void FREE(unsigned char *arena, int index){
	int back, forward, i;
	unsigned char *p, *q;
	//Il trimit pe p la inceputul zonei de gestiune
	p = arena + index - 12;
	//Gasesc urmatoarea zona de memorie
        forward = *((int*)p);
	//Gasesc zona anterioara de memorie
	back = *((int *)(p + 4));
	//Folosesc q pentru a merge de la o zona la alta
	q = arena + back;
	*((int *)q) = forward;
	if(forward != 0) {
		q = arena + forward;
		*((int *)(q + 4)) = back;
	}
}

//Functia care afiseaza informatii
void SHOW(unsigned char *arena, int n, int value) {
	int nblocks, nblocku, back, backend, forward, nbytes, 
	    bytesu, bytesru;
	unsigned char *p;
	nblocks = 0;
	nblocku = 0;
	nbytes = 0;
	p = arena;
	back = 0;
	backend = 4;
	forward = *((int *)p);
	//Verific daca memoria e libera
	if(value == 2) {
		if(p[0] == 0 && p[1] == 0 && p[2] == 0 && p[3] == 0) {
			printf("OCCUPIED 4 bytes\n");
		}
	}
	//Parcurg zonele din memorie
	while(forward != 0) {
		nblocku++;
		if(value == 2) {
                        printf("OCCUPIED %d bytes\n", backend - back);
                }
		if(forward != backend) {
			nbytes += forward - backend;	
			nblocks++;
			if(value == 2) {
				printf("FREE %d bytes\n", forward - backend);
			}
		}
		back = forward;
		p = arena + forward;
		forward = *((int *)p);
		backend = back + *((int *)(p + 8));
	}
	if(value == 2 && backend != 4) {
		printf("OCCUPIED %d bytes\n", backend - back);
	}
	if(backend < n) {
		if(value == 2) {
			printf("FREE %d bytes\n", n - backend);
		}
		nbytes += n - backend;
		nblocks++;
	}
	if(value == 0) {
		printf("%d blocks (%d bytes) free\n", nblocks, nbytes);
	} else {
		if(value == 1) {
			bytesu = n - nbytes - 12 * nblocku - 4;
			bytesru = n - nbytes;
			printf("%d blocks (%d bytes) used\n", nblocku, bytesu);
			printf("%d%% efficiency\n", (bytesu * 100) / bytesru);
			if(nblocku != 0) {
				printf("%d%% fragmentation\n", ((nblocks - 1) * 100) / nblocku);
			} else {
				printf("0%% fragmentation\n");
			}	
		}
	}
}

//Functia care aloca memorie aliniata la un index
unsigned char *ALLOCALIGNED(unsigned char *arena, int n, int size, int value) {
	int start, poz, back, backend, forward, i;
	unsigned char *p;
	//Verific daca memoria e libera
	if(arena[0] == 0 && arena[1] == 0 && arena[2] == 0 && arena[3] == 0) {
		if(size + 12 <= n - 4) {
			start = 4;
			if((start + 12) % value == 0) {
				p = arena + 4;
				for(i = 0; i < 8; i++) {
					p[i] = 0;
				}
				*((int *)arena) = start;
				*((int *)(arena + 12)) = size + 12;	
				return p + 12;
			} else {
				if(value + size <= n) {
					*((int *)arena) = value - 12;
					p = arena + value;
					*((int *)(p - 4)) = size + 12;
					return p;
				} else {
					return NULL;
				}
			}
				
		} else {
			return NULL;
		}
	} else {
		//Parcurg zonele din memorie in cautarea unei zone favorabile
		back = 0;
		backend = 4;
		p = arena;
		forward = *((int *)p);
		while(forward != 0) {
			if(forward - (backend + value - (backend + 12) % value) > size + 12) {
				backend = backend + value - (backend + 12) % value;				
				p = arena + backend;
				*((int *)p) = *((int *)(arena + back));
				*((int *)(p + 4)) = back;
				*((int *)(p + 8)) = size + 12;	
				*((int *)(arena + back)) = backend;
				*((int *)(arena + forward + 4)) = backend;				
				return p + 12;
			}
			back = forward;
			p = arena + forward;
			forward = *((int *)p);
			backend = back + *((int *)(p + 8));
		}
		if(backend + 12 + value - ((backend + 12) % value) + size < n) {
			if((backend + 12) % value != 0) {
				backend = backend + value - ((backend + 12) % value);
			}			
			p = arena + backend;
			*(int *)p = forward;
			*((int *)(p + 4)) = back;                        
                        *((int *)(p + 8)) = size + 12;
			*((int *)(arena + back)) = backend;                    
			return p + 12;
		}
	}
	return NULL; 
}

//Functia care realoca memoria
unsigned char *REALLOC(unsigned char * arena, int n, int index, int size) {
	unsigned char *p, *q;
	int i;
	p = arena + index - 12;
	int oldsize = *(int *)(p + 8);
	FREE(arena, index);
	q = ALLOC(arena, n, size);
	if(size >= oldsize - 12) {
		for(i = 0; i < oldsize - 12; i++) {
			*(q + i) = *(p + 12 + i);
		}
	} else {
		for(i = 0; i < size; i++) {
			*(q + i) = *(p + 12 + i);
		}
	} 
	return q;
}

//O functie suplimentara care verifica daca un octet (index) este utilizat sau nu
int VERIFY(unsigned char *arena, int n, int index) {
	int back, backend, forward;
	unsigned char *p;
	p = arena;
	back = 0;
	backend = 4;
	forward = *((int *)p);
	if(p[0] == 0 && p[1] == 0 && p[2] == 0 & p[3] == 0 && index < 4) {
		return 1;
	}
	while(forward != 0) {
		if(back <= index && index < backend) {
			return 1;
		}

		if(forward != backend) {
			if(backend <= index && index < forward) {
				return 0;
			}
		}
		back = forward;
		p = arena + forward;
		forward = *((int *)p);
		backend = back + *((int *)(p + 8));
	}
	if(backend != 4) {
		if(back <= index && index < backend) {
			return 1;
		}
	}
	if(backend < n) {
		if(backend <= index && index < n) {
			return 0;
		}
	}
}

//Functia care afiseaza harta memoriei
void SHOW_MAP(unsigned char *arena, int n, int value) {
	int i, j, count, count1, count2;
	double div;
	double x = (double)n / (double)value;
	if(x <= 1) {
		//Pastrez doar 2 zecimale
		div = (double)((int)((double)value / (double)n * 100)) /100;
		//contor pentru numarul de octeti afisati		
		count = 0;
		i = 0;
		//Parcurg memoria si vad cati octeti am ocupati si cati am liberi
		while(i < n) {
			//contor pentru octetii ocupati
			count1 = 0;
			//contor pentru octetii lieri
			count2 = 0;
			while(VERIFY(arena, n, i) == 1) {
				count1++;
				i++;
			}
			if(div == (int)div) {
				count1 *= div;
			} else {
				count1 = (int)(count1 * div) + 1;
			}
			for(j = 0; j < count1; j++) {
				printf("*");
				count++;
				if(count % 80 == 0) {
					printf("\n");
				}
			}
			while(VERIFY(arena, n, i) == 0) {
				count2++;
				i++;
			}
			if(div == (int)div) {
				count2 *= div;
			} else {
				count2 = (int)(count2 * div);
			}
			for(j = 0; j < count2; j++) {
				printf(".");
				count++;
				if(count % 80 == 0) {
					printf("\n");
				}
			}
		}		
		for(j = count; j < value; j++) {
			if(VERIFY(arena, n, i-1) == 1) {
				printf("*");
			} else {
				printf(".");
			}
		}
	} else {
		div = (double)n / (double)value;
		int ok, y;
		count = 0;
		//val0 si val1 vor fi capatele intervalului in care caut octei ocupati
		double val0 = 0, val1 = div;
		while(val0 < n) {
			ok = 0;
			if(val1 == (int)val1) {
				y = val1;
			} else {
				y = (int)val1 + 1;
			}
			for(i = val0; i < y; i++) {
				if(VERIFY(arena, n, i) == 1) {
					ok = 1;
				}
			} 
			if(ok == 1) {
				count++;
				printf("*");
				if(count == value) {
					break;
				}
				if(count % 80 == 0) {
					printf("\n");
				}
			} else {
				count++;
				printf(".");
				if(count == value) {
					break;
				}
				if(count % 80 == 0) {
					printf("\n");
				}
			}
			//Cele doua capete vor creste cu div de fiecare data
			val0 += div;
			val1 += div;
		}
	}
	printf("\n");	
}
