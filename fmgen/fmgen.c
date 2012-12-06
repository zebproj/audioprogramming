#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sndfile.h>

#ifndef M_PI
#define		M_PI		3.14159265358979323846264338
#endif

#define 	SR 44100.0
#define	COUNT (SR * 4)

double oscil(double amp, double freq, double *wave, int wavesize, double *phase);

double line(int *cur_time, double a, double time, double b);
int main(void){
	SNDFILE *file;
	SF_INFO  sfinfo;
	int k;
	double *buffer;

	if(!(buffer = malloc (2 * COUNT * sizeof(double)))){
		printf("Malloc failed.\n");
		exit(0);
	}

	memset(&sfinfo, 0, sizeof(sfinfo));

	sfinfo.samplerate = SR;
	sfinfo.frames = COUNT;
	sfinfo.channels = 2;
	sfinfo.format = (SF_FORMAT_WAV | SF_FORMAT_PCM_16);

	if(! (file = sf_open("out.wav", SFM_WRITE, &sfinfo))){
		printf("Error: Not able to open output file!");
		return 1;
	}

int size = 8192;
double table[size];
double table_2[size];
makesine(table,sizeof(table)/sizeof(double));
makecosine(table_2,size);

double mod_index = 3;
double car = 1;
double mod = 1;
double freq = 220.0 * car;
double modfreq = freq * mod;
double vib = freq * mod;
double vibindx = vib/(SR/(double)size);
double index = freq/(SR/(double)size);
//double index = (SR/size)/freq;
int i;
double w = 0.0;
double j = 0.0;
double x = 0.0;
double osc5;
double osc6;
double osc4;
double osc3;
double osc2;
double osc;
//double env;
double env = modfreq*mod_index/COUNT; //mod envelope
double env2 = 0.4/COUNT; //AM envelope

		for (i = 0; i < COUNT; i++){
//			env = line(&i, 0.0, COUNT, 1);
			osc3 = oscil(i*env2*0.8, freq,table_2, size, &w);
			osc2 = oscil(i*env, freq*mod, table_2, size, &x);
			osc = oscil(0.4, freq + osc2, table_2, size, &j);
			//buffer[2 * i] = (osc + osc5 + osc6) * .33;
			buffer[2 * i] = osc;

			//buffer[2 * i + 1] = (osc + osc5 + osc6) * .33;
			buffer[2 * i + 1] = osc;
		}	
/*
	if (sf_write_double (file, buffer, sfinfo.channels * COUNT )!= 
	sfinfo.channels * COUNT)
		puts (sf_strerror (file));
*/
	sf_write_double(file, buffer, sfinfo.channels * COUNT);

	sf_close(file);
	printf("Succesfully generated an FM tone to out.wav\n");
	return 0;
}

double line(int *cur_time, double a, double time, double b){

if(a > b){
printf("A is greater than B, ERROR");
return 0.0;
}

double slope = (b - a)/time;

return *cur_time * slope;

}

int makesine(double *table, int size){
	int i;
	double k, val;
	for(i = 0; i < size; i++){
		val = 2 * M_PI * i / (double)size; 
		*(table + i) = sin(val);
	}
	return 0;
}
int makecosine(double *table, int size){
	int i;
	double k, val;
	for(i = 0; i < size; i++){
		val = 2 * M_PI * i / (double)size; 
		*(table + i) = cos(val);
	}
	return 0;
}

double oscil(double amp, double freq, double *wave, int wavesize, double *phase){
	if(*phase  > wavesize - 1){
		*phase -= (wavesize - 1);
	}
	int x = (int)*phase;
	double val = *(wave + x) * amp;
	*phase += freq/(SR/(double)wavesize);
	return val;
}
