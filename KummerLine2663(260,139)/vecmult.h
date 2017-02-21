#ifndef VECMULT_H_
#define VECMULT_H_

#define VECADD(C,A,B) {C = _mm256_add_epi64(A,B);}
#define VECSUB(C,A,B) {C = _mm256_sub_epi64(A,B);}
#define VECMULT(C,A,B) {C = _mm256_mul_epu32(A,B);}
#define VECSHIFTL(C,A,B) {C=_mm256_slli_epi64(A,B);}
#define VECSHIFTR(C,A,B) {C=_mm256_srli_epi64(A,B);}
#define VECAND(C,A,B) {C=_mm256_and_si256(A,B);}

const vec vecmask28 = {0xfffffff,0xfffffff,0xfffffff,0xfffffff};
const vec vecmask27 = {0x7ffffff,0x7ffffff,0x7ffffff,0x7ffffff};

const vec vecmask26 = {0x3ffffff,0x3ffffff,0x3ffffff,0x3ffffff};
const vec vecmask25 = {0x1ffffff,0x1ffffff,0x1ffffff,0x1ffffff};

const vec vecmask23 = {0x07fffff,0x07fffff,0x07fffff,0x07fffff};

vec VECMULT2S0,VECMULT2T0,VECMULT2U0; 
vec VECMULT3S1,VECMULT3T1,VECMULT3U1,VECMULT3U2;
vec VECMULT5S1,VECMULT5S2,VECMULT5T1,VECMULT5T2,VECMULT5U0,VECMULT5U1,VECMULT5U2,VECMULT5U3,VECMULT5U4;
vec VECMULT10S0,VECMULT10S1,VECMULT10S2,VECMULT10S3,VECMULT10S4,VECMULT10T0,VECMULT10T1,VECMULT10T2,VECMULT10T3,VECMULT10T4,VECMULT10U0,VECMULT10U1,VECMULT10U2,VECMULT10U3,
	VECMULT10U4,VECMULT10U5,VECMULT10U6,VECMULT10U7,VECMULT10U8;
vec VECSQ4S0,VECSQ4T0,VECSQ4U0,VECSQ4U1,VECSQ4U2;
vec VECSQ5U0,VECSQ5U1,VECSQ5U2,VECSQ5U3,VECSQ5U4;
vec VECSQ10U0,VECSQ10U1,VECSQ10U2,VECSQ10U3,VECSQ10U4,VECSQ10U5,VECSQ10U6,VECSQ10U7,VECSQ10U8;

#define VECMULT2(C0,C1,C2,F0,F1,G0,G1){ \
	VECMULT(C0,F0,G0); VECMULT(VECMULT2S0,F0,G1); VECMULT(C2,F1,G1); VECMULT(VECMULT2T0,F1,G0); \
	VECADD(C1,VECMULT2S0,VECMULT2T0); \
}

#define ModifiedVECMULT2(C1,C2,F0,F1,G0,G1,C0){ \
	VECMULT(VECMULT2S0,F0,G1); VECMULT(C2,F1,G1); VECMULT(VECMULT2T0,F1,G0); \
	VECADD(C1,VECMULT2S0,VECMULT2T0); \
}

#define VECMULT2Karatsuba(C0,C1,C2,F0,F1,G0,G1){ \
	VECMULT(C0,F0,G0); \
	VECMULT(C2,F1,G1); \
	VECADD(VECMULT2S0,F0,F1); \
	VECADD(VECMULT2T0,G0,G1); \
	VECMULT(VECMULT2U0,VECMULT2S0,VECMULT2T0); \
	VECSUB(VECMULT2U0,VECMULT2U0,C0); \
	VECSUB(VECMULT2U0,VECMULT2U0,C2); \
	C1=VECMULT2U0; \
}

#define ModifiedVECMULT2Karatsuba(C1,C2,F0,F1,G0,G1,C0){ /* C0=F0*G0 is provided as input */ \
	VECMULT(C2,F1,G1); \
	VECADD(VECMULT2S0,F0,F1); \
	VECADD(VECMULT2T0,G0,G1); \
	VECMULT(VECMULT2U0,VECMULT2S0,VECMULT2T0); \
	VECSUB(VECMULT2U0,VECMULT2U0,C0); \
	VECSUB(VECMULT2U0,VECMULT2U0,C2); \
	C1=VECMULT2U0; \
}

