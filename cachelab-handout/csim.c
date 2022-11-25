#include "cachelab.h"
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <strings.h>

int S;
int E;
int B;
int m = 64;

int hits = 0;
int misses = 0;
int evicts = 0;

int s;
int b;
int tag;

int lru = 0;

typedef struct
    {
	unsigned long long tagline;
	unsigned long long set;
	int lastUsed;
 }line;

int getLRU(line cache[S][E], unsigned long long set)
{
	int last = cache[set][0].lastUsed;
	int idx = 0;
	for(int i=0; i<E; i++)
	{
		if(last >= cache[set][i].lastUsed)
		{
			last = cache[set][i].lastUsed;
			idx = i;
		}
	}
	return idx;
}

void makeCache(line cache[S][E])
{
	line currLine;
	currLine.tagline = -1;
	currLine.lastUsed = 0;

	for(int i=0; i<S; i++)
	{
		for(int j=0; j<E; j++)
			{
				 cache[i][j] = currLine;
		  	}
	}
}

void calculate(line cache[S][E], unsigned long long address)
{
	int check = 0;
	int flag = 0;
	int empty = 0;

	line currLine;

	currLine.tagline = address >> (s+b);
	currLine.set =  address << tag;
	currLine.set = currLine.set >> (tag + b);
	
	for(int i=0; i<E; i++)
	{
		if(cache[currLine.set][i].tagline == -1)
		{
			flag = 1;
			empty = i;
		}
		if(cache[currLine.set][i].tagline == currLine.tagline)
		{
			hits++;
			check = 1;
			lru++;
			cache[currLine.set][i].lastUsed = lru;
		}
	}

	if(check == 0 && flag ==1)
	{
		cache[currLine.set][empty].tagline = currLine.tagline;
		cache[currLine.set][empty].lastUsed = lru;
		misses++;
	}
	if(check == 0 && flag ==0)
	{
		int index = getLRU(cache,currLine.set);
		cache[currLine.set][index].tagline = currLine.tagline;
		cache[currLine.set][index].lastUsed = lru;
		misses++;
		evicts++;
	}
}

int main(int argc, char **argv)
{
	FILE *file;

	unsigned long long address;
	int size;
	char *traceFile;
	char c;

	while((c=getopt(argc,argv,"s:E:b:t:vh")) != -1)
	{
		switch(c)
		{
			case 'v':
				break;
			case 's':
				s = atoi(optarg);
				break;
			case 'E':
				E = atoi(optarg);
				break;
			case 'b':
				b = atoi(optarg);
				break;
			case 't':
				traceFile = optarg;
				break;
			case 'h':
				exit(0);
			default:
				exit(1);
		}         
       	}

	S = pow(2, s);
	B = pow(2, b);
	tag = m -(s + b); 

	line cache[S][E];
	makeCache(cache);
	file  = fopen(traceFile, "r");
	
	if (file != NULL) 
	{
		while (fscanf(file, " %c %llx,%d", &c, &address, &size) == 3)
	       	{
			switch(c) {
				case 'I':
					break;
				case 'L':
					calculate(cache, address);
					break;
				case 'S':
					calculate(cache, address);
					break;
				case 'M':
					calculate(cache, address);
					calculate(cache, address);
					break;
				default:
					break;
			}
		}
	}
	
	printSummary(hits, misses, evicts);
	
	return 0;
}
