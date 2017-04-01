#ifndef BASICS_H_
#define BASICS_H_

#include <immintrin.h>

#include<malloc.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "gfe51.h"


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
	unsigned long long v[10];
}gfe;

typedef struct {
  	vec v[10];
}gfe4x;

const unsigned char base[64] = {
  31,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
} ;
gfe p = {67108845ULL, 67108863ULL, 67108863ULL, 67108863ULL, 67108863ULL, 67108863ULL, 67108863ULL, 67108863ULL, 67108863ULL, 2097151ULL};

// gfe adjust_invert = {1ULL, 149130802ULL, 59652323ULL, 238609294ULL, 149130808ULL, 59652323ULL, 238609294ULL, 149130808ULL, 59652323ULL, 104391566ULL};

const vec zero = {0,0,0,0};
const vec plusminusplusminus = {0,-1,0,-1};
const vec and01 = {0x00000000ffffffff,0x00000000ffffffff,0x00000000ffffffff,0x00000000ffffffff};
const vec BABA = {5,159,5,159};//{20,81,20,81};
const vec bazx = {77,82,1,31};//{61,101,1,4};
const vec ab11 = {1,1,77,82};//{1,1,61,101};
const vec zxba = {1,31,77,82};//{1,4,61,101};
const vec abxz[2] = {{77,82,1,31}, {1,31,77,82}};//{{61,101,1,4}, {1,4,61,101}};

const u64 mask26 = 0x3ffffff;
const u64 mask21 = 0x1fffff;
const u64 mask51 = 0x0007FFFFFFFFFFFF;
inline void makeUnique(gfe51 *op, gfe51 *inp) {
        gfe51 t[2];
        u8 i;

        for(i=0;i<5;i++) t[0].v[i] = inp->v[i];
        for(i=1;i<5;i++) t[1].v[i] = 0; t[1].v[0] = inp->v[0] - ((1ULL<<51)-9);
        if (
                ((inp->v[4]&mask51)==mask51) &&
                ((inp->v[3]&mask51)==mask51) &&
                ((inp->v[2]&mask51)==mask51) &&
                ((inp->v[1]&mask51)==mask51) &&
                (inp->v[0]>=((1ULL<<51)-9))
        ) {
                for(i=0;i<5;i++) op->v[i] = t[1].v[i];
        } else {
                for(i=0;i<5;i++) op->v[i] = t[0].v[i];
        }
}


