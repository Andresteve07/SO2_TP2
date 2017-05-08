/*
 * prueba_00.c
 * 
 * Copyright 2017 Esteban Morales <steve-mate-ultra@steve-mate>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

struct pulse
	{
		int valid_samples;
		float *vi;
		float *vq;
		float *hi;
		float *hq;
	};
	
struct pulse_gates
	{
		float *vi_gates;
		float *vq_gates;
		float *hi_gates;
		float *hq_gates;
	};

float* calculate_gates(int valid_samples, float* sample_array){
	int samples_per_gate = 0;
	samples_per_gate = (int)ceil((0.5*valid_samples)/250);
	//float* gates = malloc((valid_samples/samples_per_gate)* sizeof(float));
	float* gates = malloc(500* sizeof(float));
	int summand_counter = 0;
	float sum = 0.0f;
	int gate_count = 0;
	for(int i=1;i<(valid_samples-1);i++){
		sum = sum + sample_array[i];
		summand_counter++;
		if(i%samples_per_gate == 0){
			//if((sum/samples_per_gate)!=0.0)
				//printf("HOLO %f\n", (float)(sum/samples_per_gate));
			gates[gate_count] = sum/samples_per_gate;
			sum = 0.0f;
			summand_counter = 0;
			gate_count++;
		} else {
			if(i==(valid_samples-2)){
				gates[gate_count] = sum/summand_counter;
				sum = 0.0f;
				gate_count++;
			}
		}
	}
	return gates;
}

void print_complex(float* complex_n){
	printf("[%.8f ; %.8f]\n",complex_n[0],complex_n[1]);
}

float* complex_add(float*complex_a,float*complex_b){
	//float result[2] = {complex_a[0]+complex_b[0],complex_a[1]+complex_b[1]};
	//print_complex(complex_a);
	//print_complex(complex_b);
	//printf("SUMA: ");
	float *result = malloc(sizeof(float)*2);
	result[0] = complex_a[0]+complex_b[0];
	result[1] = complex_a[1]+complex_b[1];
	//print_complex(result);
	return result;
}



float* complex_prod(float* complex_a, float* complex_b){
	complex_a[1] = - complex_a[1];
	//print_complex(complex_a);
	//print_complex(complex_b);
	//printf("MULT: ");
	float *result = malloc(sizeof(float)*2);
	result[0] = (complex_a[0]*complex_b[0])-(complex_a[1]*complex_b[1]);
	result[1] = (complex_a[0]*complex_b[1])+(complex_a[1]*complex_b[0]);
	//print_complex(result);
	return result;
}
	
int main(int argc, char **argv)
{
	
	FILE *ptr_myfile;
	//struct rec my_record;

	ptr_myfile=fopen("pulsos.iq","rb");
	if (!ptr_myfile)
	{
		printf("Unable to open file!");
		return 1;
	}
	
	uint16_t valid_samples = 0;
	int pulse_count = 0;
	struct pulse pulses[100];
	printf("PULSES size: %lu and length: %i",sizeof(pulses), (int)(sizeof(pulses)/sizeof(struct pulse)));
	while(fread(&valid_samples,sizeof(uint16_t),1,ptr_myfile)==1){
		//printf("Valid Samples: %d",valid_samples);		
		float *vi_reader = malloc(valid_samples* sizeof(float));
		if(fread(vi_reader,valid_samples* sizeof(float),1,ptr_myfile)!=1){
			printf("FAIL VI\n");
			break;
		}
		
		float *vq_reader = malloc(valid_samples* sizeof(float));
		if(fread(vq_reader,valid_samples* sizeof(float),1,ptr_myfile)!=1){
			printf("FAIL VQ\n");
			break;
		}
		
		float *hi_reader = malloc(valid_samples* sizeof(float));
		if(fread(hi_reader,valid_samples* sizeof(float),1,ptr_myfile)!=1){
			printf("FAIL HI\n");
			break;
		}
		
		float *hq_reader = malloc(valid_samples* sizeof(float));
		if(fread(hq_reader,valid_samples* sizeof(float),1,ptr_myfile)!=1){
			printf("FAIL HQ\n");
			break;
		}
		//printf("PULSES COUNT: %i\n",pulse_count);
		struct pulse pulse_record;
		pulse_record.valid_samples = valid_samples;
		pulse_record.vi = vi_reader;
		pulse_record.vq = vq_reader;
		pulse_record.hi = hi_reader;
		pulse_record.hq = hq_reader;
		pulses[pulse_count] = pulse_record;
		pulse_count++;
	}
	
	struct pulse_gates pulse_gates_array[pulse_count];
	//Loops over pulses array and calculates all gates for each pulse.
	for(int p=0;p<pulse_count;p++){
		struct pulse_gates pg;
		//printf("PULSE NO: %i\n",p);
		pg.vi_gates = calculate_gates(pulses[p].valid_samples, pulses[p].vi);
		pg.vq_gates = calculate_gates(pulses[p].valid_samples, pulses[p].vq);
		pg.hi_gates = calculate_gates(pulses[p].valid_samples, pulses[p].hi);
		pg.hq_gates = calculate_gates(pulses[p].valid_samples, pulses[p].hq);
		pulse_gates_array[p] = pg;	
	}
	printf("GATES PULSE 0: %f",pulse_gates_array[0].vi_gates[12]);
	printf("PULSE GATES ARRAY: %lu\n",sizeof(pulse_gates_array));
		float* corr_v_gates[500];
		float* corr_h_gates[500];
	
	for(int g=0;g<500;g++){
		//if(g==2)
			//exit(0);
		float correlated_v_gate[2];
		float correlated_h_gate[2];
		
		//float complex_v_sum[2]={0,0};
		//float complex_h_sum[2]={0,0};
		
		float *complex_v_sum = malloc(sizeof(float)*2);
		complex_v_sum[0]=0.0f;
		complex_v_sum[1]=0.0f;
		float *complex_h_sum = malloc(sizeof(float)*2);
		complex_h_sum[0]=0.0f;
		complex_h_sum[1]=0.0f;
		
		for(int p=1;p<(pulse_count-1);p++){
		//500
		//if(p==70)
			//exit(0);
		float v_gate[2] 			= {pulse_gates_array[p].vi_gates[g], pulse_gates_array[p].vq_gates[g]};
		float v_gate_next_pulse[2]	= {pulse_gates_array[p+1].vi_gates[g], pulse_gates_array[p+1].vq_gates[g]};
		float h_gate[2]				= {pulse_gates_array[p].hi_gates[g], pulse_gates_array[p].hq_gates[g]};
		float h_gate_next_pulse[2]	= {pulse_gates_array[p+1].hi_gates[g], pulse_gates_array[p+1].hq_gates[g]};
		
		complex_v_sum = complex_add(complex_v_sum,complex_prod(v_gate,v_gate_next_pulse));
		//print_complex(complex_v_sum);
	
		complex_h_sum = complex_add(complex_h_sum,complex_prod(h_gate,h_gate_next_pulse));
		//print_complex(complex_h_sum);
		}
		
		correlated_v_gate[0] = complex_v_sum[0]/pulse_count;
		correlated_v_gate[1] = complex_v_sum[1]/pulse_count;
		
		correlated_h_gate[0] = complex_h_sum[0]/pulse_count;
		correlated_h_gate[1] = complex_h_sum[1]/pulse_count;
		
		//print_complex(correlated_v_gate);
		//print_complex(correlated_h_gate);
		corr_v_gates[g] = correlated_v_gate;
		corr_h_gates[g] = correlated_h_gate;
		printf("GATE: %i  ",g);
		//print_complex(corr_v_gates[g]);
		print_complex(corr_h_gates[g]);
	}
	
	print_complex(corr_v_gates[0]);
	print_complex(corr_h_gates[0]);
	/*
	for(int i=0;i<500;i++){
		printf("V GATE real: %i, %.15f     ",i,corr_v_gates[i][0]);
		printf("H GATE real: %i, %.15f \n",i,corr_h_gates[i][0]);
	}
	*/
	fclose(ptr_myfile);
	return 0;
}