#define VECMULT3(C0,C1,C2,C3,C4,F0,F1,F2,G0,G1,G2){ \
	VECMULT(C0,F0,G0); \
	VECMULT2(C2,C3,C4,F1,F2,G1,G2); \
	VECADD(VECMULT3S1,F0,F2); \
	VECADD(VECMULT3T1,G0,G2); \
	ModifiedVECMULT2(VECMULT3U1,VECMULT3U2,F1,VECMULT3S1,G1,VECMULT3T1,C2); \
	VECSUB(VECMULT3U1,VECMULT3U1,C3); VECSUB(VECMULT3U2,VECMULT3U2,C4); VECSUB(VECMULT3U2,VECMULT3U2,C0); \
	C1 = VECMULT3U1; VECADD(C2,C2,VECMULT3U2); \
}

#define ModifiedVECMULT3(C1,C2,C3,C4,F0,F1,F2,G0,G1,G2,C0){ /* C0=F0*G0 is provided as input */ \
	VECMULT2(C2,C3,C4,F1,F2,G1,G2); \
	VECADD(VECMULT3S1,F0,F2); \
	VECADD(VECMULT3T1,G0,G2); \
	ModifiedVECMULT2(VECMULT3U1,VECMULT3U2,F1,VECMULT3S1,G1,VECMULT3T1,C2); \
	VECSUB(VECMULT3U1,VECMULT3U1,C3); VECSUB(VECMULT3U2,VECMULT3U2,C4); VECSUB(VECMULT3U2,VECMULT3U2,C0); \
	C1 = VECMULT3U1; VECADD(C2,C2,VECMULT3U2); \
}

#define VECMULT4(C0,C1,C2,C3,C4,C5,C6,F0,F1,F2,F3,G0,G1,G2,G3) { /* internal vars for VECMULT5 are used */ \
	VECMULT2(C0,C1,C2,F0,F1,G0,G1); \
	VECMULT2(C4,C5,C6,F2,F3,G2,G3);; \
	VECADD(VECMULT5S1,F0,F2); VECADD(VECMULT5S2,F1,F3); VECADD(VECMULT5T1,G0,G2); VECADD(VECMULT5T2,G1,G3); \
	VECMULT2(VECMULT5U1,VECMULT5U2,VECMULT5U3,VECMULT5S1,VECMULT5S2,VECMULT5T1,VECMULT5T2); \
	VECSUB(VECMULT5U1,VECMULT5U1,C0); VECSUB(VECMULT5U2,VECMULT5U2,C1); VECSUB(VECMULT5U3,VECMULT5U3,C2); \
	VECSUB(VECMULT5U1,VECMULT5U1,C4); VECSUB(VECMULT5U2,VECMULT5U2,C5); VECSUB(VECMULT5U3,VECMULT5U3,C6); \
	VECADD(C2,C2,VECMULT5U1); C3=VECMULT5U2; VECADD(C4,C4,VECMULT5U3); \
}