void convert_ctoi(gfe *r64, const unsigned char r[32]){
	r64->v[0]  = r[0];				/*8*/
	r64->v[0] |= ((u64)r[1] << 8);			/*16*/
	r64->v[0] |= ((u64)r[2] << 16);			/*24*/
	r64->v[0] |= (((u64)(r[3]&0x03)) << 24);	/*26*/

	r64->v[1]  = ((u64)(r[3]&0xfc)>>2);		/*6*/
	r64->v[1] |= ((u64)r[4] << 6);			/*14*/
	r64->v[1] |= ((u64)r[5] << 14);			/*22*/
	r64->v[1] |= (((u64)(r[6])&0x0f)<< 22);		/*26*/

	r64->v[2]  = (((u64)(r[6])&0xf0)>> 4);		/*4*/
	r64->v[2] |= ((u64)r[7] << 4);			/*12*/
	r64->v[2] |= ((u64)r[8] << 12);			/*20*/
	r64->v[2] |= (((u64)(r[9]&0x3f)) << 20);	/*26*/

	r64->v[3]  = ((u64)(r[9]&0xc0)>>6);		/*2*/
	r64->v[3] |= ((u64)r[10] << 2);			/*10*/
	r64->v[3] |= ((u64)r[11] << 10);		/*18*/
	r64->v[3] |= ((u64)r[12] << 18);		/*26*/

	r64->v[4]  = r[13];				/*8*/
	r64->v[4] |= ((u64)r[14] << 8);			/*16*/
	r64->v[4] |= ((u64)r[15] << 16);		/*24*/
	r64->v[4] |= (((u64)(r[16]&0x03)) << 24);	/*26*/

	r64->v[5]  = ((u64)(r[16]&0xfc)>>2);		/*6*/
	r64->v[5] |= ((u64)r[17] << 6);			/*14*/
	r64->v[5] |= ((u64)r[18] << 14);		/*22*/
	r64->v[5] |= (((u64)(r[19])&0x0f)<< 22);	/*26*/

	r64->v[6]  = (((u64)(r[19])&0xf0)>> 4);		/*4*/
	r64->v[6] |= ((u64)r[20] << 4);			/*12*/
	r64->v[6] |= ((u64)r[21] << 12);		/*20*/
	r64->v[6] |= (((u64)(r[22]&0x3f)) << 20);	/*26*/

	r64->v[7]  = ((u64)(r[22]&0xc0)>>6);		/*2*/
	r64->v[7] |= ((u64)r[23] << 2);			/*10*/
	r64->v[7] |= ((u64)r[24] << 10);		/*18*/
	r64->v[7] |= ((u64)r[25] << 18);		/*26*/

	r64->v[8]  = r[26];				/*8*/
	r64->v[8] |= ((u64)r[27] << 8);			/*16*/
	r64->v[8] |= ((u64)r[28] << 16);		/*24*/
	r64->v[8] |= (((u64)(r[29]&0x03)) << 24);	/*26*/

	r64->v[9]  = ((u64)(r[29]&0xfc)>>2);		/*6*/
	r64->v[9] |= ((u64)r[30] << 6);			/*14*/
	r64->v[9] |= ((u64)r[31] << 14);		/*21*/
}

void gfe51_pack(gfe *r64, gfe51 *r51){
	r51->v[0]  = r64->v[0]&0x3ffffff;		/*26*/
	r51->v[0] |= (r64->v[1]&0x1ffffff)<<26;		/*51*/

	r51->v[1]  = (r64->v[1] >> 25)&0x1;		/*1*/
	r51->v[1] |= ((r64->v[2]&0x3ffffff)<<1);  	/*27*/
	r51->v[1] |= ((r64->v[3]&0xffffff)<<27); 	/*51*/

	r51->v[2]  = (r64->v[3]&0x3000000) >> 24;	/*2*/
	r51->v[2] |= r64->v[4] << 2;			/*28*/
	r51->v[2] |= (r64->v[5]&0x7fffff) << 28;	/*51*/

	r51->v[3]  = r64->v[5] >> 23;			/*3*/
	r51->v[3] |= (r64->v[6]&0x3ffffff) << 3;	/*29*/
	r51->v[3] |= (r64->v[7]&0x3fffff) << 29;	/*51*/

	r51->v[4]  = (r64->v[7] >> 22)&0xf;		/*4*/
	r51->v[4] |= (r64->v[8]&0x3ffffff) << 4;	/*30*/
	r51->v[4] |= (r64->v[9]&0x1fffff) << 30;	/*51*/
}

void gfe51_unpack(gfe *r64, gfe51 *r51){
	r64->v[0] = r51->v[0] & 0x3ffffff;		/*26*/
	r64->v[1] = (r51->v[0] >> 26)&0x1ffffff;	/*51*/

	r64->v[1] = r64->v[1]|((r51->v[1] & 0x1)<<25);	/*1*/
	r64->v[2] = (r51->v[1] >> 1)&0x3ffffff;		/*27*/
	r64->v[3] = (r51->v[1] >> 27)&0xffffff;		/*51*/

	r64->v[3] = r64->v[3]|((r51->v[2] & 0x3)<<24);	/*2*/
	r64->v[4] = (r51->v[2] >> 2)&0x3ffffff;		/*28*/
	r64->v[5] = (r51->v[2] >> 28)&0x7fffff;		/*51*/

	r64->v[5] = r64->v[5]|((r51->v[3] & 0x7)<<23);	/*3*/
	r64->v[6] = (r51->v[3] >> 3)&0x3ffffff;		/*29*/
	r64->v[7] = (r51->v[3] >> 29)&0x3fffff;		/*51*/

	r64->v[7] = r64->v[7]|((r51->v[4] & 0xf)<<22);	/*4*/
	r64->v[8] = (r51->v[4] >> 4)&0x3ffffff;		/*30*/
	r64->v[9] = (r51->v[4] >> 30)&0x1fffff;		/*51*/

}




