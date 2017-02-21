#include "basics.h"
#include "kummer.h"
#include "measurement.h"

int main(){
	int i,j,k,bit;
	unsigned int dep=0;
	char c;
	//unsigned char bit;
	gfe4x npi,np,npt,temp2;
	gfe temp,work[4],re[4],x,z,xinvz;
	
	//sample scalar: you can change it and also can pass it as an argument of the main
	unsigned char n[31] = { 141, 110, 81, 215, 193, 226, 230, 93, 155, 59, 99, 26, 126, 24, 124, 160, 153,
71, 144, 248, 170, 228, 111, 35, 211, 163, 237, 228, 80, 143, 236};
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
		scalar_mult_fixed_base(op, npi, n);
	});

	printf("Total CPU cycles for fixed-base scalar multiplication: %.2f.\n", RDTSC_total_clk);
	print_c(op);

	unsigned char base_rand[64];
	for(i=0;i<32;i++) base_rand[i]=op[i];
	base_rand[32] = 1;
	for(i=33;i<64;i++) base_rand[i]=0;
	MEASURE({
		scalar_mult_var_base(op, base_rand, n);
	});
	printf("Total CPU cycles for variable-base scalar multiplication: %.2f.\n", RDTSC_total_clk);
	print_c(op);

	return 0;
}

