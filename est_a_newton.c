#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <string.h>


/*
 * computes alpha using Newton's method and method of moments
 * for TRUNCATED pareto
 *
 * M = max value
 * m = mu = mean
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


char *Usage = "est_a_newton -m mu\n\
\t -M max-value\n\
\t -I max-iterations\n\
\t -T tolerane\n";

#define ARGS "m:M:I:T:"

int Iterations;
double Tolerance;

int
main(int argc, char *argv[])
{
	int c;
	double m;
	double M;
	double alpha;
	double a;
	int i;

	Iterations = 1000;
	Tolerance = 0.000001;
	M = -1;
	m = -1;
	while((c = getopt(argc,argv,ARGS)) != EOF)
	{
		switch(c)
		{
			case 'm':
				m = atof(optarg);
				break;
			case 'M':
				M = atof(optarg);
				break;
			case 'I':
				Iterations = atoi(optarg);
				break;
			case 'T':
				Tolerance = atof(optarg);
				break;
			default:
				fprintf(stderr,"unrecognize arg: %c\n",c);
				fprintf(stderr,"usage: %s\n",Usage);
				fflush(stderr);
				exit(1);
		}
	}

	if(Iterations < 1) {
		fprintf(stderr,"Iterations must be > 1\n");
		exit(1);
	}

	if((Tolerance < 0) || (Tolerance > 1)) {
		fprintf(stderr,"Tolerance must be 0 < T < 1\n");
		exit(1);
	}

	if(M == -1) {
		fprintf(stderr,"must specify max value of pareto\n");
		exit(1);
	}

	if(m == -1) {
		fprintf(stderr,"must specify sample mean\n");
		exit(1);
	}


	alpha = 0;
	a = 0.5;	/* guess 0.5 */
	for(i=0 ; i < Iterations; i++) {
		if(fabs(alpha-a) < Tolerance) {
			break;
		}
		alpha = a;
		a = a - (3.0*m*a*a*pow(M,a) + 
			2.0*a*a*pow(M,2.0*a)-
			a*a*pow(M,a)+
			M*a*a-
			m-m*a*a + 
			6.0*m*a*pow(M,2.0*a)-
			2.0*a*a*pow(M,a+1.0)-
			3.0*m*a*a*pow(M,2.0*a)-
			6.0*m*a*pow(M,a)-
			2.0*m*a*pow(M,3.0*a) - 
			3.0*m*pow(M,2.0*a)+
			2.0*m*a-
			a*a*pow(M,3.0*a)+
			a*a*pow(M,2.0*a+1)-
			a*pow(M,2.0*a+1)+
			m*pow(M,3.0*a)+
			a*pow(M,a)+
			m*a*a*pow(M,3.0*a)-
			2.0*a*pow(M,2.0*a)+
			3.0*m*pow(M,a)+
			a*pow(M,3.0*a)+
			2.0*a*pow(M,a+1.0)-
			a*M) / 
			((pow(M,a)-1)*(-1.0*pow(M,a+1.0)+
			M+
			pow(M,2.0*a)-
			pow(M,a)+
			a*a*pow(M,a)*log(M)-
			a*pow(M,a)*log(M)-
			pow(M,a+1.0)*a*a*log(M)+
			pow(M,a+1.0)*a*log(M)));
	}

	printf("alpha: %f\n",a);

	return(0);
}