void convert_itoc(gfe *r64, unsigned char r[32]){
	r[0] = r64->v[0] & 0xff;			/*8*/
	r64->v[0] = r64->v[0]>>8;			
	r[1] = r64->v[0] & 0xff;			/*16*/
	r64->v[0] = r64->v[0]>>8;
	r[2] = r64->v[0] & 0xff;			/*24*/
	r64->v[0] = r64->v[0]>>8;
	r[3] = r64->v[0] & 0x3;				/*26*/

	r[3] = r[3]|(r64->v[1] & 0x3f) << 2;		/*6*/
	r64->v[1] = r64->v[1] >> 6;			
	r[4] = r64->v[1] & 0xff;			/*14*/	
	r64->v[1] = r64->v[1]>>8;
	r[5] = r64->v[1] & 0xff;			/*22*/
	r64->v[1] = r64->v[1]>>8;
	r[6] = r64->v[1] & 0xf;				/*26*/

	r[6] = r[6]|(r64->v[2] & 0xf) << 4;		/*4*/
	r64->v[2] = r64->v[2] >> 4;						
	r[7] = r64->v[2] & 0xff;			/*12*/
	r64->v[2] = r64->v[2]>>8;
	r[8] = r64->v[2] & 0xff;			/*20*/
	r64->v[2] = r64->v[2]>>8;
	r[9] = r64->v[2] & 0x3f;			/*26*/


	r[9] = r[9]|(r64->v[3] & 0x3) << 6;		/*2*/
	r64->v[3] = r64->v[3] >> 2;			
	r[10] = r64->v[3] & 0xff;			/*10*/
	r64->v[3] = r64->v[3] >> 8;
	r[11] = r64->v[3] & 0xff;			/*18*/
	r64->v[3] = r64->v[3]>>8;
	r[12] = r64->v[3] & 0xff;			/*26*/

	r[13] = r64->v[4] & 0xff;			/*8*/
	r64->v[4] = r64->v[4]>>8;			
	r[14] = r64->v[4] & 0xff;			/*16*/
	r64->v[4] = r64->v[4]>>8;
	r[15] = r64->v[4] & 0xff;			/*24*/
	r64->v[4] = r64->v[4]>>8;
	r[16] = r64->v[4] & 0x3;			/*26*/

	r[16] = r[16]|(r64->v[5] & 0x3f) << 2;		/*6*/
	r64->v[5] = r64->v[5] >> 6;			
	r[17] = r64->v[5] & 0xff;			/*14*/	
	r64->v[5] = r64->v[5]>>8;
	r[18] = r64->v[5] & 0xff;			/*22*/
	r64->v[5] = r64->v[5]>>8;
	r[19] = r64->v[5] & 0xf;			/*26*/

	r[19] = r[19]|(r64->v[6] & 0xf) << 4;		/*4*/
	r64->v[6] = r64->v[6] >> 4;						
	r[20] = r64->v[6] & 0xff;			/*12*/
	r64->v[6] = r64->v[6]>>8;
	r[21] = r64->v[6] & 0xff;			/*20*/
	r64->v[6] = r64->v[6]>>8;
	r[22] = r64->v[6] & 0x3f;			/*26*/

	r[22] = r[22]|(r64->v[7] & 0x3) << 6;		/*2*/
	r64->v[7] = r64->v[7] >> 2;			
	r[23] = r64->v[7] & 0xff;			/*10*/
	r64->v[7] = r64->v[7] >> 8;
	r[24] = r64->v[7] & 0xff;			/*18*/
	r64->v[7] = r64->v[7]>>8;
	r[25] = r64->v[7] & 0xff;			/*26*/

	r[26] = r64->v[8] & 0xff;			/*8*/
	r64->v[8] = r64->v[8]>>8;			
	r[27] = r64->v[8] & 0xff;			/*16*/
	r64->v[8] = r64->v[8]>>8;
	r[28] = r64->v[8] & 0xff;			/*24*/
	r64->v[8] = r64->v[8]>>8;
	r[29] = r64->v[8] & 0x3;			/*26*/

	r[29] = r[29]|(r64->v[9] & 0x3f) << 2;		/*6*/
	r64->v[9] = r64->v[9] >> 6;			
	r[30] = r64->v[9] & 0xff;			/*14*/	
	r64->v[9] = r64->v[9]>>8;
	r[31] = r64->v[9] & 0x7f;			/*22*/
}

