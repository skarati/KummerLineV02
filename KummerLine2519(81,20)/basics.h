#ifndef BASICS_H_
#define BASICS_H_

#include <immintrin.h>

#include<malloc.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


#define align __attribute__ ((aligned (32)))

#if !defined (ALIGN16)
	# if defined (__GNUC__)
		# define ALIGN32 __attribute__ ( (aligned (32)))
	# else
		# define ALIGN32 __declspec (align (32))
	# endif
#endif

#define CRYPTO_SCALARBYTES 32
#define CRYPTO_BYTES 48

typedef unsigned char u8;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef __m256i vec;

typedef struct{
	unsigned long long v[9];
}gfe;

typedef struct {
  	vec v[9];
}gfe4x;

const unsigned char base[64] = {
  64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
} ;
gfe p = {268435447ULL, 268435455ULL, 268435455ULL, 268435455ULL, 268435455ULL, 268435455ULL, 268435455ULL, 268435455ULL, 134217727ULL};
gfe adjust_invert = {149130802ULL, 59652323ULL, 238609294ULL, 149130808ULL, 59652323ULL, 238609294ULL, 149130808ULL, 59652323ULL, 104391566ULL};

const vec zero = {0,0,0,0};
const vec plusminusplusminus = {0,-1,0,-1};
const vec and01 = {0x00000000ffffffff,0x00000000ffffffff,0x00000000ffffffff,0x00000000ffffffff};
const vec BABA = {61,101,61,101};//{20,81,20,81};
const vec bazx = {20,81,1,64};//{61,101,1,4};
const vec ab11 = {1,1,20,81};//{1,1,61,101};
const vec zxba = {1,64,20,81};//{1,4,61,101}; //{1,4,61,101};
const vec abxz[2] = {{20,81,1,64}, {1,64,20,81}};//{{61,101,1,4}, {1,4,61,101}};

const u64 mask28 = 0xfffffff;
const u64 mask27 = 0x7ffffff;


inline void makeUnique(gfe *op, gfe *inp) {
        gfe t[2];
        u8 i;

        for(i=0;i<9;i++) t[0].v[i] = inp->v[i];
        for(i=1;i<9;i++) t[1].v[i] = 0; t[1].v[0] = inp->v[0] - ((1ULL<<27)-9);
        if (
                ((inp->v[9]&mask27)==mask27) &&
                ((inp->v[8]&mask28)==mask28) &&
                ((inp->v[7]&mask28)==mask28) &&
                ((inp->v[6]&mask28)==mask28) &&
                ((inp->v[5]&mask28)==mask28) &&
                ((inp->v[4]&mask28)==mask28) &&
                ((inp->v[3]&mask28)==mask28) &&
                ((inp->v[2]&mask28)==mask28) &&
                ((inp->v[1]&mask28)==mask28) &&
                (inp->v[0]>((1ULL<<27)-10))
        ) {
                for(i=0;i<9;i++) op->v[i] = t[1].v[i];
        } else {
                for(i=0;i<9;i++) op->v[i] = t[0].v[i];
        }
}

void convert_ctoi(gfe *r64, const unsigned char r[32]){
	r64->v[0]  = r[0];
	r64->v[0] |= ((u32)r[1] << 8);
	r64->v[0] |= ((u32)r[2] << 16);
	r64->v[0] |= (((u32)(r[3]&0x0f)) << 24); //28

	r64->v[1]  = ((u32)(r[3]&0xf0)>>4);
	r64->v[1] |= ((u32)r[4] << 4);
	r64->v[1] |= ((u32)r[5] << 12);
	r64->v[1] |= (((u32)(r[6]))<< 20);//56

	r64->v[2]  = r[7];
	r64->v[2] |= ((u32)r[8] << 8);
	r64->v[2] |= ((u32)r[9] << 16);
	r64->v[2] |= (((u32)(r[10]&0x0f)) << 24);//84

	r64->v[3]  = ((u32)(r[10]&0xf0)>>4);
	r64->v[3] |= ((u32)r[11] << 4);
	r64->v[3] |= ((u32)r[12] << 12);
	r64->v[3] |= ((u32)r[13]<< 20);//112

	r64->v[4]  = r[14];
	r64->v[4] |= ((u32)r[15] << 8);
	r64->v[4] |= ((u32)r[16] << 16);
	r64->v[4] |= (((u32)(r[17]&0x0f)) << 24);//140

	r64->v[5]  = ((u32)(r[17]&0xf0)>>4);
	r64->v[5] |= ((u32)r[18] << 4);
	r64->v[5] |= ((u32)r[19] << 12);
	r64->v[5] |= (((u32)(r[20]))<< 20);//168

	r64->v[6]  = r[21];
	r64->v[6] |= ((u32)r[22] << 8);
	r64->v[6] |= ((u32)r[23] << 16);
	r64->v[6] |= (((u32)(r[24]&0x0f)) << 24);//196

	r64->v[7]  = ((u32)(r[24]&0xf0)>>4);
	r64->v[7] |= ((u32)r[25] << 4);
	r64->v[7] |= ((u32)r[26] << 12);
	r64->v[7] |= ((u32)r[27]<< 20);//224

	r64->v[8]  = r[28];
	r64->v[8] |= ((u32)r[29] << 8);
	r64->v[8] |= ((u32)r[30] << 16);
	r64->v[8] |= (((u32)(r[31]&0x0f)) << 24);//252
}

