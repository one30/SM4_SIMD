// #include <stdlib.h>
// #include <time.h>
// #include "AVX_SM4.h"

// inline u4 SM4_T(u4 b)
// {
// 	return Sbox_T[get_byte(0,b)] ^ (Sbox_T8[get_byte(1,b)]) ^ (Sbox_T16[get_byte(2,b)]) ^ (Sbox_T24[get_byte(3,b)]);
// }

// // Variant of T for key schedule
// inline u4 SM4_Tp(u4 b)
// {
// 	const u4 t = make_uint32(Sbox[get_byte(0,b)], Sbox[get_byte(1,b)], Sbox[get_byte(2,b)], Sbox[get_byte(3,b)]);

// 	// L' linear transform
// 	return t ^ rotl<13>(t) ^ rotl<23>(t);
// }

// #define SM4_RNDS(k0,k1,k2,k3,F) do {        \
//   B0 ^= F(B1 ^ B2 ^ B3 ^ rkey[k0]); \
//   B1 ^= F(B0 ^ B2 ^ B3 ^ rkey[k1]); \
//   B2 ^= F(B0 ^ B1 ^ B3 ^ rkey[k2]); \
//   B3 ^= F(B0 ^ B1 ^ B2 ^ rkey[k3]); \
// } while(0)

// void SM4_key_schedule(const u1 key[SM4_KEY_SIZE], u4 rkey[SM4_RND_KEY_SIZE / sizeof(u4)])
// {
// 	// System parameter or family key
// 	const u4 FK[4] = { 0xa3b1bac6, 0x56aa3350, 0x677d9197, 0xb27022dc };

// 	const u4 CK[32] = {
// 	0x00070E15, 0x1C232A31, 0x383F464D, 0x545B6269,
// 	0x70777E85, 0x8C939AA1, 0xA8AFB6BD, 0xC4CBD2D9,
// 	0xE0E7EEF5, 0xFC030A11, 0x181F262D, 0x343B4249,
// 	0x50575E65, 0x6C737A81, 0x888F969D, 0xA4ABB2B9,
// 	0xC0C7CED5, 0xDCE3EAF1, 0xF8FF060D, 0x141B2229,
// 	0x30373E45, 0x4C535A61, 0x686F767D, 0x848B9299,
// 	0xA0A7AEB5, 0xBCC3CAD1, 0xD8DFE6ED, 0xF4FB0209,
// 	0x10171E25, 0x2C333A41, 0x484F565D, 0x646B7279
// 	};

// 	uint32_t K[4];

// 	K[0] = load_be<u4>(key, 0) ^ FK[0];
// 	K[1] = load_be<u4>(key, 1) ^ FK[1];
// 	K[2] = load_be<u4>(key, 2) ^ FK[2];
// 	K[3] = load_be<u4>(key, 3) ^ FK[3];

// 	forloop (i,0,32)
// 	{
// 		K[i % 4] ^= SM4_Tp(K[(i+1)%4] ^ K[(i+2)%4] ^ K[(i+3)%4] ^ CK[i]);
// 		rkey[i] = K[i % 4];
// 	}
// }
// inline void sms4_avx2_encrypt_8blocks(const unsigned char *in, int *out, const u4  *key)
// {

// 	const int *rk = (int *)key;
// 	__m256i x0, x1, x2, x3, x4;
// 	__m256i t0, t1, t2, t3, t4;

// 	GET_BLKS(x0, x1, x2, x3, in);

// 	ROUNDS(x0, x1, x2, x3, x4);

// 	//PUT_BLKS(out, x0, x1, x2, x3);