void convert_i51toc(gfe51 *r51, unsigned char r[32]){
	r[0] = r51->v[0] & 0xff;			/*8*/
	r51->v[0] = r51->v[0]>>8;
	r[1] = r51->v[0] & 0xff;			/*16*/
	r51->v[0] = r51->v[0]>>8;
	r[2] = r51->v[0] & 0xff;			/*24*/
	r51->v[0] = r51->v[0]>>8;
	r[3] = r51->v[0] & 0xff;			/*32*/
	r51->v[0] = r51->v[0]>>8;
	r[4] = r51->v[0] & 0xff;			/*40*/
	r51->v[0] = r51->v[0]>>8;			
	r[5] = r51->v[0] & 0xff;			/*48*/
	r51->v[0] = r51->v[0]>>8;
	r[6] = r51->v[0] & 0x7;				/*51*/

	r[6] |= (r51->v[1] & 0x1f)<<3;			/*5*/
	r51->v[1] = r51->v[1]>>5;
	r[7] = r51->v[1] & 0xff;			/*13*/
	r51->v[1] = r51->v[1]>>8;
	r[8] = r51->v[1] & 0xff;			/*21*/
	r51->v[1] = r51->v[1]>>8;
	r[9] = r51->v[1] & 0xff;			/*29*/
	r51->v[1] = r51->v[1]>>8;
	r[10] = r51->v[1] & 0xff;			/*37*/
	r51->v[1] = r51->v[1]>>8;
	r[11] = r51->v[1] & 0xff;			/*45*/
	r51->v[1] = r51->v[1]>>8;
	r[12] = r51->v[1] & 0x3f;			/*51*/

	r[12] |= (r51->v[2] & 0x3)<<6;			/*2*/
	r51->v[2] = r51->v[2]>>2;
	r[13] = r51->v[2] & 0xff;			/*10*/
	r51->v[2] = r51->v[2]>>8;
	r[14] = r51->v[2] & 0xff;			/*18*/
	r51->v[2] = r51->v[2]>>8;
	r[15] = r51->v[2] & 0xff;			/*26*/
	r51->v[2] = r51->v[2]>>8;
	r[16] = r51->v[2] & 0xff;			/*34*/
	r51->v[2] = r51->v[2]>>8;
	r[17] = r51->v[2] & 0xff;			/*42*/
	r51->v[2] = r51->v[2]>>8;
	r[18] = r51->v[2] & 0xff;			/*50*/
	r51->v[2] = r51->v[2]>>8;
	r[19] = r51->v[2] & 0x1;			/*51*/

	r[19] |= (r51->v[3] & 0x7f)<<1;			/*7*/	
	r51->v[3] = r51->v[3]>>7;		
	r[20] = r51->v[3] & 0xff;			/*15*/
	r51->v[3] = r51->v[3]>>8;
	r[21] = r51->v[3] & 0xff;			/*23*/
	r51->v[3] = r51->v[3]>>8;
	r[22] = r51->v[3] & 0xff;			/*31*/
	r51->v[3] = r51->v[3]>>8;
	r[23] = r51->v[3] & 0xff;			/*39*/
	r51->v[3] = r51->v[3]>>8;
	r[24] = r51->v[3] & 0xff;			/*47*/
	r51->v[3] = r51->v[3]>>8;
	r[25] = r51->v[3] & 0xf;			/*51*/

	r[25] |= (r51->v[4] & 0xf)<<4;			/*4*/	
	r51->v[4] = r51->v[4]>>4;
	r[26] = r51->v[4] & 0xff;			/*12*/
	r51->v[4] = r51->v[4]>>8;
	r[27] = r51->v[4] & 0xff;			/*20*/
	r51->v[4] = r51->v[4]>>8;
	r[28] = r51->v[4] & 0xff;			/*28*/
	r51->v[4] = r51->v[4]>>8;
	r[29] = r51->v[4] & 0xff;			/*36*/
	r51->v[4] = r51->v[4]>>8;
	r[30] = r51->v[4] & 0xff;			/*44*/
	r51->v[4] = r51->v[4]>>8;
	r[31] = r51->v[4] & 0x7f;			/*51*/
	
}


