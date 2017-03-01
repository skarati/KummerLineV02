#ifndef KUMMER_H_
#define KUMMER_H_
#include "vecmult.h"
#include "mult.h"

inline void mul_gfe(gfe *r32, gfe *m, gfe *n);
inline void sq_gfe(gfe *r32, gfe *m);
//inline unsigned int gt0(gfe *m);
//inline unsigned int ugtv(gfe *m, gfe *n);
//inline void div2(gfe *m);
//inline void mul2(gfe *m);
//inline void addp(gfe *m, gfe *p);
//inline void subuv(gfe *op, gfe *m, gfe *n);
//inline void adduv(gfe *op, gfe *m, gfe *n);
inline void invert(gfe *op, gfe *m);

inline void gfe4x_permute(gfe4x *op,gfe4x *ip, int hl);
inline void gfe4x_hadamard(gfe4x *op, gfe4x *ip);
inline void gfe4x_hadamardUnreduced(gfe4x *op, gfe4x *ip);
inline void mul_gfe4(gfe4x *r64, gfe4x *m, gfe4x *n);
inline void sq_gfe4(gfe4x *r64, gfe4x *m);
inline void mulconst_gfe4(gfe4x *r64, gfe4x *a, const vec *b);
inline void mulconst_gfe4Unreduced(gfe4x *r64, gfe4x *a, const vec *b);
inline u64 scalar_mult_fixed_base(unsigned char op[32], gfe4x base, unsigned char n[32]);
inline u64 scalar_mult_var_base(unsigned char op[32], unsigned char base_rand[64], unsigned char n[32]);

inline u64 scalar_mult_fixed_base(unsigned char op[32], gfe4x base, unsigned char n[32]){
	int bit, i, j, k;
	gfe4x np,npt;
	gfe re[4],x,z,temp,xinvz;
	
	np = base;
	gfe4_t_gfe(&np, re);
	bit = 0;
	j = 7;	
	while(bit == 0){
		bit = (n[31]>>j) & 1;
		j--;
	}
	k = 1;
	i=30;
  	for(i=31;i>=0;i--){
    		for(;j>=0;j--){
			bit = (n[i]>>j) & 1;
			gfe4x_hadamard(&np, &np);
			gfe4x_permute(&npt,&np,bit);
			mul_gfe4(&np, &np, &npt);
			//mulconst_gfe4(&np, &np, &BABA);
			mulconst_gfe4Unreduced(&np, &np, &BABA);
			gfe4x_hadamard(&np, &np);
			sq_gfe4(&np, &np);
			//mulconst_gfe4(&np, &np, &abxz[bit]);
			mulconst_gfe4Unreduced(&np, &np, &abxz[bit]);
		}

		j=7;
	}
	VECREDUCEPARTB25519((&np)->v[0],(&np)->v[1],(&np)->v[2],(&np)->v[3],(&np)->v[4],(&np)->v[5],(&np)->v[6],(&np)->v[7],(&np)->v[8],(&np)->v[9]);
	gfe4_t_gfe(&np, re);
	x = re[0];
	z = re[1];
	invert(&temp,&z);
	mul_gfe(&temp,&x,&temp);
	REDUCEPARTB25519(temp.v[0],temp.v[1],temp.v[2],temp.v[3],temp.v[4],temp.v[5],temp.v[6],temp.v[7],temp.v[8],temp.v[9]);
	REDUCEPARTB25519(temp.v[0],temp.v[1],temp.v[2],temp.v[3],temp.v[4],temp.v[5],temp.v[6],temp.v[7],temp.v[8],temp.v[9]);
	makeUnique(&temp,&temp);
	convert_itoc(&temp,op);

	return xinvz.v[0];

}