// 	t0 = _mm256_shuffle_epi8(x0, vindex_swap);
// 	t1 = _mm256_shuffle_epi8(x4, vindex_swap);
// 	t2 = _mm256_shuffle_epi8(x3, vindex_swap);
// 	t3 = _mm256_shuffle_epi8(x2, vindex_swap);
// 	forloop(ii, 0, 4) {
// 		x0 = t0;
// 		x1 = _mm256_slli_si256(t1, 4);
// 		x2 = _mm256_slli_si256(t2, 8);
// 		x3 = _mm256_slli_si256(t3, 12);
// 		mask = _mm256_setr_epi32(4294967295, 0, 0, 0, 0, 0, 0, 0);
// 		_mm256_maskstore_epi32(out + ii * 4, mask, x0);
// 		mask = _mm256_setr_epi32(0, 4294967295, 0, 0, 0, 0, 0, 0);
// 		_mm256_maskstore_epi32(out + ii * 4, mask, x1);
// 		mask = _mm256_setr_epi32(0, 0, 4294967295, 0, 0, 0, 0, 0);
// 		_mm256_maskstore_epi32(out + ii * 4, mask, x2);
// 		mask = _mm256_setr_epi32(0, 0, 0, 4294967295, 0, 0, 0, 0);
// 		_mm256_maskstore_epi32(out + ii * 4, mask, x3);
// 		mask = _mm256_setr_epi32(0, 0, 0, 0, 4294967295, 0, 0, 0);
// 		_mm256_maskstore_epi32(out + 12 + ii * 4, mask, x0);
// 		mask = _mm256_setr_epi32(0, 0, 0, 0, 0, 4294967295, 0, 0);
// 		_mm256_maskstore_epi32(out + 12 + ii * 4, mask, x1);
// 		mask = _mm256_setr_epi32(0, 0, 0, 0, 0, 0, 4294967295, 0);
// 		_mm256_maskstore_epi32(out + 12 + ii * 4, mask, x2);
// 		mask = _mm256_setr_epi32(0, 0, 0, 0, 0, 0, 0, 4294967295);
// 		_mm256_maskstore_epi32(out + 12 + ii * 4, mask, x3);
// 		t0 = _mm256_srli_si256(t0, 4);
// 		t1 = _mm256_srli_si256(t1, 4);
// 		t2 = _mm256_srli_si256(t2, 4);
// 		t3 = _mm256_srli_si256(t3, 4);
// 	}
// }
// inline void sms4_avx2_decrypt_8blocks(const unsigned char *in, int *out,const u4  *key)
// {
// 	const int *rk = (int *)key;
// 	__m256i x0, x1, x2, x3, x4;
// 	__m256i t0, t1, t2, t3, t4;

// 	GET_BLKS(x0, x1, x2, x3, in);