void gfe4_f_gfe(gfe4x *r64, gfe *m){
  	int i;

  	for (i = 0;i < 10;++i) {
	    	0[(u64 *) &r64->v[i]] = m[0].v[i];
	   	1[(u64 *) &r64->v[i]] = m[1].v[i];
	    	2[(u64 *) &r64->v[i]] = m[2].v[i];
	    	3[(u64 *) &r64->v[i]] = m[3].v[i];
  	}
}

void gfe4_f_gfe_part1(gfe4x *r64, gfe *m){
	int i;
	
	for (i = 0;i < 10;++i){
		0[(u64 *) &r64->v[i]] = 0;
		1[(u64 *) &r64->v[i]] = 0;
		2[(u64 *) &r64->v[i]] = m[0].v[i];
		3[(u64 *) &r64->v[i]] = m[1].v[i];
  	}
}
void gfe4_f_gfe_part2(gfe4x *r64, gfe *m){
	int i;
	
	for (i = 0;i < 10;++i){
		0[(u64 *) &r64->v[i]] = m[0].v[i];
		1[(u64 *) &r64->v[i]] = m[1].v[i];
  	}
}


void gfe4_t_gfe(gfe4x *r64, gfe *m){
	int i;

  	for (i = 0;i < 10;++i) {
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

		0[(u64 *) &r64[0].v[0]] = 77;
		1[(u64 *) &r64[0].v[0]] = 82;
		2[(u64 *) &r64[0].v[0]] = m[1].v[0];
		3[(u64 *) &r64[0].v[0]] = m[0].v[0];
	
	for (i = 1;i < 10;++i){
		0[(u64 *) &r64[0].v[i]] = 0;
		1[(u64 *) &r64[0].v[i]] = 0;
		2[(u64 *) &r64[0].v[i]] = m[1].v[i];
		3[(u64 *) &r64[0].v[i]] = m[0].v[i];
  	}

		0[(u64 *) &r64[1].v[0]] = m[1].v[0];
		1[(u64 *) &r64[1].v[0]] = m[0].v[0];
		2[(u64 *) &r64[1].v[0]] = 77;
		3[(u64 *) &r64[1].v[0]] = 82;
	
	for (i = 1;i < 10;++i){
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
	for(i=0;i<3;i++) {for(j=0;j<9;j++) printf("%3u, ",val[i*8+j]); printf("\n");}
	for(i=24;i<31;i++) printf("%3u, ",val[i]); printf("%3u ",val[31]); printf("\n");
}

void printVecFirst64Bits(vec *inputVec) { // prints only the first 64 bits

	u8 *val,i,j;
	val = (u8 *)inputVec;
	for(i=0;i<8;i++) printf("%3u, ",val[i]); printf("%3u ",val[8]); printf("\n");
}


void print_c(char *n){
	int i;
	for(i=0; i<32; i++)
		printf("%hhu, ",n[i]);
	printf("\n");

}
void print_gfe(gfe *r64){
	int i;
	for(i=0; i<10; i++)
		printf("%llu, ",r64->v[i]);
	printf("\n");
}

#endif