#define VECMULT5Hybrid(C0,C1,C2,C3,C4,C5,C6,C7,C8,F0,F1,F2,F3,F4,G0,G1,G2,G3,G4) { \
	VECMULT2(C0,C1,C2,F0,F1,G0,G1); \
	VECMULT2(C4,C5,C6,F2,F3,G2,G3); \
	VECADD(VECMULT5S1,F0,F2); VECADD(VECMULT5S2,F1,F3); VECADD(VECMULT5T1,G0,G2); VECADD(VECMULT5T2,G1,G3); \
	VECMULT2(VECMULT5U0,VECMULT5U1,VECMULT5U2,VECMULT5S1,VECMULT5S2,VECMULT5T1,VECMULT5T2); \
	VECSUB(VECMULT5U0,VECMULT5U0,C0); VECSUB(VECMULT5U0,VECMULT5U0,C4); \
	VECSUB(VECMULT5U1,VECMULT5U1,C1); VECSUB(VECMULT5U1,VECMULT5U1,C5); \
	VECSUB(VECMULT5U2,VECMULT5U2,C2); VECSUB(VECMULT5U2,VECMULT5U2,C6); \
	VECADD(C2,C2,VECMULT5U0); C3=VECMULT5U1; VECADD(C4,C4,VECMULT5U2); \
	VECMULT(VECMULT5U0,F4,G0); VECMULT(VECMULT5U1,F4,G1); VECMULT(VECMULT5U2,F4,G2); VECMULT(VECMULT5U3,F4,G3); VECMULT(VECMULT5U4,F4,G4); \
	VECMULT(VECMULT5S1,G4,F0); VECMULT(VECMULT5S2,G4,F1); VECMULT(VECMULT5T1,G4,F2); VECMULT(VECMULT5T2,G4,F3); \
	VECADD(VECMULT5U0,VECMULT5U0,VECMULT5S1); VECADD(VECMULT5U1,VECMULT5U1,VECMULT5S2); VECADD(VECMULT5U2,VECMULT5U2,VECMULT5T1); VECADD(VECMULT5U3,VECMULT5U3,VECMULT5T2); \
	VECADD(C4,C4,VECMULT5U0); VECADD(C5,C5,VECMULT5U1); VECADD(C6,C6,VECMULT5U2); \
	C7=VECMULT5U3; C8=VECMULT5U4; \
}

#define ModifiedVECMULT5Hybrid(C1,C2,C3,C4,C5,C6,C7,C8,F0,F1,F2,F3,F4,G0,G1,G2,G3,G4,C0) { \
	ModifiedVECMULT2(C1,C2,F0,F1,G0,G1,C0); \
	VECMULT2(C4,C5,C6,F2,F3,G2,G3); \
	VECADD(VECMULT5S1,F0,F2); VECADD(VECMULT5S2,F1,F3); VECADD(VECMULT5T1,G0,G2); VECADD(VECMULT5T2,G1,G3); \
	VECMULT2(VECMULT5U0,VECMULT5U1,VECMULT5U2,VECMULT5S1,VECMULT5S2,VECMULT5T1,VECMULT5T2); \
	VECSUB(VECMULT5U0,VECMULT5U0,C0); VECSUB(VECMULT5U0,VECMULT5U0,C4); \
	VECSUB(VECMULT5U1,VECMULT5U1,C1); VECSUB(VECMULT5U1,VECMULT5U1,C5); \
	VECSUB(VECMULT5U2,VECMULT5U2,C2); VECSUB(VECMULT5U2,VECMULT5U2,C6); \
	VECADD(C2,C2,VECMULT5U0); C3=VECMULT5U1; VECADD(C4,C4,VECMULT5U2); \
	VECMULT(VECMULT5U0,F4,G0); VECMULT(VECMULT5U1,F4,G1); VECMULT(VECMULT5U2,F4,G2); VECMULT(VECMULT5U3,F4,G3); VECMULT(VECMULT5U4,F4,G4); \
	VECMULT(VECMULT5S1,G4,F0); VECMULT(VECMULT5S2,G4,F1); VECMULT(VECMULT5T1,G4,F2); VECMULT(VECMULT5T2,G4,F3); \
	VECADD(VECMULT5U0,VECMULT5U0,VECMULT5S1); VECADD(VECMULT5U1,VECMULT5U1,VECMULT5S2); VECADD(VECMULT5U2,VECMULT5U2,VECMULT5T1); VECADD(VECMULT5U3,VECMULT5U3,VECMULT5T2); \
	VECADD(C4,C4,VECMULT5U0); VECADD(C5,C5,VECMULT5U1); VECADD(C6,C6,VECMULT5U2); \
	C7=VECMULT5U3; C8=VECMULT5U4; \
}

