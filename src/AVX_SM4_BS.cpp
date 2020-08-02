// #include "AVX_SM4_BS.h"
// #define BS_lenth 64
// __m256i si[8];
// uint32_t M[256][4];//256*128bit 128=4*32bit
// __m256i N[128];//128*256bit
// __m256i X[128];
// void hello()
// {
//     printf("hello world\n");
// }

// void ReverseOrder(__m256i *N,__m256i *X)
// {
//     for(int i = 0; i < 128; i++)
//     {
//         X[127-i] = N[i];
//     }
// }

// //对 256×128 比特数据，分为两个比特方阵，对方阵进行比特粒度的转置.--Bitslice implementation of AES
// void TRANS(uint32_t **M,__m256i *N)
// {
//     __m128i M1[128],M2[128];
//     //load M1 & M2
//     for(int i = 0; i < 128; i++)
//     {
//         M1[i] = _mm_load_si128((__m128i*)M[i]);//_mm256_load_si256((__m256i*)M[i]);
//         M1[i] = _mm_load_si128((__m128i*)M[i+128]);
//     }

//     //trans


// }

// //    4*4 128bit Trans
// void Transpose4x4_I(int *Src, int *Dest, int WidthS, int WidthD)
// {
//     __m128i S0 = _mm_loadu_si128((__m128i *)(Src + 0 * WidthS));          //    A3 A2 A1 A0
//     __m128i S1 = _mm_loadu_si128((__m128i *)(Src + 1 * WidthS));          //    B3 B2 B1 B0
//     __m128i S01L = _mm_unpacklo_epi32(S0, S1);                            //    B1 A1 B0 A0
//     __m128i S01H = _mm_unpackhi_epi32(S0, S1);                            //    B3 A3 B2 A2
    
//     __m128i S2 = _mm_loadu_si128((__m128i *)(Src + 2 * WidthS));          //    C3 C2 C1 C0
//     __m128i S3 = _mm_loadu_si128((__m128i *)(Src + 3 * WidthS));          //    D3 D2 D1 D0
//     __m128i S23L = _mm_unpacklo_epi32(S2, S3);                            //    D1 C1 D0 C0
//     __m128i S23H = _mm_unpackhi_epi32(S2, S3);                            //    D3 C3 D2 C2

//     _mm_storeu_si128((__m128i *)(Dest + 0 * WidthD), _mm_unpacklo_epi64(S01L, S23L));    //    D0 C0 B0 A0
//     _mm_storeu_si128((__m128i *)(Dest + 1 * WidthD), _mm_unpackhi_epi64(S01L, S23L));    //    D1 C1 B1 A1
//     _mm_storeu_si128((__m128i *)(Dest + 2 * WidthD), _mm_unpacklo_epi64(S01H, S23H));    //    D2 C2 B2 A2
//     _mm_storeu_si128((__m128i *)(Dest + 3 * WidthD), _mm_unpackhi_epi64(S01H, S23H));    //    D3 C3 B3 A3
// }

// void init_M()
// {
//     for(int i = 0; i < 256; i++)
//     {
//         M[i][0] = 0x01234567; 
//         M[i][1] = 0x89abcdef;
//         M[i][2] = 0xfedcba98;
//         M[i][3] = 0x76543210;
//         // for(int j = 0; j < 4; j++)
//         // {
//         //     printf("%08x",M[i][j]);
//         // }
//         // printf("\n");
//     }
// }

// void test_TRANS()
// {
//     init_M();

// }

