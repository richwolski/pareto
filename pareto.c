
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>

#define RAND() (drand48())


double a;
double b;
int Samples;
double Low;
double High;
int Values;
int CDF;

/*
 * parameters: a, b
 * PDF: (a * b^a) / (x^(a+1))
 * CDF: 1 - (b / x)^a
 * mean: (a * b) / (a - 1)
 * var: (a * b^2) / ((a - 1)^2 * (a - 2))
 */


#define Usage "pareto -a a -b b -c sample_count -l low -h high [-VC]\n"

#define ARGS "a:b:c:h:l:VC"

double
Pareto(double y, double a, double b)
{
        double temp;


	temp = (a * pow(b,a)) / pow(y,(a+1.0));

        return(temp);

}

double InvertedParetoCDF(double y, double a, double b)
{
	return(b / pow((1-y),1.0/a));
}

#ifdef STANDALONE

int
main(int argc, char *argv[])
{
	int c;
	int i;
	int j;
	double r;
	double y;
	double value;
	double acc;
	double incr;
	double curr;

	a = 1.0;
	b = 1.0;
	Samples = -1;
	/*
 	 * define over x >= b
	 */
	Low = b;
	High = 1.0;
	Values = 0;
	CDF = 0;

	while((c = getopt(argc,argv,ARGS)) != EOF)
	{
		switch(c)
		{
			case 'a':
				a = atof(optarg);
				break;
			case 'b':
				b = atof(optarg);
				break;
			case 'c':
				Samples = atoi(optarg);
				break;
			case 'C':
				CDF = 1;
				break;
			case 'l':
				Low = atoi(optarg);
				break;
			case 'h':
				High = atoi(optarg);
				break;
			case 'V':
				Values = 1;
				break;
			default:
				fprintf(stderr,"unrecognize arg: %c\n",c);
				fprintf(stderr,"usage: %s\n",Usage);
				fflush(stderr);
				exit(1);
		}
	}

	if(Samples < 1)
	{
		fprintf(stderr,"usage: %s\n",Usage);
		fflush(stderr);
		exit(1);
	}

	if(Low < b)
		Low = b;

	if(CDF == 0)
	{
		for(i=0; i < Samples; i++)
		{
			r = RAND();
			
			value = Low + (High - Low)*r;

			y = Pareto(value,a,b);

			if(Values == 0)
				fprintf(stdout,"%3.4f %3.4f\n",value,y);
			else
			{
				fprintf(stdout,"%f\n",
					InvertedParetoCDF(RAND(),a,b));
/*
				for(j=0; j < (int)(y*(High-Low)); j++)
				{
					fprintf(stdout,"%f\n",value);
				}
*/
			}
					
		}

		fflush(stdout);
	}
	else
	{
		incr = (High-Low)/(double)Samples;
		curr = Low;
		acc = 0;
		for(j = 0; j < Samples; j++)
		{
			y = Pareto(curr,a,b);
			acc += (y*incr);
			fprintf(stdout,"%3.4f %3.4f\n",
					curr,acc);
			curr += incr;
		}
		fflush(stdout);
	}


	exit(0);
}


#endif
