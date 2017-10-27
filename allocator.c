#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "funct.h"

int main() {
	int n, i, size, poz, value, index;
	unsigned char *arena, *memstart;
	char task[20];
	scanf("%s", task);
	while(strcmp(task, "FINALIZE") != 0) {
		if(strcmp(task, "INITIALIZE") == 0) {
			scanf("%d", &n);
			arena = INITIALIZE(n);
		}
		if(strcmp(task, "DUMP") == 0) {
			DUMP(arena, n);
		}
		if(strcmp(task, "ALLOC") == 0) {
			scanf("%d", &size);
			memstart = ALLOC(arena, n, size);
			if(memstart) {
				printf("%d\n", (int)(memstart - arena));
			} else {
				printf("0\n");
			}
		}
		if(strcmp(task, "FILL") == 0) {
			scanf("%d %d %d", &poz, &size, &value);
			FILL(arena, poz, size, value);
		}
		if(strcmp(task, "FREE") == 0) {
			scanf("%d", &index);
			FREE(arena, index);
		}
		if(strcmp(task, "SHOW") == 0) {
			scanf("%s", task);
			if(strcmp(task, "FREE") == 0) {
				SHOW(arena, n, 0);
			} else {
				if(strcmp(task, "USAGE") == 0) {
				SHOW(arena, n, 1);
				} else {
					if(strcmp(task, "ALLOCATIONS") == 0) {
						SHOW(arena, n, 2);
					} else {
						scanf("%d", &value);
						SHOW_MAP(arena, n, value);
					}
				}
			}
		}
		if(strcmp(task, "ALLOCALIGNED") == 0) {
			scanf("%d %d", &size, &value);
			memstart = ALLOCALIGNED(arena, n, size, value);
			if(memstart) {
				printf("%d\n", (int)(memstart - arena));
			} else {
				printf("0\n");
			}		
		}
		if(strcmp(task, "REALLOC") == 0) {		
			scanf("%d %d", &index, &size);
			memstart = REALLOC(arena, n, index, size);
			printf("%d\n", (int)(memstart - arena)); 
		}
		scanf("%s", task);
	}
	FINALIZE(arena);
	return 0;
}