// void DataPresentation_8bit()
// {
//     __m256i t,temp;
//     unsigned int op1[8] = {0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff};
//     //{0x12345678,0x2345689,0x3456789a,0x456789ab,0x56789abc,0x6789abcd,0x789abcde,0x89abcdef};
//     //{0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff};
//     unsigned int result[8][8];
//     unsigned int in[8][8]={
//         {0x55555555,0x55555555,0x55555555,0x55555555,0x55555555,0x55555555,0x55555555,0x55555555},
//         {0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333},
//         {0x0f0f0f0f,0x0f0f0f0f,0x0f0f0f0f,0x0f0f0f0f,0x0f0f0f0f,0x0f0f0f0f,0x0f0f0f0f,0x0f0f0f0f},
//         {0x00ff00ff,0x00ff00ff,0x00ff00ff,0x00ff00ff,0x00ff00ff,0x00ff00ff,0x00ff00ff,0x00ff00ff},
//         {0x0000ffff,0x0000ffff,0x0000ffff,0x0000ffff,0x0000ffff,0x0000ffff,0x0000ffff,0x0000ffff},
//         {0x00000000,0xffffffff,0x00000000,0xffffffff,0x00000000,0xffffffff,0x00000000,0xffffffff},
//         {0x00000000,0x00000000,0xffffffff,0xffffffff,0x00000000,0x00000000,0xffffffff,0xffffffff},
//         {0x00000000,0x00000000,0x00000000,0x00000000,0xffffffff,0xffffffff,0xffffffff,0xffffffff}
//     };
    
//     //load
//     __m256i x[8];
//     for(int i = 0;i < 8; i++)
//     {
//         x[i] = _mm256_load_si256((__m256i*)in[i]);
//     }
    
//     //calculate
//     bits sm4;
//     sm4.b0 = x[0];
//     sm4.b1 = x[1];
//     sm4.b2 = x[2];
//     sm4.b3 = x[3];
//     sm4.b4 = x[4];
//     sm4.b5 = x[5];
//     sm4.b6 = x[6];
//     sm4.b7 = x[7];
//     Sm4_BoolFun(sm4,&x[0],&x[1],&x[2],&x[3],&x[4],&x[5],&x[6],&x[7]);

//     //store
//     for(int i = 0;i < 8; i++)
//     {
//         _mm256_store_si256((__m256i*)(result[i]),x[i]);       
//     }
//     for(int i = 0;i < 8; i++)
//     {
//         printf("sm[%d]",i);
//         for(int j = 0;j < 8; j++)
//         {
//             printf(":%08x",result[i][j]);
//         }   
//         printf("\n");   
//     }
    

//     printf("\n");
// }