inline u64 scalar_mult_var_base(unsigned char op[32], unsigned char base_rand[64], unsigned char n[32]){
	int bit, i, j, k;
	gfe4x np,npt;
	gfe work[4],re[4],x,z,temp,xinvz;
	gfe4x temp2,pabxz[2],pzxba;
		
	convert_ctoi(&work[0],base_rand);
	convert_ctoi(&work[1],base_rand+32);
	set_base_point(pabxz,work);
	gfe4_f_gfe_part1(&np, work);
	gfe4x_hadamard(&np, &np);
	sq_gfe4(&np, &np);
	mulconst_gfe4(&np, &np, &BABA);
	gfe4x_hadamard(&np, &np);
	sq_gfe4(&np, &np);
	mulconst_gfe4(&np, &np, &ab11);
	gfe4_f_gfe_part2(&np, work);

	bit = 0;
	j = 7;	
	while(bit == 0){
		bit = (n[31]>>j) & 1;
		j--;
	}
	k = 1;
	
	
  	for(i=31;i>=0;i--){
    		for(;j>=0;j--){
			bit = (n[i]>>j) & 1;
			gfe4x_hadamard(&np, &np);
			//gfe4x_hadamardUnreduced(&np, &np);
			gfe4x_permute(&npt,&np,bit);
			mul_gfe4(&np, &np, &npt);
			mulconst_gfe4Unreduced(&np, &np, &BABA);
			gfe4x_hadamard(&np, &np);
			sq_gfe4(&np, &np);
			mul_gfe4(&np, &np, &pabxz[bit]);
		}
		j=7;
	}
	gfe4_t_gfe(&np, re);
	x = re[0];
	z = re[1];
	invert(&temp,&z);
	mul_gfe(&temp,&x,&temp);
	REDUCEPARTB25519(temp.v[0],temp.v[1],temp.v[2],temp.v[3],temp.v[4],temp.v[5],temp.v[6],temp.v[7],temp.v[8],temp.v[9]);
	REDUCEPARTB25519(temp.v[0],temp.v[1],temp.v[2],temp.v[3],temp.v[4],temp.v[5],temp.v[6],temp.v[7],temp.v[8],temp.v[9]);
	makeUnique(&temp,&temp);
	convert_itoc(&temp,op);

	return xinvz.v[0];
}

inline void gfe4x_permute(gfe4x *op,gfe4x *ip, int hl){
	int i,a,b;
	int hl0,hl1,hl2,hl3;
	hl0 = 4*hl; hl1 = hl0+1; hl2 = hl0+2; hl3 = hl0+3;
	vec perm_mask = _mm256_set_epi32(hl3,hl2,hl1,hl0,hl3,hl2,hl1,hl0);
	op->v[0] = _mm256_permutevar8x32_epi32(ip->v[0],perm_mask);
	op->v[1] = _mm256_permutevar8x32_epi32(ip->v[1],perm_mask);
	op->v[2] = _mm256_permutevar8x32_epi32(ip->v[2],perm_mask);
	op->v[3] = _mm256_permutevar8x32_epi32(ip->v[3],perm_mask);
	op->v[4] = _mm256_permutevar8x32_epi32(ip->v[4],perm_mask);
	op->v[5] = _mm256_permutevar8x32_epi32(ip->v[5],perm_mask);
	op->v[6] = _mm256_permutevar8x32_epi32(ip->v[6],perm_mask);
	op->v[7] = _mm256_permutevar8x32_epi32(ip->v[7],perm_mask);
	op->v[8] = _mm256_permutevar8x32_epi32(ip->v[8],perm_mask);
	op->v[9] = _mm256_permutevar8x32_epi32(ip->v[9],perm_mask);
}

inline void mul_gfe(gfe *r64, gfe *m, gfe *n){

	u64 t[19];

	MULT10(t[0],t[1],t[2],t[3],t[4],t[5],t[6],t[7],t[8],t[9],t[10],t[11],t[12],t[13],t[14],t[15],t[16],t[17],t[18],
	m->v[0], m->v[1], m->v[2], m->v[3], m->v[4], m->v[5], m->v[6], m->v[7], m->v[8], m->v[9],
	n->v[0], n->v[1], n->v[2], n->v[3], n->v[4], n->v[5], n->v[6], n->v[7], n->v[8], n->v[9]);

	REDUCE25519(r64->v[0],r64->v[1],r64->v[2],r64->v[3],r64->v[4],r64->v[5],r64->v[6],r64->v[7],r64->v[8],r64->v[9],t);

}

inline void sq_gfe(gfe *r64, gfe *m){

	u64 t[19];

	SQ10(t[0],t[1],t[2],t[3],t[4],t[5],t[6],t[7],t[8],t[9],t[10],t[11],t[12],t[13],t[14],t[15],t[16],t[17],t[18],
	m->v[0], m->v[1], m->v[2], m->v[3], m->v[4], m->v[5], m->v[6], m->v[7], m->v[8], m->v[9]);

	REDUCE25519(r64->v[0],r64->v[1],r64->v[2],r64->v[3],r64->v[4],r64->v[5],r64->v[6],r64->v[7],r64->v[8],r64->v[9],t);
}