#define VECMULT5(C0,C1,C2,C3,C4,C5,C6,C7,C8,F0,F1,F2,F3,F4,G0,G1,G2,G3,G4) { \
	VECMULT2(C0,C1,C2,F0,F1,G0,G1) \
	VECMULT3(C4,C5,C6,C7,C8,F2,F3,F4,G2,G3,G4); \
	VECADD(VECMULT5S1,F0,F3); VECADD(VECMULT5S2,F1,F4); VECADD(VECMULT5T1,G0,G3); VECADD(VECMULT5T2,G1,G4); \
	ModifiedVECMULT3(VECMULT5U1,VECMULT5U2,VECMULT5U3,VECMULT5U4,F2,VECMULT5S1,VECMULT5S2,G2,VECMULT5T1,VECMULT5T2,C4); \
	VECSUB(VECMULT5U1,VECMULT5U1,C5); VECSUB(VECMULT5U2,VECMULT5U2,C6); VECSUB(VECMULT5U3,VECMULT5U3,C7); VECSUB(VECMULT5U4,VECMULT5U4,C8); \
	VECSUB(VECMULT5U2,VECMULT5U2,C0); VECSUB(VECMULT5U3,VECMULT5U3,C1); VECSUB(VECMULT5U4,VECMULT5U4,C2); \
	VECADD(C2,C2,VECMULT5U1); C3 = VECMULT5U2; VECADD(C4,C4,VECMULT5U3); VECADD(C5,C5,VECMULT5U4); \
}

#define ModifiedVECMULT5(C1,C2,C3,C4,C5,C6,C7,C8,F0,F1,F2,F3,F4,G0,G1,G2,G3,G4,C0) { \
	ModifiedVECMULT2(C1,C2,F0,F1,G0,G1,C0) \
	VECMULT3(C4,C5,C6,C7,C8,F2,F3,F4,G2,G3,G4); \
	VECADD(VECMULT5S1,F0,F3); VECADD(VECMULT5S2,F1,F4); VECADD(VECMULT5T1,G0,G3); VECADD(VECMULT5T2,G1,G4); \
	ModifiedVECMULT3(VECMULT5U1,VECMULT5U2,VECMULT5U3,VECMULT5U4,F2,VECMULT5S1,VECMULT5S2,G2,VECMULT5T1,VECMULT5T2,C4); \
	VECSUB(VECMULT5U1,VECMULT5U1,C5); VECSUB(VECMULT5U2,VECMULT5U2,C6); VECSUB(VECMULT5U3,VECMULT5U3,C7); VECSUB(VECMULT5U4,VECMULT5U4,C8); \
	VECSUB(VECMULT5U2,VECMULT5U2,C0); VECSUB(VECMULT5U3,VECMULT5U3,C1); VECSUB(VECMULT5U4,VECMULT5U4,C2); \
	VECADD(C2,C2,VECMULT5U1); C3 = VECMULT5U2; VECADD(C4,C4,VECMULT5U3); VECADD(C5,C5,VECMULT5U4); \
}

#define VECMULT10(C0,C1,C2,C3,C4,C5,C6,C7,C8,C9,C10,C11,C12,C13,C14,C15,C16,C17,C18, \
	       F0,F1,F2,F3,F4,F5,F6,F7,F8,F9, \
	       G0,G1,G2,G3,G4,G5,G6,G7,G8,G9) { \
	VECMULT5(C0,C1,C2,C3,C4,C5,C6,C7,C8,F0,F1,F2,F3,F4,G0,G1,G2,G3,G4); \
	VECMULT5(C10,C11,C12,C13,C14,C15,C16,C17,C18,F5,F6,F7,F8,F9,G5,G6,G7,G8,G9); \
	VECADD(VECMULT10S0,F0,F5); VECADD(VECMULT10S1,F1,F6); VECADD(VECMULT10S2,F2,F7); VECADD(VECMULT10S3,F3,F8); VECADD(VECMULT10S4,F4,F9); \
	VECADD(VECMULT10T0,G0,G5); VECADD(VECMULT10T1,G1,G6); VECADD(VECMULT10T2,G2,G7); VECADD(VECMULT10T3,G3,G8); VECADD(VECMULT10T4,G4,G9); \
	VECMULT5(VECMULT10U0,VECMULT10U1,VECMULT10U2,VECMULT10U3,VECMULT10U4,VECMULT10U5,VECMULT10U6,VECMULT10U7,VECMULT10U8, \
	      VECMULT10S0,VECMULT10S1,VECMULT10S2,VECMULT10S3,VECMULT10S4,VECMULT10T0,VECMULT10T1,VECMULT10T2,VECMULT10T3,VECMULT10T4); \
	VECSUB(VECMULT10U0,VECMULT10U0,C0); VECSUB(VECMULT10U1,VECMULT10U1,C1); VECSUB(VECMULT10U2,VECMULT10U2,C2); \
	VECSUB(VECMULT10U3,VECMULT10U3,C3); VECSUB(VECMULT10U4,VECMULT10U4,C4); VECSUB(VECMULT10U5,VECMULT10U5,C5); \
	VECSUB(VECMULT10U6,VECMULT10U6,C6); VECSUB(VECMULT10U7,VECMULT10U7,C7); VECSUB(VECMULT10U8,VECMULT10U8,C8); \
	VECSUB(VECMULT10U0,VECMULT10U0,C10); VECSUB(VECMULT10U1,VECMULT10U1,C11); VECSUB(VECMULT10U2,VECMULT10U2,C12); \
	VECSUB(VECMULT10U3,VECMULT10U3,C13); VECSUB(VECMULT10U4,VECMULT10U4,C14); VECSUB(VECMULT10U5,VECMULT10U5,C15); \
	VECSUB(VECMULT10U6,VECMULT10U6,C16); VECSUB(VECMULT10U7,VECMULT10U7,C17); VECSUB(VECMULT10U8,VECMULT10U8,C18); \
	VECADD(C5,C5,VECMULT10U0); VECADD(C6,C6,VECMULT10U1); VECADD(C7,C7,VECMULT10U2); VECADD(C8,C8,VECMULT10U3); \
	C9=VECMULT10U4;  \
	VECADD(C10,C10,VECMULT10U5); VECADD(C11,C11,VECMULT10U6); VECADD(C12,C12,VECMULT10U7); VECADD(C13,C13,VECMULT10U8); \
}

