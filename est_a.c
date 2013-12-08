
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "input.h"

#define RAND() (drand48())


/*
 * implements alpha* estimate from the paper that is in this
 * directory
 *
 * T = sum(ln(X[i] / b))
 *
 * alpha* = (n-1)/T
 *
 * assumes that the minimum value is known and is set to
 * b
 */
double b;
int Use_min;

/*
 *
 * Parato -- 
 * parameters: a, b
 * PDF: (a * b^a) / (x^(a+1))
 * CDF: 1 - (b / x)^a
 * mean: (a * b) / (a - 1)
 * var: (a * b^2) / ((a - 1)^2 * (a - 2))
 */


#define Usage "est_a -b -f filename\n"

#define ARGS "b:f:"


int
main(int argc, char *argv[])
{
	int c;
	char fname[255];
	char *data;
	double acc;
	double count;
	double ts;
	double value;
	double T;
	int ierr;
	double min = 99999999999;

	b = -1.0;
	Use_min = 1;
	memset(fname,0,sizeof(fname));
	/*
 	 * define over x >= b
	 */

	while((c = getopt(argc,argv,ARGS)) != EOF)
	{
		switch(c)
		{
			case 'b':
				b = atof(optarg);
				Use_min = 0;
				break;
			case 'f':
				strncpy(fname,optarg,sizeof(fname));
				break;
			default:
				fprintf(stderr,"unrecognize arg: %c\n",c);
				fprintf(stderr,"usage: %s\n",Usage);
				fflush(stderr);
				exit(1);
		}
	}

	if(fname[0] == 0)
	{
		fprintf(stderr,"usage: %s\n",Usage);
		fflush(stderr);
		exit(1);
	}

	ierr = InitData(fname,0,0,0,&data);
	if(ierr == 0)
	{
		fprintf(stderr,"can't open file %s\n",fname);
		fflush(stderr);
		exit(1);
	}

	/*
	 * if b unspecified, use min as b
	 */
	if(Use_min == 1)
	{
		while(ReadEntry(data,&ts,&value) != 0)
		{
			if(value < min)
				min = value;
		}
		b = min;
		Rewind(data);
	}

	acc = 0.0;
	count = 0.0;
	while(ReadEntry(data,&ts,&value) != 0)
	{
		acc += log(value / b);
		count++;
	}

	T = acc;


	fprintf(stdout,"%f",(count - 1.0)/T);
/*
	fprintf(stdout,"estimate of a: %f\n",(count - 1.0)/T);
	fprintf(stdout,"estimate of b: %f\n",b);
*/
	fflush(stdout);

	RemoveData(data);

	return(0);
}

