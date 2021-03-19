#include <stdio.h>
#include <math.h>


int main(int argc, char const *argv[])
{
	/* High Pass Filter based on RBJ Cookbook linked above */
	/* Analog Transfer Function for this filter: H(s) = s^2 / (s^2 + s/Q + 1) */

	/* These floating point values are used by the filter code below */
	float Fs = 44100;      /* sample rate in samples per second */
	float Pi = 3.141592;   /* the value of Pi */

	/* These floating point values implement the specific filter type */
	float f0 = 100;                /* cut-off (or center) frequency in Hz */
	float Q = 1.5;                 /* filter Q */
	float w0 = 2 * Pi * f0 / Fs;
	float alpha = sin(w0) / (2 * Q);
	float a0 = 1 + alpha;
	float a1 = -2 * cos(w0);
	float a2 = 1 - alpha;
	float b0 = (1 + cos(w0)) / 2;
	float b1 = -(1 + cos(w0));
	float b2 = (1 + cos(w0)) / 2;

	/* The Buffer[] array holds the incoming samples, PrevSample[] holds intermediate results */
	float Buffer[1024];         /* this array holds 1024 elements numbered 0 through 1023 */
	float PrevSample[3];        /* this array holds 3 elements numbered 0 through 2 */

	/* These integer (whole number) variables are used below to process 1,024 iterations at a time*/
	int I = 0;
	int N = 1024;

	// int counter = 0;

	// unsigned char buffer[1024];
	FILE *fout = fopen("demo_out.raw", "wb"); /*demo_out_no_zero_in_if.raw*/

	FILE *fp = fopen("demo.raw", "rb");
	

	if (fp != NULL) {
		size_t byte_read;
	    // double check this
	    while(fread(Buffer,sizeof(float),N,fp) == N){
	    	
	    	/*This make output double on every single 1024 Buffer twice :)*/
	    	// fwrite(Buffer, sizeof(float), N, fout);
			
			/*TO DO: 1. check if the main buffer Buffer[I] contains less than the full 1024 samples /the last time it runs/ */

			/* The code below executes repeatedly as long as the value of I is less than N */
			/* Since I was initialized to 0 above, and N was set to 1024, this code executes 1,024 times */

	        // I = 0;

	        while (I < N) {     	/* this is the beginning of the code that loops 1,024 times */
				PrevSample[2] = PrevSample[1];   	 /* Slide the samples over one position */
				PrevSample[1] = PrevSample[0];
				PrevSample[0] = Buffer[I];

				// printf("%f\n", PrevSample[1]);

			/*TO DO:/DONE?/ 2. avoid the error when the code first run when I counter has value less than 1 and 2 */

				if (I < 2){
					PrevSample[2] = PrevSample[1];   	 /* Slide the samples over one position */
					PrevSample[1] = PrevSample[0];
					PrevSample[0] = Buffer[I];
					I++;


					// counter++;
					// printf("%d : TEST!!!\n", counter);
				}
				else{
					/* What is the meaning of the formula ??? */
					Buffer[I] = (b0 / a0 * PrevSample[0]) +
								(b1 / a0 * PrevSample[1]) +
								(b2 / a0 * PrevSample[2]) -
								(a1 / a0 * Buffer[I-1]) -
								(a2 / a0 * Buffer[I-2]);
								
					// printf("%d: %f\n", I, Buffer[I]);
					// printf("TEST else STATEMENT\n");
					
					// //printf("%d\n", I);				
					// for (int i = 0; i < N; ++i)
					// {
					// 	printf("%d %f\n", i, Buffer[I]);
					// }
					// return 0;

					I++;			/* increment the counter I by adding 1 */
				}
			}						/* this is the end of the code loop */

			fwrite(Buffer, sizeof(float), N, fout);

		}
	fclose(fp);
	fclose(fout);
	}
	
	return 0;
}


/* NOTE: 1. Another simplification is omitting code to check if the main buffer contains less than the full 1,024 samples,
 		 which is likely the last time it runs. 
 		 2. Additional code is also needed to avoid an error when the code first runs. 
 		 When the while counter "I" has a value less than 1 or 2, the statements Buffer[I-1] and Buffer[I-2]
 		 reference elements in the array that don't exist */