#define VECSQ4(C0,C1,C2,C3,C4,C5,C6,F0,F1,F2,F3){ \
\
	VECMULT(C0,F0,F0); VECMULT(C2,F1,F1); VECMULT(C1,F0,F1); \
	VECMULT(C4,F2,F2); VECMULT(C6,F3,F3); VECMULT(C5,F2,F3); \
	VECMULT(VECSQ4U0,F0,F2); VECMULT(VECSQ4U2,F1,F3); VECMULT(VECSQ4U1,F0,F3); VECMULT(VECSQ4S0,F1,F2); \
\
	VECADD(VECSQ4U1,VECSQ4U1,VECSQ4S0); \
\
	VECSHIFTL(C1,C1,1); VECSHIFTL(C5,C5,1); VECSHIFTL(VECSQ4U0,VECSQ4U0,1); VECSHIFTL(VECSQ4U1,VECSQ4U1,1); VECSHIFTL(VECSQ4U2,VECSQ4U2,1); \
\
	VECADD(C2,C2,VECSQ4U0); C3=VECSQ4U1; VECADD(C4,C4,VECSQ4U2); \
}

#define VECSQ4Karatsuba(C0,C1,C2,C3,C4,C5,C6,F0,F1,F2,F3){ \
\
	VECADD(VECSQ4S0,F0,F1); VECADD(VECSQ4T0,F2,F3); \
\
	VECMULT(C0,F0,F0); VECMULT(C2,F1,F1); VECMULT(C1,F0,F1); \
	VECMULT(C4,F2,F2); VECMULT(C6,F3,F3); VECMULT(C5,F2,F3); \
	VECMULT(VECSQ4U0,F0,F2); VECMULT(VECSQ4U2,F1,F3); VECMULT(VECSQ4U1,VECSQ4S0,VECSQ4T0); \
\
	VECSUB(VECSQ4U1,VECSQ4U1,VECSQ4U0); VECSUB(VECSQ4U1,VECSQ4U1,VECSQ4U2); \
\
	VECSHIFTL(C1,C1,1); VECSHIFTL(C5,C5,1); VECSHIFTL(VECSQ4U0,VECSQ4U0,1); VECSHIFTL(VECSQ4U1,VECSQ4U1,1); VECSHIFTL(VECSQ4U2,VECSQ4U2,1); \
\
	VECADD(C2,C2,VECSQ4U0); C3=VECSQ4U1; VECADD(C4,C4,VECSQ4U2); \
}

