#include "SM4_BS_256.h"
#ifdef BS_256bit
#include <time.h>
uint32_t BS_M[256][4];

uint64_t BS_N[128][4];
uint64_t BS_N_test[128][4];
__m256i BS_N_256[128];

uint32_t BS_M_inv[256][4];

uint64_t BS_RK[32][32][4];
uint64_t BS_RK_test[32][32][4];
__m256i BS_RK_256[32][32];

uint64_t buf[36][32][4];//enc round
__m256i buf_256[36][32];

uint64_t temp[36][32][4];//temp data
__m256i temp_256[36][32];
/*
 * Expanded SM4 S-boxes
 /* Sbox table: 8bits input convert to 8 bits output*/
 
static const unsigned char SboxTable[16][16] = 
{
    {0xd6,0x90,0xe9,0xfe,0xcc,0xe1,0x3d,0xb7,0x16,0xb6,0x14,0xc2,0x28,0xfb,0x2c,0x05},
    {0x2b,0x67,0x9a,0x76,0x2a,0xbe,0x04,0xc3,0xaa,0x44,0x13,0x26,0x49,0x86,0x06,0x99},
    {0x9c,0x42,0x50,0xf4,0x91,0xef,0x98,0x7a,0x33,0x54,0x0b,0x43,0xed,0xcf,0xac,0x62},
    {0xe4,0xb3,0x1c,0xa9,0xc9,0x08,0xe8,0x95,0x80,0xdf,0x94,0xfa,0x75,0x8f,0x3f,0xa6},
    {0x47,0x07,0xa7,0xfc,0xf3,0x73,0x17,0xba,0x83,0x59,0x3c,0x19,0xe6,0x85,0x4f,0xa8},
    {0x68,0x6b,0x81,0xb2,0x71,0x64,0xda,0x8b,0xf8,0xeb,0x0f,0x4b,0x70,0x56,0x9d,0x35},
    {0x1e,0x24,0x0e,0x5e,0x63,0x58,0xd1,0xa2,0x25,0x22,0x7c,0x3b,0x01,0x21,0x78,0x87},
    {0xd4,0x00,0x46,0x57,0x9f,0xd3,0x27,0x52,0x4c,0x36,0x02,0xe7,0xa0,0xc4,0xc8,0x9e},
    {0xea,0xbf,0x8a,0xd2,0x40,0xc7,0x38,0xb5,0xa3,0xf7,0xf2,0xce,0xf9,0x61,0x15,0xa1},
    {0xe0,0xae,0x5d,0xa4,0x9b,0x34,0x1a,0x55,0xad,0x93,0x32,0x30,0xf5,0x8c,0xb1,0xe3},
    {0x1d,0xf6,0xe2,0x2e,0x82,0x66,0xca,0x60,0xc0,0x29,0x23,0xab,0x0d,0x53,0x4e,0x6f},
    {0xd5,0xdb,0x37,0x45,0xde,0xfd,0x8e,0x2f,0x03,0xff,0x6a,0x72,0x6d,0x6c,0x5b,0x51},
    {0x8d,0x1b,0xaf,0x92,0xbb,0xdd,0xbc,0x7f,0x11,0xd9,0x5c,0x41,0x1f,0x10,0x5a,0xd8},
    {0x0a,0xc1,0x31,0x88,0xa5,0xcd,0x7b,0xbd,0x2d,0x74,0xd0,0x12,0xb8,0xe5,0xb4,0xb0},
    {0x89,0x69,0x97,0x4a,0x0c,0x96,0x77,0x7e,0x65,0xb9,0xf1,0x09,0xc5,0x6e,0xc6,0x84},
    {0x18,0xf0,0x7d,0xec,0x3a,0xdc,0x4d,0x20,0x79,0xee,0x5f,0x3e,0xd7,0xcb,0x39,0x48}
};

void hi()
{
    printf("hello world\n");
}

void BS_init_M()
{
    for(int i = 0; i < 256; i++)
    {
        BS_M[i][0] = 0x01234567; 
        BS_M[i][1] = 0x89abcdef;
        BS_M[i][2] = 0xfedcba98;
        BS_M[i][3] = 0x76543210;
        // for(int j = 0; j < 4; j++)
        // {
        //     printf("BS_M[%d]%08x",i,BS_M[i][j]);
        // }
        // printf("\n");
    }
}