void convert_itoc(gfe *r64, unsigned char r[32]){
	r[0] = r64->v[0] & 0xff;
	r64->v[0] = r64->v[0]>>8;
	r[1] = r64->v[0] & 0xff;
	r64->v[0] = r64->v[0]>>8;
	r[2] = r64->v[0] & 0xff;
	r64->v[0] = r64->v[0]>>8;
	r[3] = r64->v[0] & 0xf;

	r[3] = r[3]|(r64->v[1] & 0xf) << 4;
	r64->v[1] = r64->v[1] >> 4;
	r[4] = r64->v[1] & 0xff;
	r64->v[1] = r64->v[1]>>8;
	r[5] = r64->v[1] & 0xff;
	r64->v[1] = r64->v[1]>>8;
	r[6] = r64->v[1] & 0xff;

	r[7] = r64->v[2] & 0xff;
	r64->v[2] = r64->v[2]>>8;
	r[8] = r64->v[2] & 0xff;
	r64->v[2] = r64->v[2]>>8;
	r[9] = r64->v[2] & 0xff;
	r64->v[2] = r64->v[2]>>8;
	r[10] = r64->v[2] & 0xf;

	r[10] = r[10]|(r64->v[3] & 0xf) << 4;
	r64->v[3] = r64->v[3] >> 4;
	r[11] = r64->v[3] & 0xff;
	r64->v[3] = r64->v[3]>>8;
	r[12] = r64->v[3] & 0xff;
	r64->v[3] = r64->v[3]>>8;
	r[13] = r64->v[3] & 0xff;

	r[14] = r64->v[4] & 0xff;
	r64->v[4] = r64->v[4]>>8;
	r[15] = r64->v[4] & 0xff;
	r64->v[4] = r64->v[4]>>8;
	r[16] = r64->v[4] & 0xff;
	r64->v[4] = r64->v[4]>>8;
	r[17] = r64->v[4] & 0xf;

	r[17] = r[17]|(r64->v[5] & 0xf) << 4;
	r64->v[5] = r64->v[5] >> 4;
	r[18] = r64->v[5] & 0xff;
	r64->v[5] = r64->v[5]>>8;
	r[19] = r64->v[5] & 0xff;
	r64->v[5] = r64->v[5]>>8;
	r[20] = r64->v[5] & 0xff;

	r[21] = r64->v[6] & 0xff;
	r64->v[6] = r64->v[6]>>8;
	r[22] = r64->v[6] & 0xff;
	r64->v[6] = r64->v[6]>>8;
	r[23] = r64->v[6] & 0xff;
	r64->v[6] = r64->v[6]>>8;
	r[24] = r64->v[6] & 0xf;

	r[24] = r[24]|(r64->v[7] & 0xf) << 4;
	r64->v[7] = r64->v[7] >> 4;
	r[25] = r64->v[7] & 0xff;
	r64->v[7] = r64->v[7]>>8;
	r[26] = r64->v[7] & 0xff;
	r64->v[7] = r64->v[7]>>8;
	r[27] = r64->v[7] & 0xff;

	r[28] = r64->v[8] & 0xff;
	r64->v[8] = r64->v[8]>>8;
	r[29] = r64->v[8] & 0xff;
	r64->v[8] = r64->v[8]>>8;
	r[30] = r64->v[8] & 0xff;
	r64->v[8] = r64->v[8]>>8;
	r[31] = r64->v[8] & 0xf;
}

void gfe4_f_gfe(gfe4x *r64, gfe *m){
  	int i;

  	for (i = 0;i < 9;++i) {
	    	0[(u64 *) &r64->v[i]] = m[0].v[i];
	   	1[(u64 *) &r64->v[i]] = m[1].v[i];
	    	2[(u64 *) &r64->v[i]] = m[2].v[i];
	    	3[(u64 *) &r64->v[i]] = m[3].v[i];
  	}
}

