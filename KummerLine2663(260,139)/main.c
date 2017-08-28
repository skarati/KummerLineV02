#include "basics.h"
#include "kummer.h"
#include "measurement.h"
#include "gfe54.h"
//#include "gfe54_namespace.h"

int main(){
	int i,j,k,bit;
	unsigned int dep=0;
	char c;
	//unsigned char bit;
	gfe4x npi,np,npt,temp2;
	gfe work[4],re[4],temp,x,z,xinvz;
	gfe54 m54,n54,op54;
	
	printf("\nKL266(260,139)===\n");	
	
	//sample scalar: you can change it and also can pass it as an argument of the main
	unsigned char n[33] = { 248, 140, 32, 244, 142, 110, 98, 104, 139, 69, 181, 115, 219, 202, 144, 188, 15, 199, 29, 26, 236, 6, 78, 213, 139, 95, 154, 118, 245, 82, 50, 209, 32};
	unsigned char op[34];


	convert_ctoi(&work[0],base);
	convert_ctoi(&work[1],base+34);
		
	gfe4_f_gfe_part1(&npi, work);
	gfe4x_hadamard(&npi, &npi);
	sq_gfe4(&npi, &npi);
	mulconst_gfe4(&npi, &npi, &BABA);
	gfe4x_hadamard(&npi, &npi);
	sq_gfe4(&npi, &npi);
	mulconst_gfe4(&npi, &npi, &ab11);
	gfe4_f_gfe_part2(&npi, work);

	

	MEASURE({
		scalar_mult_fixed_base(op, npi, n); //n[0] = op[0];
	});

	printf("Total CPU cycles for fixed-base scalar multiplication Min: %.2f.\n", RDTSC_clk_min);
	printf("Total CPU cycles for fixed-base scalar multiplication Median: %.2f.\n", RDTSC_clk_median);
	printf("Total CPU cycles for fixed-base scalar multiplication Max: %.2f.\n", RDTSC_clk_max);
	print_c(op);

	unsigned char base_rand[64];
	for(i=0;i<32;i++) base_rand[i]=op[i];
	base_rand[32] = 1;
	for(i=33;i<64;i++) base_rand[i]=0;
	MEASURE({
		scalar_mult_var_base(op, base_rand, n); n[0] = op[0];
	});
	printf("Total CPU cycles for variable-base scalar multiplication Min: %.2f.\n", RDTSC_clk_min);
	printf("Total CPU cycles for variable-base scalar multiplication Median: %.2f.\n", RDTSC_clk_median);
	printf("Total CPU cycles for variable-base scalar multiplication Max: %.2f.\n", RDTSC_clk_max);
	print_c(op);

	return 0;
}