void BS_TRANS()
{
    uint64_t temp = 0;
    uint64_t value = 0;
    uint64_t shift = 0x1;

    //  for(int i = 0; i < 256; i++)
    // {
    //     for(int j = 0; j < 4; j++)
    //     {
    //         printf("BS_M[%d]%08x",i,BS_M[i][j]);
    //     }
    //     printf("\n");
    // }
    
    for(int j = 0; j < 256; j++)
    {
        for(int i = 0; i < 128; i++)
        {
            for(int k = 0;k < 4; k++)
            {
                value = BS_M[j][3-i/32];
                // printf("value = %16x ",value);
                value = value  >> (i%32);
                temp = value & 0x1;
                // printf("temp = %8x j = %d ",temp,j);
                temp = temp << j;
                // printf("temp = %8x ",temp);
                BS_N[127-i][k] |= temp;//将其逆序
                // printf("BS_N[%d]=%016x value = %16x\n",i,BS_N[i],value);
            }
        }
        shift = shift << 1;
    }

    // printf("\ttest BS_N[128]\n");
    // for(int i = 0; i < 128; i++)
    // {
    //     for(int j = 0; j < 4; j++)
    //     {
    //         printf("BS_N[%d][%d]=%016llx",i,j,BS_N[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("\ttest BS_N end\n");

    for(int i = 0; i < 128; i++)//load data
    {
        BS_N_256[i] = _mm256_loadu_si256((__m256i*)BS_N[i]);
    }


    // for(int i = 0; i < 128; i++)//store data
    // {
    //     _mm256_store_si256((__m256i*)BS_N_test[i],BS_N_256[i]);
    // }

    // printf("\ttest BS_N_test[128]\n");
    // for(int i = 0; i < 128; i++)
    // {
    //     for(int j = 0; j < 4; j++)
    //     {
    //         printf("BS_N_test[%d][%d]=%016llx",i,j,BS_N_test[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("\ttest BS_N_test end\n");


}

void BS_TRANS_inv()//trans BS_N[128][4]*64bit to BS_M_inv[256][4]*32bit
{
    // printf("\ttest BS_N[128]\n");
    // for(int i = 0; i < 128; i++)
    // {
    //     for(int j = 0; j < 4; j++)
    //     {
    //         printf("BS_N[%d][%d]=%016llx",i,j,BS_N[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("\ttest BS_N end\n");

    //trans BS_N_256[128]*256bit to BS_N[128][4]*64bit 
    // for(int i = 0; i < 128; i++)//store data
    // {
    //     _mm256_store_si256((__m256i*)BS_N[i],BS_N_256[i]);
    // }

    // printf("\ttest BS_N_test[128]\n");
    // for(int i = 0; i < 128; i++)
    // {
    //     for(int j = 0; j < 4; j++)
    //     {
    //         printf("BS_N_test[%d][%d]=%016llx",i,j,BS_N[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("\ttest BS_N_test end\n");

    uint64_t temp = 0;
    uint64_t shift = 0x1;
    uint64_t locate;
    for(int j = 0; j < 256; j++)
    {
        locate = 0;
        for(int i = 0; i < 128; i++)
        {
            
            temp = BS_N[127-i][3-j/64] & shift;
            temp = (temp << locate);
            //printf("temp:%8x ",temp);            
            BS_M_inv[j][3-i/32] |= temp;
            //printf("inv:%8x\n",BS_M_inv[j][3-i/32]);
            locate++;
            if((i+1)%32==0)
                locate = 0;
        }
    }

    // for(int i = 0; i < 256; i++)
    // {
    //     for(int j = 0; j < 4; j++)
    //     {
    //         printf("BS_M_inv[%d]%08x",i,BS_M_inv[i][j]);
    //     }
    //     printf("\n");
    // }
}

/*
 * private function:
 * look up in SboxTable and get the related value.
 * args:    [in] inch: 0x00~0xFF (8 bits unsigned value).
 */
static unsigned char sm4Sbox(unsigned char inch)
{
    unsigned char *pTable = (unsigned char *)SboxTable;
    unsigned char retVal = (unsigned char)(pTable[inch]);
    return retVal;
}

/* private function:
 * Calculating round encryption key.
 * args:    [in] a: a is a 32 bits unsigned value;
 * return: sk[i]: i{0,1,2,3,...31}.
 */
static unsigned long sm4CalciRK(unsigned long ka)
{
    unsigned long bb = 0;
    unsigned long rk = 0;
    unsigned char a[4];
    unsigned char b[4];
    PUT_ULONG_BE(ka,a,0)
    b[0] = sm4Sbox(a[0]);
    b[1] = sm4Sbox(a[1]);
    b[2] = sm4Sbox(a[2]);
    b[3] = sm4Sbox(a[3]);
	GET_ULONG_BE(bb,b,0)
    rk = bb^(ROTL(bb, 13))^(ROTL(bb, 23));
    return rk;
}