inline void mul_gfe4(gfe4x *r64, gfe4x *m, gfe4x *n) {

	vec t[19]; 

	VECMULT10(t[0],t[1],t[2],t[3],t[4],t[5],t[6],t[7],t[8],t[9],t[10],t[11],t[12],t[13],t[14],t[15],t[16],t[17],t[18],
	m->v[0], m->v[1], m->v[2], m->v[3], m->v[4], m->v[5], m->v[6], m->v[7], m->v[8], m->v[9],
	n->v[0], n->v[1], n->v[2], n->v[3], n->v[4], n->v[5], n->v[6], n->v[7], n->v[8], n->v[9]);

	
	VECREDUCE25519(r64->v[0],r64->v[1],r64->v[2],r64->v[3],r64->v[4],r64->v[5],r64->v[6],r64->v[7],r64->v[8],r64->v[9], t);
}

inline void sq_gfe4(gfe4x *r64, gfe4x *m){

	vec t[19];

	VECSQ10(t[0],t[1],t[2],t[3],t[4],t[5],t[6],t[7],t[8],t[9],t[10],t[11],t[12],t[13],t[14],t[15],t[16],t[17],t[18],
	m->v[0], m->v[1], m->v[2], m->v[3], m->v[4], m->v[5], m->v[6], m->v[7], m->v[8], m->v[9]);

	VECREDUCE25519(r64->v[0],r64->v[1],r64->v[2],r64->v[3],r64->v[4],r64->v[5],r64->v[6],r64->v[7],r64->v[8],m->v[9], t);
}

/*
const vec hadamardoffset[10] =   {{0, 0x9FFFFFA61,0, 0x9FFFFFA61},
					{0, 0x9FFFFFF61,0, 0x9FFFFFF61},
					{0, 0x9FFFFFF61,0, 0x9FFFFFF61},
					{0, 0x9FFFFFF61,0, 0x9FFFFFF61},
					{0, 0x9FFFFFF61,0, 0x9FFFFFF61},
					{0, 0x9FFFFFF61,0, 0x9FFFFFF61},
					{0, 0x9FFFFFF61,0, 0x9FFFFFF61},
					{0, 0x9FFFFFF61,0, 0x9FFFFFF61},
					{0, 0x4FFFFFF61,0, 0x4FFFFFF61}};











*/
const vec hadamardoffset[10] =   {{0, 0xFFFFFB401,0, 0xFFFFFB401},
					{0, 0xFFFFFFC01,0, 0xFFFFFFC01},
					{0, 0xFFFFFFC01,0, 0xFFFFFFC01},
					{0, 0xFFFFFFC01,0, 0xFFFFFFC01},
					{0, 0xFFFFFFC01,0, 0xFFFFFFC01},
					{0, 0xFFFFFFC01,0, 0xFFFFFFC01},
					{0, 0xFFFFFFC01,0, 0xFFFFFFC01},
					{0, 0xFFFFFFC01,0, 0xFFFFFFC01},
					{0, 0xFFFFFFC01,0, 0xFFFFFFC01},
					{0, 0x7FFFFC01,0, 0x7FFFFC01}};


inline void gfe4x_hadamard(gfe4x *op, gfe4x *ip){
  	int i;
	vec t[10];
	vec temp1,temp2,temp3,temp;

	temp = ip->v[0];
	temp2 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(2,2,0,0));
	temp3 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(3,3,1,1));
	op->v[0] = _mm256_add_epi64(_mm256_add_epi64(temp2,hadamardoffset[0]),_mm256_xor_si256(temp3,plusminusplusminus));

	temp = ip->v[1];
	temp2 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(2,2,0,0));
	temp3 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(3,3,1,1));
	op->v[1] = _mm256_add_epi64(_mm256_add_epi64(temp2,hadamardoffset[1]),_mm256_xor_si256(temp3,plusminusplusminus));

	temp = ip->v[2];
	temp2 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(2,2,0,0));
	temp3 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(3,3,1,1));
	op->v[2] = _mm256_add_epi64(_mm256_add_epi64(temp2,hadamardoffset[2]),_mm256_xor_si256(temp3,plusminusplusminus));

	temp = ip->v[3];
	temp2 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(2,2,0,0));
	temp3 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(3,3,1,1));
	op->v[3] = _mm256_add_epi64(_mm256_add_epi64(temp2,hadamardoffset[3]),_mm256_xor_si256(temp3,plusminusplusminus));

	temp = ip->v[4];
	temp2 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(2,2,0,0));
	temp3 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(3,3,1,1));
	op->v[4] = _mm256_add_epi64(_mm256_add_epi64(temp2,hadamardoffset[4]),_mm256_xor_si256(temp3,plusminusplusminus));

	temp = ip->v[5];
	temp2 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(2,2,0,0));
	temp3 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(3,3,1,1));
	op->v[5] = _mm256_add_epi64(_mm256_add_epi64(temp2,hadamardoffset[5]),_mm256_xor_si256(temp3,plusminusplusminus));

	temp = ip->v[6];
	temp2 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(2,2,0,0));
	temp3 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(3,3,1,1));
	op->v[6] = _mm256_add_epi64(_mm256_add_epi64(temp2,hadamardoffset[6]),_mm256_xor_si256(temp3,plusminusplusminus));

	temp = ip->v[7];
	temp2 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(2,2,0,0));
	temp3 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(3,3,1,1));
	op->v[7] = _mm256_add_epi64(_mm256_add_epi64(temp2,hadamardoffset[7]),_mm256_xor_si256(temp3,plusminusplusminus));

	temp = ip->v[8];
	temp2 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(2,2,0,0));
	temp3 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(3,3,1,1));
	op->v[8] = _mm256_add_epi64(_mm256_add_epi64(temp2,hadamardoffset[8]),_mm256_xor_si256(temp3,plusminusplusminus));

	temp = ip->v[9];
	temp2 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(2,2,0,0));
	temp3 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(3,3,1,1));
	op->v[9] = _mm256_add_epi64(_mm256_add_epi64(temp2,hadamardoffset[9]),_mm256_xor_si256(temp3,plusminusplusminus));

	VECREDUCEPARTB25519(op->v[0],op->v[1],op->v[2],op->v[3],op->v[4],op->v[5],op->v[6],op->v[7],op->v[8],op->v[9]);
	
}

