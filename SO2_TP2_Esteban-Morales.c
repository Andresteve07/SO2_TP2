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
	int pulses_total = 0;
	while(fread(&valid_samples,sizeof(uint16_t),1,ptr_myfile)==1){
		printf("Valid Samples: %d",valid_samples);		
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
		pulses_total++;
		printf("PULSES TOTAL: %i\n",pulses_total);
	}
	
	
	/*
	for(int i=0;i<valid_samples;i++){
		printf("Val %i, %f \n",i,hq_reader[i]);
	}
	*/
	
	fclose(ptr_myfile);
	return 0;
}

