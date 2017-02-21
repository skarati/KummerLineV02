#include "basics.h"
#include "kummer.h"
#include "measurement.h"

int main(){
	int i,j,k,bit;
	unsigned int dep=0;
	char c;
	//unsigned char bit;
	gfe4x npi,np,npt,temp2;
	gfe work[4],re[4],temp,x,z,xinvz;
	
	//sample scalar: you can change it and also can pass it as an argument of the main
	unsigned char n[33] = { 248, 120, 184, 82, 8, 149, 129, 34, 27, 154, 224, 137, 112, 239, 152, 95, 151,146, 255, 170, 108, 82, 3, 239, 167, 50, 77, 129, 124, 221, 102, 182, 51};
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

	printf("Total CPU cycles for fixed-base scalar multiplication: %.2f.\n", RDTSC_total_clk);
	print_c(op);

	unsigned char base_rand[64];
	for(i=0;i<32;i++) base_rand[i]=op[i];
	base_rand[32] = 1;
	for(i=33;i<64;i++) base_rand[i]=0;
	MEASURE({
		scalar_mult_var_base(op, base_rand, n); n[0] = op[0];
	});
	printf("Total CPU cycles for variable-base scalar multiplication: %.2f.\n", RDTSC_total_clk);
	print_c(op);

	return 0;
}