#define VECSQ5(C0,C1,C2,C3,C4,C5,C6,C7,C8,F0,F1,F2,F3,F4) { \
	VECSQ4(C0,C1,C2,C3,C4,C5,C6,F0,F1,F2,F3); \
	VECMULT(VECSQ5U0,F4,F0); VECMULT(VECSQ5U1,F4,F1); VECMULT(VECSQ5U2,F4,F2); VECMULT(VECSQ5U3,F4,F3); VECMULT(VECSQ5U4,F4,F4); \
	VECSHIFTL(VECSQ5U0,VECSQ5U0,1); VECSHIFTL(VECSQ5U1,VECSQ5U1,1); VECSHIFTL(VECSQ5U2,VECSQ5U2,1); VECSHIFTL(VECSQ5U3,VECSQ5U3,1); \
	VECADD(C4,C4,VECSQ5U0); VECADD(C5,C5,VECSQ5U1); VECADD(C6,C6,VECSQ5U2); \
	C7 = VECSQ5U3; C8 = VECSQ5U4; \
}

#define VECSQ10(C0,C1,C2,C3,C4,C5,C6,C7,C8,C9,C10,C11,C12,C13,C14,C15,C16,C17,C18,F0,F1,F2,F3,F4,F5,F6,F7,F8,F9) { \
	VECSQ5(C0,C1,C2,C3,C4,C5,C6,C7,C8,F0,F1,F2,F3,F4); \
	VECSQ5(C10,C11,C12,C13,C14,C15,C16,C17,C18,F5,F6,F7,F8,F9); \
	VECMULT5(VECSQ10U0,VECSQ10U1,VECSQ10U2,VECSQ10U3,VECSQ10U4,VECSQ10U5,VECSQ10U6,VECSQ10U7,VECSQ10U8,F0,F1,F2,F3,F4,F5,F6,F7,F8,F9); \
	VECSHIFTL(VECSQ10U0,VECSQ10U0,1); VECSHIFTL(VECSQ10U1,VECSQ10U1,1); VECSHIFTL(VECSQ10U2,VECSQ10U2,1); VECSHIFTL(VECSQ10U3,VECSQ10U3,1); VECSHIFTL(VECSQ10U4,VECSQ10U4,1); \
	VECSHIFTL(VECSQ10U5,VECSQ10U5,1); VECSHIFTL(VECSQ10U6,VECSQ10U6,1); VECSHIFTL(VECSQ10U7,VECSQ10U7,1); VECSHIFTL(VECSQ10U8,VECSQ10U8,1); \
	VECADD(C5,C5,VECSQ10U0); VECADD(C6,C6,VECSQ10U1); VECADD(C7,C7,VECSQ10U2); VECADD(C8,C8,VECSQ10U3); C9 = VECSQ10U4; \
	VECADD(C10,C10,VECSQ10U5); VECADD(C11,C11,VECSQ10U6); VECADD(C12,C12,VECSQ10U7); VECADD(C13,C13,VECSQ10U8); \
}

#define VECREDUCE2663(RES0,RES1,RES2,RES3,RES4,RES5,RES6,RES7,RES8,RES9,TEMP) { \
	VECREDUCEPARTA2663(RES0,RES1,RES2,RES3,RES4,RES5,RES6,RES7,RES8,RES9,TEMP); \
	VECREDUCEPARTB2663(RES0,RES1,RES2,RES3,RES4,RES5,RES6,RES7,RES8,RES9); \
}

