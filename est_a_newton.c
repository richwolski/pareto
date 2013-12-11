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
long double b;
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
long double Tolerance;

int
main(int argc, char *argv[])
{
	int c;
	long double m;
	long double M;
	long double alpha;
	long double a;
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
	a = 2.0/logl(M);	/* initial guess */
	for(i=0 ; i < Iterations; i++) {
		if(fabs(alpha-a) < Tolerance) {
			break;
		}
		alpha = a;
		a = a - (3.0*m*a*a*powl(M,a) + 
			2.0*a*a*powl(M,2.0*a)-
			a*a*powl(M,a)+
			M*a*a-
			m-m*a*a + 
			6.0*m*a*powl(M,2.0*a)-
			2.0*a*a*powl(M,a+1.0)-
			3.0*m*a*a*powl(M,2.0*a)-
			6.0*m*a*powl(M,a)-
			2.0*m*a*powl(M,3.0*a) - 
			3.0*m*powl(M,2.0*a)+
			2.0*m*a-
			a*a*powl(M,3.0*a)+
			a*a*powl(M,2.0*a+1)-
			a*powl(M,2.0*a+1)+
			m*powl(M,3.0*a)+
			a*powl(M,a)+
			m*a*a*powl(M,3.0*a)-
			2.0*a*powl(M,2.0*a)+
			3.0*m*powl(M,a)+
			a*powl(M,3.0*a)+
			2.0*a*powl(M,a+1.0)-
			a*M) / 
			((powl(M,a)-1)*(-1.0*powl(M,a+1.0)+
			M+
			powl(M,2.0*a)-
			powl(M,a)+
			a*a*powl(M,a)*logl(M)-
			a*powl(M,a)*logl(M)-
			powl(M,a+1.0)*a*a*logl(M)+
			powl(M,a+1.0)*a*logl(M)));
	}

	printf("alpha: %Lf\n",a);

	return(0);
}