void gfe4_f_gfe_part1(gfe4x *r64, gfe *m){
	int i;
	
	for (i = 0;i < 9;++i){
		0[(u64 *) &r64->v[i]] = 0;
		1[(u64 *) &r64->v[i]] = 0;
		2[(u64 *) &r64->v[i]] = m[0].v[i];
		3[(u64 *) &r64->v[i]] = m[1].v[i];
  	}
}
void gfe4_f_gfe_part2(gfe4x *r64, gfe *m){
	int i;
	
	for (i = 0;i < 9;++i){
		0[(u64 *) &r64->v[i]] = m[0].v[i];
		1[(u64 *) &r64->v[i]] = m[1].v[i];
  	}
}


void gfe4_t_gfe(gfe4x *r64, gfe *m){
	int i;

  	for (i = 0;i < 9;++i) {
	    	m[0].v[i] = 0[(u64 *) &r64->v[i]];
	    	m[1].v[i] = 1[(u64 *) &r64->v[i]];
	    	m[2].v[i] = 2[(u64 *) &r64->v[i]];
	    	m[3].v[i] = 3[(u64 *) &r64->v[i]];
  	}
}

/*
void set_base_point(gfe4x *r64_1, gfe4x *r64_2, gfe *m){
	int i;

		0[(u64 *) &r64_1->v[0]] = 61;
		1[(u64 *) &r64_1->v[0]] = 101;
		2[(u64 *) &r64_1->v[0]] = m[1].v[0];
		3[(u64 *) &r64_1->v[0]] = m[0].v[0];
	
	for (i = 1;i < 9;++i){
		0[(u64 *) &r64_1->v[i]] = 0;
		1[(u64 *) &r64_1->v[i]] = 0;
		2[(u64 *) &r64_1->v[i]] = m[1].v[i];
		3[(u64 *) &r64_1->v[i]] = m[0].v[i];
  	}

		0[(u64 *) &r64_2->v[0]] = m[1].v[0];
		1[(u64 *) &r64_2->v[0]] = m[0].v[0];
		2[(u64 *) &r64_2->v[0]] = 61;
		3[(u64 *) &r64_2->v[0]] = 101;
	
	for (i = 1;i < 9;++i){
		0[(u64 *) &r64_2->v[i]] = m[1].v[i];
		1[(u64 *) &r64_2->v[i]] = m[0].v[i];
		2[(u64 *) &r64_2->v[i]] = 0;
		3[(u64 *) &r64_2->v[i]] = 0;
  	}

}
*/

void set_base_point(gfe4x *r64, gfe *m){
	int i;

		0[(u64 *) &r64[0].v[0]] = 61;
		1[(u64 *) &r64[0].v[0]] = 101;
		2[(u64 *) &r64[0].v[0]] = m[1].v[0];
		3[(u64 *) &r64[0].v[0]] = m[0].v[0];
	
	for (i = 1;i < 9;++i){
		0[(u64 *) &r64[0].v[i]] = 0;
		1[(u64 *) &r64[0].v[i]] = 0;
		2[(u64 *) &r64[0].v[i]] = m[1].v[i];
		3[(u64 *) &r64[0].v[i]] = m[0].v[i];
  	}

		0[(u64 *) &r64[1].v[0]] = m[1].v[0];
		1[(u64 *) &r64[1].v[0]] = m[0].v[0];
		2[(u64 *) &r64[1].v[0]] = 61;
		3[(u64 *) &r64[1].v[0]] = 101;
	
	for (i = 1;i < 9;++i){
		0[(u64 *) &r64[1].v[i]] = m[1].v[i];
		1[(u64 *) &r64[1].v[i]] = m[0].v[i];
		2[(u64 *) &r64[1].v[i]] = 0;
		3[(u64 *) &r64[1].v[i]] = 0;
  	}

}

vec setVec(u8 *charStr) {

	vec *vecVal;
	vecVal = (vec *)charStr;
	return *vecVal;

}

void printVec(vec *inputVec) {

	u8 *val,i,j;
	val = (u8 *)inputVec;
	for(i=0;i<3;i++) {for(j=0;j<8;j++) printf("%3u, ",val[i*8+j]); printf("\n");}
	for(i=24;i<31;i++) printf("%3u, ",val[i]); printf("%3u ",val[31]); printf("\n");
}

void printVecFirst64Bits(vec *inputVec) { // prints only the first 64 bits

	u8 *val,i,j;
	val = (u8 *)inputVec;
	for(i=0;i<7;i++) printf("%3u, ",val[i]); printf("%3u ",val[7]); printf("\n");
}


void print_c(char *n){
	int i;
	for(i=0; i<32; i++)
		printf("%hhu, ",n[i]);
	printf("\n");

}
void print_gfe(gfe *r64){
	int i;
	for(i=0; i<9; i++)
		printf("%llu, ",r64->v[i]);
	printf("\n");
}

#endif