void SM4_key_schedule(uint8_t key[16], uint32_t rkey[32])
{
	// System parameter or family key
	const uint32_t FK[4] = { 0xa3b1bac6, 0x56aa3350, 0x677d9197, 0xb27022dc };

	const uint32_t CK[32] = {
	0x00070E15, 0x1C232A31, 0x383F464D, 0x545B6269,
	0x70777E85, 0x8C939AA1, 0xA8AFB6BD, 0xC4CBD2D9,
	0xE0E7EEF5, 0xFC030A11, 0x181F262D, 0x343B4249,
	0x50575E65, 0x6C737A81, 0x888F969D, 0xA4ABB2B9,
	0xC0C7CED5, 0xDCE3EAF1, 0xF8FF060D, 0x141B2229,
	0x30373E45, 0x4C535A61, 0x686F767D, 0x848B9299,
	0xA0A7AEB5, 0xBCC3CAD1, 0xD8DFE6ED, 0xF4FB0209,
	0x10171E25, 0x2C333A41, 0x484F565D, 0x646B7279
	};

	uint32_t K[4];
    uint32_t MK[4];
    GET_ULONG_BE( MK[0], key, 0 );
    GET_ULONG_BE( MK[1], key, 4 );
    GET_ULONG_BE( MK[2], key, 8 );
    GET_ULONG_BE( MK[3], key, 12 );

	K[0] = MK[0] ^ FK[0];
	K[1] = MK[1] ^ FK[1];
	K[2] = MK[2] ^ FK[2];
	K[3] = MK[3] ^ FK[3];

	// for(int i=0; i<32; i++)
	// {
	// 	K[i % 4] ^= SM4_Tp(K[(i+1)%4] ^ K[(i+2)%4] ^ K[(i+3)%4] ^ CK[i]);
	// 	rkey[i] = K[i % 4];
	// }

    for(int i = 0; i<32; i++)
    {
        K[i+4] = K[i] ^ (sm4CalciRK(K[i+1]^K[i+2]^K[i+3]^CK[i]));
        rkey[i] = K[i+4];
	}

    //rkey[] bitslices 如果用AVX指令集会不会变快
    for(int i = 0; i<32; i++)
    {
        //printf("rkey[%d]=%08x\n",i,rkey[i]);
        uint64_t t = 0x1;
        for(int j = 0; j < 32; j++)
        {
            for(int k = 0; k < 4; k++)
            {
                if(rkey[i] & t)
                    BS_RK[i][31-j][k] = ~0;
                else
                {
                    BS_RK[i][31-j][k] = 0;
                }
            }
            t = t << 1;
        }
    }

    // printf("\ttest BS key_schedule:\n");
    // for(int i = 0; i < 32; i++)
    // {
    //     printf("\t key round%d:rkey=%08x\n",i,rkey[i]);
    //     for(int j = 0; j<32; j++)
    //     {
    //         for(int k = 0; k < 4; k++)
    //         {
    //             printf("BS_RK[%d][%d][%d]:%016llx ",i,j,k,BS_RK[i][j][k]);
    //         }
    //         printf("\n");
    //     }
    // }

    for(int i = 0; i < 32; i++)//load data
    {
        for(int j = 0; j < 32; j++)
        {
            BS_RK_256[i][j] = _mm256_loadu_si256((__m256i*)BS_RK[i][j]);
        }
    }

    // for(int i = 0; i < 32; i++)//store data
    // {
    //     for(int j = 0;j < 32; j++)
    //     {
    //         _mm256_store_si256((__m256i*)BS_RK_test[i][j],BS_RK_256[i][j]);
    //     }
    // }

    // printf("\ttest BS_mm256_store key_schedule:\n");
    // for(int i = 0; i < 4; i++)
    // {
    //     printf("\t key round%d:rkey=%08x\n",i,rkey[i]);
    //     for(int j = 0; j<32; j++)
    //     {
    //         for(int k = 0; k < 4; k++)
    //         {
    //             printf("BS_RK_test[%d][%d][%d]:%016llx ",i,j,k,BS_RK_test[i][j][k]);
    //         }
    //         printf("\n");
    //     }
    // }
    
        
}