inline void gfe4x_hadamardUnreduced(gfe4x *op, gfe4x *ip){
  	int i;
	vec t[10];
	vec temp1,temp2,temp3,temp;

	temp = ip->v[0];
	temp2 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(2,2,0,0));
	temp3 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(3,3,1,1));
	op->v[0] = _mm256_add_epi64(_mm256_add_epi64(temp2,hadamardoffset[0]),_mm256_xor_si256(temp3,plusminusplusminus));

	temp = ip->v[1];
	temp2 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(2,2,0,0));
	temp3 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(3,3,1,1));
	op->v[1] = _mm256_add_epi64(_mm256_add_epi64(temp2,hadamardoffset[1]),_mm256_xor_si256(temp3,plusminusplusminus));

	temp = ip->v[2];
	temp2 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(2,2,0,0));
	temp3 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(3,3,1,1));
	op->v[2] = _mm256_add_epi64(_mm256_add_epi64(temp2,hadamardoffset[2]),_mm256_xor_si256(temp3,plusminusplusminus));

	temp = ip->v[3];
	temp2 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(2,2,0,0));
	temp3 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(3,3,1,1));
	op->v[3] = _mm256_add_epi64(_mm256_add_epi64(temp2,hadamardoffset[3]),_mm256_xor_si256(temp3,plusminusplusminus));

	temp = ip->v[4];
	temp2 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(2,2,0,0));
	temp3 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(3,3,1,1));
	op->v[4] = _mm256_add_epi64(_mm256_add_epi64(temp2,hadamardoffset[4]),_mm256_xor_si256(temp3,plusminusplusminus));

	temp = ip->v[5];
	temp2 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(2,2,0,0));
	temp3 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(3,3,1,1));
	op->v[5] = _mm256_add_epi64(_mm256_add_epi64(temp2,hadamardoffset[5]),_mm256_xor_si256(temp3,plusminusplusminus));

	temp = ip->v[6];
	temp2 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(2,2,0,0));
	temp3 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(3,3,1,1));
	op->v[6] = _mm256_add_epi64(_mm256_add_epi64(temp2,hadamardoffset[6]),_mm256_xor_si256(temp3,plusminusplusminus));

	temp = ip->v[7];
	temp2 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(2,2,0,0));
	temp3 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(3,3,1,1));
	op->v[7] = _mm256_add_epi64(_mm256_add_epi64(temp2,hadamardoffset[7]),_mm256_xor_si256(temp3,plusminusplusminus));

	temp = ip->v[8];
	temp2 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(2,2,0,0));
	temp3 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(3,3,1,1));
	op->v[8] = _mm256_add_epi64(_mm256_add_epi64(temp2,hadamardoffset[8]),_mm256_xor_si256(temp3,plusminusplusminus));
	
	temp = ip->v[9];
	temp2 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(2,2,0,0));
	temp3 = _mm256_permute4x64_epi64 (temp,_MM_SHUFFLE(3,3,1,1));
	op->v[9] = _mm256_add_epi64(_mm256_add_epi64(temp2,hadamardoffset[9]),_mm256_xor_si256(temp3,plusminusplusminus));
}