#define VECREDUCEPARTA2663(RES0,RES1,RES2,RES3,RES4,RES5,RES6,RES7,RES8,RES9,TEMP) { \
\
	VECSHIFTL(TEMP[10],TEMP[10],4); VECSHIFTL(TEMP[11],TEMP[11],4); VECSHIFTL(TEMP[12],TEMP[12],4); VECSHIFTL(TEMP[13],TEMP[13],4); \
	VECSHIFTL(TEMP[14],TEMP[14],4); VECSHIFTL(TEMP[15],TEMP[15],4); VECSHIFTL(TEMP[16],TEMP[16],4); VECSHIFTL(TEMP[17],TEMP[17],4); \
	VECSHIFTL(TEMP[18],TEMP[18],4); \
 \
	VECSHIFTL(VECMULT10U0,TEMP[10],1); VECSHIFTL(VECMULT10U1,TEMP[11],1); \
	VECSHIFTL(VECMULT10U2,TEMP[12],1); VECSHIFTL(VECMULT10U3,TEMP[13],1); \
	VECSHIFTL(VECMULT10U4,TEMP[14],1); VECSHIFTL(VECMULT10U5,TEMP[15],1); \
	VECSHIFTL(VECMULT10U6,TEMP[16],1); VECSHIFTL(VECMULT10U7,TEMP[17],1); \
	VECSHIFTL(VECMULT10U8,TEMP[18],1); \
 \
	VECADD(VECMULT10U0,VECMULT10U0,TEMP[10]); VECADD(VECMULT10U1,VECMULT10U1,TEMP[11]); \
	VECADD(VECMULT10U2,VECMULT10U2,TEMP[12]); VECADD(VECMULT10U3,VECMULT10U3,TEMP[13]); \
	VECADD(VECMULT10U4,VECMULT10U4,TEMP[14]); VECADD(VECMULT10U5,VECMULT10U5,TEMP[15]); \
	VECADD(VECMULT10U6,VECMULT10U6,TEMP[16]); VECADD(VECMULT10U7,VECMULT10U7,TEMP[17]); \
	VECADD(VECMULT10U8,VECMULT10U8,TEMP[18]); \
 \
	VECADD(RES0,TEMP[0],VECMULT10U0); VECADD(RES1,TEMP[1],VECMULT10U1); \
	VECADD(RES2,TEMP[2],VECMULT10U2); VECADD(RES3,TEMP[3],VECMULT10U3); \
	VECADD(RES4,TEMP[4],VECMULT10U4); VECADD(RES5,TEMP[5],VECMULT10U5); \
	VECADD(RES6,TEMP[6],VECMULT10U6); VECADD(RES7,TEMP[7],VECMULT10U7); \
	VECADD(RES8,TEMP[8],VECMULT10U8); RES9 = TEMP[9]; \
}

#define VECREDUCEPARTB2663(RES0,RES1,RES2,RES3,RES4,RES5,RES6,RES7,RES8,RES9) { \
\
	VECSHIFTR(VECMULT10S1,RES0,27); VECAND(RES0,RES0,vecmask27); VECADD(RES1,RES1,VECMULT10S1); \
	VECSHIFTR(VECMULT10S1,RES1,27); VECAND(RES1,RES1,vecmask27); VECADD(RES2,RES2,VECMULT10S1); \
	VECSHIFTR(VECMULT10S1,RES2,27); VECAND(RES2,RES2,vecmask27); VECADD(RES3,RES3,VECMULT10S1); \
	VECSHIFTR(VECMULT10S1,RES3,27); VECAND(RES3,RES3,vecmask27); VECADD(RES4,RES4,VECMULT10S1); \
	VECSHIFTR(VECMULT10S1,RES4,27); VECAND(RES4,RES4,vecmask27); VECADD(RES5,RES5,VECMULT10S1); \
	VECSHIFTR(VECMULT10S1,RES5,27); VECAND(RES5,RES5,vecmask27); VECADD(RES6,RES6,VECMULT10S1); \
	VECSHIFTR(VECMULT10S1,RES6,27); VECAND(RES6,RES6,vecmask27); VECADD(RES7,RES7,VECMULT10S1); \
	VECSHIFTR(VECMULT10S1,RES7,27); VECAND(RES7,RES7,vecmask27); VECADD(RES8,RES8,VECMULT10S1); \
	VECSHIFTR(VECMULT10S1,RES8,27); VECAND(RES8,RES8,vecmask27); VECADD(RES9,RES9,VECMULT10S1); \
	VECSHIFTR(VECMULT10S1,RES9,23); VECAND(RES9,RES9,vecmask23); \
	VECSHIFTL(VECMULT10S2,VECMULT10S1,1); VECADD(VECMULT10S2,VECMULT10S1,VECMULT10S2); \
	VECADD(RES0,RES0,VECMULT10S2); \
	VECSHIFTR(VECMULT10S1,RES0,27); VECAND(RES0,RES0,vecmask27); VECADD(RES1,RES1,VECMULT10S1); \
}

#endif