// 	ROUNDS_DEC(x0, x1, x2, x3, x4);
// 	t0 = _mm256_shuffle_epi8(x0, vindex_swap);
// 	t1 = _mm256_shuffle_epi8(x4, vindex_swap);
// 	t2 = _mm256_shuffle_epi8(x3, vindex_swap);
// 	t3 = _mm256_shuffle_epi8(x2, vindex_swap);
// 	forloop(ii, 0, 4) {
// 		x0 = t0;
// 		x1 = _mm256_slli_si256(t1, 4);
// 		x2 = _mm256_slli_si256(t2, 8);
// 		x3 = _mm256_slli_si256(t3, 12);
// 		mask = _mm256_setr_epi32(4294967295, 0, 0, 0, 0, 0, 0, 0);
// 		_mm256_maskstore_epi32(out + ii * 4, mask, x0);
// 		mask = _mm256_setr_epi32(0, 4294967295, 0, 0, 0, 0, 0, 0);
// 		_mm256_maskstore_epi32(out + ii * 4, mask, x1);
// 		mask = _mm256_setr_epi32(0, 0, 4294967295, 0, 0, 0, 0, 0);
// 		_mm256_maskstore_epi32(out + ii * 4, mask, x2);
// 		mask = _mm256_setr_epi32(0, 0, 0, 4294967295, 0, 0, 0, 0);
// 		_mm256_maskstore_epi32(out + ii * 4, mask, x3);
// 		mask = _mm256_setr_epi32(0, 0, 0, 0, 4294967295, 0, 0, 0);
// 		_mm256_maskstore_epi32(out + 12 + ii * 4, mask, x0);
// 		mask = _mm256_setr_epi32(0, 0, 0, 0, 0, 4294967295, 0, 0);
// 		_mm256_maskstore_epi32(out + 12 + ii * 4, mask, x1);
// 		mask = _mm256_setr_epi32(0, 0, 0, 0, 0, 0, 4294967295, 0);
// 		_mm256_maskstore_epi32(out + 12 + ii * 4, mask, x2);
// 		mask = _mm256_setr_epi32(0, 0, 0, 0, 0, 0, 0, 4294967295);
// 		_mm256_maskstore_epi32(out + 12 + ii * 4, mask, x3);
// 		t0 = _mm256_srli_si256(t0, 4);
// 		t1 = _mm256_srli_si256(t1, 4);
// 		t2 = _mm256_srli_si256(t2, 4);
// 		t3 = _mm256_srli_si256(t3, 4);
// 	}
// }
// void sms4_avx2_encrypt_blocks(const unsigned char *in, int *out, const u4 *key, u4 BLK_CNT) {
// 	for (int i = 0; i < BLK_CNT/16; i++)
// 		sms4_avx2_encrypt_16blocks(&in[i*SM4_BLOCK_SIZE * 16], &out[i*SM4_BLOCK_SIZE * 16 / sizeof(int)], key);
// }
// void sms4_avx2_decrypt_blocks(const unsigned char *in, int *out, const u4 *key, u4 BLK_CNT) {
// 	for (int i = 0; i < BLK_CNT/16; i++)
// 		sms4_avx2_decrypt_16blocks(&in[i*SM4_BLOCK_SIZE * 16], &out[i*SM4_BLOCK_SIZE * 16 / sizeof(int)], key);
// }
// void sms4_avx2_encrypt_16blocks(const unsigned char *in, int *out, const u4 *key)
// {
// 	sms4_avx2_encrypt_8blocks(in,out,key);
// 	sms4_avx2_encrypt_8blocks(in + SM4_BLOCK_SIZE * 8, out + SM4_BLOCK_SIZE *8/4, key);
// }
// void sms4_avx2_decrypt_16blocks(const unsigned char *in, int *out, const u4 *key)
// {
// 	sms4_avx2_decrypt_8blocks(in,out,key);
// 	sms4_avx2_decrypt_8blocks(in + SM4_BLOCK_SIZE * 8, out + SM4_BLOCK_SIZE *8/4, key);
// }
// inline void sms4_init_sbox32(void)
// {
// 	int i, j;
// 	uint32_t a;
// 	for (i = 0; i < 256; i++) {
// 		for (j = 0; j < 256; j++) {
// 			a = Sbox[i] << 8 | Sbox[j];
// 			SBOX32L[(i << 8) + j] = a;
// 			SBOX32H[(i << 8) + j] = a << 16;
// 		}
// 	}
// }
// void sms4_avx2_encrypt_init(const u4 *key)
// {
// 	mask_ffff = _mm256_set1_epi32(0xffff);
// 	vindex_0s = _mm256_set1_epi32(0);
// 	vindex_4i = _mm256_setr_epi32(0,4,8,12,16,20,24,28);
// 	vindex_read = _mm256_setr_epi32(0,8,16,24,1,9,17,25);
// 	vindex_swap = _mm256_setr_epi8(
// 		3,2,1,0,7,6,5,4,11,10,9,8,15,14,13,12,
// 		3,2,1,0,7,6,5,4,11,10,9,8,15,14,13,12
// 	);
// 	sms4_init_sbox32();
// }

// void benchmark_sm4_encrypt(const uint8_t *p,  u1 *c, const u4 *key, unsigned int n_block)
// {
// 	int turns = 100000;
// 	clock_t t = clock();
// 	for (int i = 0; i<turns; i++)
// 		sms4_avx2_encrypt_blocks(p, (int*)c, key, n_block);
// 	double tt = (double)(clock() - t) / (CLOCKS_PER_SEC*turns);
// 	double speed = (double)(16 * n_block) / (1024 * 1024 * tt);
// 	printf("SM4_encrypt>>> blocks: %d, time: %f s, speed: %f MB/s\n", n_block, tt, speed);

// }
// void benchmark_sm4_decrypt(const uint8_t *p, u1 *c, const u4 *key, unsigned int n_block)
// {

// 	int turns = 100;
// 	clock_t t = clock();
// 	for (int i = 0; i<turns; i++)
// 		sms4_avx2_decrypt_blocks(p, (int*)c, key, n_block);
// 	double tt = (double)(clock() - t) / (CLOCKS_PER_SEC*turns);
// 	double speed = (double)(16 * n_block) / (1024 * 1024 * tt);
// 	printf("SM4_decrypt>>> blocks: %d, time: %f s, speed: %f MB/s\n", n_block, tt, speed);

// }