inline void mulconst_gfe4Unreduced(gfe4x *r64, gfe4x *a, const vec *b){
	
	r64->v[0] = _mm256_mul_epi32(a->v[0],*b);
	r64->v[1] = _mm256_mul_epi32(a->v[1],*b);
	r64->v[2] = _mm256_mul_epi32(a->v[2],*b);
	r64->v[3] = _mm256_mul_epi32(a->v[3],*b);
	r64->v[4] = _mm256_mul_epi32(a->v[4],*b);
	r64->v[5] = _mm256_mul_epi32(a->v[5],*b);
	r64->v[6] = _mm256_mul_epi32(a->v[6],*b);
	r64->v[7] = _mm256_mul_epi32(a->v[7],*b);
	r64->v[8] = _mm256_mul_epi32(a->v[8],*b);
	r64->v[9] = _mm256_mul_epi32(a->v[9],*b);
}

inline void mulconst_gfe4(gfe4x *r64, gfe4x *a, const vec *b){
	
	r64->v[0] = _mm256_mul_epi32(a->v[0],*b);
	r64->v[1] = _mm256_mul_epi32(a->v[1],*b);
	r64->v[2] = _mm256_mul_epi32(a->v[2],*b);
	r64->v[3] = _mm256_mul_epi32(a->v[3],*b);
	r64->v[4] = _mm256_mul_epi32(a->v[4],*b);
	r64->v[5] = _mm256_mul_epi32(a->v[5],*b);
	r64->v[6] = _mm256_mul_epi32(a->v[6],*b);
	r64->v[7] = _mm256_mul_epi32(a->v[7],*b);
	r64->v[8] = _mm256_mul_epi32(a->v[8],*b);
	r64->v[9] = _mm256_mul_epi32(a->v[9],*b);

	VECREDUCEPARTB25519(r64->v[0],r64->v[1],r64->v[2],r64->v[3],r64->v[4],r64->v[5],r64->v[6],r64->v[7],r64->v[8],r64->v[9]);
}

void invert(gfe *op, gfe *m) { /* modified from the inversion function in the sandy2x code */

	int i;

	gfe z2;
	gfe z9;
	gfe z11;
	gfe z2_5_0;
	gfe z2_10_0;
	gfe z2_20_0;
	gfe z2_50_0;
	gfe z2_100_0;
	gfe t;
	
	/* 2 */ sq_gfe(&z2,m);
	/* 4 */ sq_gfe(&t,&z2);
	/* 8 */ sq_gfe(&t,&t);
	/* 9 */ mul_gfe(&z9,&t,m);
	/* 11 */ mul_gfe(&z11,&z9,&z2);
	/* 22 */ sq_gfe(&t,&z11);
	/* 2^5 - 2^0 = 31 */ mul_gfe(&z2_5_0,&t,&z9);

	/* 2^10 - 2^5 */ sq_gfe(&t,&z2_5_0); for(i=0;i<4;i++) sq_gfe(&t,&t); 
	/* 2^10 - 2^0 */ mul_gfe(&z2_10_0,&t,&z2_5_0);

	/* 2^20 - 2^10 */ sq_gfe(&t,&z2_10_0); for(i=0;i<9;i++) sq_gfe(&t,&t); 
	/* 2^20 - 2^0 */ mul_gfe(&z2_20_0,&t,&z2_10_0);

	/* 2^40 - 2^20 */ sq_gfe(&t,&z2_20_0); for(i=0;i<19;i++) sq_gfe(&t,&t); 
	/* 2^40 - 2^0 */ mul_gfe(&t,&t,&z2_20_0);


	/* 2^50 - 2^10 */ for(i=0;i<10;i++) sq_gfe(&t,&t);
	/* 2^50 - 2^0 */ mul_gfe(&z2_50_0,&t,&z2_10_0);

	/* 2^100 - 2^50 */ sq_gfe(&t,&z2_50_0); for(i=0;i<49;i++) sq_gfe(&t,&t); 
	/* 2^100 - 2^0 */ mul_gfe(&z2_100_0,&t,&z2_50_0);

	/* 2^200 - 2^100 */ sq_gfe(&t,&z2_100_0); for(i=0;i<99;i++) sq_gfe(&t,&t); 
	/* 2^200 - 2^0 */ mul_gfe(&t,&t,&z2_100_0);

	/* 2^250 - 2^50 */ for(i=0;i<50;i++) sq_gfe(&t,&t);
	/* 2^250 - 2^0 */ mul_gfe(&t,&t,&z2_50_0);

	/* 2^255 - 2^5 */ for(i=0;i<5;i++) sq_gfe(&t,&t);
	/* 2^255 - 21 */ mul_gfe(op,&t,&z11);
}

#endif