void BS_iteration()
{
    int i = 0;
    uint64_t t1 , t2;

    //printf("test init_buf[][] 4 round:\n");
    for(int j = 0; j < 4; j++)//bingo 256bit
    {
        for(int k = 0; k < 32; k++)
        {
            buf_256[j][k] = BS_N_256[32*j+k];//load data
            // _mm256_store_si256((__m256i*)buf[i][j],buf_256[j][k]);
            // for(int m = 0; m < 4; m++)
            // {
            //     printf("buf[%d][%d][%d]=%016llx",i,j,m,buf[i][j][m]);
            // }
            // printf("\n");
        }     
    }
    //printf("test init_buf[][] end:\n");
        
    while(i < 32)//32轮迭代计算
    {
        for(int j = 0; j < 32; j++)//4道32bit数据操作:
        {
            buf_256[4+i][j]= buf_256[i+1][j] ^ buf_256[i+2][j] ^ buf_256[i+3][j] ^ BS_RK_256[i][j];
            // _mm256_store_si256((__m256i*)buf[4+i][j],buf_256[4+i][j]);
            // for(int m = 0; m < 4; m++)
            // {
            //     printf("buf[%d][%d][%d]=%016llx",i,j,m,buf[4+i][j][m]);
            // }
            // printf("\n");
        }

        //printf("\ttest sbox:\n");
        S_box(i);//bingo256 合成置换T的非线性变换
        
        //printf("\tafter shift\n");
        for(int j = 0; j < 32; j++)//bingo256 4道32bit数据操作:合成置换T的线性变换L
        {
            // printf("buf[%d][%d]=%016llx ",i+4,j,buf[4+i][j]);
            // printf("buf<<<2=%016llx ",buf[4+i][(j+2)]);
            // printf("buf<<<2=%016llx ",buf[4+i][(j+10)]);
            // printf("buf<<<2=%016llx ",buf[4+i][(j+18)]);
            // printf("buf<<<2=%016llx ",buf[4+i][(j+24)]);
            temp_256[4+i][j]= buf_256[4+i][j] ^ buf_256[4+i][(j+2)%32] ^ buf_256[4+i][(j+10)%32] ^ buf_256[4+i][(j+18)%32] ^ buf_256[4+i][(j+24)%32];
            // printf("temp[%d][%d]=%016llx ",i+4,j,temp[4+i][j]);
            // if((j+1)%4==0) printf("\n");
        }
        for(int j = 0; j < 32; j++)//4道32bit数据操作
        {
            buf_256[4+i][j]= temp_256[i+4][j] ^ buf_256[i][j];
            //[4+i][j] = _mm256_xor_si256(temp_256[i+4][j],buf_256[i][j]);
        }        
        i++;
    }

    for(int j = 0; j < 4; j++)//反序计算
    {
        for(int k = 0; k < 32; k++)
        {
            //BS_N_256[32*j+k] = buf_256[35-j][k];
            _mm256_store_si256((__m256i*)BS_N[32*j+k],buf_256[35-j][k]);
        }
    }

    // for(int i = 0; i < 128; i++)//store data
    // {
    //     _mm256_store_si256((__m256i*)BS_N_test[i],BS_N_256[i]);
    // }

    // printf("\ttest BS_N_test[128]\n");
    // for(int i = 0; i < 128; i++)
    // {
    //     for(int j = 0; j < 4; j++)
    //     {
    //         printf("BS_N_test[%d][%d]=%016llx",i,j,BS_N_test[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("\ttest BS_N_test end\n");

}

void S_box(int round)
{
    bits sm4;
    for(int i = 0; i<4; i++)
    {
        sm4.b7 = buf_256[round+4][i*8];
        sm4.b6 = buf_256[round+4][i*8+1];
        sm4.b5 = buf_256[round+4][i*8+2];
        sm4.b4 = buf_256[round+4][i*8+3];
        sm4.b3 = buf_256[round+4][i*8+4];
        sm4.b2 = buf_256[round+4][i*8+5];
        sm4.b1 = buf_256[round+4][i*8+6];
        sm4.b0 = buf_256[round+4][i*8+7];

        Sm4_BoolFun(sm4,&buf_256[round+4][i*8+7],&buf_256[round+4][i*8+6],&buf_256[round+4][i*8+5],&buf_256[round+4][i*8+4],
            &buf_256[round+4][i*8+3],&buf_256[round+4][i*8+2],&buf_256[round+4][i*8+1],&buf_256[round+4][i*8]);

        // for(int j = 0; j<8; j++)
        // {
        //     _mm256_store_si256((__m256i*)buf[round+4][i*8+j],buf_256[round+4][i*8+j]);
        //     for(int m = 0; m < 4; m++)
        //     {
        //         printf("buf[%d][%d][%d]=%016llx",i,j,m,buf[round+4][i*8+j][m]);
        //     }
        //     printf("\n");
        // }
    }
    //for(int )

}