// void Sm4_BoolFun(bits in, bit_t *out0, bit_t *out1, bit_t *out2, bit_t *out3, bit_t *out4, bit_t *out5, bit_t *out6, bit_t *out7) {
//   bit_t var8 = ~in.b7;
//   bit_t var9 = var8 | in.b6;
//   bit_t var10 = var9 & in.b5;
//   bit_t var11 = in.b2 ^ var10;
//   bit_t var12 = var11 | in.b0;
//   bit_t var13 = in.b6 ^ var12;
//   bit_t var14 = in.b7 & var11;
//   bit_t var15 = var14 | in.b0;
//   bit_t var16 = var10 ^ var15;
//   bit_t var17 = var16 & in.b4;
//   bit_t var18 = var13 ^ var17;
//   bit_t var19 = var18 ^ var8;
//   bit_t var20 = var19 | in.b5;
//   bit_t var21 = in.b7 ^ var20;
//   bit_t var22 = var18 | in.b7;
//   bit_t var23 = var22 ^ var15;
//   bit_t var24 = var23 | in.b4;
//   bit_t var25 = var21 ^ var24;
//   bit_t var26 = var25 | in.b2;
//   bit_t var27 = var18 ^ var26;
//   bit_t var28 = in.b7 ^ in.b2;
//   bit_t var29 = var28 & in.b4;
//   bit_t var30 = var27 ^ var29;
//   bit_t var31 = var18 | var28;
//   bit_t var32 = var31 ^ var25;
//   bit_t var33 = var32 & in.b5;
//   bit_t var34 = var30 ^ var33;
//   bit_t var35 = var8 & var32;
//   bit_t var36 = in.b6 ^ var9;
//   bit_t var37 = var36 | in.b2;
//   bit_t var38 = var35 ^ var37;
//   bit_t var39 = var38 | in.b0;
//   bit_t var40 = var34 ^ var39;
//   bit_t var41 = var40 & in.b1;
//   bit_t var42 = var27 ^ var41;
//   bit_t var43 = var10 & in.b6;
//   bit_t var44 = var43 ^ var40;
//   bit_t var45 = var26 & var19;
//   bit_t var46 = var45 | in.b4;
//   bit_t var47 = var44 ^ var46;
//   bit_t var48 = in.b0 ^ var24;
//   bit_t var49 = var48 ^ var44;
//   bit_t var50 = var48 & var47;
//   bit_t var51 = var50 & in.b7;
//   bit_t var52 = var49 ^ var51;
//   bit_t var53 = var52 & in.b1;
//   bit_t var54 = var47 ^ var53;
//   bit_t var55 = var42 & var9;
//   bit_t var56 = var55 ^ var39;
//   bit_t var57 = var53 ^ var43;
//   bit_t var58 = var57 & in.b4;
//   bit_t var59 = var56 ^ var58;
//   bit_t var60 = var59 & var25;
//   bit_t var61 = var60 ^ var13;
//   bit_t var62 = var8 | var59;
//   bit_t var63 = var62 ^ var30;
//   bit_t var64 = var63 & in.b1;
//   bit_t var65 = var61 ^ var64;
//   bit_t var66 = var65 & in.b0;
//   bit_t var67 = var59 ^ var66;
//   bit_t var68 = var67 & in.b2;
//   bit_t var69 = var54 ^ var68;
//   bit_t var70 = var69 & in.b3;
//   *out4 = var42 ^ var70;
//   bit_t var72 = in.b4 ^ var27;
//   bit_t var73 = in.b0 ^ var30;
//   bit_t var74 = var73 & in.b6;
//   bit_t var75 = var72 ^ var74;
//   bit_t var76 = var49 & *out4;
//   bit_t var77 = in.b7 ^ var69;
//   bit_t var78 = var77 ^ var36;
//   bit_t var79 = var78 | in.b4;
//   bit_t var80 = var76 ^ var79;
//   bit_t var81 = var80 | in.b5;
//   bit_t var82 = var75 ^ var81;
//   bit_t var83 = var69 & var53;
//   bit_t var84 = var83 ^ var13;
//   bit_t var85 = var75 & in.b2;
//   bit_t var86 = var84 ^ var85;
//   bit_t var87 = in.b6 ^ var8;
//   bit_t var88 = var39 ^ var53;
//   bit_t var89 = var88 | var65;
//   bit_t var90 = var89 & in.b5;
//   bit_t var91 = var87 ^ var90;
//   bit_t var92 = var91 & in.b4;
//   bit_t var93 = var86 ^ var92;
//   bit_t var94 = var93 | in.b3;
//   bit_t var95 = var82 ^ var94;
//   bit_t var96 = var75 & var63;
//   bit_t var97 = var96 | var92;
//   bit_t var98 = var97 | in.b2;
//   bit_t var99 = var69 ^ var98;
//   bit_t var100 = var12 & var19;
//   bit_t var101 = var100 ^ var84;
//   bit_t var102 = var101 | in.b2;
//   bit_t var103 = var66 ^ var102;
//   bit_t var104 = var103 & in.b3;
//   bit_t var105 = var99 ^ var104;
//   bit_t var106 = var67 & var104;
//   bit_t var107 = var106 ^ var19;
//   bit_t var108 = var107 | in.b6;
//   bit_t var109 = var35 ^ var108;
//   bit_t var110 = var31 & *out4;
//   bit_t var111 = var110 ^ in.b3;
//   bit_t var112 = var42 ^ var101;
//   bit_t var113 = var112 & in.b6;
//   bit_t var114 = var111 ^ var113;
//   bit_t var115 = var114 & in.b4;
//   bit_t var116 = var109 ^ var115;
//   bit_t var117 = var116 & in.b5;
//   bit_t var118 = var105 ^ var117;
//   bit_t var119 = var118 | in.b1;
//   *out6 = var95 ^ var119;
//   bit_t var121 = var117 ^ var63;
//   bit_t var122 = var121 & var108;
//   bit_t var123 = var49 | in.b4;
//   bit_t var124 = var122 ^ var123;
//   bit_t var125 = var93 ^ var98;
//   bit_t var126 = var97 ^ var34;
//   bit_t var127 = var126 | var60;
//   bit_t var128 = var127 | in.b4;
//   bit_t var129 = var125 ^ var128;
//   bit_t var130 = var129 & in.b3;
//   bit_t var131 = var124 ^ var130;
//   bit_t var132 = var81 ^ var78;
//   bit_t var133 = var36 ^ var35;
//   bit_t var134 = var133 | in.b4;
//   bit_t var135 = var132 ^ var134;
//   bit_t var136 = var119 | *out6;
//   bit_t var137 = var60 | var132;
//   bit_t var138 = var137 | in.b7;
//   bit_t var139 = var136 ^ var138;
//   bit_t var140 = var139 | in.b3;
//   bit_t var141 = var135 ^ var140;
//   bit_t var142 = var141 & in.b2;
//   bit_t var143 = var131 ^ var142;
//   bit_t var144 = var49 & var121;
//   bit_t var145 = ~var144;
//   bit_t var146 = var106 | var113;
//   bit_t var147 = var146 | in.b2;
//   bit_t var148 = var145 ^ var147;
//   bit_t var149 = var40 ^ var113;
//   bit_t var150 = var149 | in.b3;
//   bit_t var151 = var148 ^ var150;
//   bit_t var152 = var109 & var105;
//   bit_t var153 = var152 ^ var97;
//   bit_t var154 = var153 & in.b0;
//   bit_t var155 = var95 ^ var154;
//   bit_t var156 = var118 | var135;
//   bit_t var157 = var156 ^ var128;
//   bit_t var158 = var157 | in.b3;
//   bit_t var159 = var147 ^ var158;
//   bit_t var160 = var159 & in.b5;
//   bit_t var161 = var155 ^ var160;
//   bit_t var162 = var161 | in.b7;
//   bit_t var163 = var151 ^ var162;
//   bit_t var164 = var163 & in.b1;
//   *out7 = var143 ^ var164;
//   bit_t var166 = var115 & var82;
//   bit_t var167 = var166 ^ var77;
//   bit_t var168 = var156 ^ var60;
//   bit_t var169 = var168 | in.b2;
//   bit_t var170 = var167 ^ var169;
//   bit_t var171 = var152 & var115;
//   bit_t var172 = var171 ^ var65;
//   bit_t var173 = var172 & in.b1;
//   bit_t var174 = var170 ^ var173;
//   bit_t var175 = var87 ^ in.b4;
//   bit_t var176 = var175 ^ var139;
//   bit_t var177 = var61 & in.b1;
//   bit_t var178 = var176 ^ var177;
//   bit_t var179 = var49 & var37;
//   bit_t var180 = var179 ^ *out7;
//   bit_t var181 = var180 | in.b7;
//   bit_t var182 = var178 ^ var181;
//   bit_t var183 = var182 | in.b3;
//   bit_t var184 = var174 ^ var183;
//   bit_t var185 = var137 & var83;
//   bit_t var186 = ~var162;
//   bit_t var187 = var186 | var95;
//   bit_t var188 = var187 | in.b1;
//   bit_t var189 = var185 ^ var188;
//   bit_t var190 = var166 | var10;
//   bit_t var191 = var160 | var100;
//   bit_t var192 = var191 | in.b1;
//   bit_t var193 = var190 ^ var192;
//   bit_t var194 = var193 & in.b3;
//   bit_t var195 = var189 ^ var194;
//   bit_t var196 = var192 ^ var149;
//   bit_t var197 = var196 & var138;
//   bit_t var198 = ~var43;
//   bit_t var199 = var128 ^ var140;
//   bit_t var200 = var199 & in.b2;
//   bit_t var201 = var198 ^ var200;
//   bit_t var202 = var201 & in.b1;
//   bit_t var203 = var197 ^ var202;
//   bit_t var204 = var203 | in.b4;
//   bit_t var205 = var195 ^ var204;
//   bit_t var206 = var205 & in.b0;
//   *out2 = var184 ^ var206;
//   bit_t var208 = var200 ^ var29;
//   bit_t var209 = var61 | var30;
//   bit_t var210 = var209 | in.b5;
//   bit_t var211 = var208 ^ var210;
//   bit_t var212 = var168 & var132;
//   bit_t var213 = var212 ^ var205;
//   bit_t var214 = var213 | in.b1;
//   bit_t var215 = var211 ^ var214;
//   bit_t var216 = var136 ^ var32;
//   bit_t var217 = var164 | var196;
//   bit_t var218 = var217 & in.b0;
//   bit_t var219 = var216 ^ var218;
//   bit_t var220 = var96 & var214;
//   bit_t var221 = var220 | var54;
//   bit_t var222 = var158 ^ var176;
//   bit_t var223 = var222 & in.b0;
//   bit_t var224 = var221 ^ var223;
//   bit_t var225 = var224 & in.b4;
//   bit_t var226 = var219 ^ var225;
//   bit_t var227 = var226 | in.b6;
//   bit_t var228 = var215 ^ var227;
//   bit_t var229 = var146 | var172;
//   bit_t var230 = var229 & var215;
//   bit_t var231 = var164 | var80;
//   bit_t var232 = var231 ^ var228;
//   bit_t var233 = var93 & in.b4;
//   bit_t var234 = var232 ^ var233;
//   bit_t var235 = var234 & in.b5;
//   bit_t var236 = var230 ^ var235;
//   bit_t var237 = var234 | var124;
//   bit_t var238 = var237 ^ var131;
//   bit_t var239 = var223 ^ var127;
//   bit_t var240 = var191 ^ var197;
//   bit_t var241 = var240 & in.b1;
//   bit_t var242 = var239 ^ var241;
//   bit_t var243 = var242 & in.b4;
//   bit_t var244 = var238 ^ var243;
//   bit_t var245 = var244 & in.b6;
//   bit_t var246 = var236 ^ var245;
//   bit_t var247 = var246 & in.b3;
//   *out1 = var228 ^ var247;
//   bit_t var249 = var200 ^ var47;
//   bit_t var250 = var184 ^ var75;
//   bit_t var251 = var250 | in.b5;
//   bit_t var252 = var249 ^ var251;
//   bit_t var253 = var92 | var53;
//   bit_t var254 = var56 ^ var153;
//   bit_t var255 = var254 | in.b3;
//   bit_t var256 = var253 ^ var255;
//   bit_t var257 = var256 | in.b0;
//   bit_t var258 = var252 ^ var257;
//   bit_t var259 = var142 ^ var158;
//   bit_t var260 = var259 | var97;
//   bit_t var261 = var73 ^ var253;
//   bit_t var262 = var169 ^ var21;
//   bit_t var263 = var262 & in.b3;
//   bit_t var264 = var261 ^ var263;
//   bit_t var265 = var264 | in.b0;
//   bit_t var266 = var260 ^ var265;
//   bit_t var267 = var266 | in.b7;
//   bit_t var268 = var258 ^ var267;
//   bit_t var269 = var245 ^ var167;
//   bit_t var270 = var269 & in.b4;
//   bit_t var271 = in.b2 ^ var270;
//   bit_t var272 = var249 & var145;
//   bit_t var273 = var84 ^ var235;
//   bit_t var274 = var273 | in.b2;
//   bit_t var275 = var272 ^ var274;
//   bit_t var276 = var275 | in.b6;
//   bit_t var277 = var271 ^ var276;
//   bit_t var278 = var193 | var67;
//   bit_t var279 = var278 & var94;
//   bit_t var280 = *out4 | var278;
//   bit_t var281 = var280 ^ var237;
//   bit_t var282 = var70 | var48;
//   bit_t var283 = var282 | in.b2;
//   bit_t var284 = var281 ^ var283;
//   bit_t var285 = var284 & in.b6;
//   bit_t var286 = var279 ^ var285;
//   bit_t var287 = var286 & in.b0;
//   bit_t var288 = var277 ^ var287;
//   bit_t var289 = var288 | in.b1;
//   *out3 = var268 ^ var289;
//   bit_t var291 = in.b7 & var216;
//   bit_t var292 = var80 ^ var259;
//   bit_t var293 = var292 | in.b4;
//   bit_t var294 = var291 ^ var293;
//   bit_t var295 = var231 ^ var140;
//   bit_t var296 = var295 | var113;
//   bit_t var297 = var296 & in.b5;
//   bit_t var298 = var294 ^ var297;
//   bit_t var299 = var31 ^ var79;
//   bit_t var300 = var78 ^ var126;
//   bit_t var301 = var300 | in.b2;
//   bit_t var302 = var299 ^ var301;
//   bit_t var303 = var50 ^ var271;
//   bit_t var304 = var303 | in.b7;
//   bit_t var305 = var216 ^ var304;
//   bit_t var306 = var305 & in.b0;
//   bit_t var307 = var302 ^ var306;
//   bit_t var308 = var307 & in.b1;
//   bit_t var309 = var298 ^ var308;
//   bit_t var310 = var168 ^ var102;
//   bit_t var311 = var181 ^ var276;
//   bit_t var312 = var311 | in.b7;
//   bit_t var313 = var310 ^ var312;
//   bit_t var314 = var305 | var243;
//   bit_t var315 = var314 ^ var134;
//   bit_t var316 = var315 & in.b5;
//   bit_t var317 = var313 ^ var316;
//   bit_t var318 = var32 ^ var154;
//   bit_t var319 = var318 ^ var186;
//   bit_t var320 = var289 ^ var194;
//   bit_t var321 = var125 & in.b0;
//   bit_t var322 = var320 ^ var321;
//   bit_t var323 = var322 | in.b2;
//   bit_t var324 = var319 ^ var323;
//   bit_t var325 = var324 | in.b1;
//   bit_t var326 = var317 ^ var325;
//   bit_t var327 = var326 & in.b3;
//   *out5 = var309 ^ var327;
//   bit_t var329 = var122 ^ var115;
//   bit_t var330 = var140 | var299;
//   bit_t var331 = var330 & in.b7;
//   bit_t var332 = var329 ^ var331;
//   bit_t var333 = var115 ^ var250;
//   bit_t var334 = var236 & in.b0;
//   bit_t var335 = var333 ^ var334;
//   bit_t var336 = var335 & in.b3;
//   bit_t var337 = var332 ^ var336;
//   bit_t var338 = var266 ^ var164;
//   bit_t var339 = var70 ^ var239;
//   bit_t var340 = var339 | in.b5;
//   bit_t var341 = var338 ^ var340;
//   bit_t var342 = var88 ^ var242;
//   bit_t var343 = var342 | var98;
//   bit_t var344 = var343 | in.b7;
//   bit_t var345 = var341 ^ var344;
//   bit_t var346 = var345 & in.b1;
//   bit_t var347 = var337 ^ var346;
//   bit_t var348 = var295 ^ var74;
//   bit_t var349 = var348 ^ var268;
//   bit_t var350 = var59 ^ var157;
//   bit_t var351 = var347 ^ var262;
//   bit_t var352 = var351 | in.b7;
//   bit_t var353 = var350 ^ var352;
//   bit_t var354 = var353 & in.b1;
//   bit_t var355 = var349 ^ var354;
//   bit_t var356 = var115 ^ var15;
//   bit_t var357 = var356 | var126;
//   bit_t var358 = var193 ^ var286;
//   bit_t var359 = var25 ^ var260;
//   bit_t var360 = var359 & in.b0;
//   bit_t var361 = var358 ^ var360;
//   bit_t var362 = var361 | in.b1;
//   bit_t var363 = var357 ^ var362;
//   bit_t var364 = var363 & in.b3;
//   bit_t var365 = var355 ^ var364;
//   bit_t var366 = var365 & in.b2;
//   *out0 = var347 ^ var366;
// }