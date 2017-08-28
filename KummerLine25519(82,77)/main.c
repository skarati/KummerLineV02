#include "basics.h"
#include "kummer.h"
#include "measurement.h"
#include "gfe51.h"


int main(){
	int i,j,k,bit;
	unsigned int dep=0;
	char c;
	//unsigned char bit;
	gfe4x npi,np,npt,temp2;
	gfe temp,work[4],re[4],x,z,xinvz;
	
	printf("\nKL25519(82,77)===\n");

	//sample scalar: you can change it and also can pass it as an argument of the main
	unsigned char n[32] = {102, 66, 236, 240, 6, 149, 92, 7, 43, 107, 163, 255, 64, 145, 5, 203, 230, 54, 147, 234, 197, 5, 215, 214, 124, 189, 226, 219, 235, 71, 20, 8 };
	unsigned char op[32];

	convert_ctoi(&work[0],base);
	convert_ctoi(&work[1],base+32);
		
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
		scalar_mult_var_base(op, base_rand, n); //n[0] = op[0];
	});
	printf("Total CPU cycles for variable-base scalar multiplication Min: %.2f.\n", RDTSC_clk_min);
	printf("Total CPU cycles for variable-base scalar multiplication Median: %.2f.\n", RDTSC_clk_median);
	printf("Total CPU cycles for variable-base scalar multiplication Max: %.2f.\n", RDTSC_clk_max);
	print_c(op);

	return 0;
}