void SM4_BS_enc()
{
    BS_TRANS();//bingo256
    BS_iteration();
    BS_TRANS_inv();//bingo256
}

void benchmark_sm4_bs_encrypt()
{
    unsigned char key[16] = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};
    uint32_t rk[32];
    SM4_key_schedule(key,rk);//bingo
    BS_init_M();//bingo

	int turns = 1000;
	clock_t t = clock();
	for (int i = 0; i<turns; i++)
		SM4_BS_enc();
	double tt = (double)(clock() - t) / (CLOCKS_PER_SEC*turns);
	double speed = (double)(128 * 256) / (1024 * 1024 * tt);
	printf("SM4_encrypt_BS256bit>>> , time: %f s, speed: %f Mb/s\n", tt,speed);

}

void Sm4_BoolFun(bits in, bit_t *out0, bit_t *out1, bit_t *out2, bit_t *out3, bit_t *out4, bit_t *out5, bit_t *out6, bit_t *out7) {
  bit_t var8 = ~in.b7;
  bit_t var9 = var8 | in.b6;
  bit_t var10 = var9 & in.b5;
  bit_t var11 = in.b2 ^ var10;
  bit_t var12 = var11 | in.b0;
  bit_t var13 = in.b6 ^ var12;
  bit_t var14 = in.b7 & var11;
  bit_t var15 = var14 | in.b0;
  bit_t var16 = var10 ^ var15;
  bit_t var17 = var16 & in.b4;
  bit_t var18 = var13 ^ var17;
  bit_t var19 = var18 ^ var8;
  bit_t var20 = var19 | in.b5;
  bit_t var21 = in.b7 ^ var20;
  bit_t var22 = var18 | in.b7;
  bit_t var23 = var22 ^ var15;
  bit_t var24 = var23 | in.b4;
  bit_t var25 = var21 ^ var24;
  bit_t var26 = var25 | in.b2;
  bit_t var27 = var18 ^ var26;
  bit_t var28 = in.b7 ^ in.b2;
  bit_t var29 = var28 & in.b4;
  bit_t var30 = var27 ^ var29;
  bit_t var31 = var18 | var28;
  bit_t var32 = var31 ^ var25;
  bit_t var33 = var32 & in.b5;
  bit_t var34 = var30 ^ var33;
  bit_t var35 = var8 & var32;
  bit_t var36 = in.b6 ^ var9;
  bit_t var37 = var36 | in.b2;
  bit_t var38 = var35 ^ var37;
  bit_t var39 = var38 | in.b0;
  bit_t var40 = var34 ^ var39;
  bit_t var41 = var40 & in.b1;
  bit_t var42 = var27 ^ var41;
  bit_t var43 = var10 & in.b6;
  bit_t var44 = var43 ^ var40;
  bit_t var45 = var26 & var19;
  bit_t var46 = var45 | in.b4;
  bit_t var47 = var44 ^ var46;
  bit_t var48 = in.b0 ^ var24;
  bit_t var49 = var48 ^ var44;
  bit_t var50 = var48 & var47;
  bit_t var51 = var50 & in.b7;
  bit_t var52 = var49 ^ var51;
  bit_t var53 = var52 & in.b1;
  bit_t var54 = var47 ^ var53;
  bit_t var55 = var42 & var9;
  bit_t var56 = var55 ^ var39;
  bit_t var57 = var53 ^ var43;
  bit_t var58 = var57 & in.b4;
  bit_t var59 = var56 ^ var58;
  bit_t var60 = var59 & var25;
  bit_t var61 = var60 ^ var13;
  bit_t var62 = var8 | var59;
  bit_t var63 = var62 ^ var30;
  bit_t var64 = var63 & in.b1;
  bit_t var65 = var61 ^ var64;
  bit_t var66 = var65 & in.b0;
  bit_t var67 = var59 ^ var66;
  bit_t var68 = var67 & in.b2;
  bit_t var69 = var54 ^ var68;
  bit_t var70 = var69 & in.b3;
  *out4 = var42 ^ var70;
  bit_t var72 = in.b4 ^ var27;
  bit_t var73 = in.b0 ^ var30;
  bit_t var74 = var73 & in.b6;
  bit_t var75 = var72 ^ var74;
  bit_t var76 = var49 & *out4;
  bit_t var77 = in.b7 ^ var69;
  bit_t var78 = var77 ^ var36;
  bit_t var79 = var78 | in.b4;
  bit_t var80 = var76 ^ var79;
  bit_t var81 = var80 | in.b5;
  bit_t var82 = var75 ^ var81;
  bit_t var83 = var69 & var53;
  bit_t var84 = var83 ^ var13;
  bit_t var85 = var75 & in.b2;
  bit_t var86 = var84 ^ var85;
  bit_t var87 = in.b6 ^ var8;
  bit_t var88 = var39 ^ var53;
  bit_t var89 = var88 | var65;
  bit_t var90 = var89 & in.b5;
  bit_t var91 = var87 ^ var90;
  bit_t var92 = var91 & in.b4;
  bit_t var93 = var86 ^ var92;
  bit_t var94 = var93 | in.b3;
  bit_t var95 = var82 ^ var94;
  bit_t var96 = var75 & var63;
  bit_t var97 = var96 | var92;
  bit_t var98 = var97 | in.b2;
  bit_t var99 = var69 ^ var98;
  bit_t var100 = var12 & var19;
  bit_t var101 = var100 ^ var84;
  bit_t var102 = var101 | in.b2;
  bit_t var103 = var66 ^ var102;
  bit_t var104 = var103 & in.b3;
  bit_t var105 = var99 ^ var104;
  bit_t var106 = var67 & var104;
  bit_t var107 = var106 ^ var19;
  bit_t var108 = var107 | in.b6;
  bit_t var109 = var35 ^ var108;
  bit_t var110 = var31 & *out4;
  bit_t var111 = var110 ^ in.b3;
  bit_t var112 = var42 ^ var101;
  bit_t var113 = var112 & in.b6;
  bit_t var114 = var111 ^ var113;
  bit_t var115 = var114 & in.b4;
  bit_t var116 = var109 ^ var115;
  bit_t var117 = var116 & in.b5;
  bit_t var118 = var105 ^ var117;
  bit_t var119 = var118 | in.b1;
  *out6 = var95 ^ var119;
  bit_t var121 = var117 ^ var63;
  bit_t var122 = var121 & var108;
  bit_t var123 = var49 | in.b4;
  bit_t var124 = var122 ^ var123;
  bit_t var125 = var93 ^ var98;
  bit_t var126 = var97 ^ var34;
  bit_t var127 = var126 | var60;
  bit_t var128 = var127 | in.b4;
  bit_t var129 = var125 ^ var128;
  bit_t var130 = var129 & in.b3;
  bit_t var131 = var124 ^ var130;
  bit_t var132 = var81 ^ var78;
  bit_t var133 = var36 ^ var35;
  bit_t var134 = var133 | in.b4;
  bit_t var135 = var132 ^ var134;
  bit_t var136 = var119 | *out6;
  bit_t var137 = var60 | var132;
  bit_t var138 = var137 | in.b7;
  bit_t var139 = var136 ^ var138;
  bit_t var140 = var139 | in.b3;
  bit_t var141 = var135 ^ var140;
  bit_t var142 = var141 & in.b2;
  bit_t var143 = var131 ^ var142;
  bit_t var144 = var49 & var121;
  bit_t var145 = ~var144;
  bit_t var146 = var106 | var113;
  bit_t var147 = var146 | in.b2;
  bit_t var148 = var145 ^ var147;
  bit_t var149 = var40 ^ var113;
  bit_t var150 = var149 | in.b3;
  bit_t var151 = var148 ^ var150;
  bit_t var152 = var109 & var105;
  bit_t var153 = var152 ^ var97;
  bit_t var154 = var153 & in.b0;
  bit_t var155 = var95 ^ var154;
  bit_t var156 = var118 | var135;
  bit_t var157 = var156 ^ var128;
  bit_t var158 = var157 | in.b3;
  bit_t var159 = var147 ^ var158;
  bit_t var160 = var159 & in.b5;
  bit_t var161 = var155 ^ var160;
  bit_t var162 = var161 | in.b7;
  bit_t var163 = var151 ^ var162;
  bit_t var164 = var163 & in.b1;
  *out7 = var143 ^ var164;
  bit_t var166 = var115 & var82;
  bit_t var167 = var166 ^ var77;
  bit_t var168 = var156 ^ var60;
  bit_t var169 = var168 | in.b2;
  bit_t var170 = var167 ^ var169;
  bit_t var171 = var152 & var115;
  bit_t var172 = var171 ^ var65;
  bit_t var173 = var172 & in.b1;
  bit_t var174 = var170 ^ var173;
  bit_t var175 = var87 ^ in.b4;
  bit_t var176 = var175 ^ var139;
  bit_t var177 = var61 & in.b1;
  bit_t var178 = var176 ^ var177;
  bit_t var179 = var49 & var37;
  bit_t var180 = var179 ^ *out7;
  bit_t var181 = var180 | in.b7;
  bit_t var182 = var178 ^ var181;
  bit_t var183 = var182 | in.b3;
  bit_t var184 = var174 ^ var183;
  bit_t var185 = var137 & var83;
  bit_t var186 = ~var162;
  bit_t var187 = var186 | var95;
  bit_t var188 = var187 | in.b1;
  bit_t var189 = var185 ^ var188;
  bit_t var190 = var166 | var10;
  bit_t var191 = var160 | var100;
  bit_t var192 = var191 | in.b1;
  bit_t var193 = var190 ^ var192;
  bit_t var194 = var193 & in.b3;
  bit_t var195 = var189 ^ var194;
  bit_t var196 = var192 ^ var149;
  bit_t var197 = var196 & var138;
  bit_t var198 = ~var43;
  bit_t var199 = var128 ^ var140;
  bit_t var200 = var199 & in.b2;
  bit_t var201 = var198 ^ var200;
  bit_t var202 = var201 & in.b1;
  bit_t var203 = var197 ^ var202;
  bit_t var204 = var203 | in.b4;
  bit_t var205 = var195 ^ var204;
  bit_t var206 = var205 & in.b0;
  *out2 = var184 ^ var206;
  bit_t var208 = var200 ^ var29;
  bit_t var209 = var61 | var30;
  bit_t var210 = var209 | in.b5;
  bit_t var211 = var208 ^ var210;
  bit_t var212 = var168 & var132;
  bit_t var213 = var212 ^ var205;
  bit_t var214 = var213 | in.b1;
  bit_t var215 = var211 ^ var214;
  bit_t var216 = var136 ^ var32;
  bit_t var217 = var164 | var196;
  bit_t var218 = var217 & in.b0;
  bit_t var219 = var216 ^ var218;
  bit_t var220 = var96 & var214;
  bit_t var221 = var220 | var54;
  bit_t var222 = var158 ^ var176;
  bit_t var223 = var222 & in.b0;
  bit_t var224 = var221 ^ var223;
  bit_t var225 = var224 & in.b4;
  bit_t var226 = var219 ^ var225;
  bit_t var227 = var226 | in.b6;
  bit_t var228 = var215 ^ var227;
  bit_t var229 = var146 | var172;
  bit_t var230 = var229 & var215;
  bit_t var231 = var164 | var80;
  bit_t var232 = var231 ^ var228;
  bit_t var233 = var93 & in.b4;
  bit_t var234 = var232 ^ var233;
  bit_t var235 = var234 & in.b5;
  bit_t var236 = var230 ^ var235;
  bit_t var237 = var234 | var124;
  bit_t var238 = var237 ^ var131;
  bit_t var239 = var223 ^ var127;
  bit_t var240 = var191 ^ var197;
  bit_t var241 = var240 & in.b1;
  bit_t var242 = var239 ^ var241;
  bit_t var243 = var242 & in.b4;
  bit_t var244 = var238 ^ var243;
  bit_t var245 = var244 & in.b6;
  bit_t var246 = var236 ^ var245;
  bit_t var247 = var246 & in.b3;
  *out1 = var228 ^ var247;
  bit_t var249 = var200 ^ var47;
  bit_t var250 = var184 ^ var75;
  bit_t var251 = var250 | in.b5;
  bit_t var252 = var249 ^ var251;
  bit_t var253 = var92 | var53;
  bit_t var254 = var56 ^ var153;
  bit_t var255 = var254 | in.b3;
  bit_t var256 = var253 ^ var255;
  bit_t var257 = var256 | in.b0;
  bit_t var258 = var252 ^ var257;
  bit_t var259 = var142 ^ var158;
  bit_t var260 = var259 | var97;
  bit_t var261 = var73 ^ var253;
  bit_t var262 = var169 ^ var21;
  bit_t var263 = var262 & in.b3;
  bit_t var264 = var261 ^ var263;
  bit_t var265 = var264 | in.b0;
  bit_t var266 = var260 ^ var265;
  bit_t var267 = var266 | in.b7;
  bit_t var268 = var258 ^ var267;
  bit_t var269 = var245 ^ var167;
  bit_t var270 = var269 & in.b4;
  bit_t var271 = in.b2 ^ var270;
  bit_t var272 = var249 & var145;
  bit_t var273 = var84 ^ var235;
  bit_t var274 = var273 | in.b2;
  bit_t var275 = var272 ^ var274;
  bit_t var276 = var275 | in.b6;
  bit_t var277 = var271 ^ var276;
  bit_t var278 = var193 | var67;
  bit_t var279 = var278 & var94;
  bit_t var280 = *out4 | var278;
  bit_t var281 = var280 ^ var237;
  bit_t var282 = var70 | var48;
  bit_t var283 = var282 | in.b2;
  bit_t var284 = var281 ^ var283;
  bit_t var285 = var284 & in.b6;
  bit_t var286 = var279 ^ var285;
  bit_t var287 = var286 & in.b0;
  bit_t var288 = var277 ^ var287;
  bit_t var289 = var288 | in.b1;
  *out3 = var268 ^ var289;
  bit_t var291 = in.b7 & var216;
  bit_t var292 = var80 ^ var259;
  bit_t var293 = var292 | in.b4;
  bit_t var294 = var291 ^ var293;
  bit_t var295 = var231 ^ var140;
  bit_t var296 = var295 | var113;
  bit_t var297 = var296 & in.b5;
  bit_t var298 = var294 ^ var297;
  bit_t var299 = var31 ^ var79;
  bit_t var300 = var78 ^ var126;
  bit_t var301 = var300 | in.b2;
  bit_t var302 = var299 ^ var301;
  bit_t var303 = var50 ^ var271;
  bit_t var304 = var303 | in.b7;
  bit_t var305 = var216 ^ var304;
  bit_t var306 = var305 & in.b0;
  bit_t var307 = var302 ^ var306;
  bit_t var308 = var307 & in.b1;
  bit_t var309 = var298 ^ var308;
  bit_t var310 = var168 ^ var102;
  bit_t var311 = var181 ^ var276;
  bit_t var312 = var311 | in.b7;
  bit_t var313 = var310 ^ var312;
  bit_t var314 = var305 | var243;
  bit_t var315 = var314 ^ var134;
  bit_t var316 = var315 & in.b5;
  bit_t var317 = var313 ^ var316;
  bit_t var318 = var32 ^ var154;
  bit_t var319 = var318 ^ var186;
  bit_t var320 = var289 ^ var194;
  bit_t var321 = var125 & in.b0;
  bit_t var322 = var320 ^ var321;
  bit_t var323 = var322 | in.b2;
  bit_t var324 = var319 ^ var323;
  bit_t var325 = var324 | in.b1;
  bit_t var326 = var317 ^ var325;
  bit_t var327 = var326 & in.b3;
  *out5 = var309 ^ var327;
  bit_t var329 = var122 ^ var115;
  bit_t var330 = var140 | var299;
  bit_t var331 = var330 & in.b7;
  bit_t var332 = var329 ^ var331;
  bit_t var333 = var115 ^ var250;
  bit_t var334 = var236 & in.b0;
  bit_t var335 = var333 ^ var334;
  bit_t var336 = var335 & in.b3;
  bit_t var337 = var332 ^ var336;
  bit_t var338 = var266 ^ var164;
  bit_t var339 = var70 ^ var239;
  bit_t var340 = var339 | in.b5;
  bit_t var341 = var338 ^ var340;
  bit_t var342 = var88 ^ var242;
  bit_t var343 = var342 | var98;
  bit_t var344 = var343 | in.b7;
  bit_t var345 = var341 ^ var344;
  bit_t var346 = var345 & in.b1;
  bit_t var347 = var337 ^ var346;
  bit_t var348 = var295 ^ var74;
  bit_t var349 = var348 ^ var268;
  bit_t var350 = var59 ^ var157;
  bit_t var351 = var347 ^ var262;
  bit_t var352 = var351 | in.b7;
  bit_t var353 = var350 ^ var352;
  bit_t var354 = var353 & in.b1;
  bit_t var355 = var349 ^ var354;
  bit_t var356 = var115 ^ var15;
  bit_t var357 = var356 | var126;
  bit_t var358 = var193 ^ var286;
  bit_t var359 = var25 ^ var260;
  bit_t var360 = var359 & in.b0;
  bit_t var361 = var358 ^ var360;
  bit_t var362 = var361 | in.b1;
  bit_t var363 = var357 ^ var362;
  bit_t var364 = var363 & in.b3;
  bit_t var365 = var355 ^ var364;
  bit_t var366 = var365 & in.b2;
  *out0 = var347 ^ var366;
}

#endif