#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>

#define NUM_CHARS 10

char *getStats (char *array)
{
	char min = CHAR_MAX, max = 0;
	int occs[CHAR_MAX + 1] = {0}; //Numero di occorrenze di ogni carattere

	for (int i = 0; i < NUM_CHARS; i++) //Carattere massimo e minimo
	{
		if (array[i] > max) max = array[i];
		if (array[i] < min) min = array[i];
		occs[array[i]]++;
	}

	char least = array[0], most = array[0];
	for (int i = 0; i < CHAR_MAX + 1; i++) //Frequenza massima e minima
	{
		if (!occs[i]) continue; //Ignoro i caratteri non inseriti (= 0 occorrenze)
		if (occs[i] > occs[most]) most = i;
		if (occs[i] < occs[least]) least = i;
	}

	char *stats = malloc (4 * sizeof (char));
	stats[0] = min; stats[1] = max; stats[2] = most; stats[3] = least;
	return stats;
}

int main (int argc, char **argv)
{
	char char_array[NUM_CHARS];
	int n = 0, processes = 0;
	while (1)
	{
		char_array[n++] = getchar ();
		if (char_array[n-1] == EOF)
		{
			printf ("Numero processi generati: %d\n.", processes);
			exit (0);
		}
		if (n == NUM_CHARS)
		{
			switch (fork ())
			{
				case 0:; //Figlio
					char *stats = getStats (char_array);
					printf ("Statistiche:\n"
							"1) Carattere minimo: %c.\n"
							"2) Carattere massimo: %c.\n"
							"3) Carattere con più occorrenze: %c.\n"
							"4) Carattere con meno occorrenze: %c.\n",
							stats[0], stats[1], stats[2], stats[3]);
					exit (0);
				case -1:
					perror ("fork");
				default: //Padre
					wait (NULL);
					n = 0;
					processes++;
					break;
			}
		}
	}
	return 0;
}
