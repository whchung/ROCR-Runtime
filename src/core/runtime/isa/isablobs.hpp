/*
 * Copyright (C) 2020 Advanced Micro Devices, Inc. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef _ISAGENERATOR_ALDEBARAN_H_
#define _ISAGENERATOR_ALDEBARAN_H_

#include <string>

const uint32_t NOOP_ISA[] = {
    0xbf810000
};

const uint32_t CUSTOM_SGPR_ISA[] = {
    0x7e040202,               // v_mov_b32 v2, s2
    0x7e060203,               // v_mov_b32 v3, s3
    0x7e08020f,               // v_mov_b32 v4, s15. s15 would be 0xCAFEBABE.
    0xdc730000, 0x007f0402,   // flat_store_dword v[2:3], v4 slc glc
    0xbf810000                // s_endpgm
};

const uint32_t SCALAR_SET_ISA[] = {
    0x7e000200,               // v_mov_b32 v0, s0
    0x7e020201,               // v_mov_b32 v1, s1
    0x7e040202,               // v_mov_b32 v2, s2
    0x7e060203,               // v_mov_b32 v3, s3
    0xdc530000, 0x067f0002,   // flat_load_dword v6, v[2:3]
    0xbf8c0000,               // s_waitcnt 0
    0x680c0d06,               // v_add_u32 v6, v6, v6
    0xdc730000, 0x007f0600,   // flat_store_dword v[0:1], v6 slc glc
    0xbf810000                // s_endpgm
};

const uint32_t SCALAR_ADD_ISA[] = {
    0x7e000200,               // v_mov_b32 v0, s0
    0x7e020201,               // v_mov_b32 v1, s1
    0x7e040202,               // v_mov_b32 v2, s2
    0x7e060203,               // v_mov_b32 v3, s3
    0x7e080204,               // v_mov_b32 v4, s4
    0x7e0A0205,               // v_mov_b32 v5, s5
    0xdc530000, 0x067f0002,   // flat_load_dword v6, v[2:3]
    0xdc530000, 0x077f0004,   // flat_load_dword v7, v[4:5]
    0xbf8c0000,               // s_waitcnt 0
    0x680c0d07,               // v_add_u32 v6, v6, v7
    0xdc730000, 0x007f0600,   // flat_store_dword v[0:1], v6 slc glc
    0xbf810000                // s_endpgm
};

const uint32_t VECTOR_SET_ISA[] = {
    0x7e080200,               // v_mov_b32 v4, s0
    0x7e0a0201,               // v_mov_b32 v5, s1

    0x68020100,               // v_add_u32 v1, v0, v0
    0x68020301,               // v_add_u32 v1, v1, v1

    0x68080901,               // v_add_u32 v4, v4, v1

    0xdc730000, 0x007f0004,   // flat_store_dword v[4:5], v0 slc glc

    0xbf810000                // s_endpgm
};

const uint32_t VECTOR_ADD_ISA[] = {
    0x7e040202,               // v_mov_b32 v2, s2
    0x7e060203,               // v_mov_b32 v3, s3
    0x7e080204,               // v_mov_b32 v4, s4
    0x7e0a0205,               // v_mov_b32 v5, s5
    0x7e0c0200,               // v_mov_b32 v6, s0
    0x7e0e0201,               // v_mov_b32 v7, s1

    0x68020100,               // v_add_u32 v1, v0, v0
    0x68020301,               // v_add_u32 v1, v1, v1

    0x68040501,               // v_add_u32 v2, v2, v1
    0x68080901,               // v_add_u32 v4, v4, v1
    0x680c0d01,               // v_add_u32 v6, v6, v1

    0xdc530000, 0x087f0002,   // flat_load_dword v8, v[2:3]
    0xdc530000, 0x097f0004,   // flat_load_dword v9, v[4:5]
    0xbf8c0000,               // s_waitcnt 0

    0x68101109,               // v_add_u32 v8, v8, v9

    0xdc730000, 0x007f0806,   // flat_store_dword v[6:7], v8 slc glc

    0xbf810000                // s_endpgm
};

const uint32_t VECTOR_GROUP_SET_ISA[] = {
    0x7e080200,               // v_mov_b32 v4, s0
    0x7e0a0201,               // v_mov_b32 v5, s1

                              // v1 = bid
    0x7e020206,               // v_mov_b32 v1, s6 TGID_X

                              // v1 <<= 2
    0x68020301,               // v_add_u32 v1, v1, v1
    0x68020301,               // v_add_u32 v1, v1, v1

                              // v1 <<= 3
    0x68020301,               // v_add_u32 v1, v1, v1
    0x68020301,               // v_add_u32 v1, v1, v1
    0x68020301,               // v_add_u32 v1, v1, v1

                              // v2 = tid << 2
    0x68040100,               // v_add_u32 v2, v0, v0
    0x68040502,               // v_add_u32 v2, v2, v2

                              // v1 = (tid << 2) + (bid << 5)
    0x68020302,               // v_add_u32 v1, v1, v2

                              // v4 = v4 + v1
    0x68080901,               // v_add_u32 v4, v4, v1

    0x7e040206,               // v_mov_b32 v2, s6 TGID_X

    0xdc730000, 0x007f0204,   // flat_store_dword v[4:5], v2 slc glc

    0xbf810000                // s_endpgm
};

const uint32_t VECTOR_GROUP_ADD_ISA[] = {
                              // v[4:5] = &C
    0x7e080200,               // v_mov_b32 v4, s0
    0x7e0a0201,               // v_mov_b32 v5, s1

                              // v[6:7] = &A
    0x7e0c0202,               // v_mov_b32 v6, s2
    0x7e0e0203,               // v_mov_b32 v7, s3

                              // v[8:9] = &B
    0x7e100204,               // v_mov_b32 v8, s4
    0x7e120205,               // v_mov_b32 v9, s5

                              // v1 = bid
    0x7e020206,               // v_mov_b32 v1, s6 TGID_X

                              // v1 <<= 2
    0x68020301,               // v_add_u32 v1, v1, v1
    0x68020301,               // v_add_u32 v1, v1, v1

                              // v1 <<= 3
    0x68020301,               // v_add_u32 v1, v1, v1
    0x68020301,               // v_add_u32 v1, v1, v1
    0x68020301,               // v_add_u32 v1, v1, v1

                              // v2 = tid << 2
    0x68040100,               // v_add_u32 v2, v0, v0
    0x68040502,               // v_add_u32 v2, v2, v2

                              // v1 = (tid << 2) + (bid << 5)
    0x68020302,               // v_add_u32 v1, v1, v2

                              // v4 = v4 + v1
    0x68080901,               // v_add_u32 v4, v4, v1

                              // v6 = v6 + v1
    0x680c0d01,               // v_add_u32 v6, v6, v1

                              // v8 = v8 + v1
    0x68101101,               // v_add_u32 v8, v8, v1

    0xdc530000, 0x0a7f0006,   // flat_load_dword v10, v[6:7]
    0xdc530000, 0x0b7f0008,   // flat_load_dword v11, v[8:9]
    0xbf8c0000,               // s_waitcnt 0

    0x6814150b,               // v_add_u32 v10, v10, v11

    0xdc730000, 0x007f0a04,   // flat_store_dword v[4:5], v10 slc glc

    0xbf810000                // s_endpgm
};

const uint32_t GEMM_ISA_16_1152_5120_TIMING[] = {
#if USE_EMPTY_KERNEL
0xBF810000,              // s_endpgm                                                         // 000000000660:
#else
// Original ISA:
// s4-s5 : kernarg pointer
// s6 : workgroup_id_x
// s7 : workgroup_id_y
// s8 : workgroup_id_z
//0xC00A0302, 0x00000000, // s_load_dwordx4 s[12:15], s[4:5], 0x0                           // 000000000000:
//0xC00A0002, 0x00000010, // s_load_dwordx4 s[0:3], s[4:5], 0x10                            // 000000000008:
0x20060085,             // v_lshrrev_b32_e32 v3, 5, v0                                    // 000000000010:
0x24080082,             // v_lshlrev_b32_e32 v4, 2, v0                                    // 000000000014:
//0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000018:

// Hacked ISA
// s0-s1: matrix C pointer
// s2-s3: matrix A pointer
// s4-s5: matrix B pointer
// s6 : workgroup_id_x
// s7 : workgroup_id_y
// s8 : workgroup_id_z
0xBE8C0002,             //     s_mov_b32 s12, s2
0xBE8D0003,             //     s_mov_b32 s13, s3
//0xBE8E0004,             //     s_mov_b32 s14, s4
//0xBE8F0005,             //     s_mov_b32 s15, s5
0xC0940780, 0x00000000,   // s_memrealtime s[30:31]
//0xC0900780, 0x00000000,   // s_memtime s[30:31]
0xBF800000,             // s_nop 0

0x8E038407,             // s_lshl_b32 s3, s7, 4                                           // 00000000001C:
0x20160084,             // v_lshrrev_b32_e32 v11, 4, v0                                   // 000000000020:
0x261808FF, 0x0000007C, // v_and_b32_e32 v12, 0x7c, v4                                    // 000000000024:
0x100A06FF, 0x00000900, // v_mul_u32_u24_e32 v5, 0x900, v3                                // 00000000002C:
0x8E028706,             // s_lshl_b32 s2, s6, 7                                           // 000000000034:
0x28261905,             // v_or_b32_e32 v19, v5, v12                                      // 000000000038:
0x2602008F,             // v_and_b32_e32 v1, 15, v0                                       // 00000000003C:
0x68141603,             // v_add_u32_e32 v10, s3, v11                                     // 000000000040:
0xB0031400,             // s_movk_i32 s3, 0x1400                                          // 000000000044:
0xD1FE0012, 0x02040513, // v_add_lshl_u32 v18, v19, s2, 1                                 // 000000000048:
0xD2850004, 0x0000070A, // v_mul_lo_u32 v4, v10, s3                                       // 000000000050:
0x24040281,             // v_lshlrev_b32_e32 v2, 1, v1                                    // 000000000058:
0xBE8700FF, 0x00020000, // s_mov_b32 s7, 0x20000                                          // 00000000005C:
0x28080504,             // v_or_b32_e32 v4, v4, v2                                        // 000000000064:
0xBE880004,             // s_mov_b32 s8, s4                                              // 000000000068:
0xBE8A00FF, 0x01680000, // s_mov_b32 s10, 0x1680000                                       // 00000000006C:
0xBE890005,             // s_mov_b32 s9, s5                                              // 000000000074:
0xBE8B0007,             // s_mov_b32 s11, s7                                              // 000000000078:
0x681A24FF, 0x00009000, // v_add_u32_e32 v13, 0x9000, v18                                 // 00000000007C:
0x24280881,             // v_lshlrev_b32_e32 v20, 1, v4                                   // 000000000084:
0xE0541000, 0x80020412, // buffer_load_dwordx2 v[4:5], v18, s[8:11], 0 offen              // 000000000088:
0xE0541900, 0x80020612, // buffer_load_dwordx2 v[6:7], v18, s[8:11], 0 offen offset:2304  // 000000000090:
0xE0541000, 0x8002080D, // buffer_load_dwordx2 v[8:9], v13, s[8:11], 0 offen              // 000000000098:
0xE0541900, 0x8002100D, // buffer_load_dwordx2 v[16:17], v13, s[8:11], 0 offen offset:2304// 0000000000A0:
0x241A0081,             // v_lshlrev_b32_e32 v13, 1, v0                                   // 0000000000A8:
0xD2010002, 0x0409410D, // v_and_or_b32 v2, v13, 32, v2                                   // 0000000000AC:
0xB00E0050,             // s_movk_i32 s14, 0x50                                           // 0000000000B4:
0x101A060E,             // v_mul_u32_u24_e32 v13, s14, v3                                 // 0000000000B8:
0x24040481,             // v_lshlrev_b32_e32 v2, 1, v2                                    // 0000000000BC:
0xB0030108,             // s_movk_i32 s3, 0x108                                           // 0000000000C0:
0xD1FD000D, 0x0409030D, // v_lshl_add_u32 v13, v13, 1, v2                                 // 0000000000C4:
0x10040603,             // v_mul_u32_u24_e32 v2, s3, v3                                   // 0000000000CC:
0x24181881,             // v_lshlrev_b32_e32 v12, 1, v12                                  // 0000000000D0:
0xD1FE0015, 0x0206050C, // v_add_lshl_u32 v21, v12, v2, 1                                 // 0000000000D4:
0x28040688,             // v_or_b32_e32 v2, 8, v3                                         // 0000000000DC:
0x10040403,             // v_mul_u32_u24_e32 v2, s3, v2                                   // 0000000000E0:
0xBE84000C,             // s_mov_b32 s4, s12                                              // 0000000000E4:
0xBE85000D,             // s_mov_b32 s5, s13                                              // 0000000000E8:
0xBE8300FF, 0x00012000, // s_mov_b32 s3, 0x12000                                          // 0000000000EC:
0xBE8600FF, 0x00050000, // s_mov_b32 s6, 0x50000                                          // 0000000000F4:
0xD1FE000C, 0x02061902, // v_add_lshl_u32 v12, v2, v12, 1                                 // 0000000000FC:
0x682C2403,             // v_add_u32_e32 v22, s3, v18                                     // 000000000104:
0x682E24FF, 0x0001B000, // v_add_u32_e32 v23, 0x1b000, v18                                // 000000000108:
0xE0501000, 0x80010214, // buffer_load_dword v2, v20, s[4:7], 0 offen                     // 000000000110:
0xE0501040, 0x80011214, // buffer_load_dword v18, v20, s[4:7], 0 offen offset:64          // 000000000118:
0xB00D0500,             // s_movk_i32 s13, 0x500                                          // 000000000120:
0x681E180D,             // v_add_u32_e32 v15, s13, v12                                    // 000000000124:
0xD3D94004, 0x18000080, // v_accvgpr_write_b32 a4, 0                                      // 000000000128:
0x7E0AA504,             // v_accvgpr_mov_b32 a5, a4                                       // 000000000130:
0x7E0CA504,             // v_accvgpr_mov_b32 a6, a4                                       // 000000000134:
0x7E0EA504,             // v_accvgpr_mov_b32 a7, a4                                       // 000000000138:
0xD3D94000, 0x18000080, // v_accvgpr_write_b32 a0, 0                                      // 00000000013C:
0x24262681,             // v_lshlrev_b32_e32 v19, 1, v19                                  // 000000000144:
0xBE8C0080,             // s_mov_b32 s12, 0                                               // 000000000148:
0x681C2A0D,             // v_add_u32_e32 v14, s13, v21                                    // 00000000014C:
0xD1FD0013, 0x044D0202, // v_lshl_add_u32 v19, s2, 1, v19                                 // 000000000150:
0x282828FF, 0x00000080, // v_or_b32_e32 v20, 0x80, v20                                    // 000000000158:
0xBF8C0F71,             // s_waitcnt vmcnt(1)                                             // 000000000160:
0xD81A0000, 0x0000020D, // ds_write_b32 v13, v2                                           // 000000000164:
0xD2A00002, 0x00020D04, // v_pack_b32_f16 v2, v4, v6                                      // 00000000016C:
0xD2A01803, 0x00020D04, // v_pack_b32_f16 v3, v4, v6 op_sel:[1,1,0]                       // 000000000174:
0xD2A00004, 0x00020F05, // v_pack_b32_f16 v4, v5, v7                                      // 00000000017C:
0xD2A01805, 0x00020F05, // v_pack_b32_f16 v5, v5, v7 op_sel:[1,1,0]                       // 000000000184:
0xD2A00006, 0x00022108, // v_pack_b32_f16 v6, v8, v16                                     // 00000000018C:
0xD2A01807, 0x00022108, // v_pack_b32_f16 v7, v8, v16 op_sel:[1,1,0]                      // 000000000194:
0xD2A00008, 0x00022309, // v_pack_b32_f16 v8, v9, v17                                     // 00000000019C:
0xD2A01809, 0x00022309, // v_pack_b32_f16 v9, v9, v17 op_sel:[1,1,0]                      // 0000000001A4:
0xD9BE0500, 0x00000215, // ds_write_b128 v21, v[2:5] offset:1280                          // 0000000001AC:
0xD9BE0500, 0x0000060C, // ds_write_b128 v12, v[6:9] offset:1280                          // 0000000001B4:
0xE0541000, 0x80020616, // buffer_load_dwordx2 v[6:7], v22, s[8:11], 0 offen              // 0000000001BC:
0xE0541900, 0x80020816, // buffer_load_dwordx2 v[8:9], v22, s[8:11], 0 offen offset:2304  // 0000000001C4:
0xE0541000, 0x80020217, // buffer_load_dwordx2 v[2:3], v23, s[8:11], 0 offen              // 0000000001CC:
0xE0541900, 0x80020417, // buffer_load_dwordx2 v[4:5], v23, s[8:11], 0 offen offset:2304  // 0000000001D4:
0x24220085,             // v_lshlrev_b32_e32 v17, 5, v0                                   // 0000000001DC:
0xD1C8000C, 0x020D0300, // v_bfe_u32 v12, v0, 1, 3                                        // 0000000001E0:
0xD1C80010, 0x02090900, // v_bfe_u32 v16, v0, 4, 2                                        // 0000000001E8:
0x262222A0,             // v_and_b32_e32 v17, 32, v17                                     // 0000000001F0:
0xD2000011, 0x04450710, // v_lshl_or_b32 v17, v16, 3, v17                                 // 0000000001F4:
0x1018180E,             // v_mul_u32_u24_e32 v12, s14, v12                                // 0000000001FC:
0xD1FE0011, 0x02061911, // v_add_lshl_u32 v17, v17, v12, 1                                // 000000000200:
0x20180082,             // v_lshrrev_b32_e32 v12, 2, v0                                   // 000000000208:
0xD3D84017, 0x18000105, // v_accvgpr_read_b32 v23, a5                                     // 00000000020C:
0xD2010000, 0x0405610C, // v_and_or_b32 v0, v12, 48, v1                                   // 000000000214:
0x102020FF, 0x00000420, // v_mul_u32_u24_e32 v16, 0x420, v16                              // 00000000021C:
0xD1FD0010, 0x04410300, // v_lshl_add_u32 v16, v0, 1, v16                                 // 000000000224:
0xD3D94001, 0x18000117, // v_accvgpr_write_b32 a1, v23                                    // 00000000022C:
0xD3D84017, 0x18000106, // v_accvgpr_read_b32 v23, a6                                     // 000000000234:
0xD1FD0010, 0x00350310, // v_lshl_add_u32 v16, v16, 1, s13                                // 00000000023C:
0xD3D94002, 0x18000117, // v_accvgpr_write_b32 a2, v23                                    // 000000000244:
0xD3D84017, 0x18000107, // v_accvgpr_read_b32 v23, a7                                     // 00000000024C:
0x682A20A0,             // v_add_u32_e32 v21, 32, v16                                     // 000000000254:
0x682C20B0,             // v_add_u32_e32 v22, 48, v16                                     // 000000000258:
0xD3D94003, 0x18000117, // v_accvgpr_write_b32 a3, v23                                    // 00000000025C:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0) 264 <BB0_1>                               // 000000000264:
0xBF8A0000,             // s_barrier                                                      // 000000000268:
0xD9FE0000, 0x18000011, // ds_read_b128 v[24:27], v17                                     // 00000000026C:
0xD8700100, 0x24000010, // ds_read2st64_b32 v[36:37], v16 offset1:1                       // 000000000274:
0xD86EC484, 0x26000010, // ds_read2_b32 v[38:39], v16 offset0:132 offset1:196             // 00000000027C:
0xD8700504, 0x28000015, // ds_read2st64_b32 v[40:41], v21 offset0:4 offset1:5             // 000000000284:
0xD8700706, 0x2A000016, // ds_read2st64_b32 v[42:43], v22 offset0:6 offset1:7             // 00000000028C:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000294:
0xBF8A0000,             // s_barrier                                                      // 000000000298:
0xBF8CC37F,             // s_waitcnt lgkmcnt(3)                                           // 00000000029C:
0x7E400324,             // v_mov_b32_e32 v32, v36                                         // 0000000002A0:
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                           // 0000000002A4:
0x7E420326,             // v_mov_b32_e32 v33, v38                                         // 0000000002A8:
0xBF8C0F74,             // s_waitcnt vmcnt(4)                                             // 0000000002AC:
0xD81A0000, 0x0000120D, // ds_write_b32 v13, v18                                          // 0000000002B0:
0xE0501000, 0x80011214, // buffer_load_dword v18, v20, s[4:7], 0 offen                    // 0000000002B8:
0xD3CD8004, 0x04124118, // v_mfma_f32_16x16x16f16 a[4:7], v[24:25], v[32:33], a[4:7]      // 0000000002C0:
0xBF8C0F73,             // s_waitcnt vmcnt(3)                                             // 0000000002C8:
0xD2A0001C, 0x00021106, // v_pack_b32_f16 v28, v6, v8                                     // 0000000002CC:
0xD2A0181D, 0x00021106, // v_pack_b32_f16 v29, v6, v8 op_sel:[1,1,0]                      // 0000000002D4:
0xD2A0001E, 0x00021307, // v_pack_b32_f16 v30, v7, v9                                     // 0000000002DC:
0xD2A0181F, 0x00021307, // v_pack_b32_f16 v31, v7, v9 op_sel:[1,1,0]                      // 0000000002E4:
0xBF8C0F71,             // s_waitcnt vmcnt(1)                                             // 0000000002EC:
0xD2A00020, 0x00020902, // v_pack_b32_f16 v32, v2, v4                                     // 0000000002F0:
0xD2A01821, 0x00020902, // v_pack_b32_f16 v33, v2, v4 op_sel:[1,1,0]                      // 0000000002F8:
0x6804260C,             // v_add_u32_e32 v2, s12, v19                                     // 000000000300:
0x680804FF, 0x00024000, // v_add_u32_e32 v4, 0x24000, v2                                  // 000000000304:
0x810C030C,             // s_add_i32 s12, s12, s3                                         // 00000000030C:
0xD2A00022, 0x00020B03, // v_pack_b32_f16 v34, v3, v5                                     // 000000000310:
0xD2A01823, 0x00020B03, // v_pack_b32_f16 v35, v3, v5 op_sel:[1,1,0]                      // 000000000318:
0x682E04FF, 0x0002D000, // v_add_u32_e32 v23, 0x2d000, v2                                 // 000000000320:
0xE0541000, 0x80020604, // buffer_load_dwordx2 v[6:7], v4, s[8:11], 0 offen               // 000000000328:
0xE0541900, 0x80020804, // buffer_load_dwordx2 v[8:9], v4, s[8:11], 0 offen offset:2304   // 000000000330:
0xE0541000, 0x80020217, // buffer_load_dwordx2 v[2:3], v23, s[8:11], 0 offen              // 000000000338:
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                           // 000000000340:
0x7E0A032A,             // v_mov_b32_e32 v5, v42                                          // 000000000344:
0x7E080328,             // v_mov_b32_e32 v4, v40                                          // 000000000348:
0xBF07FF0C, 0x00B1C000, // s_cmp_lg_u32 s12, 0xb1c000                                     // 00000000034C:
0x7E4C0325,             // v_mov_b32_e32 v38, v37                                         // 000000000354:
0xD3CD8004, 0x0412091A, // v_mfma_f32_16x16x16f16 a[4:7], v[26:27], v[4:5], a[4:7]        // 000000000358:
0xE0541900, 0x80020417, // buffer_load_dwordx2 v[4:5], v23, s[8:11], 0 offen offset:2304  // 000000000360:
0x7E540329,             // v_mov_b32_e32 v42, v41                                         // 000000000368:
0x682828C0,             // v_add_u32_e32 v20, 64, v20                                     // 00000000036C:
0xD9BE0000, 0x00001C0E, // ds_write_b128 v14, v[28:31]                                    // 000000000370:
0xD9BE0000, 0x0000200F, // ds_write_b128 v15, v[32:35]                                    // 000000000378:
0xD3CD8000, 0x04024D18, // v_mfma_f32_16x16x16f16 a[0:3], v[24:25], v[38:39], a[0:3]      // 000000000380:
0xD3CD8000, 0x0402551A, // v_mfma_f32_16x16x16f16 a[0:3], v[26:27], v[42:43], a[0:3]      // 000000000388:
0xBF85FFB4,             // s_cbranch_scc1 BB0_1                                           // 000000000390:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000394:
0xBF8A0000,             // s_barrier                                                      // 000000000398:
0xD9FE0000, 0x14000011, // ds_read_b128 v[20:23], v17                                     // 00000000039C:
0xD8700100, 0x18000010, // ds_read2st64_b32 v[24:25], v16 offset1:1                       // 0000000003A4:
0xD86EC484, 0x1C000010, // ds_read2_b32 v[28:29], v16 offset0:132 offset1:196             // 0000000003AC:
0x682620A0,             // v_add_u32_e32 v19, 32, v16                                     // 0000000003B4:
0x684420B0,             // v_add_u32_e32 v34, 48, v16                                     // 0000000003B8:
0x24000082,             // v_lshlrev_b32_e32 v0, 2, v0                                    // 0000000003BC:
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                           // 0000000003C0:
0x7E340318,             // v_mov_b32_e32 v26, v24                                         // 0000000003C4:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000003C8:
0x7E36031C,             // v_mov_b32_e32 v27, v28                                         // 0000000003CC:
0x7E380319,             // v_mov_b32_e32 v28, v25                                         // 0000000003D0:
0xBF800000,             // s_nop 0                                                        // 0000000003D4:
0xD3CD8004, 0x04123514, // v_mfma_f32_16x16x16f16 a[4:7], v[20:21], v[26:27], a[4:7]      // 0000000003D8:
0xD8700504, 0x1A000013, // ds_read2st64_b32 v[26:27], v19 offset0:4 offset1:5             // 0000000003E0:
0xD8700706, 0x1E000022, // ds_read2st64_b32 v[30:31], v34 offset0:6 offset1:7             // 0000000003E8:
0xB0030480,             // s_movk_i32 s3, 0x480                                           // 0000000003F0:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000003F4:
0xBF8A0000,             // s_barrier                                                      // 0000000003F8:
0xBF8C0F74,             // s_waitcnt vmcnt(4)                                             // 0000000003FC:
0xD81A0000, 0x0000120D, // ds_write_b32 v13, v18                                          // 000000000400:
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                           // 000000000408:
0x7E40031A,             // v_mov_b32_e32 v32, v26                                         // 00000000040C:
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                           // 000000000410:
0x7E42031E,             // v_mov_b32_e32 v33, v30                                         // 000000000414:
0xBF8C0F72,             // s_waitcnt vmcnt(2)                                             // 000000000418:
0xD2A00018, 0x00021106, // v_pack_b32_f16 v24, v6, v8                                     // 00000000041C:
0xD2A01819, 0x00021106, // v_pack_b32_f16 v25, v6, v8 op_sel:[1,1,0]                      // 000000000424:
0x7E3C031B,             // v_mov_b32_e32 v30, v27                                         // 00000000042C:
0xD2A0001A, 0x00021307, // v_pack_b32_f16 v26, v7, v9                                     // 000000000430:
0xD2A0181B, 0x00021307, // v_pack_b32_f16 v27, v7, v9 op_sel:[1,1,0]                      // 000000000438:
0xBF8C0F70,             // s_waitcnt vmcnt(0)                                             // 000000000440:
0xD2A00006, 0x00020902, // v_pack_b32_f16 v6, v2, v4                                      // 000000000444:
0xD2A01807, 0x00020902, // v_pack_b32_f16 v7, v2, v4 op_sel:[1,1,0]                       // 00000000044C:
0xD2A00008, 0x00020B03, // v_pack_b32_f16 v8, v3, v5                                      // 000000000454:
0xD2A01809, 0x00020B03, // v_pack_b32_f16 v9, v3, v5 op_sel:[1,1,0]                       // 00000000045C:
0xD9BE0000, 0x0000180E, // ds_write_b128 v14, v[24:27]                                    // 000000000464:
0xD9BE0000, 0x0000060F, // ds_write_b128 v15, v[6:9]                                      // 00000000046C:
0xD3CD8004, 0x04124116, // v_mfma_f32_16x16x16f16 a[4:7], v[22:23], v[32:33], a[4:7]      // 000000000474:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 00000000047C:
0xBF8A0000,             // s_barrier                                                      // 000000000480:
0xD9FE0000, 0x02000011, // ds_read_b128 v[2:5], v17                                       // 000000000484:
0xD8700100, 0x06000010, // ds_read2st64_b32 v[6:7], v16 offset1:1                         // 00000000048C:
0xD86EC484, 0x08000010, // ds_read2_b32 v[8:9], v16 offset0:132 offset1:196               // 000000000494:
0xD8700504, 0x10000013, // ds_read2st64_b32 v[16:17], v19 offset0:4 offset1:5             // 00000000049C:
0xD8700706, 0x12000022, // ds_read2st64_b32 v[18:19], v34 offset0:6 offset1:7             // 0000000004A4:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000004AC:
0xBF8A0000,             // s_barrier                                                      // 0000000004B0:
0xD3CD8000, 0x04023914, // v_mfma_f32_16x16x16f16 a[0:3], v[20:21], v[28:29], a[0:3]      // 0000000004B4:
0xBF8CC37F,             // s_waitcnt lgkmcnt(3)                                           // 0000000004BC:
0x7E1C0306,             // v_mov_b32_e32 v14, v6                                          // 0000000004C0:
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                           // 0000000004C4:
0x7E1E0308,             // v_mov_b32_e32 v15, v8                                          // 0000000004C8:
0x7E100307,             // v_mov_b32_e32 v8, v7                                           // 0000000004CC:
0x260C188C,             // v_and_b32_e32 v6, 12, v12                                      // 0000000004D0:
0xD200000C, 0x04011106, // v_lshl_or_b32 v12, v6, 8, v0                                   // 0000000004D4:
0x24000284,             // v_lshlrev_b32_e32 v0, 4, v1                                    // 0000000004DC:
0xD3CD8004, 0x04121D02, // v_mfma_f32_16x16x16f16 a[4:7], v[2:3], v[14:15], a[4:7]        // 0000000004E0:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000004E8:
0x7E1E0312,             // v_mov_b32_e32 v15, v18                                         // 0000000004EC:
0x7E1C0310,             // v_mov_b32_e32 v14, v16                                         // 0000000004F0:
0x7E240311,             // v_mov_b32_e32 v18, v17                                         // 0000000004F4:
0xD3CD8000, 0x04023D16, // v_mfma_f32_16x16x16f16 a[0:3], v[22:23], v[30:31], a[0:3]      // 0000000004F8:
0xD3CD8004, 0x04121D04, // v_mfma_f32_16x16x16f16 a[4:7], v[4:5], v[14:15], a[4:7]        // 000000000500:
0xD3CD8000, 0x04021102, // v_mfma_f32_16x16x16f16 a[0:3], v[2:3], v[8:9], a[0:3]          // 000000000508:
0xD2000002, 0x0401110B, // v_lshl_or_b32 v2, v11, 8, v0                                   // 000000000510:
0xBF800007,             // s_nop 7                                                        // 000000000518:
0xBF800000,             // s_nop 0                                                        // 00000000051C:
0xDA1A0000, 0x0000040C, // ds_write_b32 v12, a4                                           // 000000000520:
0xDA1A0100, 0x0000050C, // ds_write_b32 v12, a5 offset:256                                // 000000000528:
0xDA1A0200, 0x0000060C, // ds_write_b32 v12, a6 offset:512                                // 000000000530:
0xDA1A0300, 0x0000070C, // ds_write_b32 v12, a7 offset:768                                // 000000000538:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000540:
0xBF8A0000,             // s_barrier                                                      // 000000000544:
0xD9FE0000, 0x06000002, // ds_read_b128 v[6:9], v2                                        // 000000000548:
0xD3CD8000, 0x04022504, // v_mfma_f32_16x16x16f16 a[0:3], v[4:5], v[18:19], a[0:3]        // 000000000550:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000558:
0x7E001506,             // v_cvt_f16_f32_e32 v0, v6                                       // 00000000055C:
0x7E061508,             // v_cvt_f16_f32_e32 v3, v8                                       // 000000000560:
0x7E0C1509,             // v_cvt_f16_f32_e32 v6, v9                                       // 000000000564:
0x7E0E1507,             // v_cvt_f16_f32_e32 v7, v7                                       // 000000000568:
0x24100282,             // v_lshlrev_b32_e32 v8, 2, v1                                    // 00000000056C:
0xD2A00001, 0x00020D03, // v_pack_b32_f16 v1, v3, v6                                      // 000000000570:
0xD2850003, 0x0000070A, // v_mul_lo_u32 v3, v10, s3                                       // 000000000578:
0x68060602,             // v_add_u32_e32 v3, s2, v3                                       // 000000000580:
0xD2A00000, 0x00020F00, // v_pack_b32_f16 v0, v0, v7                                      // 000000000584:
0xBE8300FF, 0x00020000, // s_mov_b32 s3, 0x20000                                          // 00000000058C:
0xBE8200FF, 0x00012000, // s_mov_b32 s2, 0x12000                                          // 000000000594:
0xD1FE0004, 0x02061103, // v_add_lshl_u32 v4, v3, v8, 1                                   // 00000000059C:
//0xE0741000, 0x80000004, // buffer_store_dwordx2 v[0:1], v4, s[0:3], 0 offen               // 0000000005A4:

0x7e00021E,             // v_mov_b32 v0, s30
0x7e02021F,             // v_mov_b32 v1, s31
0xE0741000, 0x80000004, // buffer_store_dwordx2 v[0:1], v4, s[0:3], 0 offen               // 0000000005A4:

0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000005AC:
0xBF8A0000,             // s_barrier                                                      // 0000000005B0:
0xDA1A0000, 0x0000000C, // ds_write_b32 v12, a0                                           // 0000000005B4:
0xDA1A0100, 0x0000010C, // ds_write_b32 v12, a1 offset:256                                // 0000000005BC:
0xDA1A0200, 0x0000020C, // ds_write_b32 v12, a2 offset:512                                // 0000000005C4:
0xDA1A0300, 0x0000030C, // ds_write_b32 v12, a3 offset:768                                // 0000000005CC:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000005D4:
0xBF8A0000,             // s_barrier                                                      // 0000000005D8:
0xD9FE0000, 0x00000002, // ds_read_b128 v[0:3], v2                                        // 0000000005DC:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000005E4:
0x7E001500,             // v_cvt_f16_f32_e32 v0, v0                                       // 0000000005E8:
0x7E0A1501,             // v_cvt_f16_f32_e32 v5, v1                                       // 0000000005EC:
0x7E041502,             // v_cvt_f16_f32_e32 v2, v2                                       // 0000000005F0:
0x7E061503,             // v_cvt_f16_f32_e32 v3, v3                                       // 0000000005F4:
0xD2A00000, 0x00020B00, // v_pack_b32_f16 v0, v0, v5                                      // 0000000005F8:
0xD2A00001, 0x00020702, // v_pack_b32_f16 v1, v2, v3                                      // 000000000600:
//0xE0741080, 0x80000004, // buffer_store_dwordx2 v[0:1], v4, s[0:3], 0 offen offset:128    // 000000000608:

0xC0940700, 0x00000000, // s_memrealtime s[28:29]
0x7e00021C,             // v_mov_b32 v0, s28
0x7e02021D,             // v_mov_b32 v1, s29

//0x809F1E1C,             // s_sub_u32 s31, s28, s30
//0x7e00021F,             // v_mov_b32 v0, s31
//0x7e02021F,             // v_mov_b32 v1, s31

0xE0741080, 0x80000004, // buffer_store_dwordx2 v[0:1], v4, s[0:3], 0 offen offset:128    // 000000000608:
0xBF810000              // s_endpgm                                                       // 000000000610:
#endif
};

const uint32_t GEMM_ISA_16_1152_5120[] = {
#if USE_EMPTY_KERNEL
0xBF810000,              // s_endpgm                                                         // 000000000660:
#else
// Original ISA:
// s4-s5 : kernarg pointer
// s6 : workgroup_id_x
// s7 : workgroup_id_y
// s8 : workgroup_id_z
//0xC00A0302, 0x00000000, // s_load_dwordx4 s[12:15], s[4:5], 0x0                           // 000000000000:
//0xC00A0002, 0x00000010, // s_load_dwordx4 s[0:3], s[4:5], 0x10                            // 000000000008:
0x20060085,             // v_lshrrev_b32_e32 v3, 5, v0                                    // 000000000010:
0x24080082,             // v_lshlrev_b32_e32 v4, 2, v0                                    // 000000000014:
//0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000018:

// Hacked ISA
// s0-s1: matrix C pointer
// s2-s3: matrix A pointer
// s4-s5: matrix B pointer
// s6 : workgroup_id_x
// s7 : workgroup_id_y
// s8 : workgroup_id_z
0xBE8C0002,             //     s_mov_b32 s12, s2
0xBE8D0003,             //     s_mov_b32 s13, s3
0xBE8E0004,             //     s_mov_b32 s14, s4
0xBE8F0005,             //     s_mov_b32 s15, s5
0xBF800000,             // s_nop 0

0x8E038407,             // s_lshl_b32 s3, s7, 4                                           // 00000000001C:
0x20160084,             // v_lshrrev_b32_e32 v11, 4, v0                                   // 000000000020:
0x261808FF, 0x0000007C, // v_and_b32_e32 v12, 0x7c, v4                                    // 000000000024:
0x100A06FF, 0x00000900, // v_mul_u32_u24_e32 v5, 0x900, v3                                // 00000000002C:
0x8E028706,             // s_lshl_b32 s2, s6, 7                                           // 000000000034:
0x28261905,             // v_or_b32_e32 v19, v5, v12                                      // 000000000038:
0x2602008F,             // v_and_b32_e32 v1, 15, v0                                       // 00000000003C:
0x68141603,             // v_add_u32_e32 v10, s3, v11                                     // 000000000040:
0xB0031400,             // s_movk_i32 s3, 0x1400                                          // 000000000044:
0xD1FE0012, 0x02040513, // v_add_lshl_u32 v18, v19, s2, 1                                 // 000000000048:
0xD2850004, 0x0000070A, // v_mul_lo_u32 v4, v10, s3                                       // 000000000050:
0x24040281,             // v_lshlrev_b32_e32 v2, 1, v1                                    // 000000000058:
0xBE8700FF, 0x00020000, // s_mov_b32 s7, 0x20000                                          // 00000000005C:
0x28080504,             // v_or_b32_e32 v4, v4, v2                                        // 000000000064:
0xBE88000E,             // s_mov_b32 s8, s14                                              // 000000000068:
0xBE8A00FF, 0x01680000, // s_mov_b32 s10, 0x1680000                                       // 00000000006C:
0xBE89000F,             // s_mov_b32 s9, s15                                              // 000000000074:
0xBE8B0007,             // s_mov_b32 s11, s7                                              // 000000000078:
0x681A24FF, 0x00009000, // v_add_u32_e32 v13, 0x9000, v18                                 // 00000000007C:
0x24280881,             // v_lshlrev_b32_e32 v20, 1, v4                                   // 000000000084:
0xE0541000, 0x80020412, // buffer_load_dwordx2 v[4:5], v18, s[8:11], 0 offen              // 000000000088:
0xE0541900, 0x80020612, // buffer_load_dwordx2 v[6:7], v18, s[8:11], 0 offen offset:2304  // 000000000090:
0xE0541000, 0x8002080D, // buffer_load_dwordx2 v[8:9], v13, s[8:11], 0 offen              // 000000000098:
0xE0541900, 0x8002100D, // buffer_load_dwordx2 v[16:17], v13, s[8:11], 0 offen offset:2304// 0000000000A0:
0x241A0081,             // v_lshlrev_b32_e32 v13, 1, v0                                   // 0000000000A8:
0xD2010002, 0x0409410D, // v_and_or_b32 v2, v13, 32, v2                                   // 0000000000AC:
0xB00E0050,             // s_movk_i32 s14, 0x50                                           // 0000000000B4:
0x101A060E,             // v_mul_u32_u24_e32 v13, s14, v3                                 // 0000000000B8:
0x24040481,             // v_lshlrev_b32_e32 v2, 1, v2                                    // 0000000000BC:
0xB0030108,             // s_movk_i32 s3, 0x108                                           // 0000000000C0:
0xD1FD000D, 0x0409030D, // v_lshl_add_u32 v13, v13, 1, v2                                 // 0000000000C4:
0x10040603,             // v_mul_u32_u24_e32 v2, s3, v3                                   // 0000000000CC:
0x24181881,             // v_lshlrev_b32_e32 v12, 1, v12                                  // 0000000000D0:
0xD1FE0015, 0x0206050C, // v_add_lshl_u32 v21, v12, v2, 1                                 // 0000000000D4:
0x28040688,             // v_or_b32_e32 v2, 8, v3                                         // 0000000000DC:
0x10040403,             // v_mul_u32_u24_e32 v2, s3, v2                                   // 0000000000E0:
0xBE84000C,             // s_mov_b32 s4, s12                                              // 0000000000E4:
0xBE85000D,             // s_mov_b32 s5, s13                                              // 0000000000E8:
0xBE8300FF, 0x00012000, // s_mov_b32 s3, 0x12000                                          // 0000000000EC:
0xBE8600FF, 0x00050000, // s_mov_b32 s6, 0x50000                                          // 0000000000F4:
0xD1FE000C, 0x02061902, // v_add_lshl_u32 v12, v2, v12, 1                                 // 0000000000FC:
0x682C2403,             // v_add_u32_e32 v22, s3, v18                                     // 000000000104:
0x682E24FF, 0x0001B000, // v_add_u32_e32 v23, 0x1b000, v18                                // 000000000108:
0xE0501000, 0x80010214, // buffer_load_dword v2, v20, s[4:7], 0 offen                     // 000000000110:
0xE0501040, 0x80011214, // buffer_load_dword v18, v20, s[4:7], 0 offen offset:64          // 000000000118:
0xB00D0500,             // s_movk_i32 s13, 0x500                                          // 000000000120:
0x681E180D,             // v_add_u32_e32 v15, s13, v12                                    // 000000000124:
0xD3D94004, 0x18000080, // v_accvgpr_write_b32 a4, 0                                      // 000000000128:
0x7E0AA504,             // v_accvgpr_mov_b32 a5, a4                                       // 000000000130:
0x7E0CA504,             // v_accvgpr_mov_b32 a6, a4                                       // 000000000134:
0x7E0EA504,             // v_accvgpr_mov_b32 a7, a4                                       // 000000000138:
0xD3D94000, 0x18000080, // v_accvgpr_write_b32 a0, 0                                      // 00000000013C:
0x24262681,             // v_lshlrev_b32_e32 v19, 1, v19                                  // 000000000144:
0xBE8C0080,             // s_mov_b32 s12, 0                                               // 000000000148:
0x681C2A0D,             // v_add_u32_e32 v14, s13, v21                                    // 00000000014C:
0xD1FD0013, 0x044D0202, // v_lshl_add_u32 v19, s2, 1, v19                                 // 000000000150:
0x282828FF, 0x00000080, // v_or_b32_e32 v20, 0x80, v20                                    // 000000000158:
0xBF8C0F71,             // s_waitcnt vmcnt(1)                                             // 000000000160:
0xD81A0000, 0x0000020D, // ds_write_b32 v13, v2                                           // 000000000164:
0xD2A00002, 0x00020D04, // v_pack_b32_f16 v2, v4, v6                                      // 00000000016C:
0xD2A01803, 0x00020D04, // v_pack_b32_f16 v3, v4, v6 op_sel:[1,1,0]                       // 000000000174:
0xD2A00004, 0x00020F05, // v_pack_b32_f16 v4, v5, v7                                      // 00000000017C:
0xD2A01805, 0x00020F05, // v_pack_b32_f16 v5, v5, v7 op_sel:[1,1,0]                       // 000000000184:
0xD2A00006, 0x00022108, // v_pack_b32_f16 v6, v8, v16                                     // 00000000018C:
0xD2A01807, 0x00022108, // v_pack_b32_f16 v7, v8, v16 op_sel:[1,1,0]                      // 000000000194:
0xD2A00008, 0x00022309, // v_pack_b32_f16 v8, v9, v17                                     // 00000000019C:
0xD2A01809, 0x00022309, // v_pack_b32_f16 v9, v9, v17 op_sel:[1,1,0]                      // 0000000001A4:
0xD9BE0500, 0x00000215, // ds_write_b128 v21, v[2:5] offset:1280                          // 0000000001AC:
0xD9BE0500, 0x0000060C, // ds_write_b128 v12, v[6:9] offset:1280                          // 0000000001B4:
0xE0541000, 0x80020616, // buffer_load_dwordx2 v[6:7], v22, s[8:11], 0 offen              // 0000000001BC:
0xE0541900, 0x80020816, // buffer_load_dwordx2 v[8:9], v22, s[8:11], 0 offen offset:2304  // 0000000001C4:
0xE0541000, 0x80020217, // buffer_load_dwordx2 v[2:3], v23, s[8:11], 0 offen              // 0000000001CC:
0xE0541900, 0x80020417, // buffer_load_dwordx2 v[4:5], v23, s[8:11], 0 offen offset:2304  // 0000000001D4:
0x24220085,             // v_lshlrev_b32_e32 v17, 5, v0                                   // 0000000001DC:
0xD1C8000C, 0x020D0300, // v_bfe_u32 v12, v0, 1, 3                                        // 0000000001E0:
0xD1C80010, 0x02090900, // v_bfe_u32 v16, v0, 4, 2                                        // 0000000001E8:
0x262222A0,             // v_and_b32_e32 v17, 32, v17                                     // 0000000001F0:
0xD2000011, 0x04450710, // v_lshl_or_b32 v17, v16, 3, v17                                 // 0000000001F4:
0x1018180E,             // v_mul_u32_u24_e32 v12, s14, v12                                // 0000000001FC:
0xD1FE0011, 0x02061911, // v_add_lshl_u32 v17, v17, v12, 1                                // 000000000200:
0x20180082,             // v_lshrrev_b32_e32 v12, 2, v0                                   // 000000000208:
0xD3D84017, 0x18000105, // v_accvgpr_read_b32 v23, a5                                     // 00000000020C:
0xD2010000, 0x0405610C, // v_and_or_b32 v0, v12, 48, v1                                   // 000000000214:
0x102020FF, 0x00000420, // v_mul_u32_u24_e32 v16, 0x420, v16                              // 00000000021C:
0xD1FD0010, 0x04410300, // v_lshl_add_u32 v16, v0, 1, v16                                 // 000000000224:
0xD3D94001, 0x18000117, // v_accvgpr_write_b32 a1, v23                                    // 00000000022C:
0xD3D84017, 0x18000106, // v_accvgpr_read_b32 v23, a6                                     // 000000000234:
0xD1FD0010, 0x00350310, // v_lshl_add_u32 v16, v16, 1, s13                                // 00000000023C:
0xD3D94002, 0x18000117, // v_accvgpr_write_b32 a2, v23                                    // 000000000244:
0xD3D84017, 0x18000107, // v_accvgpr_read_b32 v23, a7                                     // 00000000024C:
0x682A20A0,             // v_add_u32_e32 v21, 32, v16                                     // 000000000254:
0x682C20B0,             // v_add_u32_e32 v22, 48, v16                                     // 000000000258:
0xD3D94003, 0x18000117, // v_accvgpr_write_b32 a3, v23                                    // 00000000025C:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0) 264 <BB0_1>                               // 000000000264:
0xBF8A0000,             // s_barrier                                                      // 000000000268:
0xD9FE0000, 0x18000011, // ds_read_b128 v[24:27], v17                                     // 00000000026C:
0xD8700100, 0x24000010, // ds_read2st64_b32 v[36:37], v16 offset1:1                       // 000000000274:
0xD86EC484, 0x26000010, // ds_read2_b32 v[38:39], v16 offset0:132 offset1:196             // 00000000027C:
0xD8700504, 0x28000015, // ds_read2st64_b32 v[40:41], v21 offset0:4 offset1:5             // 000000000284:
0xD8700706, 0x2A000016, // ds_read2st64_b32 v[42:43], v22 offset0:6 offset1:7             // 00000000028C:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000294:
0xBF8A0000,             // s_barrier                                                      // 000000000298:
0xBF8CC37F,             // s_waitcnt lgkmcnt(3)                                           // 00000000029C:
0x7E400324,             // v_mov_b32_e32 v32, v36                                         // 0000000002A0:
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                           // 0000000002A4:
0x7E420326,             // v_mov_b32_e32 v33, v38                                         // 0000000002A8:
0xBF8C0F74,             // s_waitcnt vmcnt(4)                                             // 0000000002AC:
0xD81A0000, 0x0000120D, // ds_write_b32 v13, v18                                          // 0000000002B0:
0xE0501000, 0x80011214, // buffer_load_dword v18, v20, s[4:7], 0 offen                    // 0000000002B8:
0xD3CD8004, 0x04124118, // v_mfma_f32_16x16x16f16 a[4:7], v[24:25], v[32:33], a[4:7]      // 0000000002C0:
0xBF8C0F73,             // s_waitcnt vmcnt(3)                                             // 0000000002C8:
0xD2A0001C, 0x00021106, // v_pack_b32_f16 v28, v6, v8                                     // 0000000002CC:
0xD2A0181D, 0x00021106, // v_pack_b32_f16 v29, v6, v8 op_sel:[1,1,0]                      // 0000000002D4:
0xD2A0001E, 0x00021307, // v_pack_b32_f16 v30, v7, v9                                     // 0000000002DC:
0xD2A0181F, 0x00021307, // v_pack_b32_f16 v31, v7, v9 op_sel:[1,1,0]                      // 0000000002E4:
0xBF8C0F71,             // s_waitcnt vmcnt(1)                                             // 0000000002EC:
0xD2A00020, 0x00020902, // v_pack_b32_f16 v32, v2, v4                                     // 0000000002F0:
0xD2A01821, 0x00020902, // v_pack_b32_f16 v33, v2, v4 op_sel:[1,1,0]                      // 0000000002F8:
0x6804260C,             // v_add_u32_e32 v2, s12, v19                                     // 000000000300:
0x680804FF, 0x00024000, // v_add_u32_e32 v4, 0x24000, v2                                  // 000000000304:
0x810C030C,             // s_add_i32 s12, s12, s3                                         // 00000000030C:
0xD2A00022, 0x00020B03, // v_pack_b32_f16 v34, v3, v5                                     // 000000000310:
0xD2A01823, 0x00020B03, // v_pack_b32_f16 v35, v3, v5 op_sel:[1,1,0]                      // 000000000318:
0x682E04FF, 0x0002D000, // v_add_u32_e32 v23, 0x2d000, v2                                 // 000000000320:
0xE0541000, 0x80020604, // buffer_load_dwordx2 v[6:7], v4, s[8:11], 0 offen               // 000000000328:
0xE0541900, 0x80020804, // buffer_load_dwordx2 v[8:9], v4, s[8:11], 0 offen offset:2304   // 000000000330:
0xE0541000, 0x80020217, // buffer_load_dwordx2 v[2:3], v23, s[8:11], 0 offen              // 000000000338:
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                           // 000000000340:
0x7E0A032A,             // v_mov_b32_e32 v5, v42                                          // 000000000344:
0x7E080328,             // v_mov_b32_e32 v4, v40                                          // 000000000348:
0xBF07FF0C, 0x00B1C000, // s_cmp_lg_u32 s12, 0xb1c000                                     // 00000000034C:
0x7E4C0325,             // v_mov_b32_e32 v38, v37                                         // 000000000354:
0xD3CD8004, 0x0412091A, // v_mfma_f32_16x16x16f16 a[4:7], v[26:27], v[4:5], a[4:7]        // 000000000358:
0xE0541900, 0x80020417, // buffer_load_dwordx2 v[4:5], v23, s[8:11], 0 offen offset:2304  // 000000000360:
0x7E540329,             // v_mov_b32_e32 v42, v41                                         // 000000000368:
0x682828C0,             // v_add_u32_e32 v20, 64, v20                                     // 00000000036C:
0xD9BE0000, 0x00001C0E, // ds_write_b128 v14, v[28:31]                                    // 000000000370:
0xD9BE0000, 0x0000200F, // ds_write_b128 v15, v[32:35]                                    // 000000000378:
0xD3CD8000, 0x04024D18, // v_mfma_f32_16x16x16f16 a[0:3], v[24:25], v[38:39], a[0:3]      // 000000000380:
0xD3CD8000, 0x0402551A, // v_mfma_f32_16x16x16f16 a[0:3], v[26:27], v[42:43], a[0:3]      // 000000000388:
0xBF85FFB4,             // s_cbranch_scc1 BB0_1                                           // 000000000390:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000394:
0xBF8A0000,             // s_barrier                                                      // 000000000398:
0xD9FE0000, 0x14000011, // ds_read_b128 v[20:23], v17                                     // 00000000039C:
0xD8700100, 0x18000010, // ds_read2st64_b32 v[24:25], v16 offset1:1                       // 0000000003A4:
0xD86EC484, 0x1C000010, // ds_read2_b32 v[28:29], v16 offset0:132 offset1:196             // 0000000003AC:
0x682620A0,             // v_add_u32_e32 v19, 32, v16                                     // 0000000003B4:
0x684420B0,             // v_add_u32_e32 v34, 48, v16                                     // 0000000003B8:
0x24000082,             // v_lshlrev_b32_e32 v0, 2, v0                                    // 0000000003BC:
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                           // 0000000003C0:
0x7E340318,             // v_mov_b32_e32 v26, v24                                         // 0000000003C4:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000003C8:
0x7E36031C,             // v_mov_b32_e32 v27, v28                                         // 0000000003CC:
0x7E380319,             // v_mov_b32_e32 v28, v25                                         // 0000000003D0:
0xBF800000,             // s_nop 0                                                        // 0000000003D4:
0xD3CD8004, 0x04123514, // v_mfma_f32_16x16x16f16 a[4:7], v[20:21], v[26:27], a[4:7]      // 0000000003D8:
0xD8700504, 0x1A000013, // ds_read2st64_b32 v[26:27], v19 offset0:4 offset1:5             // 0000000003E0:
0xD8700706, 0x1E000022, // ds_read2st64_b32 v[30:31], v34 offset0:6 offset1:7             // 0000000003E8:
0xB0030480,             // s_movk_i32 s3, 0x480                                           // 0000000003F0:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000003F4:
0xBF8A0000,             // s_barrier                                                      // 0000000003F8:
0xBF8C0F74,             // s_waitcnt vmcnt(4)                                             // 0000000003FC:
0xD81A0000, 0x0000120D, // ds_write_b32 v13, v18                                          // 000000000400:
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                           // 000000000408:
0x7E40031A,             // v_mov_b32_e32 v32, v26                                         // 00000000040C:
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                           // 000000000410:
0x7E42031E,             // v_mov_b32_e32 v33, v30                                         // 000000000414:
0xBF8C0F72,             // s_waitcnt vmcnt(2)                                             // 000000000418:
0xD2A00018, 0x00021106, // v_pack_b32_f16 v24, v6, v8                                     // 00000000041C:
0xD2A01819, 0x00021106, // v_pack_b32_f16 v25, v6, v8 op_sel:[1,1,0]                      // 000000000424:
0x7E3C031B,             // v_mov_b32_e32 v30, v27                                         // 00000000042C:
0xD2A0001A, 0x00021307, // v_pack_b32_f16 v26, v7, v9                                     // 000000000430:
0xD2A0181B, 0x00021307, // v_pack_b32_f16 v27, v7, v9 op_sel:[1,1,0]                      // 000000000438:
0xBF8C0F70,             // s_waitcnt vmcnt(0)                                             // 000000000440:
0xD2A00006, 0x00020902, // v_pack_b32_f16 v6, v2, v4                                      // 000000000444:
0xD2A01807, 0x00020902, // v_pack_b32_f16 v7, v2, v4 op_sel:[1,1,0]                       // 00000000044C:
0xD2A00008, 0x00020B03, // v_pack_b32_f16 v8, v3, v5                                      // 000000000454:
0xD2A01809, 0x00020B03, // v_pack_b32_f16 v9, v3, v5 op_sel:[1,1,0]                       // 00000000045C:
0xD9BE0000, 0x0000180E, // ds_write_b128 v14, v[24:27]                                    // 000000000464:
0xD9BE0000, 0x0000060F, // ds_write_b128 v15, v[6:9]                                      // 00000000046C:
0xD3CD8004, 0x04124116, // v_mfma_f32_16x16x16f16 a[4:7], v[22:23], v[32:33], a[4:7]      // 000000000474:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 00000000047C:
0xBF8A0000,             // s_barrier                                                      // 000000000480:
0xD9FE0000, 0x02000011, // ds_read_b128 v[2:5], v17                                       // 000000000484:
0xD8700100, 0x06000010, // ds_read2st64_b32 v[6:7], v16 offset1:1                         // 00000000048C:
0xD86EC484, 0x08000010, // ds_read2_b32 v[8:9], v16 offset0:132 offset1:196               // 000000000494:
0xD8700504, 0x10000013, // ds_read2st64_b32 v[16:17], v19 offset0:4 offset1:5             // 00000000049C:
0xD8700706, 0x12000022, // ds_read2st64_b32 v[18:19], v34 offset0:6 offset1:7             // 0000000004A4:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000004AC:
0xBF8A0000,             // s_barrier                                                      // 0000000004B0:
0xD3CD8000, 0x04023914, // v_mfma_f32_16x16x16f16 a[0:3], v[20:21], v[28:29], a[0:3]      // 0000000004B4:
0xBF8CC37F,             // s_waitcnt lgkmcnt(3)                                           // 0000000004BC:
0x7E1C0306,             // v_mov_b32_e32 v14, v6                                          // 0000000004C0:
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                           // 0000000004C4:
0x7E1E0308,             // v_mov_b32_e32 v15, v8                                          // 0000000004C8:
0x7E100307,             // v_mov_b32_e32 v8, v7                                           // 0000000004CC:
0x260C188C,             // v_and_b32_e32 v6, 12, v12                                      // 0000000004D0:
0xD200000C, 0x04011106, // v_lshl_or_b32 v12, v6, 8, v0                                   // 0000000004D4:
0x24000284,             // v_lshlrev_b32_e32 v0, 4, v1                                    // 0000000004DC:
0xD3CD8004, 0x04121D02, // v_mfma_f32_16x16x16f16 a[4:7], v[2:3], v[14:15], a[4:7]        // 0000000004E0:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000004E8:
0x7E1E0312,             // v_mov_b32_e32 v15, v18                                         // 0000000004EC:
0x7E1C0310,             // v_mov_b32_e32 v14, v16                                         // 0000000004F0:
0x7E240311,             // v_mov_b32_e32 v18, v17                                         // 0000000004F4:
0xD3CD8000, 0x04023D16, // v_mfma_f32_16x16x16f16 a[0:3], v[22:23], v[30:31], a[0:3]      // 0000000004F8:
0xD3CD8004, 0x04121D04, // v_mfma_f32_16x16x16f16 a[4:7], v[4:5], v[14:15], a[4:7]        // 000000000500:
0xD3CD8000, 0x04021102, // v_mfma_f32_16x16x16f16 a[0:3], v[2:3], v[8:9], a[0:3]          // 000000000508:
0xD2000002, 0x0401110B, // v_lshl_or_b32 v2, v11, 8, v0                                   // 000000000510:
0xBF800007,             // s_nop 7                                                        // 000000000518:
0xBF800000,             // s_nop 0                                                        // 00000000051C:
0xDA1A0000, 0x0000040C, // ds_write_b32 v12, a4                                           // 000000000520:
0xDA1A0100, 0x0000050C, // ds_write_b32 v12, a5 offset:256                                // 000000000528:
0xDA1A0200, 0x0000060C, // ds_write_b32 v12, a6 offset:512                                // 000000000530:
0xDA1A0300, 0x0000070C, // ds_write_b32 v12, a7 offset:768                                // 000000000538:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000540:
0xBF8A0000,             // s_barrier                                                      // 000000000544:
0xD9FE0000, 0x06000002, // ds_read_b128 v[6:9], v2                                        // 000000000548:
0xD3CD8000, 0x04022504, // v_mfma_f32_16x16x16f16 a[0:3], v[4:5], v[18:19], a[0:3]        // 000000000550:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000558:
0x7E001506,             // v_cvt_f16_f32_e32 v0, v6                                       // 00000000055C:
0x7E061508,             // v_cvt_f16_f32_e32 v3, v8                                       // 000000000560:
0x7E0C1509,             // v_cvt_f16_f32_e32 v6, v9                                       // 000000000564:
0x7E0E1507,             // v_cvt_f16_f32_e32 v7, v7                                       // 000000000568:
0x24100282,             // v_lshlrev_b32_e32 v8, 2, v1                                    // 00000000056C:
0xD2A00001, 0x00020D03, // v_pack_b32_f16 v1, v3, v6                                      // 000000000570:
0xD2850003, 0x0000070A, // v_mul_lo_u32 v3, v10, s3                                       // 000000000578:
0x68060602,             // v_add_u32_e32 v3, s2, v3                                       // 000000000580:
0xD2A00000, 0x00020F00, // v_pack_b32_f16 v0, v0, v7                                      // 000000000584:
0xBE8300FF, 0x00020000, // s_mov_b32 s3, 0x20000                                          // 00000000058C:
0xBE8200FF, 0x00012000, // s_mov_b32 s2, 0x12000                                          // 000000000594:
0xD1FE0004, 0x02061103, // v_add_lshl_u32 v4, v3, v8, 1                                   // 00000000059C:
0xE0741000, 0x80000004, // buffer_store_dwordx2 v[0:1], v4, s[0:3], 0 offen               // 0000000005A4:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000005AC:
0xBF8A0000,             // s_barrier                                                      // 0000000005B0:
0xDA1A0000, 0x0000000C, // ds_write_b32 v12, a0                                           // 0000000005B4:
0xDA1A0100, 0x0000010C, // ds_write_b32 v12, a1 offset:256                                // 0000000005BC:
0xDA1A0200, 0x0000020C, // ds_write_b32 v12, a2 offset:512                                // 0000000005C4:
0xDA1A0300, 0x0000030C, // ds_write_b32 v12, a3 offset:768                                // 0000000005CC:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000005D4:
0xBF8A0000,             // s_barrier                                                      // 0000000005D8:
0xD9FE0000, 0x00000002, // ds_read_b128 v[0:3], v2                                        // 0000000005DC:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000005E4:
0x7E001500,             // v_cvt_f16_f32_e32 v0, v0                                       // 0000000005E8:
0x7E0A1501,             // v_cvt_f16_f32_e32 v5, v1                                       // 0000000005EC:
0x7E041502,             // v_cvt_f16_f32_e32 v2, v2                                       // 0000000005F0:
0x7E061503,             // v_cvt_f16_f32_e32 v3, v3                                       // 0000000005F4:
0xD2A00000, 0x00020B00, // v_pack_b32_f16 v0, v0, v5                                      // 0000000005F8:
0xD2A00001, 0x00020702, // v_pack_b32_f16 v1, v2, v3                                      // 000000000600:
0xE0741080, 0x80000004, // buffer_store_dwordx2 v[0:1], v4, s[0:3], 0 offen offset:128    // 000000000608:
0xBF810000              // s_endpgm                                                       // 000000000610:
#endif
};

const uint32_t GEMM_ISA_16_5120_384[] = {
#if USE_EMPTY_KERNEL
0xBF810000,              // s_endpgm                                                         // 000000000660:
#else
// Original ISA:
// s4-s5 : kernarg pointer
// s6 : workgroup_id_x
// s7 : workgroup_id_y
// s8 : workgroup_id_z
//0xC00A0302, 0x00000000, // s_load_dwordx4 s[12:15], s[4:5], 0x0                           // 000000000000:
//0xC00A0002, 0x00000010, // s_load_dwordx4 s[0:3], s[4:5], 0x10                            // 000000000008:
0x20060085,             // v_lshrrev_b32_e32 v3, 5, v0                                    // 000000000010:
0x24080082,             // v_lshlrev_b32_e32 v4, 2, v0                                    // 000000000014:
//0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000018:

// Hacked ISA
// s0-s1: matrix C pointer
// s2-s3: matrix A pointer
// s4-s5: matrix B pointer
// s6 : workgroup_id_x
// s7 : workgroup_id_y
// s8 : workgroup_id_z
0xBE8C0002,             //     s_mov_b32 s12, s2
0xBE8D0003,             //     s_mov_b32 s13, s3
0xBE8E0004,             //     s_mov_b32 s14, s4
0xBE8F0005,             //     s_mov_b32 s15, s5
0xBF800000,             // s_nop 0

0x8E038407,             // s_lshl_b32 s3, s7, 4                                           // 00000000001C:
0x20160084,             // v_lshrrev_b32_e32 v11, 4, v0                                   // 000000000020:
0x261A08FF, 0x0000007C, // v_and_b32_e32 v13, 0x7c, v4                                    // 000000000024:
0x100A06FF, 0x00002800, // v_mul_u32_u24_e32 v5, 0x2800, v3                               // 00000000002C:
0x8E028706,             // s_lshl_b32 s2, s6, 7                                           // 000000000034:
0x28261B05,             // v_or_b32_e32 v19, v5, v13                                      // 000000000038:
0x2602008F,             // v_and_b32_e32 v1, 15, v0                                       // 00000000003C:
0x68141603,             // v_add_u32_e32 v10, s3, v11                                     // 000000000040:
0xB0030180,             // s_movk_i32 s3, 0x180                                           // 000000000044:
0xD1FE0012, 0x02040513, // v_add_lshl_u32 v18, v19, s2, 1                                 // 000000000048:
0xD2850004, 0x0000070A, // v_mul_lo_u32 v4, v10, s3                                       // 000000000050:
0x24040281,             // v_lshlrev_b32_e32 v2, 1, v1                                    // 000000000058:
0xBE8700FF, 0x00020000, // s_mov_b32 s7, 0x20000                                          // 00000000005C:
0x28080504,             // v_or_b32_e32 v4, v4, v2                                        // 000000000064:
0xBE88000E,             // s_mov_b32 s8, s14                                              // 000000000068:
0xBE8A00FF, 0x00780000, // s_mov_b32 s10, 0x780000                                        // 00000000006C:
0xBE89000F,             // s_mov_b32 s9, s15                                              // 000000000074:
0xBE8B0007,             // s_mov_b32 s11, s7                                              // 000000000078:
0x681824FF, 0x00002000, // v_add_u32_e32 v12, 0x2000, v18                                 // 00000000007C:
0x24280881,             // v_lshlrev_b32_e32 v20, 1, v4                                   // 000000000084:
0x681C24FF, 0x00028000, // v_add_u32_e32 v14, 0x28000, v18                                // 000000000088:
0x681E24FF, 0x0002A000, // v_add_u32_e32 v15, 0x2a000, v18                                // 000000000090:
0xE0541000, 0x80020412, // buffer_load_dwordx2 v[4:5], v18, s[8:11], 0 offen              // 000000000098:
0xE0541800, 0x8002060C, // buffer_load_dwordx2 v[6:7], v12, s[8:11], 0 offen offset:2048  // 0000000000A0:
0xE0541000, 0x8002080E, // buffer_load_dwordx2 v[8:9], v14, s[8:11], 0 offen              // 0000000000A8:
0xE0541800, 0x8002100F, // buffer_load_dwordx2 v[16:17], v15, s[8:11], 0 offen offset:2048// 0000000000B0:
0x24180081,             // v_lshlrev_b32_e32 v12, 1, v0                                   // 0000000000B8:
0xD2010002, 0x0409410C, // v_and_or_b32 v2, v12, 32, v2                                   // 0000000000BC:
0xB00E0050,             // s_movk_i32 s14, 0x50                                           // 0000000000C4:
0x1018060E,             // v_mul_u32_u24_e32 v12, s14, v3                                 // 0000000000C8:
0x24040481,             // v_lshlrev_b32_e32 v2, 1, v2                                    // 0000000000CC:
0xB0030108,             // s_movk_i32 s3, 0x108                                           // 0000000000D0:
0xD1FD000C, 0x0409030C, // v_lshl_add_u32 v12, v12, 1, v2                                 // 0000000000D4:
0x10040603,             // v_mul_u32_u24_e32 v2, s3, v3                                   // 0000000000DC:
0x241A1A81,             // v_lshlrev_b32_e32 v13, 1, v13                                  // 0000000000E0:
0xD1FE0015, 0x0206050D, // v_add_lshl_u32 v21, v13, v2, 1                                 // 0000000000E4:
0x28040688,             // v_or_b32_e32 v2, 8, v3                                         // 0000000000EC:
0x10040403,             // v_mul_u32_u24_e32 v2, s3, v2                                   // 0000000000F0:
0xBE84000C,             // s_mov_b32 s4, s12                                              // 0000000000F4:
0xBE85000D,             // s_mov_b32 s5, s13                                              // 0000000000F8:
0xBE8300FF, 0x00050000, // s_mov_b32 s3, 0x50000                                          // 0000000000FC:
0xB0066000,             // s_movk_i32 s6, 0x6000                                          // 000000000104:
0xD1FE000D, 0x02061B02, // v_add_lshl_u32 v13, v2, v13, 1                                 // 000000000108:
0x682C2403,             // v_add_u32_e32 v22, s3, v18                                     // 000000000110:
0x682E24FF, 0x00052000, // v_add_u32_e32 v23, 0x52000, v18                                // 000000000114:
0x683024FF, 0x00078000, // v_add_u32_e32 v24, 0x78000, v18                                // 00000000011C:
0x683224FF, 0x0007A000, // v_add_u32_e32 v25, 0x7a000, v18                                // 000000000124:
0xE0501000, 0x80010214, // buffer_load_dword v2, v20, s[4:7], 0 offen                     // 00000000012C:
0xE0501040, 0x80011214, // buffer_load_dword v18, v20, s[4:7], 0 offen offset:64          // 000000000134:
0xB00D0500,             // s_movk_i32 s13, 0x500                                          // 00000000013C:
0x681E1A0D,             // v_add_u32_e32 v15, s13, v13                                    // 000000000140:
0xD3D94004, 0x18000080, // v_accvgpr_write_b32 a4, 0                                      // 000000000144:
0x7E0AA504,             // v_accvgpr_mov_b32 a5, a4                                       // 00000000014C:
0x7E0CA504,             // v_accvgpr_mov_b32 a6, a4                                       // 000000000150:
0x7E0EA504,             // v_accvgpr_mov_b32 a7, a4                                       // 000000000154:
0xD3D94000, 0x18000080, // v_accvgpr_write_b32 a0, 0                                      // 000000000158:
0x24262681,             // v_lshlrev_b32_e32 v19, 1, v19                                  // 000000000160:
0xBE8C0080,             // s_mov_b32 s12, 0                                               // 000000000164:
0x681C2A0D,             // v_add_u32_e32 v14, s13, v21                                    // 000000000168:
0xD1FD0013, 0x044D0202, // v_lshl_add_u32 v19, s2, 1, v19                                 // 00000000016C:
0x282828FF, 0x00000080, // v_or_b32_e32 v20, 0x80, v20                                    // 000000000174:
0xBF8C0F71,             // s_waitcnt vmcnt(1)                                             // 00000000017C:
0xD81A0000, 0x0000020C, // ds_write_b32 v12, v2                                           // 000000000180:
0xD2A00002, 0x00020D04, // v_pack_b32_f16 v2, v4, v6                                      // 000000000188:
0xD2A01803, 0x00020D04, // v_pack_b32_f16 v3, v4, v6 op_sel:[1,1,0]                       // 000000000190:
0xD2A00004, 0x00020F05, // v_pack_b32_f16 v4, v5, v7                                      // 000000000198:
0xD2A01805, 0x00020F05, // v_pack_b32_f16 v5, v5, v7 op_sel:[1,1,0]                       // 0000000001A0:
0xD2A00006, 0x00022108, // v_pack_b32_f16 v6, v8, v16                                     // 0000000001A8:
0xD2A01807, 0x00022108, // v_pack_b32_f16 v7, v8, v16 op_sel:[1,1,0]                      // 0000000001B0:
0xD2A00008, 0x00022309, // v_pack_b32_f16 v8, v9, v17                                     // 0000000001B8:
0xD2A01809, 0x00022309, // v_pack_b32_f16 v9, v9, v17 op_sel:[1,1,0]                      // 0000000001C0:
0xD9BE0500, 0x00000215, // ds_write_b128 v21, v[2:5] offset:1280                          // 0000000001C8:
0xD9BE0500, 0x0000060D, // ds_write_b128 v13, v[6:9] offset:1280                          // 0000000001D0:
0xE0541000, 0x80020616, // buffer_load_dwordx2 v[6:7], v22, s[8:11], 0 offen              // 0000000001D8:
0xE0541800, 0x80020817, // buffer_load_dwordx2 v[8:9], v23, s[8:11], 0 offen offset:2048  // 0000000001E0:
0xE0541000, 0x80020218, // buffer_load_dwordx2 v[2:3], v24, s[8:11], 0 offen              // 0000000001E8:
0xE0541800, 0x80020419, // buffer_load_dwordx2 v[4:5], v25, s[8:11], 0 offen offset:2048  // 0000000001F0:
0x24220085,             // v_lshlrev_b32_e32 v17, 5, v0                                   // 0000000001F8:
0xD1C8000D, 0x020D0300, // v_bfe_u32 v13, v0, 1, 3                                        // 0000000001FC:
0xD1C80010, 0x02090900, // v_bfe_u32 v16, v0, 4, 2                                        // 000000000204:
0x262222A0,             // v_and_b32_e32 v17, 32, v17                                     // 00000000020C:
0xD2000011, 0x04450710, // v_lshl_or_b32 v17, v16, 3, v17                                 // 000000000210:
0x101A1A0E,             // v_mul_u32_u24_e32 v13, s14, v13                                // 000000000218:
0xD1FE0011, 0x02061B11, // v_add_lshl_u32 v17, v17, v13, 1                                // 00000000021C:
0x201A0082,             // v_lshrrev_b32_e32 v13, 2, v0                                   // 000000000224:
0xD3D84017, 0x18000105, // v_accvgpr_read_b32 v23, a5                                     // 000000000228:
0xD2010000, 0x0405610D, // v_and_or_b32 v0, v13, 48, v1                                   // 000000000230:
0x102020FF, 0x00000420, // v_mul_u32_u24_e32 v16, 0x420, v16                              // 000000000238:
0xD1FD0010, 0x04410300, // v_lshl_add_u32 v16, v0, 1, v16                                 // 000000000240:
0xD3D94001, 0x18000117, // v_accvgpr_write_b32 a1, v23                                    // 000000000248:
0xD3D84017, 0x18000106, // v_accvgpr_read_b32 v23, a6                                     // 000000000250:
0xD1FD0010, 0x00350310, // v_lshl_add_u32 v16, v16, 1, s13                                // 000000000258:
0xD3D94002, 0x18000117, // v_accvgpr_write_b32 a2, v23                                    // 000000000260:
0xD3D84017, 0x18000107, // v_accvgpr_read_b32 v23, a7                                     // 000000000268:
0x682A20A0,             // v_add_u32_e32 v21, 32, v16                                     // 000000000270:
0x682C20B0,             // v_add_u32_e32 v22, 48, v16                                     // 000000000274:
0xD3D94003, 0x18000117, // v_accvgpr_write_b32 a3, v23                                    // 000000000278:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0) <BB0_1>                                   // 000000000280:
0xBF8A0000,             // s_barrier                                                      // 000000000284:
0xD9FE0000, 0x18000011, // ds_read_b128 v[24:27], v17                                     // 000000000288:
0xD8700100, 0x24000010, // ds_read2st64_b32 v[36:37], v16 offset1:1                       // 000000000290:
0xD86EC484, 0x26000010, // ds_read2_b32 v[38:39], v16 offset0:132 offset1:196             // 000000000298:
0xD8700504, 0x28000015, // ds_read2st64_b32 v[40:41], v21 offset0:4 offset1:5             // 0000000002A0:
0xD8700706, 0x2A000016, // ds_read2st64_b32 v[42:43], v22 offset0:6 offset1:7             // 0000000002A8:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000002B0:
0xBF8A0000,             // s_barrier                                                      // 0000000002B4:
0xBF8C0F74,             // s_waitcnt vmcnt(4)                                             // 0000000002B8:
0xD81A0000, 0x0000120C, // ds_write_b32 v12, v18                                          // 0000000002BC:
0xE0501000, 0x80011214, // buffer_load_dword v18, v20, s[4:7], 0 offen                    // 0000000002C4:
0xBF8C0F73,             // s_waitcnt vmcnt(3)                                             // 0000000002CC:
0xD2A0001C, 0x00021106, // v_pack_b32_f16 v28, v6, v8                                     // 0000000002D0:
0xD2A0181D, 0x00021106, // v_pack_b32_f16 v29, v6, v8 op_sel:[1,1,0]                      // 0000000002D8:
0xD2A0001E, 0x00021307, // v_pack_b32_f16 v30, v7, v9                                     // 0000000002E0:
0xD2A0181F, 0x00021307, // v_pack_b32_f16 v31, v7, v9 op_sel:[1,1,0]                      // 0000000002E8:
0xBF8CC37F,             // s_waitcnt lgkmcnt(3)                                           // 0000000002F0:
0x7E0E0326,             // v_mov_b32_e32 v7, v38                                          // 0000000002F4:
0x7E0C0324,             // v_mov_b32_e32 v6, v36                                          // 0000000002F8:
0xBF8C0F71,             // s_waitcnt vmcnt(1)                                             // 0000000002FC:
0xD2A00020, 0x00020902, // v_pack_b32_f16 v32, v2, v4                                     // 000000000300:
0xD2A01821, 0x00020902, // v_pack_b32_f16 v33, v2, v4 op_sel:[1,1,0]                      // 000000000308:
0x6804260C,             // v_add_u32_e32 v2, s12, v19                                     // 000000000310:
0xD2A00022, 0x00020B03, // v_pack_b32_f16 v34, v3, v5                                     // 000000000314:
0xD2A01823, 0x00020B03, // v_pack_b32_f16 v35, v3, v5 op_sel:[1,1,0]                      // 00000000031C:
0x7E4C0325,             // v_mov_b32_e32 v38, v37                                         // 000000000324:
0xD3CD8004, 0x04120D18, // v_mfma_f32_16x16x16f16 a[4:7], v[24:25], v[6:7], a[4:7]        // 000000000328:
0x810C030C,             // s_add_i32 s12, s12, s3                                         // 000000000330:
0x680804FF, 0x000A0000, // v_add_u32_e32 v4, 0xa0000, v2                                  // 000000000334:
0x680A04FF, 0x000A2000, // v_add_u32_e32 v5, 0xa2000, v2                                  // 00000000033C:
0x682E04FF, 0x000C8000, // v_add_u32_e32 v23, 0xc8000, v2                                 // 000000000344:
0xBF07FF0C, 0x00320000, // s_cmp_lg_u32 s12, 0x320000                                     // 00000000034C:
0x682828C0,             // v_add_u32_e32 v20, 64, v20                                     // 000000000354:
0xD3CD8000, 0x04024D18, // v_mfma_f32_16x16x16f16 a[0:3], v[24:25], v[38:39], a[0:3]      // 000000000358:
0x683004FF, 0x000CA000, // v_add_u32_e32 v24, 0xca000, v2                                 // 000000000360:
0xE0541000, 0x80020604, // buffer_load_dwordx2 v[6:7], v4, s[8:11], 0 offen               // 000000000368:
0xE0541800, 0x80020805, // buffer_load_dwordx2 v[8:9], v5, s[8:11], 0 offen offset:2048   // 000000000370:
0xE0541000, 0x80020217, // buffer_load_dwordx2 v[2:3], v23, s[8:11], 0 offen              // 000000000378:
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                           // 000000000380:
0x7E0A032A,             // v_mov_b32_e32 v5, v42                                          // 000000000384:
0x7E080328,             // v_mov_b32_e32 v4, v40                                          // 000000000388:
0x7E540329,             // v_mov_b32_e32 v42, v41                                         // 00000000038C:
0xD9BE0000, 0x00001C0E, // ds_write_b128 v14, v[28:31]                                    // 000000000390:
0xD3CD8004, 0x0412091A, // v_mfma_f32_16x16x16f16 a[4:7], v[26:27], v[4:5], a[4:7]        // 000000000398:
0xE0541800, 0x80020418, // buffer_load_dwordx2 v[4:5], v24, s[8:11], 0 offen offset:2048  // 0000000003A0:
0xD9BE0000, 0x0000200F, // ds_write_b128 v15, v[32:35]                                    // 0000000003A8:
0xD3CD8000, 0x0402551A, // v_mfma_f32_16x16x16f16 a[0:3], v[26:27], v[42:43], a[0:3]      // 0000000003B0:
0xBF85FFB1,             // s_cbranch_scc1 BB0_1                                           // 0000000003B8:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000003BC:
0xBF8A0000,             // s_barrier                                                      // 0000000003C0:
0xD9FE0000, 0x14000011, // ds_read_b128 v[20:23], v17                                     // 0000000003C4:
0xD8700100, 0x18000010, // ds_read2st64_b32 v[24:25], v16 offset1:1                       // 0000000003CC:
0xD86EC484, 0x1C000010, // ds_read2_b32 v[28:29], v16 offset0:132 offset1:196             // 0000000003D4:
0x682620A0,             // v_add_u32_e32 v19, 32, v16                                     // 0000000003DC:
0x684420B0,             // v_add_u32_e32 v34, 48, v16                                     // 0000000003E0:
0x24000082,             // v_lshlrev_b32_e32 v0, 2, v0                                    // 0000000003E4:
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                           // 0000000003E8:
0x7E340318,             // v_mov_b32_e32 v26, v24                                         // 0000000003EC:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000003F0:
0x7E36031C,             // v_mov_b32_e32 v27, v28                                         // 0000000003F4:
0x7E380319,             // v_mov_b32_e32 v28, v25                                         // 0000000003F8:
0xBF800000,             // s_nop 0                                                        // 0000000003FC:
0xD3CD8004, 0x04123514, // v_mfma_f32_16x16x16f16 a[4:7], v[20:21], v[26:27], a[4:7]      // 000000000400:
0xD8700504, 0x1A000013, // ds_read2st64_b32 v[26:27], v19 offset0:4 offset1:5             // 000000000408:
0xD8700706, 0x1E000022, // ds_read2st64_b32 v[30:31], v34 offset0:6 offset1:7             // 000000000410:
0xB0031400,             // s_movk_i32 s3, 0x1400                                          // 000000000418:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 00000000041C:
0xBF8A0000,             // s_barrier                                                      // 000000000420:
0xBF8C0F74,             // s_waitcnt vmcnt(4)                                             // 000000000424:
0xD81A0000, 0x0000120C, // ds_write_b32 v12, v18                                          // 000000000428:
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                           // 000000000430:
0x7E40031A,             // v_mov_b32_e32 v32, v26                                         // 000000000434:
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                           // 000000000438:
0x7E42031E,             // v_mov_b32_e32 v33, v30                                         // 00000000043C:
0xBF8C0F72,             // s_waitcnt vmcnt(2)                                             // 000000000440:
0xD2A00018, 0x00021106, // v_pack_b32_f16 v24, v6, v8                                     // 000000000444:
0xD2A01819, 0x00021106, // v_pack_b32_f16 v25, v6, v8 op_sel:[1,1,0]                      // 00000000044C:
0x7E3C031B,             // v_mov_b32_e32 v30, v27                                         // 000000000454:
0xD2A0001A, 0x00021307, // v_pack_b32_f16 v26, v7, v9                                     // 000000000458:
0xD2A0181B, 0x00021307, // v_pack_b32_f16 v27, v7, v9 op_sel:[1,1,0]                      // 000000000460:
0xBF8C0F70,             // s_waitcnt vmcnt(0)                                             // 000000000468:
0xD2A00006, 0x00020902, // v_pack_b32_f16 v6, v2, v4                                      // 00000000046C:
0xD2A01807, 0x00020902, // v_pack_b32_f16 v7, v2, v4 op_sel:[1,1,0]                       // 000000000474:
0xD2A00008, 0x00020B03, // v_pack_b32_f16 v8, v3, v5                                      // 00000000047C:
0xD2A01809, 0x00020B03, // v_pack_b32_f16 v9, v3, v5 op_sel:[1,1,0]                       // 000000000484:
0xD9BE0000, 0x0000180E, // ds_write_b128 v14, v[24:27]                                    // 00000000048C:
0xD9BE0000, 0x0000060F, // ds_write_b128 v15, v[6:9]                                      // 000000000494:
0xD3CD8004, 0x04124116, // v_mfma_f32_16x16x16f16 a[4:7], v[22:23], v[32:33], a[4:7]      // 00000000049C:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000004A4:
0xBF8A0000,             // s_barrier                                                      // 0000000004A8:
0xD9FE0000, 0x02000011, // ds_read_b128 v[2:5], v17                                       // 0000000004AC:
0xD8700100, 0x06000010, // ds_read2st64_b32 v[6:7], v16 offset1:1                         // 0000000004B4:
0xD86EC484, 0x08000010, // ds_read2_b32 v[8:9], v16 offset0:132 offset1:196               // 0000000004BC:
0xD8700504, 0x10000013, // ds_read2st64_b32 v[16:17], v19 offset0:4 offset1:5             // 0000000004C4:
0xD8700706, 0x12000022, // ds_read2st64_b32 v[18:19], v34 offset0:6 offset1:7             // 0000000004CC:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000004D4:
0xBF8A0000,             // s_barrier                                                      // 0000000004D8:
0xD3CD8000, 0x04023914, // v_mfma_f32_16x16x16f16 a[0:3], v[20:21], v[28:29], a[0:3]      // 0000000004DC:
0xBF8CC37F,             // s_waitcnt lgkmcnt(3)                                           // 0000000004E4:
0x7E1C0306,             // v_mov_b32_e32 v14, v6                                          // 0000000004E8:
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                           // 0000000004EC:
0x7E1E0308,             // v_mov_b32_e32 v15, v8                                          // 0000000004F0:
0x7E100307,             // v_mov_b32_e32 v8, v7                                           // 0000000004F4:
0x260C1A8C,             // v_and_b32_e32 v6, 12, v13                                      // 0000000004F8:
0xD200000C, 0x04011106, // v_lshl_or_b32 v12, v6, 8, v0                                   // 0000000004FC:
0x24000284,             // v_lshlrev_b32_e32 v0, 4, v1                                    // 000000000504:
0xD3CD8004, 0x04121D02, // v_mfma_f32_16x16x16f16 a[4:7], v[2:3], v[14:15], a[4:7]        // 000000000508:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000510:
0x7E1E0312,             // v_mov_b32_e32 v15, v18                                         // 000000000514:
0x7E1C0310,             // v_mov_b32_e32 v14, v16                                         // 000000000518:
0x7E240311,             // v_mov_b32_e32 v18, v17                                         // 00000000051C:
0xD3CD8000, 0x04023D16, // v_mfma_f32_16x16x16f16 a[0:3], v[22:23], v[30:31], a[0:3]      // 000000000520:
0xD3CD8004, 0x04121D04, // v_mfma_f32_16x16x16f16 a[4:7], v[4:5], v[14:15], a[4:7]        // 000000000528:
0xD3CD8000, 0x04021102, // v_mfma_f32_16x16x16f16 a[0:3], v[2:3], v[8:9], a[0:3]          // 000000000530:
0xD2000002, 0x0401110B, // v_lshl_or_b32 v2, v11, 8, v0                                   // 000000000538:
0xBF800007,             // s_nop 7                                                        // 000000000540:
0xBF800000,             // s_nop 0                                                        // 000000000544:
0xDA1A0000, 0x0000040C, // ds_write_b32 v12, a4                                           // 000000000548:
0xDA1A0100, 0x0000050C, // ds_write_b32 v12, a5 offset:256                                // 000000000550:
0xDA1A0200, 0x0000060C, // ds_write_b32 v12, a6 offset:512                                // 000000000558:
0xDA1A0300, 0x0000070C, // ds_write_b32 v12, a7 offset:768                                // 000000000560:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000568:
0xBF8A0000,             // s_barrier                                                      // 00000000056C:
0xD9FE0000, 0x06000002, // ds_read_b128 v[6:9], v2                                        // 000000000570:
0xD3CD8000, 0x04022504, // v_mfma_f32_16x16x16f16 a[0:3], v[4:5], v[18:19], a[0:3]        // 000000000578:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000580:
0x7E001506,             // v_cvt_f16_f32_e32 v0, v6                                       // 000000000584:
0x7E061508,             // v_cvt_f16_f32_e32 v3, v8                                       // 000000000588:
0x7E0C1509,             // v_cvt_f16_f32_e32 v6, v9                                       // 00000000058C:
0x7E0E1507,             // v_cvt_f16_f32_e32 v7, v7                                       // 000000000590:
0x24100282,             // v_lshlrev_b32_e32 v8, 2, v1                                    // 000000000594:
0xD2A00001, 0x00020D03, // v_pack_b32_f16 v1, v3, v6                                      // 000000000598:
0xD2850003, 0x0000070A, // v_mul_lo_u32 v3, v10, s3                                       // 0000000005A0:
0x68060602,             // v_add_u32_e32 v3, s2, v3                                       // 0000000005A8:
0xD2A00000, 0x00020F00, // v_pack_b32_f16 v0, v0, v7                                      // 0000000005AC:
0xBE8300FF, 0x00020000, // s_mov_b32 s3, 0x20000                                          // 0000000005B4:
0xBE8200FF, 0x00050000, // s_mov_b32 s2, 0x50000                                          // 0000000005BC:
0xD1FE0004, 0x02061103, // v_add_lshl_u32 v4, v3, v8, 1                                   // 0000000005C4:
0xE0741000, 0x80000004, // buffer_store_dwordx2 v[0:1], v4, s[0:3], 0 offen               // 0000000005CC:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000005D4:
0xBF8A0000,             // s_barrier                                                      // 0000000005D8:
0xDA1A0000, 0x0000000C, // ds_write_b32 v12, a0                                           // 0000000005DC:
0xDA1A0100, 0x0000010C, // ds_write_b32 v12, a1 offset:256                                // 0000000005E4:
0xDA1A0200, 0x0000020C, // ds_write_b32 v12, a2 offset:512                                // 0000000005EC:
0xDA1A0300, 0x0000030C, // ds_write_b32 v12, a3 offset:768                                // 0000000005F4:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000005FC:
0xBF8A0000,             // s_barrier                                                      // 000000000600:
0xD9FE0000, 0x00000002, // ds_read_b128 v[0:3], v2                                        // 000000000604:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 00000000060C:
0x7E001500,             // v_cvt_f16_f32_e32 v0, v0                                       // 000000000610:
0x7E0A1501,             // v_cvt_f16_f32_e32 v5, v1                                       // 000000000614:
0x7E041502,             // v_cvt_f16_f32_e32 v2, v2                                       // 000000000618:
0x7E061503,             // v_cvt_f16_f32_e32 v3, v3                                       // 00000000061C:
0xD2A00000, 0x00020B00, // v_pack_b32_f16 v0, v0, v5                                      // 000000000620:
0xD2A00001, 0x00020702, // v_pack_b32_f16 v1, v2, v3                                      // 000000000628:
0xE0741080, 0x80000004, // buffer_store_dwordx2 v[0:1], v4, s[0:3], 0 offen offset:128    // 000000000630:
0xBF810000              // s_endpgm                                                       // 000000000638:
#endif
};

const uint32_t GEMM_ISA_16_1280_5120[] = {
#if USE_EMPTY_KERNEL
0xBF810000,              // s_endpgm                                                         // 000000000660:
#else
// Original ISA:
// s4-s5 : kernarg pointer
// s6 : workgroup_id_x
// s7 : workgroup_id_y
// s8 : workgroup_id_z
//0xC00A0302, 0x00000000, // s_load_dwordx4 s[12:15], s[4:5], 0x0                           // 000000000000: 
//0xC00A0002, 0x00000010, // s_load_dwordx4 s[0:3], s[4:5], 0x10                            // 000000000008: 
0x20060085,             // v_lshrrev_b32_e32 v3, 5, v0                                    // 000000000010: 
0x24080082,             // v_lshlrev_b32_e32 v4, 2, v0                                    // 000000000014: 
//0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000018: 

// Hacked ISA
// s0-s1: matrix C pointer
// s2-s3: matrix A pointer
// s4-s5: matrix B pointer
// s6 : workgroup_id_x
// s7 : workgroup_id_y
// s8 : workgroup_id_z
0xBE8C0002,             //     s_mov_b32 s12, s2
0xBE8D0003,             //     s_mov_b32 s13, s3
0xBE8E0004,             //     s_mov_b32 s14, s4
0xBE8F0005,             //     s_mov_b32 s15, s5
0xBF800000,             // s_nop 0

0x8E038407,             // s_lshl_b32 s3, s7, 4                                           // 00000000001C: 
0x20160084,             // v_lshrrev_b32_e32 v11, 4, v0                                   // 000000000020: 
0x261808FF, 0x0000007C, // v_and_b32_e32 v12, 0x7c, v4                                    // 000000000024: 
0x100A06FF, 0x00000A00, // v_mul_u32_u24_e32 v5, 0xa00, v3                                // 00000000002C: 
0x8E028706,             // s_lshl_b32 s2, s6, 7                                           // 000000000034: 
0x28261905,             // v_or_b32_e32 v19, v5, v12                                      // 000000000038: 
0x2602008F,             // v_and_b32_e32 v1, 15, v0                                       // 00000000003C: 
0x68141603,             // v_add_u32_e32 v10, s3, v11                                     // 000000000040: 
0xB0031400,             // s_movk_i32 s3, 0x1400                                          // 000000000044: 
0xD1FE0012, 0x02040513, // v_add_lshl_u32 v18, v19, s2, 1                                 // 000000000048: 
0xD2850004, 0x0000070A, // v_mul_lo_u32 v4, v10, s3                                       // 000000000050: 
0x24040281,             // v_lshlrev_b32_e32 v2, 1, v1                                    // 000000000058: 
0xBE8700FF, 0x00020000, // s_mov_b32 s7, 0x20000                                          // 00000000005C: 
0x28080504,             // v_or_b32_e32 v4, v4, v2                                        // 000000000064: 
0xBE88000E,             // s_mov_b32 s8, s14                                              // 000000000068: 
0xBE8A00FF, 0x01900000, // s_mov_b32 s10, 0x1900000                                       // 00000000006C: 
0xBE89000F,             // s_mov_b32 s9, s15                                              // 000000000074: 
0xBE8B0007,             // s_mov_b32 s11, s7                                              // 000000000078: 
0x681A24FF, 0x0000A000, // v_add_u32_e32 v13, 0xa000, v18                                 // 00000000007C: 
0x24280881,             // v_lshlrev_b32_e32 v20, 1, v4                                   // 000000000084: 
0xE0541000, 0x80020412, // buffer_load_dwordx2 v[4:5], v18, s[8:11], 0 offen              // 000000000088: 
0xE0541A00, 0x80020612, // buffer_load_dwordx2 v[6:7], v18, s[8:11], 0 offen offset:2560  // 000000000090: 
0xE0541000, 0x8002080D, // buffer_load_dwordx2 v[8:9], v13, s[8:11], 0 offen              // 000000000098: 
0xE0541A00, 0x8002100D, // buffer_load_dwordx2 v[16:17], v13, s[8:11], 0 offen offset:2560// 0000000000A0: 
0x241A0081,             // v_lshlrev_b32_e32 v13, 1, v0                                   // 0000000000A8: 
0xD2010002, 0x0409410D, // v_and_or_b32 v2, v13, 32, v2                                   // 0000000000AC: 
0xB00E0050,             // s_movk_i32 s14, 0x50                                           // 0000000000B4: 
0x101A060E,             // v_mul_u32_u24_e32 v13, s14, v3                                 // 0000000000B8: 
0x24040481,             // v_lshlrev_b32_e32 v2, 1, v2                                    // 0000000000BC: 
0xB0030108,             // s_movk_i32 s3, 0x108                                           // 0000000000C0: 
0xD1FD000D, 0x0409030D, // v_lshl_add_u32 v13, v13, 1, v2                                 // 0000000000C4: 
0x10040603,             // v_mul_u32_u24_e32 v2, s3, v3                                   // 0000000000CC: 
0x24181881,             // v_lshlrev_b32_e32 v12, 1, v12                                  // 0000000000D0: 
0xD1FE0015, 0x0206050C, // v_add_lshl_u32 v21, v12, v2, 1                                 // 0000000000D4: 
0x28040688,             // v_or_b32_e32 v2, 8, v3                                         // 0000000000DC: 
0x10040403,             // v_mul_u32_u24_e32 v2, s3, v2                                   // 0000000000E0: 
0xBE84000C,             // s_mov_b32 s4, s12                                              // 0000000000E4: 
0xBE85000D,             // s_mov_b32 s5, s13                                              // 0000000000E8: 
0xBE8300FF, 0x00014000, // s_mov_b32 s3, 0x14000                                          // 0000000000EC: 
0xBE8600FF, 0x00050000, // s_mov_b32 s6, 0x50000                                          // 0000000000F4: 
0xD1FE000C, 0x02061902, // v_add_lshl_u32 v12, v2, v12, 1                                 // 0000000000FC: 
0x682C2403,             // v_add_u32_e32 v22, s3, v18                                     // 000000000104: 
0x682E24FF, 0x0001E000, // v_add_u32_e32 v23, 0x1e000, v18                                // 000000000108: 
0xE0501000, 0x80010214, // buffer_load_dword v2, v20, s[4:7], 0 offen                     // 000000000110: 
0xE0501040, 0x80011214, // buffer_load_dword v18, v20, s[4:7], 0 offen offset:64          // 000000000118: 
0xB00D0500,             // s_movk_i32 s13, 0x500                                          // 000000000120: 
0x681E180D,             // v_add_u32_e32 v15, s13, v12                                    // 000000000124: 
0xD3D94004, 0x18000080, // v_accvgpr_write_b32 a4, 0                                      // 000000000128: 
0x7E0AA504,             // v_accvgpr_mov_b32 a5, a4                                       // 000000000130: 
0x7E0CA504,             // v_accvgpr_mov_b32 a6, a4                                       // 000000000134: 
0x7E0EA504,             // v_accvgpr_mov_b32 a7, a4                                       // 000000000138: 
0xD3D94000, 0x18000080, // v_accvgpr_write_b32 a0, 0                                      // 00000000013C: 
0x24262681,             // v_lshlrev_b32_e32 v19, 1, v19                                  // 000000000144: 
0xBE8C0080,             // s_mov_b32 s12, 0                                               // 000000000148: 
0x681C2A0D,             // v_add_u32_e32 v14, s13, v21                                    // 00000000014C: 
0xD1FD0013, 0x044D0202, // v_lshl_add_u32 v19, s2, 1, v19                                 // 000000000150: 
0x282828FF, 0x00000080, // v_or_b32_e32 v20, 0x80, v20                                    // 000000000158: 
0xBF8C0F71,             // s_waitcnt vmcnt(1)                                             // 000000000160: 
0xD81A0000, 0x0000020D, // ds_write_b32 v13, v2                                           // 000000000164: 
0xD2A00002, 0x00020D04, // v_pack_b32_f16 v2, v4, v6                                      // 00000000016C: 
0xD2A01803, 0x00020D04, // v_pack_b32_f16 v3, v4, v6 op_sel:[1,1,0]                       // 000000000174: 
0xD2A00004, 0x00020F05, // v_pack_b32_f16 v4, v5, v7                                      // 00000000017C: 
0xD2A01805, 0x00020F05, // v_pack_b32_f16 v5, v5, v7 op_sel:[1,1,0]                       // 000000000184: 
0xD2A00006, 0x00022108, // v_pack_b32_f16 v6, v8, v16                                     // 00000000018C: 
0xD2A01807, 0x00022108, // v_pack_b32_f16 v7, v8, v16 op_sel:[1,1,0]                      // 000000000194: 
0xD2A00008, 0x00022309, // v_pack_b32_f16 v8, v9, v17                                     // 00000000019C: 
0xD2A01809, 0x00022309, // v_pack_b32_f16 v9, v9, v17 op_sel:[1,1,0]                      // 0000000001A4: 
0xD9BE0500, 0x00000215, // ds_write_b128 v21, v[2:5] offset:1280                          // 0000000001AC: 
0xD9BE0500, 0x0000060C, // ds_write_b128 v12, v[6:9] offset:1280                          // 0000000001B4: 
0xE0541000, 0x80020616, // buffer_load_dwordx2 v[6:7], v22, s[8:11], 0 offen              // 0000000001BC: 
0xE0541A00, 0x80020816, // buffer_load_dwordx2 v[8:9], v22, s[8:11], 0 offen offset:2560  // 0000000001C4: 
0xE0541000, 0x80020217, // buffer_load_dwordx2 v[2:3], v23, s[8:11], 0 offen              // 0000000001CC: 
0xE0541A00, 0x80020417, // buffer_load_dwordx2 v[4:5], v23, s[8:11], 0 offen offset:2560  // 0000000001D4: 
0x24220085,             // v_lshlrev_b32_e32 v17, 5, v0                                   // 0000000001DC: 
0xD1C8000C, 0x020D0300, // v_bfe_u32 v12, v0, 1, 3                                        // 0000000001E0: 
0xD1C80010, 0x02090900, // v_bfe_u32 v16, v0, 4, 2                                        // 0000000001E8: 
0x262222A0,             // v_and_b32_e32 v17, 32, v17                                     // 0000000001F0: 
0xD2000011, 0x04450710, // v_lshl_or_b32 v17, v16, 3, v17                                 // 0000000001F4: 
0x1018180E,             // v_mul_u32_u24_e32 v12, s14, v12                                // 0000000001FC: 
0xD1FE0011, 0x02061911, // v_add_lshl_u32 v17, v17, v12, 1                                // 000000000200: 
0x20180082,             // v_lshrrev_b32_e32 v12, 2, v0                                   // 000000000208: 
0xD3D84017, 0x18000105, // v_accvgpr_read_b32 v23, a5                                     // 00000000020C: 
0xD2010000, 0x0405610C, // v_and_or_b32 v0, v12, 48, v1                                   // 000000000214: 
0x102020FF, 0x00000420, // v_mul_u32_u24_e32 v16, 0x420, v16                              // 00000000021C: 
0xD1FD0010, 0x04410300, // v_lshl_add_u32 v16, v0, 1, v16                                 // 000000000224: 
0xD3D94001, 0x18000117, // v_accvgpr_write_b32 a1, v23                                    // 00000000022C: 
0xD3D84017, 0x18000106, // v_accvgpr_read_b32 v23, a6                                     // 000000000234: 
0xD1FD0010, 0x00350310, // v_lshl_add_u32 v16, v16, 1, s13                                // 00000000023C: 
0xD3D94002, 0x18000117, // v_accvgpr_write_b32 a2, v23                                    // 000000000244: 
0xD3D84017, 0x18000107, // v_accvgpr_read_b32 v23, a7                                     // 00000000024C: 
0x682A20A0,             // v_add_u32_e32 v21, 32, v16                                     // 000000000254: 
0x682C20B0,             // v_add_u32_e32 v22, 48, v16                                     // 000000000258: 
0xD3D94003, 0x18000117, // v_accvgpr_write_b32 a3, v23                                    // 00000000025C: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0) <BB0_1>                                   // 000000000264: 
0xBF8A0000,             // s_barrier                                                      // 000000000268: 
0xD9FE0000, 0x18000011, // ds_read_b128 v[24:27], v17                                     // 00000000026C: 
0xD8700100, 0x24000010, // ds_read2st64_b32 v[36:37], v16 offset1:1                       // 000000000274: 
0xD86EC484, 0x26000010, // ds_read2_b32 v[38:39], v16 offset0:132 offset1:196             // 00000000027C: 
0xD8700504, 0x28000015, // ds_read2st64_b32 v[40:41], v21 offset0:4 offset1:5             // 000000000284: 
0xD8700706, 0x2A000016, // ds_read2st64_b32 v[42:43], v22 offset0:6 offset1:7             // 00000000028C: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000294: 
0xBF8A0000,             // s_barrier                                                      // 000000000298: 
0xBF8CC37F,             // s_waitcnt lgkmcnt(3)                                           // 00000000029C: 
0x7E400324,             // v_mov_b32_e32 v32, v36                                         // 0000000002A0: 
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                           // 0000000002A4: 
0x7E420326,             // v_mov_b32_e32 v33, v38                                         // 0000000002A8: 
0xBF8C0F74,             // s_waitcnt vmcnt(4)                                             // 0000000002AC: 
0xD81A0000, 0x0000120D, // ds_write_b32 v13, v18                                          // 0000000002B0: 
0xE0501000, 0x80011214, // buffer_load_dword v18, v20, s[4:7], 0 offen                    // 0000000002B8: 
0xD3CD8004, 0x04124118, // v_mfma_f32_16x16x16f16 a[4:7], v[24:25], v[32:33], a[4:7]      // 0000000002C0: 
0xBF8C0F73,             // s_waitcnt vmcnt(3)                                             // 0000000002C8: 
0xD2A0001C, 0x00021106, // v_pack_b32_f16 v28, v6, v8                                     // 0000000002CC: 
0xD2A0181D, 0x00021106, // v_pack_b32_f16 v29, v6, v8 op_sel:[1,1,0]                      // 0000000002D4: 
0xD2A0001E, 0x00021307, // v_pack_b32_f16 v30, v7, v9                                     // 0000000002DC: 
0xD2A0181F, 0x00021307, // v_pack_b32_f16 v31, v7, v9 op_sel:[1,1,0]                      // 0000000002E4: 
0xBF8C0F71,             // s_waitcnt vmcnt(1)                                             // 0000000002EC: 
0xD2A00020, 0x00020902, // v_pack_b32_f16 v32, v2, v4                                     // 0000000002F0: 
0xD2A01821, 0x00020902, // v_pack_b32_f16 v33, v2, v4 op_sel:[1,1,0]                      // 0000000002F8: 
0x6804260C,             // v_add_u32_e32 v2, s12, v19                                     // 000000000300: 
0x680804FF, 0x00028000, // v_add_u32_e32 v4, 0x28000, v2                                  // 000000000304: 
0x810C030C,             // s_add_i32 s12, s12, s3                                         // 00000000030C: 
0xD2A00022, 0x00020B03, // v_pack_b32_f16 v34, v3, v5                                     // 000000000310: 
0xD2A01823, 0x00020B03, // v_pack_b32_f16 v35, v3, v5 op_sel:[1,1,0]                      // 000000000318: 
0x682E04FF, 0x00032000, // v_add_u32_e32 v23, 0x32000, v2                                 // 000000000320: 
0xE0541000, 0x80020604, // buffer_load_dwordx2 v[6:7], v4, s[8:11], 0 offen               // 000000000328: 
0xE0541A00, 0x80020804, // buffer_load_dwordx2 v[8:9], v4, s[8:11], 0 offen offset:2560   // 000000000330: 
0xE0541000, 0x80020217, // buffer_load_dwordx2 v[2:3], v23, s[8:11], 0 offen              // 000000000338: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                           // 000000000340: 
0x7E0A032A,             // v_mov_b32_e32 v5, v42                                          // 000000000344: 
0x7E080328,             // v_mov_b32_e32 v4, v40                                          // 000000000348: 
0xBF07FF0C, 0x00C58000, // s_cmp_lg_u32 s12, 0xc58000                                     // 00000000034C: 
0x7E4C0325,             // v_mov_b32_e32 v38, v37                                         // 000000000354: 
0xD3CD8004, 0x0412091A, // v_mfma_f32_16x16x16f16 a[4:7], v[26:27], v[4:5], a[4:7]        // 000000000358: 
0xE0541A00, 0x80020417, // buffer_load_dwordx2 v[4:5], v23, s[8:11], 0 offen offset:2560  // 000000000360: 
0x7E540329,             // v_mov_b32_e32 v42, v41                                         // 000000000368: 
0x682828C0,             // v_add_u32_e32 v20, 64, v20                                     // 00000000036C: 
0xD9BE0000, 0x00001C0E, // ds_write_b128 v14, v[28:31]                                    // 000000000370: 
0xD9BE0000, 0x0000200F, // ds_write_b128 v15, v[32:35]                                    // 000000000378: 
0xD3CD8000, 0x04024D18, // v_mfma_f32_16x16x16f16 a[0:3], v[24:25], v[38:39], a[0:3]      // 000000000380: 
0xD3CD8000, 0x0402551A, // v_mfma_f32_16x16x16f16 a[0:3], v[26:27], v[42:43], a[0:3]      // 000000000388: 
0xBF85FFB4,             // s_cbranch_scc1 BB0_1                                           // 000000000390: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000394: 
0xBF8A0000,             // s_barrier                                                      // 000000000398: 
0xD9FE0000, 0x14000011, // ds_read_b128 v[20:23], v17                                     // 00000000039C: 
0xD8700100, 0x18000010, // ds_read2st64_b32 v[24:25], v16 offset1:1                       // 0000000003A4: 
0xD86EC484, 0x1C000010, // ds_read2_b32 v[28:29], v16 offset0:132 offset1:196             // 0000000003AC: 
0x682620A0,             // v_add_u32_e32 v19, 32, v16                                     // 0000000003B4: 
0x684420B0,             // v_add_u32_e32 v34, 48, v16                                     // 0000000003B8: 
0x24000082,             // v_lshlrev_b32_e32 v0, 2, v0                                    // 0000000003BC: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                           // 0000000003C0: 
0x7E340318,             // v_mov_b32_e32 v26, v24                                         // 0000000003C4: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000003C8: 
0x7E36031C,             // v_mov_b32_e32 v27, v28                                         // 0000000003CC: 
0x7E380319,             // v_mov_b32_e32 v28, v25                                         // 0000000003D0: 
0xBF800000,             // s_nop 0                                                        // 0000000003D4: 
0xD3CD8004, 0x04123514, // v_mfma_f32_16x16x16f16 a[4:7], v[20:21], v[26:27], a[4:7]      // 0000000003D8: 
0xD8700504, 0x1A000013, // ds_read2st64_b32 v[26:27], v19 offset0:4 offset1:5             // 0000000003E0: 
0xD8700706, 0x1E000022, // ds_read2st64_b32 v[30:31], v34 offset0:6 offset1:7             // 0000000003E8: 
0xB0030500,             // s_movk_i32 s3, 0x500                                           // 0000000003F0: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000003F4: 
0xBF8A0000,             // s_barrier                                                      // 0000000003F8: 
0xBF8C0F74,             // s_waitcnt vmcnt(4)                                             // 0000000003FC: 
0xD81A0000, 0x0000120D, // ds_write_b32 v13, v18                                          // 000000000400: 
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                           // 000000000408: 
0x7E40031A,             // v_mov_b32_e32 v32, v26                                         // 00000000040C: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                           // 000000000410: 
0x7E42031E,             // v_mov_b32_e32 v33, v30                                         // 000000000414: 
0xBF8C0F72,             // s_waitcnt vmcnt(2)                                             // 000000000418: 
0xD2A00018, 0x00021106, // v_pack_b32_f16 v24, v6, v8                                     // 00000000041C: 
0xD2A01819, 0x00021106, // v_pack_b32_f16 v25, v6, v8 op_sel:[1,1,0]                      // 000000000424: 
0x7E3C031B,             // v_mov_b32_e32 v30, v27                                         // 00000000042C: 
0xD2A0001A, 0x00021307, // v_pack_b32_f16 v26, v7, v9                                     // 000000000430: 
0xD2A0181B, 0x00021307, // v_pack_b32_f16 v27, v7, v9 op_sel:[1,1,0]                      // 000000000438: 
0xBF8C0F70,             // s_waitcnt vmcnt(0)                                             // 000000000440: 
0xD2A00006, 0x00020902, // v_pack_b32_f16 v6, v2, v4                                      // 000000000444: 
0xD2A01807, 0x00020902, // v_pack_b32_f16 v7, v2, v4 op_sel:[1,1,0]                       // 00000000044C: 
0xD2A00008, 0x00020B03, // v_pack_b32_f16 v8, v3, v5                                      // 000000000454: 
0xD2A01809, 0x00020B03, // v_pack_b32_f16 v9, v3, v5 op_sel:[1,1,0]                       // 00000000045C: 
0xD9BE0000, 0x0000180E, // ds_write_b128 v14, v[24:27]                                    // 000000000464: 
0xD9BE0000, 0x0000060F, // ds_write_b128 v15, v[6:9]                                      // 00000000046C: 
0xD3CD8004, 0x04124116, // v_mfma_f32_16x16x16f16 a[4:7], v[22:23], v[32:33], a[4:7]      // 000000000474: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 00000000047C: 
0xBF8A0000,             // s_barrier                                                      // 000000000480: 
0xD9FE0000, 0x02000011, // ds_read_b128 v[2:5], v17                                       // 000000000484: 
0xD8700100, 0x06000010, // ds_read2st64_b32 v[6:7], v16 offset1:1                         // 00000000048C: 
0xD86EC484, 0x08000010, // ds_read2_b32 v[8:9], v16 offset0:132 offset1:196               // 000000000494: 
0xD8700504, 0x10000013, // ds_read2st64_b32 v[16:17], v19 offset0:4 offset1:5             // 00000000049C: 
0xD8700706, 0x12000022, // ds_read2st64_b32 v[18:19], v34 offset0:6 offset1:7             // 0000000004A4: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000004AC: 
0xBF8A0000,             // s_barrier                                                      // 0000000004B0: 
0xD3CD8000, 0x04023914, // v_mfma_f32_16x16x16f16 a[0:3], v[20:21], v[28:29], a[0:3]      // 0000000004B4: 
0xBF8CC37F,             // s_waitcnt lgkmcnt(3)                                           // 0000000004BC: 
0x7E1C0306,             // v_mov_b32_e32 v14, v6                                          // 0000000004C0: 
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                           // 0000000004C4: 
0x7E1E0308,             // v_mov_b32_e32 v15, v8                                          // 0000000004C8: 
0x7E100307,             // v_mov_b32_e32 v8, v7                                           // 0000000004CC: 
0x260C188C,             // v_and_b32_e32 v6, 12, v12                                      // 0000000004D0: 
0xD200000C, 0x04011106, // v_lshl_or_b32 v12, v6, 8, v0                                   // 0000000004D4: 
0x24000284,             // v_lshlrev_b32_e32 v0, 4, v1                                    // 0000000004DC: 
0xD3CD8004, 0x04121D02, // v_mfma_f32_16x16x16f16 a[4:7], v[2:3], v[14:15], a[4:7]        // 0000000004E0: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000004E8: 
0x7E1E0312,             // v_mov_b32_e32 v15, v18                                         // 0000000004EC: 
0x7E1C0310,             // v_mov_b32_e32 v14, v16                                         // 0000000004F0: 
0x7E240311,             // v_mov_b32_e32 v18, v17                                         // 0000000004F4: 
0xD3CD8000, 0x04023D16, // v_mfma_f32_16x16x16f16 a[0:3], v[22:23], v[30:31], a[0:3]      // 0000000004F8: 
0xD3CD8004, 0x04121D04, // v_mfma_f32_16x16x16f16 a[4:7], v[4:5], v[14:15], a[4:7]        // 000000000500: 
0xD3CD8000, 0x04021102, // v_mfma_f32_16x16x16f16 a[0:3], v[2:3], v[8:9], a[0:3]          // 000000000508: 
0xD2000002, 0x0401110B, // v_lshl_or_b32 v2, v11, 8, v0                                   // 000000000510: 
0xBF800007,             // s_nop 7                                                        // 000000000518: 
0xBF800000,             // s_nop 0                                                        // 00000000051C: 
0xDA1A0000, 0x0000040C, // ds_write_b32 v12, a4                                           // 000000000520: 
0xDA1A0100, 0x0000050C, // ds_write_b32 v12, a5 offset:256                                // 000000000528: 
0xDA1A0200, 0x0000060C, // ds_write_b32 v12, a6 offset:512                                // 000000000530: 
0xDA1A0300, 0x0000070C, // ds_write_b32 v12, a7 offset:768                                // 000000000538: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000540: 
0xBF8A0000,             // s_barrier                                                      // 000000000544: 
0xD9FE0000, 0x06000002, // ds_read_b128 v[6:9], v2                                        // 000000000548: 
0xD3CD8000, 0x04022504, // v_mfma_f32_16x16x16f16 a[0:3], v[4:5], v[18:19], a[0:3]        // 000000000550: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000558: 
0x7E001506,             // v_cvt_f16_f32_e32 v0, v6                                       // 00000000055C: 
0x7E061508,             // v_cvt_f16_f32_e32 v3, v8                                       // 000000000560: 
0x7E0C1509,             // v_cvt_f16_f32_e32 v6, v9                                       // 000000000564: 
0x7E0E1507,             // v_cvt_f16_f32_e32 v7, v7                                       // 000000000568: 
0x24100282,             // v_lshlrev_b32_e32 v8, 2, v1                                    // 00000000056C: 
0xD2A00001, 0x00020D03, // v_pack_b32_f16 v1, v3, v6                                      // 000000000570: 
0xD2850003, 0x0000070A, // v_mul_lo_u32 v3, v10, s3                                       // 000000000578: 
0x68060602,             // v_add_u32_e32 v3, s2, v3                                       // 000000000580: 
0xD2A00000, 0x00020F00, // v_pack_b32_f16 v0, v0, v7                                      // 000000000584: 
0xBE8300FF, 0x00020000, // s_mov_b32 s3, 0x20000                                          // 00000000058C: 
0xBE8200FF, 0x00014000, // s_mov_b32 s2, 0x14000                                          // 000000000594: 
0xD1FE0004, 0x02061103, // v_add_lshl_u32 v4, v3, v8, 1                                   // 00000000059C: 
0xE0741000, 0x80000004, // buffer_store_dwordx2 v[0:1], v4, s[0:3], 0 offen               // 0000000005A4: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000005AC: 
0xBF8A0000,             // s_barrier                                                      // 0000000005B0: 
0xDA1A0000, 0x0000000C, // ds_write_b32 v12, a0                                           // 0000000005B4: 
0xDA1A0100, 0x0000010C, // ds_write_b32 v12, a1 offset:256                                // 0000000005BC: 
0xDA1A0200, 0x0000020C, // ds_write_b32 v12, a2 offset:512                                // 0000000005C4: 
0xDA1A0300, 0x0000030C, // ds_write_b32 v12, a3 offset:768                                // 0000000005CC: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000005D4: 
0xBF8A0000,             // s_barrier                                                      // 0000000005D8: 
0xD9FE0000, 0x00000002, // ds_read_b128 v[0:3], v2                                        // 0000000005DC: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000005E4: 
0x7E001500,             // v_cvt_f16_f32_e32 v0, v0                                       // 0000000005E8: 
0x7E0A1501,             // v_cvt_f16_f32_e32 v5, v1                                       // 0000000005EC: 
0x7E041502,             // v_cvt_f16_f32_e32 v2, v2                                       // 0000000005F0: 
0x7E061503,             // v_cvt_f16_f32_e32 v3, v3                                       // 0000000005F4: 
0xD2A00000, 0x00020B00, // v_pack_b32_f16 v0, v0, v5                                      // 0000000005F8: 
0xD2A00001, 0x00020702, // v_pack_b32_f16 v1, v2, v3                                      // 000000000600: 
0xE0741080, 0x80000004, // buffer_store_dwordx2 v[0:1], v4, s[0:3], 0 offen offset:128    // 000000000608: 
0xBF810000              // s_endpgm                                                       // 000000000610: 
#endif
};

const uint32_t GEMM_ISA_16_5120_1280[] = {
#if USE_EMPTY_KERNEL
0xBF810000,              // s_endpgm                                                         // 000000000660:
#else
// Original ISA:
// s4-s5 : kernarg pointer
// s6 : workgroup_id_x
// s7 : workgroup_id_y
// s8 : workgroup_id_z
//0xC00A0302, 0x00000000, // s_load_dwordx4 s[12:15], s[4:5], 0x0                           // 000000000000: 
//0xC00A0002, 0x00000010, // s_load_dwordx4 s[0:3], s[4:5], 0x10                            // 000000000008: 
0x20060085,             // v_lshrrev_b32_e32 v3, 5, v0                                    // 000000000010: 
0x24080082,             // v_lshlrev_b32_e32 v4, 2, v0                                    // 000000000014: 
//0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000018: 

// Hacked ISA
// s0-s1: matrix C pointer
// s2-s3: matrix A pointer
// s4-s5: matrix B pointer
// s6 : workgroup_id_x
// s7 : workgroup_id_y
// s8 : workgroup_id_z
0xBE8C0002,             //     s_mov_b32 s12, s2
0xBE8D0003,             //     s_mov_b32 s13, s3
0xBE8E0004,             //     s_mov_b32 s14, s4
0xBE8F0005,             //     s_mov_b32 s15, s5
0xBF800000,             // s_nop 0

0x8E038407,             // s_lshl_b32 s3, s7, 4                                           // 00000000001C: 
0x20160084,             // v_lshrrev_b32_e32 v11, 4, v0                                   // 000000000020: 
0x261808FF, 0x0000007C, // v_and_b32_e32 v12, 0x7c, v4                                    // 000000000024: 
0x100A06FF, 0x00002800, // v_mul_u32_u24_e32 v5, 0x2800, v3                               // 00000000002C: 
0x8E028706,             // s_lshl_b32 s2, s6, 7                                           // 000000000034: 
0x28261905,             // v_or_b32_e32 v19, v5, v12                                      // 000000000038: 
0x2602008F,             // v_and_b32_e32 v1, 15, v0                                       // 00000000003C: 
0x68141603,             // v_add_u32_e32 v10, s3, v11                                     // 000000000040: 
0xB0100500,             // s_movk_i32 s16, 0x500                                          // 000000000044: 
0xD1FE0012, 0x02040513, // v_add_lshl_u32 v18, v19, s2, 1                                 // 000000000048: 
0xBE8700FF, 0x00020000, // s_mov_b32 s7, 0x20000                                          // 000000000050: 
0x24040281,             // v_lshlrev_b32_e32 v2, 1, v1                                    // 000000000058: 
0xD2850004, 0x0000210A, // v_mul_lo_u32 v4, v10, s16                                      // 00000000005C: 
0x28080504,             // v_or_b32_e32 v4, v4, v2                                        // 000000000064: 
0xBE8A00FF, 0x01900000, // s_mov_b32 s10, 0x1900000                                       // 000000000068: 
0xBE88000E,             // s_mov_b32 s8, s14                                              // 000000000070: 
0xBE89000F,             // s_mov_b32 s9, s15                                              // 000000000074: 
0xBE8B0007,             // s_mov_b32 s11, s7                                              // 000000000078: 
0x681A24FF, 0x00002000, // v_add_u32_e32 v13, 0x2000, v18                                 // 00000000007C: 
0x24280881,             // v_lshlrev_b32_e32 v20, 1, v4                                   // 000000000084: 
0x681C24FF, 0x00028000, // v_add_u32_e32 v14, 0x28000, v18                                // 000000000088: 
0x681E24FF, 0x0002A000, // v_add_u32_e32 v15, 0x2a000, v18                                // 000000000090: 
0xE0541000, 0x80020412, // buffer_load_dwordx2 v[4:5], v18, s[8:11], 0 offen              // 000000000098: 
0xE0541800, 0x8002060D, // buffer_load_dwordx2 v[6:7], v13, s[8:11], 0 offen offset:2048  // 0000000000A0: 
0xE0541000, 0x8002080E, // buffer_load_dwordx2 v[8:9], v14, s[8:11], 0 offen              // 0000000000A8: 
0xE0541800, 0x8002100F, // buffer_load_dwordx2 v[16:17], v15, s[8:11], 0 offen offset:2048// 0000000000B0: 
0x241A0081,             // v_lshlrev_b32_e32 v13, 1, v0                                   // 0000000000B8: 
0xD2010002, 0x0409410D, // v_and_or_b32 v2, v13, 32, v2                                   // 0000000000BC: 
0xBE85000D,             // s_mov_b32 s5, s13                                              // 0000000000C4: 
0xB00D0050,             // s_movk_i32 s13, 0x50                                           // 0000000000C8: 
0x101A060D,             // v_mul_u32_u24_e32 v13, s13, v3                                 // 0000000000CC: 
0x24040481,             // v_lshlrev_b32_e32 v2, 1, v2                                    // 0000000000D0: 
0xB0030108,             // s_movk_i32 s3, 0x108                                           // 0000000000D4: 
0xD1FD000D, 0x0409030D, // v_lshl_add_u32 v13, v13, 1, v2                                 // 0000000000D8: 
0x10040603,             // v_mul_u32_u24_e32 v2, s3, v3                                   // 0000000000E0: 
0x24181881,             // v_lshlrev_b32_e32 v12, 1, v12                                  // 0000000000E4: 
0xD1FE0015, 0x0206050C, // v_add_lshl_u32 v21, v12, v2, 1                                 // 0000000000E8: 
0x28040688,             // v_or_b32_e32 v2, 8, v3                                         // 0000000000F0: 
0x10040403,             // v_mul_u32_u24_e32 v2, s3, v2                                   // 0000000000F4: 
0xBE84000C,             // s_mov_b32 s4, s12                                              // 0000000000F8: 
0xBE8300FF, 0x00050000, // s_mov_b32 s3, 0x50000                                          // 0000000000FC: 
0xBE8600FF, 0x00014000, // s_mov_b32 s6, 0x14000                                          // 000000000104: 
0xD1FE000C, 0x02061902, // v_add_lshl_u32 v12, v2, v12, 1                                 // 00000000010C: 
0x682C2403,             // v_add_u32_e32 v22, s3, v18                                     // 000000000114: 
0x682E24FF, 0x00052000, // v_add_u32_e32 v23, 0x52000, v18                                // 000000000118: 
0x683024FF, 0x00078000, // v_add_u32_e32 v24, 0x78000, v18                                // 000000000120: 
0x683224FF, 0x0007A000, // v_add_u32_e32 v25, 0x7a000, v18                                // 000000000128: 
0xE0501000, 0x80010214, // buffer_load_dword v2, v20, s[4:7], 0 offen                     // 000000000130: 
0xE0501040, 0x80011214, // buffer_load_dword v18, v20, s[4:7], 0 offen offset:64          // 000000000138: 
0x681E1810,             // v_add_u32_e32 v15, s16, v12                                    // 000000000140: 
0xD3D94004, 0x18000080, // v_accvgpr_write_b32 a4, 0                                      // 000000000144: 
0x7E0AA504,             // v_accvgpr_mov_b32 a5, a4                                       // 00000000014C: 
0x7E0CA504,             // v_accvgpr_mov_b32 a6, a4                                       // 000000000150: 
0x7E0EA504,             // v_accvgpr_mov_b32 a7, a4                                       // 000000000154: 
0xD3D94000, 0x18000080, // v_accvgpr_write_b32 a0, 0                                      // 000000000158: 
0x24262681,             // v_lshlrev_b32_e32 v19, 1, v19                                  // 000000000160: 
0xBE8C0080,             // s_mov_b32 s12, 0                                               // 000000000164: 
0x681C2A10,             // v_add_u32_e32 v14, s16, v21                                    // 000000000168: 
0xD1FD0013, 0x044D0202, // v_lshl_add_u32 v19, s2, 1, v19                                 // 00000000016C: 
0x282828FF, 0x00000080, // v_or_b32_e32 v20, 0x80, v20                                    // 000000000174: 
0xBF8C0F71,             // s_waitcnt vmcnt(1)                                             // 00000000017C: 
0xD81A0000, 0x0000020D, // ds_write_b32 v13, v2                                           // 000000000180: 
0xD2A00002, 0x00020D04, // v_pack_b32_f16 v2, v4, v6                                      // 000000000188: 
0xD2A01803, 0x00020D04, // v_pack_b32_f16 v3, v4, v6 op_sel:[1,1,0]                       // 000000000190: 
0xD2A00004, 0x00020F05, // v_pack_b32_f16 v4, v5, v7                                      // 000000000198: 
0xD2A01805, 0x00020F05, // v_pack_b32_f16 v5, v5, v7 op_sel:[1,1,0]                       // 0000000001A0: 
0xD2A00006, 0x00022108, // v_pack_b32_f16 v6, v8, v16                                     // 0000000001A8: 
0xD2A01807, 0x00022108, // v_pack_b32_f16 v7, v8, v16 op_sel:[1,1,0]                      // 0000000001B0: 
0xD2A00008, 0x00022309, // v_pack_b32_f16 v8, v9, v17                                     // 0000000001B8: 
0xD2A01809, 0x00022309, // v_pack_b32_f16 v9, v9, v17 op_sel:[1,1,0]                      // 0000000001C0: 
0xD9BE0500, 0x00000215, // ds_write_b128 v21, v[2:5] offset:1280                          // 0000000001C8: 
0xD9BE0500, 0x0000060C, // ds_write_b128 v12, v[6:9] offset:1280                          // 0000000001D0: 
0xE0541000, 0x80020616, // buffer_load_dwordx2 v[6:7], v22, s[8:11], 0 offen              // 0000000001D8: 
0xE0541800, 0x80020817, // buffer_load_dwordx2 v[8:9], v23, s[8:11], 0 offen offset:2048  // 0000000001E0: 
0xE0541000, 0x80020218, // buffer_load_dwordx2 v[2:3], v24, s[8:11], 0 offen              // 0000000001E8: 
0xE0541800, 0x80020419, // buffer_load_dwordx2 v[4:5], v25, s[8:11], 0 offen offset:2048  // 0000000001F0: 
0x24220085,             // v_lshlrev_b32_e32 v17, 5, v0                                   // 0000000001F8: 
0xD1C8000C, 0x020D0300, // v_bfe_u32 v12, v0, 1, 3                                        // 0000000001FC: 
0xD1C80010, 0x02090900, // v_bfe_u32 v16, v0, 4, 2                                        // 000000000204: 
0x262222A0,             // v_and_b32_e32 v17, 32, v17                                     // 00000000020C: 
0xD2000011, 0x04450710, // v_lshl_or_b32 v17, v16, 3, v17                                 // 000000000210: 
0x1018180D,             // v_mul_u32_u24_e32 v12, s13, v12                                // 000000000218: 
0xD1FE0011, 0x02061911, // v_add_lshl_u32 v17, v17, v12, 1                                // 00000000021C: 
0x20180082,             // v_lshrrev_b32_e32 v12, 2, v0                                   // 000000000224: 
0xD3D84017, 0x18000105, // v_accvgpr_read_b32 v23, a5                                     // 000000000228: 
0xD2010000, 0x0405610C, // v_and_or_b32 v0, v12, 48, v1                                   // 000000000230: 
0x102020FF, 0x00000420, // v_mul_u32_u24_e32 v16, 0x420, v16                              // 000000000238: 
0xD1FD0010, 0x04410300, // v_lshl_add_u32 v16, v0, 1, v16                                 // 000000000240: 
0xD3D94001, 0x18000117, // v_accvgpr_write_b32 a1, v23                                    // 000000000248: 
0xD3D84017, 0x18000106, // v_accvgpr_read_b32 v23, a6                                     // 000000000250: 
0xD1FD0010, 0x00410310, // v_lshl_add_u32 v16, v16, 1, s16                                // 000000000258: 
0xD3D94002, 0x18000117, // v_accvgpr_write_b32 a2, v23                                    // 000000000260: 
0xD3D84017, 0x18000107, // v_accvgpr_read_b32 v23, a7                                     // 000000000268: 
0x682A20A0,             // v_add_u32_e32 v21, 32, v16                                     // 000000000270: 
0x682C20B0,             // v_add_u32_e32 v22, 48, v16                                     // 000000000274: 
0xD3D94003, 0x18000117, // v_accvgpr_write_b32 a3, v23                                    // 000000000278: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0) <BB0_1>                                   // 000000000280: 
0xBF8A0000,             // s_barrier                                                      // 000000000284: 
0xD9FE0000, 0x18000011, // ds_read_b128 v[24:27], v17                                     // 000000000288: 
0xD8700100, 0x24000010, // ds_read2st64_b32 v[36:37], v16 offset1:1                       // 000000000290: 
0xD86EC484, 0x26000010, // ds_read2_b32 v[38:39], v16 offset0:132 offset1:196             // 000000000298: 
0xD8700504, 0x28000015, // ds_read2st64_b32 v[40:41], v21 offset0:4 offset1:5             // 0000000002A0: 
0xD8700706, 0x2A000016, // ds_read2st64_b32 v[42:43], v22 offset0:6 offset1:7             // 0000000002A8: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000002B0: 
0xBF8A0000,             // s_barrier                                                      // 0000000002B4: 
0xBF8C0F74,             // s_waitcnt vmcnt(4)                                             // 0000000002B8: 
0xD81A0000, 0x0000120D, // ds_write_b32 v13, v18                                          // 0000000002BC: 
0xE0501000, 0x80011214, // buffer_load_dword v18, v20, s[4:7], 0 offen                    // 0000000002C4: 
0xBF8C0F73,             // s_waitcnt vmcnt(3)                                             // 0000000002CC: 
0xD2A0001C, 0x00021106, // v_pack_b32_f16 v28, v6, v8                                     // 0000000002D0: 
0xD2A0181D, 0x00021106, // v_pack_b32_f16 v29, v6, v8 op_sel:[1,1,0]                      // 0000000002D8: 
0xD2A0001E, 0x00021307, // v_pack_b32_f16 v30, v7, v9                                     // 0000000002E0: 
0xD2A0181F, 0x00021307, // v_pack_b32_f16 v31, v7, v9 op_sel:[1,1,0]                      // 0000000002E8: 
0xBF8CC37F,             // s_waitcnt lgkmcnt(3)                                           // 0000000002F0: 
0x7E0E0326,             // v_mov_b32_e32 v7, v38                                          // 0000000002F4: 
0x7E0C0324,             // v_mov_b32_e32 v6, v36                                          // 0000000002F8: 
0xBF8C0F71,             // s_waitcnt vmcnt(1)                                             // 0000000002FC: 
0xD2A00020, 0x00020902, // v_pack_b32_f16 v32, v2, v4                                     // 000000000300: 
0xD2A01821, 0x00020902, // v_pack_b32_f16 v33, v2, v4 op_sel:[1,1,0]                      // 000000000308: 
0x6804260C,             // v_add_u32_e32 v2, s12, v19                                     // 000000000310: 
0xD2A00022, 0x00020B03, // v_pack_b32_f16 v34, v3, v5                                     // 000000000314: 
0xD2A01823, 0x00020B03, // v_pack_b32_f16 v35, v3, v5 op_sel:[1,1,0]                      // 00000000031C: 
0x7E4C0325,             // v_mov_b32_e32 v38, v37                                         // 000000000324: 
0xD3CD8004, 0x04120D18, // v_mfma_f32_16x16x16f16 a[4:7], v[24:25], v[6:7], a[4:7]        // 000000000328: 
0x810C030C,             // s_add_i32 s12, s12, s3                                         // 000000000330: 
0x680804FF, 0x000A0000, // v_add_u32_e32 v4, 0xa0000, v2                                  // 000000000334: 
0x680A04FF, 0x000A2000, // v_add_u32_e32 v5, 0xa2000, v2                                  // 00000000033C: 
0x682E04FF, 0x000C8000, // v_add_u32_e32 v23, 0xc8000, v2                                 // 000000000344: 
0xBF07FF0C, 0x00BE0000, // s_cmp_lg_u32 s12, 0xbe0000                                     // 00000000034C: 
0x682828C0,             // v_add_u32_e32 v20, 64, v20                                     // 000000000354: 
0xD3CD8000, 0x04024D18, // v_mfma_f32_16x16x16f16 a[0:3], v[24:25], v[38:39], a[0:3]      // 000000000358: 
0x683004FF, 0x000CA000, // v_add_u32_e32 v24, 0xca000, v2                                 // 000000000360: 
0xE0541000, 0x80020604, // buffer_load_dwordx2 v[6:7], v4, s[8:11], 0 offen               // 000000000368: 
0xE0541800, 0x80020805, // buffer_load_dwordx2 v[8:9], v5, s[8:11], 0 offen offset:2048   // 000000000370: 
0xE0541000, 0x80020217, // buffer_load_dwordx2 v[2:3], v23, s[8:11], 0 offen              // 000000000378: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                           // 000000000380: 
0x7E0A032A,             // v_mov_b32_e32 v5, v42                                          // 000000000384: 
0x7E080328,             // v_mov_b32_e32 v4, v40                                          // 000000000388: 
0x7E540329,             // v_mov_b32_e32 v42, v41                                         // 00000000038C: 
0xD9BE0000, 0x00001C0E, // ds_write_b128 v14, v[28:31]                                    // 000000000390: 
0xD3CD8004, 0x0412091A, // v_mfma_f32_16x16x16f16 a[4:7], v[26:27], v[4:5], a[4:7]        // 000000000398: 
0xE0541800, 0x80020418, // buffer_load_dwordx2 v[4:5], v24, s[8:11], 0 offen offset:2048  // 0000000003A0: 
0xD9BE0000, 0x0000200F, // ds_write_b128 v15, v[32:35]                                    // 0000000003A8: 
0xD3CD8000, 0x0402551A, // v_mfma_f32_16x16x16f16 a[0:3], v[26:27], v[42:43], a[0:3]      // 0000000003B0: 
0xBF85FFB1,             // s_cbranch_scc1 BB0_1                                           // 0000000003B8: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000003BC: 
0xBF8A0000,             // s_barrier                                                      // 0000000003C0: 
0xD9FE0000, 0x14000011, // ds_read_b128 v[20:23], v17                                     // 0000000003C4: 
0xD8700100, 0x18000010, // ds_read2st64_b32 v[24:25], v16 offset1:1                       // 0000000003CC: 
0xD86EC484, 0x1C000010, // ds_read2_b32 v[28:29], v16 offset0:132 offset1:196             // 0000000003D4: 
0x682620A0,             // v_add_u32_e32 v19, 32, v16                                     // 0000000003DC: 
0x684420B0,             // v_add_u32_e32 v34, 48, v16                                     // 0000000003E0: 
0x24000082,             // v_lshlrev_b32_e32 v0, 2, v0                                    // 0000000003E4: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                           // 0000000003E8: 
0x7E340318,             // v_mov_b32_e32 v26, v24                                         // 0000000003EC: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000003F0: 
0x7E36031C,             // v_mov_b32_e32 v27, v28                                         // 0000000003F4: 
0x7E380319,             // v_mov_b32_e32 v28, v25                                         // 0000000003F8: 
0xBF800000,             // s_nop 0                                                        // 0000000003FC: 
0xD3CD8004, 0x04123514, // v_mfma_f32_16x16x16f16 a[4:7], v[20:21], v[26:27], a[4:7]      // 000000000400: 
0xD8700504, 0x1A000013, // ds_read2st64_b32 v[26:27], v19 offset0:4 offset1:5             // 000000000408: 
0xD8700706, 0x1E000022, // ds_read2st64_b32 v[30:31], v34 offset0:6 offset1:7             // 000000000410: 
0xB0031400,             // s_movk_i32 s3, 0x1400                                          // 000000000418: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 00000000041C: 
0xBF8A0000,             // s_barrier                                                      // 000000000420: 
0xBF8C0F74,             // s_waitcnt vmcnt(4)                                             // 000000000424: 
0xD81A0000, 0x0000120D, // ds_write_b32 v13, v18                                          // 000000000428: 
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                           // 000000000430: 
0x7E40031A,             // v_mov_b32_e32 v32, v26                                         // 000000000434: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                           // 000000000438: 
0x7E42031E,             // v_mov_b32_e32 v33, v30                                         // 00000000043C: 
0xBF8C0F72,             // s_waitcnt vmcnt(2)                                             // 000000000440: 
0xD2A00018, 0x00021106, // v_pack_b32_f16 v24, v6, v8                                     // 000000000444: 
0xD2A01819, 0x00021106, // v_pack_b32_f16 v25, v6, v8 op_sel:[1,1,0]                      // 00000000044C: 
0x7E3C031B,             // v_mov_b32_e32 v30, v27                                         // 000000000454: 
0xD2A0001A, 0x00021307, // v_pack_b32_f16 v26, v7, v9                                     // 000000000458: 
0xD2A0181B, 0x00021307, // v_pack_b32_f16 v27, v7, v9 op_sel:[1,1,0]                      // 000000000460: 
0xBF8C0F70,             // s_waitcnt vmcnt(0)                                             // 000000000468: 
0xD2A00006, 0x00020902, // v_pack_b32_f16 v6, v2, v4                                      // 00000000046C: 
0xD2A01807, 0x00020902, // v_pack_b32_f16 v7, v2, v4 op_sel:[1,1,0]                       // 000000000474: 
0xD2A00008, 0x00020B03, // v_pack_b32_f16 v8, v3, v5                                      // 00000000047C: 
0xD2A01809, 0x00020B03, // v_pack_b32_f16 v9, v3, v5 op_sel:[1,1,0]                       // 000000000484: 
0xD9BE0000, 0x0000180E, // ds_write_b128 v14, v[24:27]                                    // 00000000048C: 
0xD9BE0000, 0x0000060F, // ds_write_b128 v15, v[6:9]                                      // 000000000494: 
0xD3CD8004, 0x04124116, // v_mfma_f32_16x16x16f16 a[4:7], v[22:23], v[32:33], a[4:7]      // 00000000049C: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000004A4: 
0xBF8A0000,             // s_barrier                                                      // 0000000004A8: 
0xD9FE0000, 0x02000011, // ds_read_b128 v[2:5], v17                                       // 0000000004AC: 
0xD8700100, 0x06000010, // ds_read2st64_b32 v[6:7], v16 offset1:1                         // 0000000004B4: 
0xD86EC484, 0x08000010, // ds_read2_b32 v[8:9], v16 offset0:132 offset1:196               // 0000000004BC: 
0xD8700504, 0x10000013, // ds_read2st64_b32 v[16:17], v19 offset0:4 offset1:5             // 0000000004C4: 
0xD8700706, 0x12000022, // ds_read2st64_b32 v[18:19], v34 offset0:6 offset1:7             // 0000000004CC: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000004D4: 
0xBF8A0000,             // s_barrier                                                      // 0000000004D8: 
0xD3CD8000, 0x04023914, // v_mfma_f32_16x16x16f16 a[0:3], v[20:21], v[28:29], a[0:3]      // 0000000004DC: 
0xBF8CC37F,             // s_waitcnt lgkmcnt(3)                                           // 0000000004E4: 
0x7E1C0306,             // v_mov_b32_e32 v14, v6                                          // 0000000004E8: 
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                           // 0000000004EC: 
0x7E1E0308,             // v_mov_b32_e32 v15, v8                                          // 0000000004F0: 
0x7E100307,             // v_mov_b32_e32 v8, v7                                           // 0000000004F4: 
0x260C188C,             // v_and_b32_e32 v6, 12, v12                                      // 0000000004F8: 
0xD200000C, 0x04011106, // v_lshl_or_b32 v12, v6, 8, v0                                   // 0000000004FC: 
0x24000284,             // v_lshlrev_b32_e32 v0, 4, v1                                    // 000000000504: 
0xD3CD8004, 0x04121D02, // v_mfma_f32_16x16x16f16 a[4:7], v[2:3], v[14:15], a[4:7]        // 000000000508: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000510: 
0x7E1E0312,             // v_mov_b32_e32 v15, v18                                         // 000000000514: 
0x7E1C0310,             // v_mov_b32_e32 v14, v16                                         // 000000000518: 
0x7E240311,             // v_mov_b32_e32 v18, v17                                         // 00000000051C: 
0xD3CD8000, 0x04023D16, // v_mfma_f32_16x16x16f16 a[0:3], v[22:23], v[30:31], a[0:3]      // 000000000520: 
0xD3CD8004, 0x04121D04, // v_mfma_f32_16x16x16f16 a[4:7], v[4:5], v[14:15], a[4:7]        // 000000000528: 
0xD3CD8000, 0x04021102, // v_mfma_f32_16x16x16f16 a[0:3], v[2:3], v[8:9], a[0:3]          // 000000000530: 
0xD2000002, 0x0401110B, // v_lshl_or_b32 v2, v11, 8, v0                                   // 000000000538: 
0xBF800007,             // s_nop 7                                                        // 000000000540: 
0xBF800000,             // s_nop 0                                                        // 000000000544: 
0xDA1A0000, 0x0000040C, // ds_write_b32 v12, a4                                           // 000000000548: 
0xDA1A0100, 0x0000050C, // ds_write_b32 v12, a5 offset:256                                // 000000000550: 
0xDA1A0200, 0x0000060C, // ds_write_b32 v12, a6 offset:512                                // 000000000558: 
0xDA1A0300, 0x0000070C, // ds_write_b32 v12, a7 offset:768                                // 000000000560: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000568: 
0xBF8A0000,             // s_barrier                                                      // 00000000056C: 
0xD9FE0000, 0x06000002, // ds_read_b128 v[6:9], v2                                        // 000000000570: 
0xD3CD8000, 0x04022504, // v_mfma_f32_16x16x16f16 a[0:3], v[4:5], v[18:19], a[0:3]        // 000000000578: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000580: 
0x7E001506,             // v_cvt_f16_f32_e32 v0, v6                                       // 000000000584: 
0x7E061508,             // v_cvt_f16_f32_e32 v3, v8                                       // 000000000588: 
0x7E0C1509,             // v_cvt_f16_f32_e32 v6, v9                                       // 00000000058C: 
0x7E0E1507,             // v_cvt_f16_f32_e32 v7, v7                                       // 000000000590: 
0x24100282,             // v_lshlrev_b32_e32 v8, 2, v1                                    // 000000000594: 
0xD2A00001, 0x00020D03, // v_pack_b32_f16 v1, v3, v6                                      // 000000000598: 
0xD2850003, 0x0000070A, // v_mul_lo_u32 v3, v10, s3                                       // 0000000005A0: 
0x68060602,             // v_add_u32_e32 v3, s2, v3                                       // 0000000005A8: 
0xD2A00000, 0x00020F00, // v_pack_b32_f16 v0, v0, v7                                      // 0000000005AC: 
0xBE8300FF, 0x00020000, // s_mov_b32 s3, 0x20000                                          // 0000000005B4: 
0xBE8200FF, 0x00050000, // s_mov_b32 s2, 0x50000                                          // 0000000005BC: 
0xD1FE0004, 0x02061103, // v_add_lshl_u32 v4, v3, v8, 1                                   // 0000000005C4: 
0xE0741000, 0x80000004, // buffer_store_dwordx2 v[0:1], v4, s[0:3], 0 offen               // 0000000005CC: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000005D4: 
0xBF8A0000,             // s_barrier                                                      // 0000000005D8: 
0xDA1A0000, 0x0000000C, // ds_write_b32 v12, a0                                           // 0000000005DC: 
0xDA1A0100, 0x0000010C, // ds_write_b32 v12, a1 offset:256                                // 0000000005E4: 
0xDA1A0200, 0x0000020C, // ds_write_b32 v12, a2 offset:512                                // 0000000005EC: 
0xDA1A0300, 0x0000030C, // ds_write_b32 v12, a3 offset:768                                // 0000000005F4: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000005FC: 
0xBF8A0000,             // s_barrier                                                      // 000000000600: 
0xD9FE0000, 0x00000002, // ds_read_b128 v[0:3], v2                                        // 000000000604: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 00000000060C: 
0x7E001500,             // v_cvt_f16_f32_e32 v0, v0                                       // 000000000610: 
0x7E0A1501,             // v_cvt_f16_f32_e32 v5, v1                                       // 000000000614: 
0x7E041502,             // v_cvt_f16_f32_e32 v2, v2                                       // 000000000618: 
0x7E061503,             // v_cvt_f16_f32_e32 v3, v3                                       // 00000000061C: 
0xD2A00000, 0x00020B00, // v_pack_b32_f16 v0, v0, v5                                      // 000000000620: 
0xD2A00001, 0x00020702, // v_pack_b32_f16 v1, v2, v3                                      // 000000000628: 
0xE0741080, 0x80000004, // buffer_store_dwordx2 v[0:1], v4, s[0:3], 0 offen offset:128    // 000000000630: 
0xBF810000              // s_endpgm                                                       // 000000000638: 
#endif
};

const uint32_t GEMM_ISA_B2B[] = {
// Original ISA:
// s4-s5 : kernarg pointer
// s6 : workgroup_id_x
// s7 : workgroup_id_y
// s8 : workgroup_id_z
//0xC00A0302, 0x00000000, // s_load_dwordx4 s[12:15], s[4:5], 0x0                           // 000000000000:
//0xC00A0002, 0x00000010, // s_load_dwordx4 s[0:3], s[4:5], 0x10                            // 000000000008:
0x20060085,             // v_lshrrev_b32_e32 v3, 5, v0                                    // 000000000010:
0x24080082,             // v_lshlrev_b32_e32 v4, 2, v0                                    // 000000000014:
//0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000018:

// Hacked ISA
// s0-s1: matrix C pointer
// s2-s3: matrix A pointer
// s4-s5: matrix B pointer
// s6 : workgroup_id_x
// s7 : workgroup_id_y
// s8 : workgroup_id_z
0xBE8C0002,             //     s_mov_b32 s12, s2
0xBE8D0003,             //     s_mov_b32 s13, s3
0xBE8E0004,             //     s_mov_b32 s14, s4
0xBE8F0005,             //     s_mov_b32 s15, s5
0xBF800000,             // s_nop 0

0x8E038407,             // s_lshl_b32 s3, s7, 4                                           // 00000000001C:
0x20160084,             // v_lshrrev_b32_e32 v11, 4, v0                                   // 000000000020:
0x261808FF, 0x0000007C, // v_and_b32_e32 v12, 0x7c, v4                                    // 000000000024:
0x100A06FF, 0x00000900, // v_mul_u32_u24_e32 v5, 0x900, v3                                // 00000000002C:
0x8E028706,             // s_lshl_b32 s2, s6, 7                                           // 000000000034:
0x28261905,             // v_or_b32_e32 v19, v5, v12                                      // 000000000038:
0x2602008F,             // v_and_b32_e32 v1, 15, v0                                       // 00000000003C:
0x68141603,             // v_add_u32_e32 v10, s3, v11                                     // 000000000040:
0xB0031400,             // s_movk_i32 s3, 0x1400                                          // 000000000044:
0xD1FE0012, 0x02040513, // v_add_lshl_u32 v18, v19, s2, 1                                 // 000000000048:
0xD2850004, 0x0000070A, // v_mul_lo_u32 v4, v10, s3                                       // 000000000050:
0x24040281,             // v_lshlrev_b32_e32 v2, 1, v1                                    // 000000000058:
0xBE8700FF, 0x00020000, // s_mov_b32 s7, 0x20000                                          // 00000000005C:
0x28080504,             // v_or_b32_e32 v4, v4, v2                                        // 000000000064:
0xBE88000E,             // s_mov_b32 s8, s14                                              // 000000000068:
0xBE8A00FF, 0x01680000, // s_mov_b32 s10, 0x1680000                                       // 00000000006C:
0xBE89000F,             // s_mov_b32 s9, s15                                              // 000000000074:
0xBE8B0007,             // s_mov_b32 s11, s7                                              // 000000000078:
0x681A24FF, 0x00009000, // v_add_u32_e32 v13, 0x9000, v18                                 // 00000000007C:
0x24280881,             // v_lshlrev_b32_e32 v20, 1, v4                                   // 000000000084:
0xE0541000, 0x80020412, // buffer_load_dwordx2 v[4:5], v18, s[8:11], 0 offen              // 000000000088:
0xE0541900, 0x80020612, // buffer_load_dwordx2 v[6:7], v18, s[8:11], 0 offen offset:2304  // 000000000090:
0xE0541000, 0x8002080D, // buffer_load_dwordx2 v[8:9], v13, s[8:11], 0 offen              // 000000000098:
0xE0541900, 0x8002100D, // buffer_load_dwordx2 v[16:17], v13, s[8:11], 0 offen offset:2304// 0000000000A0:
0x241A0081,             // v_lshlrev_b32_e32 v13, 1, v0                                   // 0000000000A8:
0xD2010002, 0x0409410D, // v_and_or_b32 v2, v13, 32, v2                                   // 0000000000AC:
0xB00E0050,             // s_movk_i32 s14, 0x50                                           // 0000000000B4:
0x101A060E,             // v_mul_u32_u24_e32 v13, s14, v3                                 // 0000000000B8:
0x24040481,             // v_lshlrev_b32_e32 v2, 1, v2                                    // 0000000000BC:
0xB0030108,             // s_movk_i32 s3, 0x108                                           // 0000000000C0:
0xD1FD000D, 0x0409030D, // v_lshl_add_u32 v13, v13, 1, v2                                 // 0000000000C4:
0x10040603,             // v_mul_u32_u24_e32 v2, s3, v3                                   // 0000000000CC:
0x24181881,             // v_lshlrev_b32_e32 v12, 1, v12                                  // 0000000000D0:
0xD1FE0015, 0x0206050C, // v_add_lshl_u32 v21, v12, v2, 1                                 // 0000000000D4:
0x28040688,             // v_or_b32_e32 v2, 8, v3                                         // 0000000000DC:
0x10040403,             // v_mul_u32_u24_e32 v2, s3, v2                                   // 0000000000E0:
0xBE84000C,             // s_mov_b32 s4, s12                                              // 0000000000E4:
0xBE85000D,             // s_mov_b32 s5, s13                                              // 0000000000E8:
0xBE8300FF, 0x00012000, // s_mov_b32 s3, 0x12000                                          // 0000000000EC:
0xBE8600FF, 0x00050000, // s_mov_b32 s6, 0x50000                                          // 0000000000F4:
0xD1FE000C, 0x02061902, // v_add_lshl_u32 v12, v2, v12, 1                                 // 0000000000FC:
0x682C2403,             // v_add_u32_e32 v22, s3, v18                                     // 000000000104:
0x682E24FF, 0x0001B000, // v_add_u32_e32 v23, 0x1b000, v18                                // 000000000108:
0xE0501000, 0x80010214, // buffer_load_dword v2, v20, s[4:7], 0 offen                     // 000000000110:
0xE0501040, 0x80011214, // buffer_load_dword v18, v20, s[4:7], 0 offen offset:64          // 000000000118:
0xB00D0500,             // s_movk_i32 s13, 0x500                                          // 000000000120:
0x681E180D,             // v_add_u32_e32 v15, s13, v12                                    // 000000000124:
0xD3D94004, 0x18000080, // v_accvgpr_write_b32 a4, 0                                      // 000000000128:
0x7E0AA504,             // v_accvgpr_mov_b32 a5, a4                                       // 000000000130:
0x7E0CA504,             // v_accvgpr_mov_b32 a6, a4                                       // 000000000134:
0x7E0EA504,             // v_accvgpr_mov_b32 a7, a4                                       // 000000000138:
0xD3D94000, 0x18000080, // v_accvgpr_write_b32 a0, 0                                      // 00000000013C:
0x24262681,             // v_lshlrev_b32_e32 v19, 1, v19                                  // 000000000144:
0xBE8C0080,             // s_mov_b32 s12, 0                                               // 000000000148:
0x681C2A0D,             // v_add_u32_e32 v14, s13, v21                                    // 00000000014C:
0xD1FD0013, 0x044D0202, // v_lshl_add_u32 v19, s2, 1, v19                                 // 000000000150:
0x282828FF, 0x00000080, // v_or_b32_e32 v20, 0x80, v20                                    // 000000000158:
0xBF8C0F71,             // s_waitcnt vmcnt(1)                                             // 000000000160:
0xD81A0000, 0x0000020D, // ds_write_b32 v13, v2                                           // 000000000164:
0xD2A00002, 0x00020D04, // v_pack_b32_f16 v2, v4, v6                                      // 00000000016C:
0xD2A01803, 0x00020D04, // v_pack_b32_f16 v3, v4, v6 op_sel:[1,1,0]                       // 000000000174:
0xD2A00004, 0x00020F05, // v_pack_b32_f16 v4, v5, v7                                      // 00000000017C:
0xD2A01805, 0x00020F05, // v_pack_b32_f16 v5, v5, v7 op_sel:[1,1,0]                       // 000000000184:
0xD2A00006, 0x00022108, // v_pack_b32_f16 v6, v8, v16                                     // 00000000018C:
0xD2A01807, 0x00022108, // v_pack_b32_f16 v7, v8, v16 op_sel:[1,1,0]                      // 000000000194:
0xD2A00008, 0x00022309, // v_pack_b32_f16 v8, v9, v17                                     // 00000000019C:
0xD2A01809, 0x00022309, // v_pack_b32_f16 v9, v9, v17 op_sel:[1,1,0]                      // 0000000001A4:
0xD9BE0500, 0x00000215, // ds_write_b128 v21, v[2:5] offset:1280                          // 0000000001AC:
0xD9BE0500, 0x0000060C, // ds_write_b128 v12, v[6:9] offset:1280                          // 0000000001B4:
0xE0541000, 0x80020616, // buffer_load_dwordx2 v[6:7], v22, s[8:11], 0 offen              // 0000000001BC:
0xE0541900, 0x80020816, // buffer_load_dwordx2 v[8:9], v22, s[8:11], 0 offen offset:2304  // 0000000001C4:
0xE0541000, 0x80020217, // buffer_load_dwordx2 v[2:3], v23, s[8:11], 0 offen              // 0000000001CC:
0xE0541900, 0x80020417, // buffer_load_dwordx2 v[4:5], v23, s[8:11], 0 offen offset:2304  // 0000000001D4:
0x24220085,             // v_lshlrev_b32_e32 v17, 5, v0                                   // 0000000001DC:
0xD1C8000C, 0x020D0300, // v_bfe_u32 v12, v0, 1, 3                                        // 0000000001E0:
0xD1C80010, 0x02090900, // v_bfe_u32 v16, v0, 4, 2                                        // 0000000001E8:
0x262222A0,             // v_and_b32_e32 v17, 32, v17                                     // 0000000001F0:
0xD2000011, 0x04450710, // v_lshl_or_b32 v17, v16, 3, v17                                 // 0000000001F4:
0x1018180E,             // v_mul_u32_u24_e32 v12, s14, v12                                // 0000000001FC:
0xD1FE0011, 0x02061911, // v_add_lshl_u32 v17, v17, v12, 1                                // 000000000200:
0x20180082,             // v_lshrrev_b32_e32 v12, 2, v0                                   // 000000000208:
0xD3D84017, 0x18000105, // v_accvgpr_read_b32 v23, a5                                     // 00000000020C:
0xD2010000, 0x0405610C, // v_and_or_b32 v0, v12, 48, v1                                   // 000000000214:
0x102020FF, 0x00000420, // v_mul_u32_u24_e32 v16, 0x420, v16                              // 00000000021C:
0xD1FD0010, 0x04410300, // v_lshl_add_u32 v16, v0, 1, v16                                 // 000000000224:
0xD3D94001, 0x18000117, // v_accvgpr_write_b32 a1, v23                                    // 00000000022C:
0xD3D84017, 0x18000106, // v_accvgpr_read_b32 v23, a6                                     // 000000000234:
0xD1FD0010, 0x00350310, // v_lshl_add_u32 v16, v16, 1, s13                                // 00000000023C:
0xD3D94002, 0x18000117, // v_accvgpr_write_b32 a2, v23                                    // 000000000244:
0xD3D84017, 0x18000107, // v_accvgpr_read_b32 v23, a7                                     // 00000000024C:
0x682A20A0,             // v_add_u32_e32 v21, 32, v16                                     // 000000000254:
0x682C20B0,             // v_add_u32_e32 v22, 48, v16                                     // 000000000258:
0xD3D94003, 0x18000117, // v_accvgpr_write_b32 a3, v23                                    // 00000000025C:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0) 264 <BB0_1>                               // 000000000264:
0xBF8A0000,             // s_barrier                                                      // 000000000268:
0xD9FE0000, 0x18000011, // ds_read_b128 v[24:27], v17                                     // 00000000026C:
0xD8700100, 0x24000010, // ds_read2st64_b32 v[36:37], v16 offset1:1                       // 000000000274:
0xD86EC484, 0x26000010, // ds_read2_b32 v[38:39], v16 offset0:132 offset1:196             // 00000000027C:
0xD8700504, 0x28000015, // ds_read2st64_b32 v[40:41], v21 offset0:4 offset1:5             // 000000000284:
0xD8700706, 0x2A000016, // ds_read2st64_b32 v[42:43], v22 offset0:6 offset1:7             // 00000000028C:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000294:
0xBF8A0000,             // s_barrier                                                      // 000000000298:
0xBF8CC37F,             // s_waitcnt lgkmcnt(3)                                           // 00000000029C:
0x7E400324,             // v_mov_b32_e32 v32, v36                                         // 0000000002A0:
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                           // 0000000002A4:
0x7E420326,             // v_mov_b32_e32 v33, v38                                         // 0000000002A8:
0xBF8C0F74,             // s_waitcnt vmcnt(4)                                             // 0000000002AC:
0xD81A0000, 0x0000120D, // ds_write_b32 v13, v18                                          // 0000000002B0:
0xE0501000, 0x80011214, // buffer_load_dword v18, v20, s[4:7], 0 offen                    // 0000000002B8:
0xD3CD8004, 0x04124118, // v_mfma_f32_16x16x16f16 a[4:7], v[24:25], v[32:33], a[4:7]      // 0000000002C0:
0xBF8C0F73,             // s_waitcnt vmcnt(3)                                             // 0000000002C8:
0xD2A0001C, 0x00021106, // v_pack_b32_f16 v28, v6, v8                                     // 0000000002CC:
0xD2A0181D, 0x00021106, // v_pack_b32_f16 v29, v6, v8 op_sel:[1,1,0]                      // 0000000002D4:
0xD2A0001E, 0x00021307, // v_pack_b32_f16 v30, v7, v9                                     // 0000000002DC:
0xD2A0181F, 0x00021307, // v_pack_b32_f16 v31, v7, v9 op_sel:[1,1,0]                      // 0000000002E4:
0xBF8C0F71,             // s_waitcnt vmcnt(1)                                             // 0000000002EC:
0xD2A00020, 0x00020902, // v_pack_b32_f16 v32, v2, v4                                     // 0000000002F0:
0xD2A01821, 0x00020902, // v_pack_b32_f16 v33, v2, v4 op_sel:[1,1,0]                      // 0000000002F8:
0x6804260C,             // v_add_u32_e32 v2, s12, v19                                     // 000000000300:
0x680804FF, 0x00024000, // v_add_u32_e32 v4, 0x24000, v2                                  // 000000000304:
0x810C030C,             // s_add_i32 s12, s12, s3                                         // 00000000030C:
0xD2A00022, 0x00020B03, // v_pack_b32_f16 v34, v3, v5                                     // 000000000310:
0xD2A01823, 0x00020B03, // v_pack_b32_f16 v35, v3, v5 op_sel:[1,1,0]                      // 000000000318:
0x682E04FF, 0x0002D000, // v_add_u32_e32 v23, 0x2d000, v2                                 // 000000000320:
0xE0541000, 0x80020604, // buffer_load_dwordx2 v[6:7], v4, s[8:11], 0 offen               // 000000000328:
0xE0541900, 0x80020804, // buffer_load_dwordx2 v[8:9], v4, s[8:11], 0 offen offset:2304   // 000000000330:
0xE0541000, 0x80020217, // buffer_load_dwordx2 v[2:3], v23, s[8:11], 0 offen              // 000000000338:
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                           // 000000000340:
0x7E0A032A,             // v_mov_b32_e32 v5, v42                                          // 000000000344:
0x7E080328,             // v_mov_b32_e32 v4, v40                                          // 000000000348:
0xBF07FF0C, 0x00B1C000, // s_cmp_lg_u32 s12, 0xb1c000                                     // 00000000034C:
0x7E4C0325,             // v_mov_b32_e32 v38, v37                                         // 000000000354:
0xD3CD8004, 0x0412091A, // v_mfma_f32_16x16x16f16 a[4:7], v[26:27], v[4:5], a[4:7]        // 000000000358:
0xE0541900, 0x80020417, // buffer_load_dwordx2 v[4:5], v23, s[8:11], 0 offen offset:2304  // 000000000360:
0x7E540329,             // v_mov_b32_e32 v42, v41                                         // 000000000368:
0x682828C0,             // v_add_u32_e32 v20, 64, v20                                     // 00000000036C:
0xD9BE0000, 0x00001C0E, // ds_write_b128 v14, v[28:31]                                    // 000000000370:
0xD9BE0000, 0x0000200F, // ds_write_b128 v15, v[32:35]                                    // 000000000378:
0xD3CD8000, 0x04024D18, // v_mfma_f32_16x16x16f16 a[0:3], v[24:25], v[38:39], a[0:3]      // 000000000380:
0xD3CD8000, 0x0402551A, // v_mfma_f32_16x16x16f16 a[0:3], v[26:27], v[42:43], a[0:3]      // 000000000388:
0xBF85FFB4,             // s_cbranch_scc1 BB0_1                                           // 000000000390:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000394:
0xBF8A0000,             // s_barrier                                                      // 000000000398:
0xD9FE0000, 0x14000011, // ds_read_b128 v[20:23], v17                                     // 00000000039C:
0xD8700100, 0x18000010, // ds_read2st64_b32 v[24:25], v16 offset1:1                       // 0000000003A4:
0xD86EC484, 0x1C000010, // ds_read2_b32 v[28:29], v16 offset0:132 offset1:196             // 0000000003AC:
0x682620A0,             // v_add_u32_e32 v19, 32, v16                                     // 0000000003B4:
0x684420B0,             // v_add_u32_e32 v34, 48, v16                                     // 0000000003B8:
0x24000082,             // v_lshlrev_b32_e32 v0, 2, v0                                    // 0000000003BC:
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                           // 0000000003C0:
0x7E340318,             // v_mov_b32_e32 v26, v24                                         // 0000000003C4:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000003C8:
0x7E36031C,             // v_mov_b32_e32 v27, v28                                         // 0000000003CC:
0x7E380319,             // v_mov_b32_e32 v28, v25                                         // 0000000003D0:
0xBF800000,             // s_nop 0                                                        // 0000000003D4:
0xD3CD8004, 0x04123514, // v_mfma_f32_16x16x16f16 a[4:7], v[20:21], v[26:27], a[4:7]      // 0000000003D8:
0xD8700504, 0x1A000013, // ds_read2st64_b32 v[26:27], v19 offset0:4 offset1:5             // 0000000003E0:
0xD8700706, 0x1E000022, // ds_read2st64_b32 v[30:31], v34 offset0:6 offset1:7             // 0000000003E8:
0xB0030480,             // s_movk_i32 s3, 0x480                                           // 0000000003F0:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000003F4:
0xBF8A0000,             // s_barrier                                                      // 0000000003F8:
0xBF8C0F74,             // s_waitcnt vmcnt(4)                                             // 0000000003FC:
0xD81A0000, 0x0000120D, // ds_write_b32 v13, v18                                          // 000000000400:
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                           // 000000000408:
0x7E40031A,             // v_mov_b32_e32 v32, v26                                         // 00000000040C:
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                           // 000000000410:
0x7E42031E,             // v_mov_b32_e32 v33, v30                                         // 000000000414:
0xBF8C0F72,             // s_waitcnt vmcnt(2)                                             // 000000000418:
0xD2A00018, 0x00021106, // v_pack_b32_f16 v24, v6, v8                                     // 00000000041C:
0xD2A01819, 0x00021106, // v_pack_b32_f16 v25, v6, v8 op_sel:[1,1,0]                      // 000000000424:
0x7E3C031B,             // v_mov_b32_e32 v30, v27                                         // 00000000042C:
0xD2A0001A, 0x00021307, // v_pack_b32_f16 v26, v7, v9                                     // 000000000430:
0xD2A0181B, 0x00021307, // v_pack_b32_f16 v27, v7, v9 op_sel:[1,1,0]                      // 000000000438:
0xBF8C0F70,             // s_waitcnt vmcnt(0)                                             // 000000000440:
0xD2A00006, 0x00020902, // v_pack_b32_f16 v6, v2, v4                                      // 000000000444:
0xD2A01807, 0x00020902, // v_pack_b32_f16 v7, v2, v4 op_sel:[1,1,0]                       // 00000000044C:
0xD2A00008, 0x00020B03, // v_pack_b32_f16 v8, v3, v5                                      // 000000000454:
0xD2A01809, 0x00020B03, // v_pack_b32_f16 v9, v3, v5 op_sel:[1,1,0]                       // 00000000045C:
0xD9BE0000, 0x0000180E, // ds_write_b128 v14, v[24:27]                                    // 000000000464:
0xD9BE0000, 0x0000060F, // ds_write_b128 v15, v[6:9]                                      // 00000000046C:
0xD3CD8004, 0x04124116, // v_mfma_f32_16x16x16f16 a[4:7], v[22:23], v[32:33], a[4:7]      // 000000000474:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 00000000047C:
0xBF8A0000,             // s_barrier                                                      // 000000000480:
0xD9FE0000, 0x02000011, // ds_read_b128 v[2:5], v17                                       // 000000000484:
0xD8700100, 0x06000010, // ds_read2st64_b32 v[6:7], v16 offset1:1                         // 00000000048C:
0xD86EC484, 0x08000010, // ds_read2_b32 v[8:9], v16 offset0:132 offset1:196               // 000000000494:
0xD8700504, 0x10000013, // ds_read2st64_b32 v[16:17], v19 offset0:4 offset1:5             // 00000000049C:
0xD8700706, 0x12000022, // ds_read2st64_b32 v[18:19], v34 offset0:6 offset1:7             // 0000000004A4:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000004AC:
0xBF8A0000,             // s_barrier                                                      // 0000000004B0:
0xD3CD8000, 0x04023914, // v_mfma_f32_16x16x16f16 a[0:3], v[20:21], v[28:29], a[0:3]      // 0000000004B4:
0xBF8CC37F,             // s_waitcnt lgkmcnt(3)                                           // 0000000004BC:
0x7E1C0306,             // v_mov_b32_e32 v14, v6                                          // 0000000004C0:
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                           // 0000000004C4:
0x7E1E0308,             // v_mov_b32_e32 v15, v8                                          // 0000000004C8:
0x7E100307,             // v_mov_b32_e32 v8, v7                                           // 0000000004CC:
0x260C188C,             // v_and_b32_e32 v6, 12, v12                                      // 0000000004D0:
0xD200000C, 0x04011106, // v_lshl_or_b32 v12, v6, 8, v0                                   // 0000000004D4:
0x24000284,             // v_lshlrev_b32_e32 v0, 4, v1                                    // 0000000004DC:
0xD3CD8004, 0x04121D02, // v_mfma_f32_16x16x16f16 a[4:7], v[2:3], v[14:15], a[4:7]        // 0000000004E0:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000004E8:
0x7E1E0312,             // v_mov_b32_e32 v15, v18                                         // 0000000004EC:
0x7E1C0310,             // v_mov_b32_e32 v14, v16                                         // 0000000004F0:
0x7E240311,             // v_mov_b32_e32 v18, v17                                         // 0000000004F4:
0xD3CD8000, 0x04023D16, // v_mfma_f32_16x16x16f16 a[0:3], v[22:23], v[30:31], a[0:3]      // 0000000004F8:
0xD3CD8004, 0x04121D04, // v_mfma_f32_16x16x16f16 a[4:7], v[4:5], v[14:15], a[4:7]        // 000000000500:
0xD3CD8000, 0x04021102, // v_mfma_f32_16x16x16f16 a[0:3], v[2:3], v[8:9], a[0:3]          // 000000000508:
0xD2000002, 0x0401110B, // v_lshl_or_b32 v2, v11, 8, v0                                   // 000000000510:
0xBF800007,             // s_nop 7                                                        // 000000000518:
0xBF800000,             // s_nop 0                                                        // 00000000051C:
0xDA1A0000, 0x0000040C, // ds_write_b32 v12, a4                                           // 000000000520:
0xDA1A0100, 0x0000050C, // ds_write_b32 v12, a5 offset:256                                // 000000000528:
0xDA1A0200, 0x0000060C, // ds_write_b32 v12, a6 offset:512                                // 000000000530:
0xDA1A0300, 0x0000070C, // ds_write_b32 v12, a7 offset:768                                // 000000000538:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000540:
0xBF8A0000,             // s_barrier                                                      // 000000000544:
0xD9FE0000, 0x06000002, // ds_read_b128 v[6:9], v2                                        // 000000000548:
0xD3CD8000, 0x04022504, // v_mfma_f32_16x16x16f16 a[0:3], v[4:5], v[18:19], a[0:3]        // 000000000550:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000558:
0x7E001506,             // v_cvt_f16_f32_e32 v0, v6                                       // 00000000055C:
0x7E061508,             // v_cvt_f16_f32_e32 v3, v8                                       // 000000000560:
0x7E0C1509,             // v_cvt_f16_f32_e32 v6, v9                                       // 000000000564:
0x7E0E1507,             // v_cvt_f16_f32_e32 v7, v7                                       // 000000000568:
0x24100282,             // v_lshlrev_b32_e32 v8, 2, v1                                    // 00000000056C:
0xD2A00001, 0x00020D03, // v_pack_b32_f16 v1, v3, v6                                      // 000000000570:
0xD2850003, 0x0000070A, // v_mul_lo_u32 v3, v10, s3                                       // 000000000578:
0x68060602,             // v_add_u32_e32 v3, s2, v3                                       // 000000000580:
0xD2A00000, 0x00020F00, // v_pack_b32_f16 v0, v0, v7                                      // 000000000584:
0xBE8300FF, 0x00020000, // s_mov_b32 s3, 0x20000                                          // 00000000058C:
0xBE8200FF, 0x00012000, // s_mov_b32 s2, 0x12000                                          // 000000000594:
0xD1FE0004, 0x02061103, // v_add_lshl_u32 v4, v3, v8, 1                                   // 00000000059C:
0xE0741000, 0x80000004, // buffer_store_dwordx2 v[0:1], v4, s[0:3], 0 offen               // 0000000005A4:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000005AC:
0xBF8A0000,             // s_barrier                                                      // 0000000005B0:
0xDA1A0000, 0x0000000C, // ds_write_b32 v12, a0                                           // 0000000005B4:
0xDA1A0100, 0x0000010C, // ds_write_b32 v12, a1 offset:256                                // 0000000005BC:
0xDA1A0200, 0x0000020C, // ds_write_b32 v12, a2 offset:512                                // 0000000005C4:
0xDA1A0300, 0x0000030C, // ds_write_b32 v12, a3 offset:768                                // 0000000005CC:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000005D4:
0xBF8A0000,             // s_barrier                                                      // 0000000005D8:
0xD9FE0000, 0x00000002, // ds_read_b128 v[0:3], v2                                        // 0000000005DC:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000005E4:
0x7E001500,             // v_cvt_f16_f32_e32 v0, v0                                       // 0000000005E8:
0x7E0A1501,             // v_cvt_f16_f32_e32 v5, v1                                       // 0000000005EC:
0x7E041502,             // v_cvt_f16_f32_e32 v2, v2                                       // 0000000005F0:
0x7E061503,             // v_cvt_f16_f32_e32 v3, v3                                       // 0000000005F4:
0xD2A00000, 0x00020B00, // v_pack_b32_f16 v0, v0, v5                                      // 0000000005F8:
0xD2A00001, 0x00020702, // v_pack_b32_f16 v1, v2, v3                                      // 000000000600:
0xE0741080, 0x80000004, // buffer_store_dwordx2 v[0:1], v4, s[0:3], 0 offen offset:128    // 000000000608:
0xBF810000,             // s_endpgm                                                       // 000000000610:

0xBF800000,             // s_nop 0                                                        // 000000000614:
0xBF800000,             // s_nop 0                                                        // 000000000618:
0xBF800000,             // s_nop 0                                                        // 00000000061C:
0xBF800000,             // s_nop 0                                                        // 000000000620:
0xBF800000,             // s_nop 0                                                        // 000000000624:
0xBF800000,             // s_nop 0                                                        // 000000000628:
0xBF800000,             // s_nop 0                                                        // 00000000062C:
0xBF800000,             // s_nop 0                                                        // 000000000630:
0xBF800000,             // s_nop 0                                                        // 000000000634:
0xBF800000,             // s_nop 0                                                        // 000000000638:
0xBF800000,             // s_nop 0                                                        // 00000000063C:
0xBF800000,             // s_nop 0                                                        // 000000000640:
0xBF800000,             // s_nop 0                                                        // 000000000644:
0xBF800000,             // s_nop 0                                                        // 000000000648:
0xBF800000,             // s_nop 0                                                        // 00000000064C:
0xBF800000,             // s_nop 0                                                        // 000000000650:
0xBF800000,             // s_nop 0                                                        // 000000000654:
0xBF800000,             // s_nop 0                                                        // 000000000658:
0xBF800000,             // s_nop 0                                                        // 00000000065C:
0xBF800000,             // s_nop 0                                                        // 000000000660:
0xBF800000,             // s_nop 0                                                        // 000000000664:
0xBF800000,             // s_nop 0                                                        // 000000000668:
0xBF800000,             // s_nop 0                                                        // 00000000066C:
0xBF800000,             // s_nop 0                                                        // 000000000670:
0xBF800000,             // s_nop 0                                                        // 000000000674:
0xBF800000,             // s_nop 0                                                        // 000000000678:
0xBF800000,             // s_nop 0                                                        // 00000000067C:
0xBF800000,             // s_nop 0                                                        // 000000000680:
0xBF800000,             // s_nop 0                                                        // 000000000684:
0xBF800000,             // s_nop 0                                                        // 000000000688:
0xBF800000,             // s_nop 0                                                        // 00000000068C:
0xBF800000,             // s_nop 0                                                        // 000000000690:
0xBF800000,             // s_nop 0                                                        // 000000000694:
0xBF800000,             // s_nop 0                                                        // 000000000698:
0xBF800000,             // s_nop 0                                                        // 00000000069C:
0xBF800000,             // s_nop 0                                                        // 0000000006A0:
0xBF800000,             // s_nop 0                                                        // 0000000006A4:
0xBF800000,             // s_nop 0                                                        // 0000000006A8:
0xBF800000,             // s_nop 0                                                        // 0000000006AC:
0xBF800000,             // s_nop 0                                                        // 0000000006B0:
0xBF800000,             // s_nop 0                                                        // 0000000006B4:
0xBF800000,             // s_nop 0                                                        // 0000000006B8:
0xBF800000,             // s_nop 0                                                        // 0000000006BC:
0xBF800000,             // s_nop 0                                                        // 0000000006C0:
0xBF800000,             // s_nop 0                                                        // 0000000006C4:
0xBF800000,             // s_nop 0                                                        // 0000000006C8:
0xBF800000,             // s_nop 0                                                        // 0000000006CC:
0xBF800000,             // s_nop 0                                                        // 0000000006D0:
0xBF800000,             // s_nop 0                                                        // 0000000006D4:
0xBF800000,             // s_nop 0                                                        // 0000000006D8:
0xBF800000,             // s_nop 0                                                        // 0000000006DC:
0xBF800000,             // s_nop 0                                                        // 0000000006E0:
0xBF800000,             // s_nop 0                                                        // 0000000006E4:
0xBF800000,             // s_nop 0                                                        // 0000000006E8:
0xBF800000,             // s_nop 0                                                        // 0000000006EC:
0xBF800000,             // s_nop 0                                                        // 0000000006F0:
0xBF800000,             // s_nop 0                                                        // 0000000006F4:
0xBF800000,             // s_nop 0                                                        // 0000000006F8:
0xBF800000,             // s_nop 0                                                        // 0000000006FC:

// 0x700

// 16_5120_384 kernel begins from 0x700
// Original ISA:
// s4-s5 : kernarg pointer
// s6 : workgroup_id_x
// s7 : workgroup_id_y
// s8 : workgroup_id_z
//0xC00A0302, 0x00000000, // s_load_dwordx4 s[12:15], s[4:5], 0x0                           // 000000000000:
//0xC00A0002, 0x00000010, // s_load_dwordx4 s[0:3], s[4:5], 0x10                            // 000000000008:
0x20060085,             // v_lshrrev_b32_e32 v3, 5, v0                                    // 000000000010:
0x24080082,             // v_lshlrev_b32_e32 v4, 2, v0                                    // 000000000014:
//0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000018:

// Hacked ISA
// s0-s1: matrix C pointer
// s2-s3: matrix A pointer
// s4-s5: matrix B pointer
// s6 : workgroup_id_x
// s7 : workgroup_id_y
// s8 : workgroup_id_z
0xBE8C0002,             //     s_mov_b32 s12, s2
0xBE8D0003,             //     s_mov_b32 s13, s3
0xBE8E0004,             //     s_mov_b32 s14, s4
0xBE8F0005,             //     s_mov_b32 s15, s5
0xBF800000,             // s_nop 0

0x8E038407,             // s_lshl_b32 s3, s7, 4                                           // 00000000001C:
0x20160084,             // v_lshrrev_b32_e32 v11, 4, v0                                   // 000000000020:
0x261A08FF, 0x0000007C, // v_and_b32_e32 v13, 0x7c, v4                                    // 000000000024:
0x100A06FF, 0x00002800, // v_mul_u32_u24_e32 v5, 0x2800, v3                               // 00000000002C:
0x8E028706,             // s_lshl_b32 s2, s6, 7                                           // 000000000034:
0x28261B05,             // v_or_b32_e32 v19, v5, v13                                      // 000000000038:
0x2602008F,             // v_and_b32_e32 v1, 15, v0                                       // 00000000003C:
0x68141603,             // v_add_u32_e32 v10, s3, v11                                     // 000000000040:
0xB0030180,             // s_movk_i32 s3, 0x180                                           // 000000000044:
0xD1FE0012, 0x02040513, // v_add_lshl_u32 v18, v19, s2, 1                                 // 000000000048:
0xD2850004, 0x0000070A, // v_mul_lo_u32 v4, v10, s3                                       // 000000000050:
0x24040281,             // v_lshlrev_b32_e32 v2, 1, v1                                    // 000000000058:
0xBE8700FF, 0x00020000, // s_mov_b32 s7, 0x20000                                          // 00000000005C:
0x28080504,             // v_or_b32_e32 v4, v4, v2                                        // 000000000064:
0xBE88000E,             // s_mov_b32 s8, s14                                              // 000000000068:
0xBE8A00FF, 0x00780000, // s_mov_b32 s10, 0x780000                                        // 00000000006C:
0xBE89000F,             // s_mov_b32 s9, s15                                              // 000000000074:
0xBE8B0007,             // s_mov_b32 s11, s7                                              // 000000000078:
0x681824FF, 0x00002000, // v_add_u32_e32 v12, 0x2000, v18                                 // 00000000007C:
0x24280881,             // v_lshlrev_b32_e32 v20, 1, v4                                   // 000000000084:
0x681C24FF, 0x00028000, // v_add_u32_e32 v14, 0x28000, v18                                // 000000000088:
0x681E24FF, 0x0002A000, // v_add_u32_e32 v15, 0x2a000, v18                                // 000000000090:
0xE0541000, 0x80020412, // buffer_load_dwordx2 v[4:5], v18, s[8:11], 0 offen              // 000000000098:
0xE0541800, 0x8002060C, // buffer_load_dwordx2 v[6:7], v12, s[8:11], 0 offen offset:2048  // 0000000000A0:
0xE0541000, 0x8002080E, // buffer_load_dwordx2 v[8:9], v14, s[8:11], 0 offen              // 0000000000A8:
0xE0541800, 0x8002100F, // buffer_load_dwordx2 v[16:17], v15, s[8:11], 0 offen offset:2048// 0000000000B0:
0x24180081,             // v_lshlrev_b32_e32 v12, 1, v0                                   // 0000000000B8:
0xD2010002, 0x0409410C, // v_and_or_b32 v2, v12, 32, v2                                   // 0000000000BC:
0xB00E0050,             // s_movk_i32 s14, 0x50                                           // 0000000000C4:
0x1018060E,             // v_mul_u32_u24_e32 v12, s14, v3                                 // 0000000000C8:
0x24040481,             // v_lshlrev_b32_e32 v2, 1, v2                                    // 0000000000CC:
0xB0030108,             // s_movk_i32 s3, 0x108                                           // 0000000000D0:
0xD1FD000C, 0x0409030C, // v_lshl_add_u32 v12, v12, 1, v2                                 // 0000000000D4:
0x10040603,             // v_mul_u32_u24_e32 v2, s3, v3                                   // 0000000000DC:
0x241A1A81,             // v_lshlrev_b32_e32 v13, 1, v13                                  // 0000000000E0:
0xD1FE0015, 0x0206050D, // v_add_lshl_u32 v21, v13, v2, 1                                 // 0000000000E4:
0x28040688,             // v_or_b32_e32 v2, 8, v3                                         // 0000000000EC:
0x10040403,             // v_mul_u32_u24_e32 v2, s3, v2                                   // 0000000000F0:
0xBE84000C,             // s_mov_b32 s4, s12                                              // 0000000000F4:
0xBE85000D,             // s_mov_b32 s5, s13                                              // 0000000000F8:
0xBE8300FF, 0x00050000, // s_mov_b32 s3, 0x50000                                          // 0000000000FC:
0xB0066000,             // s_movk_i32 s6, 0x6000                                          // 000000000104:
0xD1FE000D, 0x02061B02, // v_add_lshl_u32 v13, v2, v13, 1                                 // 000000000108:
0x682C2403,             // v_add_u32_e32 v22, s3, v18                                     // 000000000110:
0x682E24FF, 0x00052000, // v_add_u32_e32 v23, 0x52000, v18                                // 000000000114:
0x683024FF, 0x00078000, // v_add_u32_e32 v24, 0x78000, v18                                // 00000000011C:
0x683224FF, 0x0007A000, // v_add_u32_e32 v25, 0x7a000, v18                                // 000000000124:
0xE0501000, 0x80010214, // buffer_load_dword v2, v20, s[4:7], 0 offen                     // 00000000012C:
0xE0501040, 0x80011214, // buffer_load_dword v18, v20, s[4:7], 0 offen offset:64          // 000000000134:
0xB00D0500,             // s_movk_i32 s13, 0x500                                          // 00000000013C:
0x681E1A0D,             // v_add_u32_e32 v15, s13, v13                                    // 000000000140:
0xD3D94004, 0x18000080, // v_accvgpr_write_b32 a4, 0                                      // 000000000144:
0x7E0AA504,             // v_accvgpr_mov_b32 a5, a4                                       // 00000000014C:
0x7E0CA504,             // v_accvgpr_mov_b32 a6, a4                                       // 000000000150:
0x7E0EA504,             // v_accvgpr_mov_b32 a7, a4                                       // 000000000154:
0xD3D94000, 0x18000080, // v_accvgpr_write_b32 a0, 0                                      // 000000000158:
0x24262681,             // v_lshlrev_b32_e32 v19, 1, v19                                  // 000000000160:
0xBE8C0080,             // s_mov_b32 s12, 0                                               // 000000000164:
0x681C2A0D,             // v_add_u32_e32 v14, s13, v21                                    // 000000000168:
0xD1FD0013, 0x044D0202, // v_lshl_add_u32 v19, s2, 1, v19                                 // 00000000016C:
0x282828FF, 0x00000080, // v_or_b32_e32 v20, 0x80, v20                                    // 000000000174:
0xBF8C0F71,             // s_waitcnt vmcnt(1)                                             // 00000000017C:
0xD81A0000, 0x0000020C, // ds_write_b32 v12, v2                                           // 000000000180:
0xD2A00002, 0x00020D04, // v_pack_b32_f16 v2, v4, v6                                      // 000000000188:
0xD2A01803, 0x00020D04, // v_pack_b32_f16 v3, v4, v6 op_sel:[1,1,0]                       // 000000000190:
0xD2A00004, 0x00020F05, // v_pack_b32_f16 v4, v5, v7                                      // 000000000198:
0xD2A01805, 0x00020F05, // v_pack_b32_f16 v5, v5, v7 op_sel:[1,1,0]                       // 0000000001A0:
0xD2A00006, 0x00022108, // v_pack_b32_f16 v6, v8, v16                                     // 0000000001A8:
0xD2A01807, 0x00022108, // v_pack_b32_f16 v7, v8, v16 op_sel:[1,1,0]                      // 0000000001B0:
0xD2A00008, 0x00022309, // v_pack_b32_f16 v8, v9, v17                                     // 0000000001B8:
0xD2A01809, 0x00022309, // v_pack_b32_f16 v9, v9, v17 op_sel:[1,1,0]                      // 0000000001C0:
0xD9BE0500, 0x00000215, // ds_write_b128 v21, v[2:5] offset:1280                          // 0000000001C8:
0xD9BE0500, 0x0000060D, // ds_write_b128 v13, v[6:9] offset:1280                          // 0000000001D0:
0xE0541000, 0x80020616, // buffer_load_dwordx2 v[6:7], v22, s[8:11], 0 offen              // 0000000001D8:
0xE0541800, 0x80020817, // buffer_load_dwordx2 v[8:9], v23, s[8:11], 0 offen offset:2048  // 0000000001E0:
0xE0541000, 0x80020218, // buffer_load_dwordx2 v[2:3], v24, s[8:11], 0 offen              // 0000000001E8:
0xE0541800, 0x80020419, // buffer_load_dwordx2 v[4:5], v25, s[8:11], 0 offen offset:2048  // 0000000001F0:
0x24220085,             // v_lshlrev_b32_e32 v17, 5, v0                                   // 0000000001F8:
0xD1C8000D, 0x020D0300, // v_bfe_u32 v13, v0, 1, 3                                        // 0000000001FC:
0xD1C80010, 0x02090900, // v_bfe_u32 v16, v0, 4, 2                                        // 000000000204:
0x262222A0,             // v_and_b32_e32 v17, 32, v17                                     // 00000000020C:
0xD2000011, 0x04450710, // v_lshl_or_b32 v17, v16, 3, v17                                 // 000000000210:
0x101A1A0E,             // v_mul_u32_u24_e32 v13, s14, v13                                // 000000000218:
0xD1FE0011, 0x02061B11, // v_add_lshl_u32 v17, v17, v13, 1                                // 00000000021C:
0x201A0082,             // v_lshrrev_b32_e32 v13, 2, v0                                   // 000000000224:
0xD3D84017, 0x18000105, // v_accvgpr_read_b32 v23, a5                                     // 000000000228:
0xD2010000, 0x0405610D, // v_and_or_b32 v0, v13, 48, v1                                   // 000000000230:
0x102020FF, 0x00000420, // v_mul_u32_u24_e32 v16, 0x420, v16                              // 000000000238:
0xD1FD0010, 0x04410300, // v_lshl_add_u32 v16, v0, 1, v16                                 // 000000000240:
0xD3D94001, 0x18000117, // v_accvgpr_write_b32 a1, v23                                    // 000000000248:
0xD3D84017, 0x18000106, // v_accvgpr_read_b32 v23, a6                                     // 000000000250:
0xD1FD0010, 0x00350310, // v_lshl_add_u32 v16, v16, 1, s13                                // 000000000258:
0xD3D94002, 0x18000117, // v_accvgpr_write_b32 a2, v23                                    // 000000000260:
0xD3D84017, 0x18000107, // v_accvgpr_read_b32 v23, a7                                     // 000000000268:
0x682A20A0,             // v_add_u32_e32 v21, 32, v16                                     // 000000000270:
0x682C20B0,             // v_add_u32_e32 v22, 48, v16                                     // 000000000274:
0xD3D94003, 0x18000117, // v_accvgpr_write_b32 a3, v23                                    // 000000000278:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0) <BB0_1>                                   // 000000000280:
0xBF8A0000,             // s_barrier                                                      // 000000000284:
0xD9FE0000, 0x18000011, // ds_read_b128 v[24:27], v17                                     // 000000000288:
0xD8700100, 0x24000010, // ds_read2st64_b32 v[36:37], v16 offset1:1                       // 000000000290:
0xD86EC484, 0x26000010, // ds_read2_b32 v[38:39], v16 offset0:132 offset1:196             // 000000000298:
0xD8700504, 0x28000015, // ds_read2st64_b32 v[40:41], v21 offset0:4 offset1:5             // 0000000002A0:
0xD8700706, 0x2A000016, // ds_read2st64_b32 v[42:43], v22 offset0:6 offset1:7             // 0000000002A8:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000002B0:
0xBF8A0000,             // s_barrier                                                      // 0000000002B4:
0xBF8C0F74,             // s_waitcnt vmcnt(4)                                             // 0000000002B8:
0xD81A0000, 0x0000120C, // ds_write_b32 v12, v18                                          // 0000000002BC:
0xE0501000, 0x80011214, // buffer_load_dword v18, v20, s[4:7], 0 offen                    // 0000000002C4:
0xBF8C0F73,             // s_waitcnt vmcnt(3)                                             // 0000000002CC:
0xD2A0001C, 0x00021106, // v_pack_b32_f16 v28, v6, v8                                     // 0000000002D0:
0xD2A0181D, 0x00021106, // v_pack_b32_f16 v29, v6, v8 op_sel:[1,1,0]                      // 0000000002D8:
0xD2A0001E, 0x00021307, // v_pack_b32_f16 v30, v7, v9                                     // 0000000002E0:
0xD2A0181F, 0x00021307, // v_pack_b32_f16 v31, v7, v9 op_sel:[1,1,0]                      // 0000000002E8:
0xBF8CC37F,             // s_waitcnt lgkmcnt(3)                                           // 0000000002F0:
0x7E0E0326,             // v_mov_b32_e32 v7, v38                                          // 0000000002F4:
0x7E0C0324,             // v_mov_b32_e32 v6, v36                                          // 0000000002F8:
0xBF8C0F71,             // s_waitcnt vmcnt(1)                                             // 0000000002FC:
0xD2A00020, 0x00020902, // v_pack_b32_f16 v32, v2, v4                                     // 000000000300:
0xD2A01821, 0x00020902, // v_pack_b32_f16 v33, v2, v4 op_sel:[1,1,0]                      // 000000000308:
0x6804260C,             // v_add_u32_e32 v2, s12, v19                                     // 000000000310:
0xD2A00022, 0x00020B03, // v_pack_b32_f16 v34, v3, v5                                     // 000000000314:
0xD2A01823, 0x00020B03, // v_pack_b32_f16 v35, v3, v5 op_sel:[1,1,0]                      // 00000000031C:
0x7E4C0325,             // v_mov_b32_e32 v38, v37                                         // 000000000324:
0xD3CD8004, 0x04120D18, // v_mfma_f32_16x16x16f16 a[4:7], v[24:25], v[6:7], a[4:7]        // 000000000328:
0x810C030C,             // s_add_i32 s12, s12, s3                                         // 000000000330:
0x680804FF, 0x000A0000, // v_add_u32_e32 v4, 0xa0000, v2                                  // 000000000334:
0x680A04FF, 0x000A2000, // v_add_u32_e32 v5, 0xa2000, v2                                  // 00000000033C:
0x682E04FF, 0x000C8000, // v_add_u32_e32 v23, 0xc8000, v2                                 // 000000000344:
0xBF07FF0C, 0x00320000, // s_cmp_lg_u32 s12, 0x320000                                     // 00000000034C:
0x682828C0,             // v_add_u32_e32 v20, 64, v20                                     // 000000000354:
0xD3CD8000, 0x04024D18, // v_mfma_f32_16x16x16f16 a[0:3], v[24:25], v[38:39], a[0:3]      // 000000000358:
0x683004FF, 0x000CA000, // v_add_u32_e32 v24, 0xca000, v2                                 // 000000000360:
0xE0541000, 0x80020604, // buffer_load_dwordx2 v[6:7], v4, s[8:11], 0 offen               // 000000000368:
0xE0541800, 0x80020805, // buffer_load_dwordx2 v[8:9], v5, s[8:11], 0 offen offset:2048   // 000000000370:
0xE0541000, 0x80020217, // buffer_load_dwordx2 v[2:3], v23, s[8:11], 0 offen              // 000000000378:
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                           // 000000000380:
0x7E0A032A,             // v_mov_b32_e32 v5, v42                                          // 000000000384:
0x7E080328,             // v_mov_b32_e32 v4, v40                                          // 000000000388:
0x7E540329,             // v_mov_b32_e32 v42, v41                                         // 00000000038C:
0xD9BE0000, 0x00001C0E, // ds_write_b128 v14, v[28:31]                                    // 000000000390:
0xD3CD8004, 0x0412091A, // v_mfma_f32_16x16x16f16 a[4:7], v[26:27], v[4:5], a[4:7]        // 000000000398:
0xE0541800, 0x80020418, // buffer_load_dwordx2 v[4:5], v24, s[8:11], 0 offen offset:2048  // 0000000003A0:
0xD9BE0000, 0x0000200F, // ds_write_b128 v15, v[32:35]                                    // 0000000003A8:
0xD3CD8000, 0x0402551A, // v_mfma_f32_16x16x16f16 a[0:3], v[26:27], v[42:43], a[0:3]      // 0000000003B0:
0xBF85FFB1,             // s_cbranch_scc1 BB0_1                                           // 0000000003B8:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000003BC:
0xBF8A0000,             // s_barrier                                                      // 0000000003C0:
0xD9FE0000, 0x14000011, // ds_read_b128 v[20:23], v17                                     // 0000000003C4:
0xD8700100, 0x18000010, // ds_read2st64_b32 v[24:25], v16 offset1:1                       // 0000000003CC:
0xD86EC484, 0x1C000010, // ds_read2_b32 v[28:29], v16 offset0:132 offset1:196             // 0000000003D4:
0x682620A0,             // v_add_u32_e32 v19, 32, v16                                     // 0000000003DC:
0x684420B0,             // v_add_u32_e32 v34, 48, v16                                     // 0000000003E0:
0x24000082,             // v_lshlrev_b32_e32 v0, 2, v0                                    // 0000000003E4:
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                           // 0000000003E8:
0x7E340318,             // v_mov_b32_e32 v26, v24                                         // 0000000003EC:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000003F0:
0x7E36031C,             // v_mov_b32_e32 v27, v28                                         // 0000000003F4:
0x7E380319,             // v_mov_b32_e32 v28, v25                                         // 0000000003F8:
0xBF800000,             // s_nop 0                                                        // 0000000003FC:
0xD3CD8004, 0x04123514, // v_mfma_f32_16x16x16f16 a[4:7], v[20:21], v[26:27], a[4:7]      // 000000000400:
0xD8700504, 0x1A000013, // ds_read2st64_b32 v[26:27], v19 offset0:4 offset1:5             // 000000000408:
0xD8700706, 0x1E000022, // ds_read2st64_b32 v[30:31], v34 offset0:6 offset1:7             // 000000000410:
0xB0031400,             // s_movk_i32 s3, 0x1400                                          // 000000000418:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 00000000041C:
0xBF8A0000,             // s_barrier                                                      // 000000000420:
0xBF8C0F74,             // s_waitcnt vmcnt(4)                                             // 000000000424:
0xD81A0000, 0x0000120C, // ds_write_b32 v12, v18                                          // 000000000428:
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                           // 000000000430:
0x7E40031A,             // v_mov_b32_e32 v32, v26                                         // 000000000434:
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                           // 000000000438:
0x7E42031E,             // v_mov_b32_e32 v33, v30                                         // 00000000043C:
0xBF8C0F72,             // s_waitcnt vmcnt(2)                                             // 000000000440:
0xD2A00018, 0x00021106, // v_pack_b32_f16 v24, v6, v8                                     // 000000000444:
0xD2A01819, 0x00021106, // v_pack_b32_f16 v25, v6, v8 op_sel:[1,1,0]                      // 00000000044C:
0x7E3C031B,             // v_mov_b32_e32 v30, v27                                         // 000000000454:
0xD2A0001A, 0x00021307, // v_pack_b32_f16 v26, v7, v9                                     // 000000000458:
0xD2A0181B, 0x00021307, // v_pack_b32_f16 v27, v7, v9 op_sel:[1,1,0]                      // 000000000460:
0xBF8C0F70,             // s_waitcnt vmcnt(0)                                             // 000000000468:
0xD2A00006, 0x00020902, // v_pack_b32_f16 v6, v2, v4                                      // 00000000046C:
0xD2A01807, 0x00020902, // v_pack_b32_f16 v7, v2, v4 op_sel:[1,1,0]                       // 000000000474:
0xD2A00008, 0x00020B03, // v_pack_b32_f16 v8, v3, v5                                      // 00000000047C:
0xD2A01809, 0x00020B03, // v_pack_b32_f16 v9, v3, v5 op_sel:[1,1,0]                       // 000000000484:
0xD9BE0000, 0x0000180E, // ds_write_b128 v14, v[24:27]                                    // 00000000048C:
0xD9BE0000, 0x0000060F, // ds_write_b128 v15, v[6:9]                                      // 000000000494:
0xD3CD8004, 0x04124116, // v_mfma_f32_16x16x16f16 a[4:7], v[22:23], v[32:33], a[4:7]      // 00000000049C:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000004A4:
0xBF8A0000,             // s_barrier                                                      // 0000000004A8:
0xD9FE0000, 0x02000011, // ds_read_b128 v[2:5], v17                                       // 0000000004AC:
0xD8700100, 0x06000010, // ds_read2st64_b32 v[6:7], v16 offset1:1                         // 0000000004B4:
0xD86EC484, 0x08000010, // ds_read2_b32 v[8:9], v16 offset0:132 offset1:196               // 0000000004BC:
0xD8700504, 0x10000013, // ds_read2st64_b32 v[16:17], v19 offset0:4 offset1:5             // 0000000004C4:
0xD8700706, 0x12000022, // ds_read2st64_b32 v[18:19], v34 offset0:6 offset1:7             // 0000000004CC:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000004D4:
0xBF8A0000,             // s_barrier                                                      // 0000000004D8:
0xD3CD8000, 0x04023914, // v_mfma_f32_16x16x16f16 a[0:3], v[20:21], v[28:29], a[0:3]      // 0000000004DC:
0xBF8CC37F,             // s_waitcnt lgkmcnt(3)                                           // 0000000004E4:
0x7E1C0306,             // v_mov_b32_e32 v14, v6                                          // 0000000004E8:
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                           // 0000000004EC:
0x7E1E0308,             // v_mov_b32_e32 v15, v8                                          // 0000000004F0:
0x7E100307,             // v_mov_b32_e32 v8, v7                                           // 0000000004F4:
0x260C1A8C,             // v_and_b32_e32 v6, 12, v13                                      // 0000000004F8:
0xD200000C, 0x04011106, // v_lshl_or_b32 v12, v6, 8, v0                                   // 0000000004FC:
0x24000284,             // v_lshlrev_b32_e32 v0, 4, v1                                    // 000000000504:
0xD3CD8004, 0x04121D02, // v_mfma_f32_16x16x16f16 a[4:7], v[2:3], v[14:15], a[4:7]        // 000000000508:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000510:
0x7E1E0312,             // v_mov_b32_e32 v15, v18                                         // 000000000514:
0x7E1C0310,             // v_mov_b32_e32 v14, v16                                         // 000000000518:
0x7E240311,             // v_mov_b32_e32 v18, v17                                         // 00000000051C:
0xD3CD8000, 0x04023D16, // v_mfma_f32_16x16x16f16 a[0:3], v[22:23], v[30:31], a[0:3]      // 000000000520:
0xD3CD8004, 0x04121D04, // v_mfma_f32_16x16x16f16 a[4:7], v[4:5], v[14:15], a[4:7]        // 000000000528:
0xD3CD8000, 0x04021102, // v_mfma_f32_16x16x16f16 a[0:3], v[2:3], v[8:9], a[0:3]          // 000000000530:
0xD2000002, 0x0401110B, // v_lshl_or_b32 v2, v11, 8, v0                                   // 000000000538:
0xBF800007,             // s_nop 7                                                        // 000000000540:
0xBF800000,             // s_nop 0                                                        // 000000000544:
0xDA1A0000, 0x0000040C, // ds_write_b32 v12, a4                                           // 000000000548:
0xDA1A0100, 0x0000050C, // ds_write_b32 v12, a5 offset:256                                // 000000000550:
0xDA1A0200, 0x0000060C, // ds_write_b32 v12, a6 offset:512                                // 000000000558:
0xDA1A0300, 0x0000070C, // ds_write_b32 v12, a7 offset:768                                // 000000000560:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000568:
0xBF8A0000,             // s_barrier                                                      // 00000000056C:
0xD9FE0000, 0x06000002, // ds_read_b128 v[6:9], v2                                        // 000000000570:
0xD3CD8000, 0x04022504, // v_mfma_f32_16x16x16f16 a[0:3], v[4:5], v[18:19], a[0:3]        // 000000000578:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000580:
0x7E001506,             // v_cvt_f16_f32_e32 v0, v6                                       // 000000000584:
0x7E061508,             // v_cvt_f16_f32_e32 v3, v8                                       // 000000000588:
0x7E0C1509,             // v_cvt_f16_f32_e32 v6, v9                                       // 00000000058C:
0x7E0E1507,             // v_cvt_f16_f32_e32 v7, v7                                       // 000000000590:
0x24100282,             // v_lshlrev_b32_e32 v8, 2, v1                                    // 000000000594:
0xD2A00001, 0x00020D03, // v_pack_b32_f16 v1, v3, v6                                      // 000000000598:
0xD2850003, 0x0000070A, // v_mul_lo_u32 v3, v10, s3                                       // 0000000005A0:
0x68060602,             // v_add_u32_e32 v3, s2, v3                                       // 0000000005A8:
0xD2A00000, 0x00020F00, // v_pack_b32_f16 v0, v0, v7                                      // 0000000005AC:
0xBE8300FF, 0x00020000, // s_mov_b32 s3, 0x20000                                          // 0000000005B4:
0xBE8200FF, 0x00050000, // s_mov_b32 s2, 0x50000                                          // 0000000005BC:
0xD1FE0004, 0x02061103, // v_add_lshl_u32 v4, v3, v8, 1                                   // 0000000005C4:
0xE0741000, 0x80000004, // buffer_store_dwordx2 v[0:1], v4, s[0:3], 0 offen               // 0000000005CC:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000005D4:
0xBF8A0000,             // s_barrier                                                      // 0000000005D8:
0xDA1A0000, 0x0000000C, // ds_write_b32 v12, a0                                           // 0000000005DC:
0xDA1A0100, 0x0000010C, // ds_write_b32 v12, a1 offset:256                                // 0000000005E4:
0xDA1A0200, 0x0000020C, // ds_write_b32 v12, a2 offset:512                                // 0000000005EC:
0xDA1A0300, 0x0000030C, // ds_write_b32 v12, a3 offset:768                                // 0000000005F4:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000005FC:
0xBF8A0000,             // s_barrier                                                      // 000000000600:
0xD9FE0000, 0x00000002, // ds_read_b128 v[0:3], v2                                        // 000000000604:
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 00000000060C:
0x7E001500,             // v_cvt_f16_f32_e32 v0, v0                                       // 000000000610:
0x7E0A1501,             // v_cvt_f16_f32_e32 v5, v1                                       // 000000000614:
0x7E041502,             // v_cvt_f16_f32_e32 v2, v2                                       // 000000000618:
0x7E061503,             // v_cvt_f16_f32_e32 v3, v3                                       // 00000000061C:
0xD2A00000, 0x00020B00, // v_pack_b32_f16 v0, v0, v5                                      // 000000000620:
0xD2A00001, 0x00020702, // v_pack_b32_f16 v1, v2, v3                                      // 000000000628:
0xE0741080, 0x80000004, // buffer_store_dwordx2 v[0:1], v4, s[0:3], 0 offen offset:128    // 000000000630:
0xBF810000,             // s_endpgm                                                       // 000000000638: 0xD38

0xBF800000,             // s_nop 0                                                        // 000000000D3C:
0xBF800000,             // s_nop 0                                                        // 000000000D40:
0xBF800000,             // s_nop 0                                                        // 000000000D44:
0xBF800000,             // s_nop 0                                                        // 000000000D48:
0xBF800000,             // s_nop 0                                                        // 000000000D4C:
0xBF800000,             // s_nop 0                                                        // 000000000D50:
0xBF800000,             // s_nop 0                                                        // 000000000D54:
0xBF800000,             // s_nop 0                                                        // 000000000D58:
0xBF800000,             // s_nop 0                                                        // 000000000D5C:
0xBF800000,             // s_nop 0                                                        // 000000000D60:
0xBF800000,             // s_nop 0                                                        // 000000000D64:
0xBF800000,             // s_nop 0                                                        // 000000000D68:
0xBF800000,             // s_nop 0                                                        // 000000000D6C:
0xBF800000,             // s_nop 0                                                        // 000000000D70:
0xBF800000,             // s_nop 0                                                        // 000000000D74:
0xBF800000,             // s_nop 0                                                        // 000000000D78:
0xBF800000,             // s_nop 0                                                        // 000000000D7C:
0xBF800000,             // s_nop 0                                                        // 000000000D80:
0xBF800000,             // s_nop 0                                                        // 000000000D84:
0xBF800000,             // s_nop 0                                                        // 000000000D88:
0xBF800000,             // s_nop 0                                                        // 000000000D8C:
0xBF800000,             // s_nop 0                                                        // 000000000D90:
0xBF800000,             // s_nop 0                                                        // 000000000D94:
0xBF800000,             // s_nop 0                                                        // 000000000D98:
0xBF800000,             // s_nop 0                                                        // 000000000D9C:
0xBF800000,             // s_nop 0                                                        // 000000000DA0:
0xBF800000,             // s_nop 0                                                        // 000000000DA4:
0xBF800000,             // s_nop 0                                                        // 000000000DA8:
0xBF800000,             // s_nop 0                                                        // 000000000DAC:
0xBF800000,             // s_nop 0                                                        // 000000000DB0:
0xBF800000,             // s_nop 0                                                        // 000000000DB4:
0xBF800000,             // s_nop 0                                                        // 000000000DB8:
0xBF800000,             // s_nop 0                                                        // 000000000DBC:
0xBF800000,             // s_nop 0                                                        // 000000000DC0:
0xBF800000,             // s_nop 0                                                        // 000000000DC4:
0xBF800000,             // s_nop 0                                                        // 000000000DC8:
0xBF800000,             // s_nop 0                                                        // 000000000DCC:
0xBF800000,             // s_nop 0                                                        // 000000000DD0:
0xBF800000,             // s_nop 0                                                        // 000000000DD4:
0xBF800000,             // s_nop 0                                                        // 000000000DD8:
0xBF800000,             // s_nop 0                                                        // 000000000DDC:
0xBF800000,             // s_nop 0                                                        // 000000000DE0:
0xBF800000,             // s_nop 0                                                        // 000000000DE4:
0xBF800000,             // s_nop 0                                                        // 000000000DE8:
0xBF800000,             // s_nop 0                                                        // 000000000DEC:
0xBF800000,             // s_nop 0                                                        // 000000000DF0:
0xBF800000,             // s_nop 0                                                        // 000000000DF4:
0xBF800000,             // s_nop 0                                                        // 000000000DF8:
0xBF800000,             // s_nop 0                                                        // 000000000DFC:

// 0xE00

// Original ISA:
// s4-s5 : kernarg pointer
// s6 : workgroup_id_x
// s7 : workgroup_id_y
// s8 : workgroup_id_z
//0xC00A0302, 0x00000000, // s_load_dwordx4 s[12:15], s[4:5], 0x0                           // 000000000000: 
//0xC00A0002, 0x00000010, // s_load_dwordx4 s[0:3], s[4:5], 0x10                            // 000000000008: 
0x20060085,             // v_lshrrev_b32_e32 v3, 5, v0                                    // 000000000010: 
0x24080082,             // v_lshlrev_b32_e32 v4, 2, v0                                    // 000000000014: 
//0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000018: 

// Hacked ISA
// s0-s1: matrix C pointer
// s2-s3: matrix A pointer
// s4-s5: matrix B pointer
// s6 : workgroup_id_x
// s7 : workgroup_id_y
// s8 : workgroup_id_z
0xBE8C0002,             //     s_mov_b32 s12, s2
0xBE8D0003,             //     s_mov_b32 s13, s3
0xBE8E0004,             //     s_mov_b32 s14, s4
0xBE8F0005,             //     s_mov_b32 s15, s5
0xBF800000,             // s_nop 0

0x8E038407,             // s_lshl_b32 s3, s7, 4                                           // 00000000001C: 
0x20160084,             // v_lshrrev_b32_e32 v11, 4, v0                                   // 000000000020: 
0x261808FF, 0x0000007C, // v_and_b32_e32 v12, 0x7c, v4                                    // 000000000024: 
0x100A06FF, 0x00000A00, // v_mul_u32_u24_e32 v5, 0xa00, v3                                // 00000000002C: 
0x8E028706,             // s_lshl_b32 s2, s6, 7                                           // 000000000034: 
0x28261905,             // v_or_b32_e32 v19, v5, v12                                      // 000000000038: 
0x2602008F,             // v_and_b32_e32 v1, 15, v0                                       // 00000000003C: 
0x68141603,             // v_add_u32_e32 v10, s3, v11                                     // 000000000040: 
0xB0031400,             // s_movk_i32 s3, 0x1400                                          // 000000000044: 
0xD1FE0012, 0x02040513, // v_add_lshl_u32 v18, v19, s2, 1                                 // 000000000048: 
0xD2850004, 0x0000070A, // v_mul_lo_u32 v4, v10, s3                                       // 000000000050: 
0x24040281,             // v_lshlrev_b32_e32 v2, 1, v1                                    // 000000000058: 
0xBE8700FF, 0x00020000, // s_mov_b32 s7, 0x20000                                          // 00000000005C: 
0x28080504,             // v_or_b32_e32 v4, v4, v2                                        // 000000000064: 
0xBE88000E,             // s_mov_b32 s8, s14                                              // 000000000068: 
0xBE8A00FF, 0x01900000, // s_mov_b32 s10, 0x1900000                                       // 00000000006C: 
0xBE89000F,             // s_mov_b32 s9, s15                                              // 000000000074: 
0xBE8B0007,             // s_mov_b32 s11, s7                                              // 000000000078: 
0x681A24FF, 0x0000A000, // v_add_u32_e32 v13, 0xa000, v18                                 // 00000000007C: 
0x24280881,             // v_lshlrev_b32_e32 v20, 1, v4                                   // 000000000084: 
0xE0541000, 0x80020412, // buffer_load_dwordx2 v[4:5], v18, s[8:11], 0 offen              // 000000000088: 
0xE0541A00, 0x80020612, // buffer_load_dwordx2 v[6:7], v18, s[8:11], 0 offen offset:2560  // 000000000090: 
0xE0541000, 0x8002080D, // buffer_load_dwordx2 v[8:9], v13, s[8:11], 0 offen              // 000000000098: 
0xE0541A00, 0x8002100D, // buffer_load_dwordx2 v[16:17], v13, s[8:11], 0 offen offset:2560// 0000000000A0: 
0x241A0081,             // v_lshlrev_b32_e32 v13, 1, v0                                   // 0000000000A8: 
0xD2010002, 0x0409410D, // v_and_or_b32 v2, v13, 32, v2                                   // 0000000000AC: 
0xB00E0050,             // s_movk_i32 s14, 0x50                                           // 0000000000B4: 
0x101A060E,             // v_mul_u32_u24_e32 v13, s14, v3                                 // 0000000000B8: 
0x24040481,             // v_lshlrev_b32_e32 v2, 1, v2                                    // 0000000000BC: 
0xB0030108,             // s_movk_i32 s3, 0x108                                           // 0000000000C0: 
0xD1FD000D, 0x0409030D, // v_lshl_add_u32 v13, v13, 1, v2                                 // 0000000000C4: 
0x10040603,             // v_mul_u32_u24_e32 v2, s3, v3                                   // 0000000000CC: 
0x24181881,             // v_lshlrev_b32_e32 v12, 1, v12                                  // 0000000000D0: 
0xD1FE0015, 0x0206050C, // v_add_lshl_u32 v21, v12, v2, 1                                 // 0000000000D4: 
0x28040688,             // v_or_b32_e32 v2, 8, v3                                         // 0000000000DC: 
0x10040403,             // v_mul_u32_u24_e32 v2, s3, v2                                   // 0000000000E0: 
0xBE84000C,             // s_mov_b32 s4, s12                                              // 0000000000E4: 
0xBE85000D,             // s_mov_b32 s5, s13                                              // 0000000000E8: 
0xBE8300FF, 0x00014000, // s_mov_b32 s3, 0x14000                                          // 0000000000EC: 
0xBE8600FF, 0x00050000, // s_mov_b32 s6, 0x50000                                          // 0000000000F4: 
0xD1FE000C, 0x02061902, // v_add_lshl_u32 v12, v2, v12, 1                                 // 0000000000FC: 
0x682C2403,             // v_add_u32_e32 v22, s3, v18                                     // 000000000104: 
0x682E24FF, 0x0001E000, // v_add_u32_e32 v23, 0x1e000, v18                                // 000000000108: 
0xE0501000, 0x80010214, // buffer_load_dword v2, v20, s[4:7], 0 offen                     // 000000000110: 
0xE0501040, 0x80011214, // buffer_load_dword v18, v20, s[4:7], 0 offen offset:64          // 000000000118: 
0xB00D0500,             // s_movk_i32 s13, 0x500                                          // 000000000120: 
0x681E180D,             // v_add_u32_e32 v15, s13, v12                                    // 000000000124: 
0xD3D94004, 0x18000080, // v_accvgpr_write_b32 a4, 0                                      // 000000000128: 
0x7E0AA504,             // v_accvgpr_mov_b32 a5, a4                                       // 000000000130: 
0x7E0CA504,             // v_accvgpr_mov_b32 a6, a4                                       // 000000000134: 
0x7E0EA504,             // v_accvgpr_mov_b32 a7, a4                                       // 000000000138: 
0xD3D94000, 0x18000080, // v_accvgpr_write_b32 a0, 0                                      // 00000000013C: 
0x24262681,             // v_lshlrev_b32_e32 v19, 1, v19                                  // 000000000144: 
0xBE8C0080,             // s_mov_b32 s12, 0                                               // 000000000148: 
0x681C2A0D,             // v_add_u32_e32 v14, s13, v21                                    // 00000000014C: 
0xD1FD0013, 0x044D0202, // v_lshl_add_u32 v19, s2, 1, v19                                 // 000000000150: 
0x282828FF, 0x00000080, // v_or_b32_e32 v20, 0x80, v20                                    // 000000000158: 
0xBF8C0F71,             // s_waitcnt vmcnt(1)                                             // 000000000160: 
0xD81A0000, 0x0000020D, // ds_write_b32 v13, v2                                           // 000000000164: 
0xD2A00002, 0x00020D04, // v_pack_b32_f16 v2, v4, v6                                      // 00000000016C: 
0xD2A01803, 0x00020D04, // v_pack_b32_f16 v3, v4, v6 op_sel:[1,1,0]                       // 000000000174: 
0xD2A00004, 0x00020F05, // v_pack_b32_f16 v4, v5, v7                                      // 00000000017C: 
0xD2A01805, 0x00020F05, // v_pack_b32_f16 v5, v5, v7 op_sel:[1,1,0]                       // 000000000184: 
0xD2A00006, 0x00022108, // v_pack_b32_f16 v6, v8, v16                                     // 00000000018C: 
0xD2A01807, 0x00022108, // v_pack_b32_f16 v7, v8, v16 op_sel:[1,1,0]                      // 000000000194: 
0xD2A00008, 0x00022309, // v_pack_b32_f16 v8, v9, v17                                     // 00000000019C: 
0xD2A01809, 0x00022309, // v_pack_b32_f16 v9, v9, v17 op_sel:[1,1,0]                      // 0000000001A4: 
0xD9BE0500, 0x00000215, // ds_write_b128 v21, v[2:5] offset:1280                          // 0000000001AC: 
0xD9BE0500, 0x0000060C, // ds_write_b128 v12, v[6:9] offset:1280                          // 0000000001B4: 
0xE0541000, 0x80020616, // buffer_load_dwordx2 v[6:7], v22, s[8:11], 0 offen              // 0000000001BC: 
0xE0541A00, 0x80020816, // buffer_load_dwordx2 v[8:9], v22, s[8:11], 0 offen offset:2560  // 0000000001C4: 
0xE0541000, 0x80020217, // buffer_load_dwordx2 v[2:3], v23, s[8:11], 0 offen              // 0000000001CC: 
0xE0541A00, 0x80020417, // buffer_load_dwordx2 v[4:5], v23, s[8:11], 0 offen offset:2560  // 0000000001D4: 
0x24220085,             // v_lshlrev_b32_e32 v17, 5, v0                                   // 0000000001DC: 
0xD1C8000C, 0x020D0300, // v_bfe_u32 v12, v0, 1, 3                                        // 0000000001E0: 
0xD1C80010, 0x02090900, // v_bfe_u32 v16, v0, 4, 2                                        // 0000000001E8: 
0x262222A0,             // v_and_b32_e32 v17, 32, v17                                     // 0000000001F0: 
0xD2000011, 0x04450710, // v_lshl_or_b32 v17, v16, 3, v17                                 // 0000000001F4: 
0x1018180E,             // v_mul_u32_u24_e32 v12, s14, v12                                // 0000000001FC: 
0xD1FE0011, 0x02061911, // v_add_lshl_u32 v17, v17, v12, 1                                // 000000000200: 
0x20180082,             // v_lshrrev_b32_e32 v12, 2, v0                                   // 000000000208: 
0xD3D84017, 0x18000105, // v_accvgpr_read_b32 v23, a5                                     // 00000000020C: 
0xD2010000, 0x0405610C, // v_and_or_b32 v0, v12, 48, v1                                   // 000000000214: 
0x102020FF, 0x00000420, // v_mul_u32_u24_e32 v16, 0x420, v16                              // 00000000021C: 
0xD1FD0010, 0x04410300, // v_lshl_add_u32 v16, v0, 1, v16                                 // 000000000224: 
0xD3D94001, 0x18000117, // v_accvgpr_write_b32 a1, v23                                    // 00000000022C: 
0xD3D84017, 0x18000106, // v_accvgpr_read_b32 v23, a6                                     // 000000000234: 
0xD1FD0010, 0x00350310, // v_lshl_add_u32 v16, v16, 1, s13                                // 00000000023C: 
0xD3D94002, 0x18000117, // v_accvgpr_write_b32 a2, v23                                    // 000000000244: 
0xD3D84017, 0x18000107, // v_accvgpr_read_b32 v23, a7                                     // 00000000024C: 
0x682A20A0,             // v_add_u32_e32 v21, 32, v16                                     // 000000000254: 
0x682C20B0,             // v_add_u32_e32 v22, 48, v16                                     // 000000000258: 
0xD3D94003, 0x18000117, // v_accvgpr_write_b32 a3, v23                                    // 00000000025C: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0) <BB0_1>                                   // 000000000264: 
0xBF8A0000,             // s_barrier                                                      // 000000000268: 
0xD9FE0000, 0x18000011, // ds_read_b128 v[24:27], v17                                     // 00000000026C: 
0xD8700100, 0x24000010, // ds_read2st64_b32 v[36:37], v16 offset1:1                       // 000000000274: 
0xD86EC484, 0x26000010, // ds_read2_b32 v[38:39], v16 offset0:132 offset1:196             // 00000000027C: 
0xD8700504, 0x28000015, // ds_read2st64_b32 v[40:41], v21 offset0:4 offset1:5             // 000000000284: 
0xD8700706, 0x2A000016, // ds_read2st64_b32 v[42:43], v22 offset0:6 offset1:7             // 00000000028C: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000294: 
0xBF8A0000,             // s_barrier                                                      // 000000000298: 
0xBF8CC37F,             // s_waitcnt lgkmcnt(3)                                           // 00000000029C: 
0x7E400324,             // v_mov_b32_e32 v32, v36                                         // 0000000002A0: 
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                           // 0000000002A4: 
0x7E420326,             // v_mov_b32_e32 v33, v38                                         // 0000000002A8: 
0xBF8C0F74,             // s_waitcnt vmcnt(4)                                             // 0000000002AC: 
0xD81A0000, 0x0000120D, // ds_write_b32 v13, v18                                          // 0000000002B0: 
0xE0501000, 0x80011214, // buffer_load_dword v18, v20, s[4:7], 0 offen                    // 0000000002B8: 
0xD3CD8004, 0x04124118, // v_mfma_f32_16x16x16f16 a[4:7], v[24:25], v[32:33], a[4:7]      // 0000000002C0: 
0xBF8C0F73,             // s_waitcnt vmcnt(3)                                             // 0000000002C8: 
0xD2A0001C, 0x00021106, // v_pack_b32_f16 v28, v6, v8                                     // 0000000002CC: 
0xD2A0181D, 0x00021106, // v_pack_b32_f16 v29, v6, v8 op_sel:[1,1,0]                      // 0000000002D4: 
0xD2A0001E, 0x00021307, // v_pack_b32_f16 v30, v7, v9                                     // 0000000002DC: 
0xD2A0181F, 0x00021307, // v_pack_b32_f16 v31, v7, v9 op_sel:[1,1,0]                      // 0000000002E4: 
0xBF8C0F71,             // s_waitcnt vmcnt(1)                                             // 0000000002EC: 
0xD2A00020, 0x00020902, // v_pack_b32_f16 v32, v2, v4                                     // 0000000002F0: 
0xD2A01821, 0x00020902, // v_pack_b32_f16 v33, v2, v4 op_sel:[1,1,0]                      // 0000000002F8: 
0x6804260C,             // v_add_u32_e32 v2, s12, v19                                     // 000000000300: 
0x680804FF, 0x00028000, // v_add_u32_e32 v4, 0x28000, v2                                  // 000000000304: 
0x810C030C,             // s_add_i32 s12, s12, s3                                         // 00000000030C: 
0xD2A00022, 0x00020B03, // v_pack_b32_f16 v34, v3, v5                                     // 000000000310: 
0xD2A01823, 0x00020B03, // v_pack_b32_f16 v35, v3, v5 op_sel:[1,1,0]                      // 000000000318: 
0x682E04FF, 0x00032000, // v_add_u32_e32 v23, 0x32000, v2                                 // 000000000320: 
0xE0541000, 0x80020604, // buffer_load_dwordx2 v[6:7], v4, s[8:11], 0 offen               // 000000000328: 
0xE0541A00, 0x80020804, // buffer_load_dwordx2 v[8:9], v4, s[8:11], 0 offen offset:2560   // 000000000330: 
0xE0541000, 0x80020217, // buffer_load_dwordx2 v[2:3], v23, s[8:11], 0 offen              // 000000000338: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                           // 000000000340: 
0x7E0A032A,             // v_mov_b32_e32 v5, v42                                          // 000000000344: 
0x7E080328,             // v_mov_b32_e32 v4, v40                                          // 000000000348: 
0xBF07FF0C, 0x00C58000, // s_cmp_lg_u32 s12, 0xc58000                                     // 00000000034C: 
0x7E4C0325,             // v_mov_b32_e32 v38, v37                                         // 000000000354: 
0xD3CD8004, 0x0412091A, // v_mfma_f32_16x16x16f16 a[4:7], v[26:27], v[4:5], a[4:7]        // 000000000358: 
0xE0541A00, 0x80020417, // buffer_load_dwordx2 v[4:5], v23, s[8:11], 0 offen offset:2560  // 000000000360: 
0x7E540329,             // v_mov_b32_e32 v42, v41                                         // 000000000368: 
0x682828C0,             // v_add_u32_e32 v20, 64, v20                                     // 00000000036C: 
0xD9BE0000, 0x00001C0E, // ds_write_b128 v14, v[28:31]                                    // 000000000370: 
0xD9BE0000, 0x0000200F, // ds_write_b128 v15, v[32:35]                                    // 000000000378: 
0xD3CD8000, 0x04024D18, // v_mfma_f32_16x16x16f16 a[0:3], v[24:25], v[38:39], a[0:3]      // 000000000380: 
0xD3CD8000, 0x0402551A, // v_mfma_f32_16x16x16f16 a[0:3], v[26:27], v[42:43], a[0:3]      // 000000000388: 
0xBF85FFB4,             // s_cbranch_scc1 BB0_1                                           // 000000000390: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000394: 
0xBF8A0000,             // s_barrier                                                      // 000000000398: 
0xD9FE0000, 0x14000011, // ds_read_b128 v[20:23], v17                                     // 00000000039C: 
0xD8700100, 0x18000010, // ds_read2st64_b32 v[24:25], v16 offset1:1                       // 0000000003A4: 
0xD86EC484, 0x1C000010, // ds_read2_b32 v[28:29], v16 offset0:132 offset1:196             // 0000000003AC: 
0x682620A0,             // v_add_u32_e32 v19, 32, v16                                     // 0000000003B4: 
0x684420B0,             // v_add_u32_e32 v34, 48, v16                                     // 0000000003B8: 
0x24000082,             // v_lshlrev_b32_e32 v0, 2, v0                                    // 0000000003BC: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                           // 0000000003C0: 
0x7E340318,             // v_mov_b32_e32 v26, v24                                         // 0000000003C4: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000003C8: 
0x7E36031C,             // v_mov_b32_e32 v27, v28                                         // 0000000003CC: 
0x7E380319,             // v_mov_b32_e32 v28, v25                                         // 0000000003D0: 
0xBF800000,             // s_nop 0                                                        // 0000000003D4: 
0xD3CD8004, 0x04123514, // v_mfma_f32_16x16x16f16 a[4:7], v[20:21], v[26:27], a[4:7]      // 0000000003D8: 
0xD8700504, 0x1A000013, // ds_read2st64_b32 v[26:27], v19 offset0:4 offset1:5             // 0000000003E0: 
0xD8700706, 0x1E000022, // ds_read2st64_b32 v[30:31], v34 offset0:6 offset1:7             // 0000000003E8: 
0xB0030500,             // s_movk_i32 s3, 0x500                                           // 0000000003F0: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000003F4: 
0xBF8A0000,             // s_barrier                                                      // 0000000003F8: 
0xBF8C0F74,             // s_waitcnt vmcnt(4)                                             // 0000000003FC: 
0xD81A0000, 0x0000120D, // ds_write_b32 v13, v18                                          // 000000000400: 
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                           // 000000000408: 
0x7E40031A,             // v_mov_b32_e32 v32, v26                                         // 00000000040C: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                           // 000000000410: 
0x7E42031E,             // v_mov_b32_e32 v33, v30                                         // 000000000414: 
0xBF8C0F72,             // s_waitcnt vmcnt(2)                                             // 000000000418: 
0xD2A00018, 0x00021106, // v_pack_b32_f16 v24, v6, v8                                     // 00000000041C: 
0xD2A01819, 0x00021106, // v_pack_b32_f16 v25, v6, v8 op_sel:[1,1,0]                      // 000000000424: 
0x7E3C031B,             // v_mov_b32_e32 v30, v27                                         // 00000000042C: 
0xD2A0001A, 0x00021307, // v_pack_b32_f16 v26, v7, v9                                     // 000000000430: 
0xD2A0181B, 0x00021307, // v_pack_b32_f16 v27, v7, v9 op_sel:[1,1,0]                      // 000000000438: 
0xBF8C0F70,             // s_waitcnt vmcnt(0)                                             // 000000000440: 
0xD2A00006, 0x00020902, // v_pack_b32_f16 v6, v2, v4                                      // 000000000444: 
0xD2A01807, 0x00020902, // v_pack_b32_f16 v7, v2, v4 op_sel:[1,1,0]                       // 00000000044C: 
0xD2A00008, 0x00020B03, // v_pack_b32_f16 v8, v3, v5                                      // 000000000454: 
0xD2A01809, 0x00020B03, // v_pack_b32_f16 v9, v3, v5 op_sel:[1,1,0]                       // 00000000045C: 
0xD9BE0000, 0x0000180E, // ds_write_b128 v14, v[24:27]                                    // 000000000464: 
0xD9BE0000, 0x0000060F, // ds_write_b128 v15, v[6:9]                                      // 00000000046C: 
0xD3CD8004, 0x04124116, // v_mfma_f32_16x16x16f16 a[4:7], v[22:23], v[32:33], a[4:7]      // 000000000474: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 00000000047C: 
0xBF8A0000,             // s_barrier                                                      // 000000000480: 
0xD9FE0000, 0x02000011, // ds_read_b128 v[2:5], v17                                       // 000000000484: 
0xD8700100, 0x06000010, // ds_read2st64_b32 v[6:7], v16 offset1:1                         // 00000000048C: 
0xD86EC484, 0x08000010, // ds_read2_b32 v[8:9], v16 offset0:132 offset1:196               // 000000000494: 
0xD8700504, 0x10000013, // ds_read2st64_b32 v[16:17], v19 offset0:4 offset1:5             // 00000000049C: 
0xD8700706, 0x12000022, // ds_read2st64_b32 v[18:19], v34 offset0:6 offset1:7             // 0000000004A4: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000004AC: 
0xBF8A0000,             // s_barrier                                                      // 0000000004B0: 
0xD3CD8000, 0x04023914, // v_mfma_f32_16x16x16f16 a[0:3], v[20:21], v[28:29], a[0:3]      // 0000000004B4: 
0xBF8CC37F,             // s_waitcnt lgkmcnt(3)                                           // 0000000004BC: 
0x7E1C0306,             // v_mov_b32_e32 v14, v6                                          // 0000000004C0: 
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                           // 0000000004C4: 
0x7E1E0308,             // v_mov_b32_e32 v15, v8                                          // 0000000004C8: 
0x7E100307,             // v_mov_b32_e32 v8, v7                                           // 0000000004CC: 
0x260C188C,             // v_and_b32_e32 v6, 12, v12                                      // 0000000004D0: 
0xD200000C, 0x04011106, // v_lshl_or_b32 v12, v6, 8, v0                                   // 0000000004D4: 
0x24000284,             // v_lshlrev_b32_e32 v0, 4, v1                                    // 0000000004DC: 
0xD3CD8004, 0x04121D02, // v_mfma_f32_16x16x16f16 a[4:7], v[2:3], v[14:15], a[4:7]        // 0000000004E0: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000004E8: 
0x7E1E0312,             // v_mov_b32_e32 v15, v18                                         // 0000000004EC: 
0x7E1C0310,             // v_mov_b32_e32 v14, v16                                         // 0000000004F0: 
0x7E240311,             // v_mov_b32_e32 v18, v17                                         // 0000000004F4: 
0xD3CD8000, 0x04023D16, // v_mfma_f32_16x16x16f16 a[0:3], v[22:23], v[30:31], a[0:3]      // 0000000004F8: 
0xD3CD8004, 0x04121D04, // v_mfma_f32_16x16x16f16 a[4:7], v[4:5], v[14:15], a[4:7]        // 000000000500: 
0xD3CD8000, 0x04021102, // v_mfma_f32_16x16x16f16 a[0:3], v[2:3], v[8:9], a[0:3]          // 000000000508: 
0xD2000002, 0x0401110B, // v_lshl_or_b32 v2, v11, 8, v0                                   // 000000000510: 
0xBF800007,             // s_nop 7                                                        // 000000000518: 
0xBF800000,             // s_nop 0                                                        // 00000000051C: 
0xDA1A0000, 0x0000040C, // ds_write_b32 v12, a4                                           // 000000000520: 
0xDA1A0100, 0x0000050C, // ds_write_b32 v12, a5 offset:256                                // 000000000528: 
0xDA1A0200, 0x0000060C, // ds_write_b32 v12, a6 offset:512                                // 000000000530: 
0xDA1A0300, 0x0000070C, // ds_write_b32 v12, a7 offset:768                                // 000000000538: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000540: 
0xBF8A0000,             // s_barrier                                                      // 000000000544: 
0xD9FE0000, 0x06000002, // ds_read_b128 v[6:9], v2                                        // 000000000548: 
0xD3CD8000, 0x04022504, // v_mfma_f32_16x16x16f16 a[0:3], v[4:5], v[18:19], a[0:3]        // 000000000550: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000558: 
0x7E001506,             // v_cvt_f16_f32_e32 v0, v6                                       // 00000000055C: 
0x7E061508,             // v_cvt_f16_f32_e32 v3, v8                                       // 000000000560: 
0x7E0C1509,             // v_cvt_f16_f32_e32 v6, v9                                       // 000000000564: 
0x7E0E1507,             // v_cvt_f16_f32_e32 v7, v7                                       // 000000000568: 
0x24100282,             // v_lshlrev_b32_e32 v8, 2, v1                                    // 00000000056C: 
0xD2A00001, 0x00020D03, // v_pack_b32_f16 v1, v3, v6                                      // 000000000570: 
0xD2850003, 0x0000070A, // v_mul_lo_u32 v3, v10, s3                                       // 000000000578: 
0x68060602,             // v_add_u32_e32 v3, s2, v3                                       // 000000000580: 
0xD2A00000, 0x00020F00, // v_pack_b32_f16 v0, v0, v7                                      // 000000000584: 
0xBE8300FF, 0x00020000, // s_mov_b32 s3, 0x20000                                          // 00000000058C: 
0xBE8200FF, 0x00014000, // s_mov_b32 s2, 0x14000                                          // 000000000594: 
0xD1FE0004, 0x02061103, // v_add_lshl_u32 v4, v3, v8, 1                                   // 00000000059C: 
0xE0741000, 0x80000004, // buffer_store_dwordx2 v[0:1], v4, s[0:3], 0 offen               // 0000000005A4: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000005AC: 
0xBF8A0000,             // s_barrier                                                      // 0000000005B0: 
0xDA1A0000, 0x0000000C, // ds_write_b32 v12, a0                                           // 0000000005B4: 
0xDA1A0100, 0x0000010C, // ds_write_b32 v12, a1 offset:256                                // 0000000005BC: 
0xDA1A0200, 0x0000020C, // ds_write_b32 v12, a2 offset:512                                // 0000000005C4: 
0xDA1A0300, 0x0000030C, // ds_write_b32 v12, a3 offset:768                                // 0000000005CC: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000005D4: 
0xBF8A0000,             // s_barrier                                                      // 0000000005D8: 
0xD9FE0000, 0x00000002, // ds_read_b128 v[0:3], v2                                        // 0000000005DC: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000005E4: 
0x7E001500,             // v_cvt_f16_f32_e32 v0, v0                                       // 0000000005E8: 
0x7E0A1501,             // v_cvt_f16_f32_e32 v5, v1                                       // 0000000005EC: 
0x7E041502,             // v_cvt_f16_f32_e32 v2, v2                                       // 0000000005F0: 
0x7E061503,             // v_cvt_f16_f32_e32 v3, v3                                       // 0000000005F4: 
0xD2A00000, 0x00020B00, // v_pack_b32_f16 v0, v0, v5                                      // 0000000005F8: 
0xD2A00001, 0x00020702, // v_pack_b32_f16 v1, v2, v3                                      // 000000000600: 
0xE0741080, 0x80000004, // buffer_store_dwordx2 v[0:1], v4, s[0:3], 0 offen offset:128    // 000000000608: 
0xBF810000,             // s_endpgm                                                       // 000000000610: 0x1410

0xBF800000,             // s_nop 0                                                        // 000000001414:
0xBF800000,             // s_nop 0                                                        // 000000001418:
0xBF800000,             // s_nop 0                                                        // 00000000141C:
0xBF800000,             // s_nop 0                                                        // 000000001420:
0xBF800000,             // s_nop 0                                                        // 000000001424:
0xBF800000,             // s_nop 0                                                        // 000000001428:
0xBF800000,             // s_nop 0                                                        // 00000000142C:
0xBF800000,             // s_nop 0                                                        // 000000001430:
0xBF800000,             // s_nop 0                                                        // 000000001434:
0xBF800000,             // s_nop 0                                                        // 000000001438:
0xBF800000,             // s_nop 0                                                        // 00000000143C:
0xBF800000,             // s_nop 0                                                        // 000000001440:
0xBF800000,             // s_nop 0                                                        // 000000001444:
0xBF800000,             // s_nop 0                                                        // 000000001448:
0xBF800000,             // s_nop 0                                                        // 00000000144C:
0xBF800000,             // s_nop 0                                                        // 000000001450:
0xBF800000,             // s_nop 0                                                        // 000000001454:
0xBF800000,             // s_nop 0                                                        // 000000001458:
0xBF800000,             // s_nop 0                                                        // 00000000145C:
0xBF800000,             // s_nop 0                                                        // 000000001460:
0xBF800000,             // s_nop 0                                                        // 000000001464:
0xBF800000,             // s_nop 0                                                        // 000000001468:
0xBF800000,             // s_nop 0                                                        // 00000000146C:
0xBF800000,             // s_nop 0                                                        // 000000001470:
0xBF800000,             // s_nop 0                                                        // 000000001474:
0xBF800000,             // s_nop 0                                                        // 000000001478:
0xBF800000,             // s_nop 0                                                        // 00000000147C:
0xBF800000,             // s_nop 0                                                        // 000000001480:
0xBF800000,             // s_nop 0                                                        // 000000001484:
0xBF800000,             // s_nop 0                                                        // 000000001488:
0xBF800000,             // s_nop 0                                                        // 00000000148C:
0xBF800000,             // s_nop 0                                                        // 000000001490:
0xBF800000,             // s_nop 0                                                        // 000000001494:
0xBF800000,             // s_nop 0                                                        // 000000001498:
0xBF800000,             // s_nop 0                                                        // 00000000149C:
0xBF800000,             // s_nop 0                                                        // 0000000014A0:
0xBF800000,             // s_nop 0                                                        // 0000000014A4:
0xBF800000,             // s_nop 0                                                        // 0000000014A8:
0xBF800000,             // s_nop 0                                                        // 0000000014AC:
0xBF800000,             // s_nop 0                                                        // 0000000014B0:
0xBF800000,             // s_nop 0                                                        // 0000000014B4:
0xBF800000,             // s_nop 0                                                        // 0000000014B8:
0xBF800000,             // s_nop 0                                                        // 0000000014BC:
0xBF800000,             // s_nop 0                                                        // 0000000014C0:
0xBF800000,             // s_nop 0                                                        // 0000000014C4:
0xBF800000,             // s_nop 0                                                        // 0000000014C8:
0xBF800000,             // s_nop 0                                                        // 0000000014CC:
0xBF800000,             // s_nop 0                                                        // 0000000014D0:
0xBF800000,             // s_nop 0                                                        // 0000000014D4:
0xBF800000,             // s_nop 0                                                        // 0000000014D8:
0xBF800000,             // s_nop 0                                                        // 0000000014DC:
0xBF800000,             // s_nop 0                                                        // 0000000014E0:
0xBF800000,             // s_nop 0                                                        // 0000000014E4:
0xBF800000,             // s_nop 0                                                        // 0000000014E8:
0xBF800000,             // s_nop 0                                                        // 0000000014EC:
0xBF800000,             // s_nop 0                                                        // 0000000014F0:
0xBF800000,             // s_nop 0                                                        // 0000000014F4:
0xBF800000,             // s_nop 0                                                        // 0000000014F8:
0xBF800000,             // s_nop 0                                                        // 0000000014FC:

// 0x1500

// Original ISA:
// s4-s5 : kernarg pointer
// s6 : workgroup_id_x
// s7 : workgroup_id_y
// s8 : workgroup_id_z
//0xC00A0302, 0x00000000, // s_load_dwordx4 s[12:15], s[4:5], 0x0                           // 000000000000: 
//0xC00A0002, 0x00000010, // s_load_dwordx4 s[0:3], s[4:5], 0x10                            // 000000000008: 
0x20060085,             // v_lshrrev_b32_e32 v3, 5, v0                                    // 000000000010: 
0x24080082,             // v_lshlrev_b32_e32 v4, 2, v0                                    // 000000000014: 
//0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000018: 

// Hacked ISA
// s0-s1: matrix C pointer
// s2-s3: matrix A pointer
// s4-s5: matrix B pointer
// s6 : workgroup_id_x
// s7 : workgroup_id_y
// s8 : workgroup_id_z
0xBE8C0002,             //     s_mov_b32 s12, s2
0xBE8D0003,             //     s_mov_b32 s13, s3
0xBE8E0004,             //     s_mov_b32 s14, s4
0xBE8F0005,             //     s_mov_b32 s15, s5
0xBF800000,             // s_nop 0

0x8E038407,             // s_lshl_b32 s3, s7, 4                                           // 00000000001C: 
0x20160084,             // v_lshrrev_b32_e32 v11, 4, v0                                   // 000000000020: 
0x261808FF, 0x0000007C, // v_and_b32_e32 v12, 0x7c, v4                                    // 000000000024: 
0x100A06FF, 0x00002800, // v_mul_u32_u24_e32 v5, 0x2800, v3                               // 00000000002C: 
0x8E028706,             // s_lshl_b32 s2, s6, 7                                           // 000000000034: 
0x28261905,             // v_or_b32_e32 v19, v5, v12                                      // 000000000038: 
0x2602008F,             // v_and_b32_e32 v1, 15, v0                                       // 00000000003C: 
0x68141603,             // v_add_u32_e32 v10, s3, v11                                     // 000000000040: 
0xB0100500,             // s_movk_i32 s16, 0x500                                          // 000000000044: 
0xD1FE0012, 0x02040513, // v_add_lshl_u32 v18, v19, s2, 1                                 // 000000000048: 
0xBE8700FF, 0x00020000, // s_mov_b32 s7, 0x20000                                          // 000000000050: 
0x24040281,             // v_lshlrev_b32_e32 v2, 1, v1                                    // 000000000058: 
0xD2850004, 0x0000210A, // v_mul_lo_u32 v4, v10, s16                                      // 00000000005C: 
0x28080504,             // v_or_b32_e32 v4, v4, v2                                        // 000000000064: 
0xBE8A00FF, 0x01900000, // s_mov_b32 s10, 0x1900000                                       // 000000000068: 
0xBE88000E,             // s_mov_b32 s8, s14                                              // 000000000070: 
0xBE89000F,             // s_mov_b32 s9, s15                                              // 000000000074: 
0xBE8B0007,             // s_mov_b32 s11, s7                                              // 000000000078: 
0x681A24FF, 0x00002000, // v_add_u32_e32 v13, 0x2000, v18                                 // 00000000007C: 
0x24280881,             // v_lshlrev_b32_e32 v20, 1, v4                                   // 000000000084: 
0x681C24FF, 0x00028000, // v_add_u32_e32 v14, 0x28000, v18                                // 000000000088: 
0x681E24FF, 0x0002A000, // v_add_u32_e32 v15, 0x2a000, v18                                // 000000000090: 
0xE0541000, 0x80020412, // buffer_load_dwordx2 v[4:5], v18, s[8:11], 0 offen              // 000000000098: 
0xE0541800, 0x8002060D, // buffer_load_dwordx2 v[6:7], v13, s[8:11], 0 offen offset:2048  // 0000000000A0: 
0xE0541000, 0x8002080E, // buffer_load_dwordx2 v[8:9], v14, s[8:11], 0 offen              // 0000000000A8: 
0xE0541800, 0x8002100F, // buffer_load_dwordx2 v[16:17], v15, s[8:11], 0 offen offset:2048// 0000000000B0: 
0x241A0081,             // v_lshlrev_b32_e32 v13, 1, v0                                   // 0000000000B8: 
0xD2010002, 0x0409410D, // v_and_or_b32 v2, v13, 32, v2                                   // 0000000000BC: 
0xBE85000D,             // s_mov_b32 s5, s13                                              // 0000000000C4: 
0xB00D0050,             // s_movk_i32 s13, 0x50                                           // 0000000000C8: 
0x101A060D,             // v_mul_u32_u24_e32 v13, s13, v3                                 // 0000000000CC: 
0x24040481,             // v_lshlrev_b32_e32 v2, 1, v2                                    // 0000000000D0: 
0xB0030108,             // s_movk_i32 s3, 0x108                                           // 0000000000D4: 
0xD1FD000D, 0x0409030D, // v_lshl_add_u32 v13, v13, 1, v2                                 // 0000000000D8: 
0x10040603,             // v_mul_u32_u24_e32 v2, s3, v3                                   // 0000000000E0: 
0x24181881,             // v_lshlrev_b32_e32 v12, 1, v12                                  // 0000000000E4: 
0xD1FE0015, 0x0206050C, // v_add_lshl_u32 v21, v12, v2, 1                                 // 0000000000E8: 
0x28040688,             // v_or_b32_e32 v2, 8, v3                                         // 0000000000F0: 
0x10040403,             // v_mul_u32_u24_e32 v2, s3, v2                                   // 0000000000F4: 
0xBE84000C,             // s_mov_b32 s4, s12                                              // 0000000000F8: 
0xBE8300FF, 0x00050000, // s_mov_b32 s3, 0x50000                                          // 0000000000FC: 
0xBE8600FF, 0x00014000, // s_mov_b32 s6, 0x14000                                          // 000000000104: 
0xD1FE000C, 0x02061902, // v_add_lshl_u32 v12, v2, v12, 1                                 // 00000000010C: 
0x682C2403,             // v_add_u32_e32 v22, s3, v18                                     // 000000000114: 
0x682E24FF, 0x00052000, // v_add_u32_e32 v23, 0x52000, v18                                // 000000000118: 
0x683024FF, 0x00078000, // v_add_u32_e32 v24, 0x78000, v18                                // 000000000120: 
0x683224FF, 0x0007A000, // v_add_u32_e32 v25, 0x7a000, v18                                // 000000000128: 
0xE0501000, 0x80010214, // buffer_load_dword v2, v20, s[4:7], 0 offen                     // 000000000130: 
0xE0501040, 0x80011214, // buffer_load_dword v18, v20, s[4:7], 0 offen offset:64          // 000000000138: 
0x681E1810,             // v_add_u32_e32 v15, s16, v12                                    // 000000000140: 
0xD3D94004, 0x18000080, // v_accvgpr_write_b32 a4, 0                                      // 000000000144: 
0x7E0AA504,             // v_accvgpr_mov_b32 a5, a4                                       // 00000000014C: 
0x7E0CA504,             // v_accvgpr_mov_b32 a6, a4                                       // 000000000150: 
0x7E0EA504,             // v_accvgpr_mov_b32 a7, a4                                       // 000000000154: 
0xD3D94000, 0x18000080, // v_accvgpr_write_b32 a0, 0                                      // 000000000158: 
0x24262681,             // v_lshlrev_b32_e32 v19, 1, v19                                  // 000000000160: 
0xBE8C0080,             // s_mov_b32 s12, 0                                               // 000000000164: 
0x681C2A10,             // v_add_u32_e32 v14, s16, v21                                    // 000000000168: 
0xD1FD0013, 0x044D0202, // v_lshl_add_u32 v19, s2, 1, v19                                 // 00000000016C: 
0x282828FF, 0x00000080, // v_or_b32_e32 v20, 0x80, v20                                    // 000000000174: 
0xBF8C0F71,             // s_waitcnt vmcnt(1)                                             // 00000000017C: 
0xD81A0000, 0x0000020D, // ds_write_b32 v13, v2                                           // 000000000180: 
0xD2A00002, 0x00020D04, // v_pack_b32_f16 v2, v4, v6                                      // 000000000188: 
0xD2A01803, 0x00020D04, // v_pack_b32_f16 v3, v4, v6 op_sel:[1,1,0]                       // 000000000190: 
0xD2A00004, 0x00020F05, // v_pack_b32_f16 v4, v5, v7                                      // 000000000198: 
0xD2A01805, 0x00020F05, // v_pack_b32_f16 v5, v5, v7 op_sel:[1,1,0]                       // 0000000001A0: 
0xD2A00006, 0x00022108, // v_pack_b32_f16 v6, v8, v16                                     // 0000000001A8: 
0xD2A01807, 0x00022108, // v_pack_b32_f16 v7, v8, v16 op_sel:[1,1,0]                      // 0000000001B0: 
0xD2A00008, 0x00022309, // v_pack_b32_f16 v8, v9, v17                                     // 0000000001B8: 
0xD2A01809, 0x00022309, // v_pack_b32_f16 v9, v9, v17 op_sel:[1,1,0]                      // 0000000001C0: 
0xD9BE0500, 0x00000215, // ds_write_b128 v21, v[2:5] offset:1280                          // 0000000001C8: 
0xD9BE0500, 0x0000060C, // ds_write_b128 v12, v[6:9] offset:1280                          // 0000000001D0: 
0xE0541000, 0x80020616, // buffer_load_dwordx2 v[6:7], v22, s[8:11], 0 offen              // 0000000001D8: 
0xE0541800, 0x80020817, // buffer_load_dwordx2 v[8:9], v23, s[8:11], 0 offen offset:2048  // 0000000001E0: 
0xE0541000, 0x80020218, // buffer_load_dwordx2 v[2:3], v24, s[8:11], 0 offen              // 0000000001E8: 
0xE0541800, 0x80020419, // buffer_load_dwordx2 v[4:5], v25, s[8:11], 0 offen offset:2048  // 0000000001F0: 
0x24220085,             // v_lshlrev_b32_e32 v17, 5, v0                                   // 0000000001F8: 
0xD1C8000C, 0x020D0300, // v_bfe_u32 v12, v0, 1, 3                                        // 0000000001FC: 
0xD1C80010, 0x02090900, // v_bfe_u32 v16, v0, 4, 2                                        // 000000000204: 
0x262222A0,             // v_and_b32_e32 v17, 32, v17                                     // 00000000020C: 
0xD2000011, 0x04450710, // v_lshl_or_b32 v17, v16, 3, v17                                 // 000000000210: 
0x1018180D,             // v_mul_u32_u24_e32 v12, s13, v12                                // 000000000218: 
0xD1FE0011, 0x02061911, // v_add_lshl_u32 v17, v17, v12, 1                                // 00000000021C: 
0x20180082,             // v_lshrrev_b32_e32 v12, 2, v0                                   // 000000000224: 
0xD3D84017, 0x18000105, // v_accvgpr_read_b32 v23, a5                                     // 000000000228: 
0xD2010000, 0x0405610C, // v_and_or_b32 v0, v12, 48, v1                                   // 000000000230: 
0x102020FF, 0x00000420, // v_mul_u32_u24_e32 v16, 0x420, v16                              // 000000000238: 
0xD1FD0010, 0x04410300, // v_lshl_add_u32 v16, v0, 1, v16                                 // 000000000240: 
0xD3D94001, 0x18000117, // v_accvgpr_write_b32 a1, v23                                    // 000000000248: 
0xD3D84017, 0x18000106, // v_accvgpr_read_b32 v23, a6                                     // 000000000250: 
0xD1FD0010, 0x00410310, // v_lshl_add_u32 v16, v16, 1, s16                                // 000000000258: 
0xD3D94002, 0x18000117, // v_accvgpr_write_b32 a2, v23                                    // 000000000260: 
0xD3D84017, 0x18000107, // v_accvgpr_read_b32 v23, a7                                     // 000000000268: 
0x682A20A0,             // v_add_u32_e32 v21, 32, v16                                     // 000000000270: 
0x682C20B0,             // v_add_u32_e32 v22, 48, v16                                     // 000000000274: 
0xD3D94003, 0x18000117, // v_accvgpr_write_b32 a3, v23                                    // 000000000278: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0) <BB0_1>                                   // 000000000280: 
0xBF8A0000,             // s_barrier                                                      // 000000000284: 
0xD9FE0000, 0x18000011, // ds_read_b128 v[24:27], v17                                     // 000000000288: 
0xD8700100, 0x24000010, // ds_read2st64_b32 v[36:37], v16 offset1:1                       // 000000000290: 
0xD86EC484, 0x26000010, // ds_read2_b32 v[38:39], v16 offset0:132 offset1:196             // 000000000298: 
0xD8700504, 0x28000015, // ds_read2st64_b32 v[40:41], v21 offset0:4 offset1:5             // 0000000002A0: 
0xD8700706, 0x2A000016, // ds_read2st64_b32 v[42:43], v22 offset0:6 offset1:7             // 0000000002A8: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000002B0: 
0xBF8A0000,             // s_barrier                                                      // 0000000002B4: 
0xBF8C0F74,             // s_waitcnt vmcnt(4)                                             // 0000000002B8: 
0xD81A0000, 0x0000120D, // ds_write_b32 v13, v18                                          // 0000000002BC: 
0xE0501000, 0x80011214, // buffer_load_dword v18, v20, s[4:7], 0 offen                    // 0000000002C4: 
0xBF8C0F73,             // s_waitcnt vmcnt(3)                                             // 0000000002CC: 
0xD2A0001C, 0x00021106, // v_pack_b32_f16 v28, v6, v8                                     // 0000000002D0: 
0xD2A0181D, 0x00021106, // v_pack_b32_f16 v29, v6, v8 op_sel:[1,1,0]                      // 0000000002D8: 
0xD2A0001E, 0x00021307, // v_pack_b32_f16 v30, v7, v9                                     // 0000000002E0: 
0xD2A0181F, 0x00021307, // v_pack_b32_f16 v31, v7, v9 op_sel:[1,1,0]                      // 0000000002E8: 
0xBF8CC37F,             // s_waitcnt lgkmcnt(3)                                           // 0000000002F0: 
0x7E0E0326,             // v_mov_b32_e32 v7, v38                                          // 0000000002F4: 
0x7E0C0324,             // v_mov_b32_e32 v6, v36                                          // 0000000002F8: 
0xBF8C0F71,             // s_waitcnt vmcnt(1)                                             // 0000000002FC: 
0xD2A00020, 0x00020902, // v_pack_b32_f16 v32, v2, v4                                     // 000000000300: 
0xD2A01821, 0x00020902, // v_pack_b32_f16 v33, v2, v4 op_sel:[1,1,0]                      // 000000000308: 
0x6804260C,             // v_add_u32_e32 v2, s12, v19                                     // 000000000310: 
0xD2A00022, 0x00020B03, // v_pack_b32_f16 v34, v3, v5                                     // 000000000314: 
0xD2A01823, 0x00020B03, // v_pack_b32_f16 v35, v3, v5 op_sel:[1,1,0]                      // 00000000031C: 
0x7E4C0325,             // v_mov_b32_e32 v38, v37                                         // 000000000324: 
0xD3CD8004, 0x04120D18, // v_mfma_f32_16x16x16f16 a[4:7], v[24:25], v[6:7], a[4:7]        // 000000000328: 
0x810C030C,             // s_add_i32 s12, s12, s3                                         // 000000000330: 
0x680804FF, 0x000A0000, // v_add_u32_e32 v4, 0xa0000, v2                                  // 000000000334: 
0x680A04FF, 0x000A2000, // v_add_u32_e32 v5, 0xa2000, v2                                  // 00000000033C: 
0x682E04FF, 0x000C8000, // v_add_u32_e32 v23, 0xc8000, v2                                 // 000000000344: 
0xBF07FF0C, 0x00BE0000, // s_cmp_lg_u32 s12, 0xbe0000                                     // 00000000034C: 
0x682828C0,             // v_add_u32_e32 v20, 64, v20                                     // 000000000354: 
0xD3CD8000, 0x04024D18, // v_mfma_f32_16x16x16f16 a[0:3], v[24:25], v[38:39], a[0:3]      // 000000000358: 
0x683004FF, 0x000CA000, // v_add_u32_e32 v24, 0xca000, v2                                 // 000000000360: 
0xE0541000, 0x80020604, // buffer_load_dwordx2 v[6:7], v4, s[8:11], 0 offen               // 000000000368: 
0xE0541800, 0x80020805, // buffer_load_dwordx2 v[8:9], v5, s[8:11], 0 offen offset:2048   // 000000000370: 
0xE0541000, 0x80020217, // buffer_load_dwordx2 v[2:3], v23, s[8:11], 0 offen              // 000000000378: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                           // 000000000380: 
0x7E0A032A,             // v_mov_b32_e32 v5, v42                                          // 000000000384: 
0x7E080328,             // v_mov_b32_e32 v4, v40                                          // 000000000388: 
0x7E540329,             // v_mov_b32_e32 v42, v41                                         // 00000000038C: 
0xD9BE0000, 0x00001C0E, // ds_write_b128 v14, v[28:31]                                    // 000000000390: 
0xD3CD8004, 0x0412091A, // v_mfma_f32_16x16x16f16 a[4:7], v[26:27], v[4:5], a[4:7]        // 000000000398: 
0xE0541800, 0x80020418, // buffer_load_dwordx2 v[4:5], v24, s[8:11], 0 offen offset:2048  // 0000000003A0: 
0xD9BE0000, 0x0000200F, // ds_write_b128 v15, v[32:35]                                    // 0000000003A8: 
0xD3CD8000, 0x0402551A, // v_mfma_f32_16x16x16f16 a[0:3], v[26:27], v[42:43], a[0:3]      // 0000000003B0: 
0xBF85FFB1,             // s_cbranch_scc1 BB0_1                                           // 0000000003B8: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000003BC: 
0xBF8A0000,             // s_barrier                                                      // 0000000003C0: 
0xD9FE0000, 0x14000011, // ds_read_b128 v[20:23], v17                                     // 0000000003C4: 
0xD8700100, 0x18000010, // ds_read2st64_b32 v[24:25], v16 offset1:1                       // 0000000003CC: 
0xD86EC484, 0x1C000010, // ds_read2_b32 v[28:29], v16 offset0:132 offset1:196             // 0000000003D4: 
0x682620A0,             // v_add_u32_e32 v19, 32, v16                                     // 0000000003DC: 
0x684420B0,             // v_add_u32_e32 v34, 48, v16                                     // 0000000003E0: 
0x24000082,             // v_lshlrev_b32_e32 v0, 2, v0                                    // 0000000003E4: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                           // 0000000003E8: 
0x7E340318,             // v_mov_b32_e32 v26, v24                                         // 0000000003EC: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000003F0: 
0x7E36031C,             // v_mov_b32_e32 v27, v28                                         // 0000000003F4: 
0x7E380319,             // v_mov_b32_e32 v28, v25                                         // 0000000003F8: 
0xBF800000,             // s_nop 0                                                        // 0000000003FC: 
0xD3CD8004, 0x04123514, // v_mfma_f32_16x16x16f16 a[4:7], v[20:21], v[26:27], a[4:7]      // 000000000400: 
0xD8700504, 0x1A000013, // ds_read2st64_b32 v[26:27], v19 offset0:4 offset1:5             // 000000000408: 
0xD8700706, 0x1E000022, // ds_read2st64_b32 v[30:31], v34 offset0:6 offset1:7             // 000000000410: 
0xB0031400,             // s_movk_i32 s3, 0x1400                                          // 000000000418: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 00000000041C: 
0xBF8A0000,             // s_barrier                                                      // 000000000420: 
0xBF8C0F74,             // s_waitcnt vmcnt(4)                                             // 000000000424: 
0xD81A0000, 0x0000120D, // ds_write_b32 v13, v18                                          // 000000000428: 
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                           // 000000000430: 
0x7E40031A,             // v_mov_b32_e32 v32, v26                                         // 000000000434: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                           // 000000000438: 
0x7E42031E,             // v_mov_b32_e32 v33, v30                                         // 00000000043C: 
0xBF8C0F72,             // s_waitcnt vmcnt(2)                                             // 000000000440: 
0xD2A00018, 0x00021106, // v_pack_b32_f16 v24, v6, v8                                     // 000000000444: 
0xD2A01819, 0x00021106, // v_pack_b32_f16 v25, v6, v8 op_sel:[1,1,0]                      // 00000000044C: 
0x7E3C031B,             // v_mov_b32_e32 v30, v27                                         // 000000000454: 
0xD2A0001A, 0x00021307, // v_pack_b32_f16 v26, v7, v9                                     // 000000000458: 
0xD2A0181B, 0x00021307, // v_pack_b32_f16 v27, v7, v9 op_sel:[1,1,0]                      // 000000000460: 
0xBF8C0F70,             // s_waitcnt vmcnt(0)                                             // 000000000468: 
0xD2A00006, 0x00020902, // v_pack_b32_f16 v6, v2, v4                                      // 00000000046C: 
0xD2A01807, 0x00020902, // v_pack_b32_f16 v7, v2, v4 op_sel:[1,1,0]                       // 000000000474: 
0xD2A00008, 0x00020B03, // v_pack_b32_f16 v8, v3, v5                                      // 00000000047C: 
0xD2A01809, 0x00020B03, // v_pack_b32_f16 v9, v3, v5 op_sel:[1,1,0]                       // 000000000484: 
0xD9BE0000, 0x0000180E, // ds_write_b128 v14, v[24:27]                                    // 00000000048C: 
0xD9BE0000, 0x0000060F, // ds_write_b128 v15, v[6:9]                                      // 000000000494: 
0xD3CD8004, 0x04124116, // v_mfma_f32_16x16x16f16 a[4:7], v[22:23], v[32:33], a[4:7]      // 00000000049C: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000004A4: 
0xBF8A0000,             // s_barrier                                                      // 0000000004A8: 
0xD9FE0000, 0x02000011, // ds_read_b128 v[2:5], v17                                       // 0000000004AC: 
0xD8700100, 0x06000010, // ds_read2st64_b32 v[6:7], v16 offset1:1                         // 0000000004B4: 
0xD86EC484, 0x08000010, // ds_read2_b32 v[8:9], v16 offset0:132 offset1:196               // 0000000004BC: 
0xD8700504, 0x10000013, // ds_read2st64_b32 v[16:17], v19 offset0:4 offset1:5             // 0000000004C4: 
0xD8700706, 0x12000022, // ds_read2st64_b32 v[18:19], v34 offset0:6 offset1:7             // 0000000004CC: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000004D4: 
0xBF8A0000,             // s_barrier                                                      // 0000000004D8: 
0xD3CD8000, 0x04023914, // v_mfma_f32_16x16x16f16 a[0:3], v[20:21], v[28:29], a[0:3]      // 0000000004DC: 
0xBF8CC37F,             // s_waitcnt lgkmcnt(3)                                           // 0000000004E4: 
0x7E1C0306,             // v_mov_b32_e32 v14, v6                                          // 0000000004E8: 
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                           // 0000000004EC: 
0x7E1E0308,             // v_mov_b32_e32 v15, v8                                          // 0000000004F0: 
0x7E100307,             // v_mov_b32_e32 v8, v7                                           // 0000000004F4: 
0x260C188C,             // v_and_b32_e32 v6, 12, v12                                      // 0000000004F8: 
0xD200000C, 0x04011106, // v_lshl_or_b32 v12, v6, 8, v0                                   // 0000000004FC: 
0x24000284,             // v_lshlrev_b32_e32 v0, 4, v1                                    // 000000000504: 
0xD3CD8004, 0x04121D02, // v_mfma_f32_16x16x16f16 a[4:7], v[2:3], v[14:15], a[4:7]        // 000000000508: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000510: 
0x7E1E0312,             // v_mov_b32_e32 v15, v18                                         // 000000000514: 
0x7E1C0310,             // v_mov_b32_e32 v14, v16                                         // 000000000518: 
0x7E240311,             // v_mov_b32_e32 v18, v17                                         // 00000000051C: 
0xD3CD8000, 0x04023D16, // v_mfma_f32_16x16x16f16 a[0:3], v[22:23], v[30:31], a[0:3]      // 000000000520: 
0xD3CD8004, 0x04121D04, // v_mfma_f32_16x16x16f16 a[4:7], v[4:5], v[14:15], a[4:7]        // 000000000528: 
0xD3CD8000, 0x04021102, // v_mfma_f32_16x16x16f16 a[0:3], v[2:3], v[8:9], a[0:3]          // 000000000530: 
0xD2000002, 0x0401110B, // v_lshl_or_b32 v2, v11, 8, v0                                   // 000000000538: 
0xBF800007,             // s_nop 7                                                        // 000000000540: 
0xBF800000,             // s_nop 0                                                        // 000000000544: 
0xDA1A0000, 0x0000040C, // ds_write_b32 v12, a4                                           // 000000000548: 
0xDA1A0100, 0x0000050C, // ds_write_b32 v12, a5 offset:256                                // 000000000550: 
0xDA1A0200, 0x0000060C, // ds_write_b32 v12, a6 offset:512                                // 000000000558: 
0xDA1A0300, 0x0000070C, // ds_write_b32 v12, a7 offset:768                                // 000000000560: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000568: 
0xBF8A0000,             // s_barrier                                                      // 00000000056C: 
0xD9FE0000, 0x06000002, // ds_read_b128 v[6:9], v2                                        // 000000000570: 
0xD3CD8000, 0x04022504, // v_mfma_f32_16x16x16f16 a[0:3], v[4:5], v[18:19], a[0:3]        // 000000000578: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 000000000580: 
0x7E001506,             // v_cvt_f16_f32_e32 v0, v6                                       // 000000000584: 
0x7E061508,             // v_cvt_f16_f32_e32 v3, v8                                       // 000000000588: 
0x7E0C1509,             // v_cvt_f16_f32_e32 v6, v9                                       // 00000000058C: 
0x7E0E1507,             // v_cvt_f16_f32_e32 v7, v7                                       // 000000000590: 
0x24100282,             // v_lshlrev_b32_e32 v8, 2, v1                                    // 000000000594: 
0xD2A00001, 0x00020D03, // v_pack_b32_f16 v1, v3, v6                                      // 000000000598: 
0xD2850003, 0x0000070A, // v_mul_lo_u32 v3, v10, s3                                       // 0000000005A0: 
0x68060602,             // v_add_u32_e32 v3, s2, v3                                       // 0000000005A8: 
0xD2A00000, 0x00020F00, // v_pack_b32_f16 v0, v0, v7                                      // 0000000005AC: 
0xBE8300FF, 0x00020000, // s_mov_b32 s3, 0x20000                                          // 0000000005B4: 
0xBE8200FF, 0x00050000, // s_mov_b32 s2, 0x50000                                          // 0000000005BC: 
0xD1FE0004, 0x02061103, // v_add_lshl_u32 v4, v3, v8, 1                                   // 0000000005C4: 
0xE0741000, 0x80000004, // buffer_store_dwordx2 v[0:1], v4, s[0:3], 0 offen               // 0000000005CC: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000005D4: 
0xBF8A0000,             // s_barrier                                                      // 0000000005D8: 
0xDA1A0000, 0x0000000C, // ds_write_b32 v12, a0                                           // 0000000005DC: 
0xDA1A0100, 0x0000010C, // ds_write_b32 v12, a1 offset:256                                // 0000000005E4: 
0xDA1A0200, 0x0000020C, // ds_write_b32 v12, a2 offset:512                                // 0000000005EC: 
0xDA1A0300, 0x0000030C, // ds_write_b32 v12, a3 offset:768                                // 0000000005F4: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 0000000005FC: 
0xBF8A0000,             // s_barrier                                                      // 000000000600: 
0xD9FE0000, 0x00000002, // ds_read_b128 v[0:3], v2                                        // 000000000604: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                           // 00000000060C: 
0x7E001500,             // v_cvt_f16_f32_e32 v0, v0                                       // 000000000610: 
0x7E0A1501,             // v_cvt_f16_f32_e32 v5, v1                                       // 000000000614: 
0x7E041502,             // v_cvt_f16_f32_e32 v2, v2                                       // 000000000618: 
0x7E061503,             // v_cvt_f16_f32_e32 v3, v3                                       // 00000000061C: 
0xD2A00000, 0x00020B00, // v_pack_b32_f16 v0, v0, v5                                      // 000000000620: 
0xD2A00001, 0x00020702, // v_pack_b32_f16 v1, v2, v3                                      // 000000000628: 
0xE0741080, 0x80000004, // buffer_store_dwordx2 v[0:1], v4, s[0:3], 0 offen offset:128    // 000000000630: 
0xBF810000              // s_endpgm                                                       // 000000000638: 
};

const uint32_t GEMM_CK_ISA_16_1152_5120[] = {
// Original ISA:
// s4-s5 : kernarg pointer
// s6 : workgroup_id_x

// Hacked ISA
// s0-s1: matrix C pointer
// s2-s3: matrix A pointer
// s4-s5: matrix B pointer
// s6 : workgroup_id_x
0xBE8C0002,             // s_mov_b32 s12, s2
0xBE8D0003,             // s_mov_b32 s13, s3
0xBE880004,             // s_mov_b32 s8, s4
0xBE890005,             // s_mov_b32 s9, s5
0xBF800000,             // s_nop 0
0xBF800000,             // s_nop 0

0x9682FF06, 0x38E38E39, // s_mul_hi_i32 s2, s6, 0x38e38e39                                 // 000000000000: 
0x20040082,             // v_lshrrev_b32_e32 v2, 2, v0                                     // 000000000008: 
0x204A0084,             // v_lshrrev_b32_e32 v37, 4, v0                                    // 00000000000C: 
0x8F039F02,             // s_lshr_b32 s3, s2, 31                                           // 000000000010: 
0x90028102,             // s_ashr_i32 s2, s2, 1                                            // 000000000014: 
0x81110302,             // s_add_i32 s17, s2, s3                                           // 000000000018: 
0xD1C20024, 0x04018902, // v_mad_i32_i24 v36, v2, -4, v0                                   // 00000000001C: 
0xD1C20026, 0x04098925, // v_mad_i32_i24 v38, v37, -4, v2                                  // 000000000024: 
0x24064881,             // v_lshlrev_b32_e32 v3, 1, v36                                    // 00000000002C: 
0x20460085,             // v_lshrrev_b32_e32 v35, 5, v0                                    // 000000000030: 
0x9203FF11, 0x00000280, // s_mul_i32 s3, s17, 0x280                                        // 000000000034: 
0x24024C83,             // v_lshlrev_b32_e32 v1, 3, v38                                    // 00000000003C: 

//0xC0060302, 0x00000000, // s_load_dwordx2 s[12:13], s[4:5], 0x0                            // 000000000040: 
//0xC0060202, 0x00000008, // s_load_dwordx2 s[8:9], s[4:5], 0x8                              // 000000000048: 
//0xC0060002, 0x00000010, // s_load_dwordx2 s[0:1], s[4:5], 0x10                             // 000000000050: 

0x92028911,             // s_mul_i32 s2, s17, 9                                            // 000000000058: 
0xB004FFE0,             // s_movk_i32 s4, 0xffe0                                           // 00000000005C: 
0xD1FF0001, 0x040C0701, // v_add3_u32 v1, v1, s3, v3                                       // 000000000060: 
0xD2000006, 0x000D0523, // v_lshl_or_b32 v6, v35, 2, s3                                    // 000000000068: 
0xB0030480,             // s_movk_i32 s3, 0x480                                            // 000000000070: 
0x81820206,             // s_sub_i32 s2, s6, s2                                            // 000000000074: 
0xD1C20005, 0x04000923, // v_mad_i32_i24 v5, v35, s4, v0                                   // 000000000078: 
0xD2850006, 0x00000706, // v_mul_lo_u32 v6, v6, s3                                         // 000000000080: 
0x8E028702,             // s_lshl_b32 s2, s2, 7                                            // 000000000088: 
0xD1FD0006, 0x04190505, // v_lshl_add_u32 v6, v5, 2, v6                                    // 00000000008C: 
0x10084AFF, 0x00001400, // v_mul_u32_u24_e32 v4, 0x1400, v37                               // 000000000094: 
0xBE8F00FF, 0x00020000, // s_mov_b32 s15, 0x20000                                          // 00000000009C: 
0xD1FE0027, 0x02040506, // v_add_lshl_u32 v39, v6, s2, 1                                   // 0000000000A4: 
0xB0031000,             // s_movk_i32 s3, 0x1000                                           // 0000000000AC: 
0xD1FE0007, 0x02060901, // v_add_lshl_u32 v7, v1, v4, 1                                    // 0000000000B0: 
0xBE8E00FF, 0x00028000, // s_mov_b32 s14, 0x28000                                          // 0000000000B8: 
0xBE8A00FF, 0x00B40000, // s_mov_b32 s10, 0xb40000                                         // 0000000000C0: 
0xBE8B000F,             // s_mov_b32 s11, s15                                              // 0000000000C8: 
0x68024E03,             // v_add_u32_e32 v1, s3, v39                                       // 0000000000CC: 

//0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 0000000000D0: 
0xBF800000,             // s_nop 0

0xE0501000, 0x80031F07, // buffer_load_dword v31, v7, s[12:15], 0 offen                    // 0000000000D4: 
0xBF800000,             // s_nop 0                                                         // 0000000000DC: 
0xE0541000, 0x80020827, // buffer_load_dwordx2 v[8:9], v39, s[8:11], 0 offen               // 0000000000E0: 
0xE0541900, 0x80021627, // buffer_load_dwordx2 v[22:23], v39, s[8:11], 0 offen offset:2304 // 0000000000E8: 
0xE0541B00, 0x80021801, // buffer_load_dwordx2 v[24:25], v1, s[8:11], 0 offen offset:2816  // 0000000000F0: 
0xE0541200, 0x80021C01, // buffer_load_dwordx2 v[28:29], v1, s[8:11], 0 offen offset:512   // 0000000000F8: 
0xB0100088,             // s_movk_i32 s16, 0x88                                            // 000000000100: 
0x2634048C,             // v_and_b32_e32 v26, 12, v2                                       // 000000000104: 
0xD2850002, 0x00002126, // v_mul_lo_u32 v2, v38, s16                                       // 000000000108: 
0x260800BF,             // v_and_b32_e32 v4, 63, v0                                        // 000000000110: 
0x260C00B0,             // v_and_b32_e32 v6, 48, v0                                        // 000000000114: 
0xD1FD0002, 0x04090725, // v_lshl_add_u32 v2, v37, 3, v2                                   // 000000000118: 
0xBE8600FF, 0x00024000, // s_mov_b32 s6, 0x24000                                           // 000000000120: 
0xD1C8000A, 0x02090900, // v_bfe_u32 v10, v0, 4, 2                                         // 000000000128: 
0x6A080D04,             // v_sub_u32_e32 v4, v4, v6                                        // 000000000130: 
0xBE8400FF, 0x00012000, // s_mov_b32 s4, 0x12000                                           // 000000000134: 
0x20020086,             // v_lshrrev_b32_e32 v1, 6, v0                                     // 00000000013C: 
0xBE8500FF, 0x00013000, // s_mov_b32 s5, 0x13000                                           // 000000000140: 
0x10521410,             // v_mul_u32_u24_e32 v41, s16, v10                                 // 000000000148: 
0xBE8700FF, 0x00025000, // s_mov_b32 s7, 0x25000                                           // 00000000014C: 
0x105046FF, 0x00000220, // v_mul_u32_u24_e32 v40, 0x220, v35                               // 000000000154: 
0x24540A83,             // v_lshlrev_b32_e32 v42, 3, v5                                    // 00000000015C: 
0x24560883,             // v_lshlrev_b32_e32 v43, 3, v4                                    // 000000000160: 
0xD1FD001B, 0x04110901, // v_lshl_add_u32 v27, v1, 4, v4                                   // 000000000164: 
0x100834FF, 0x00000110, // v_mul_u32_u24_e32 v4, 0x110, v26                                // 00000000016C: 
0xD1FE001E, 0x02060702, // v_add_lshl_u32 v30, v2, v3, 1                                   // 000000000174: 
0x680C4E04,             // v_add_u32_e32 v6, s4, v39                                       // 00000000017C: 
0x68144E05,             // v_add_u32_e32 v10, s5, v39                                      // 000000000180: 
0x68404E06,             // v_add_u32_e32 v32, s6, v39                                      // 000000000184: 
0xBF800000,             // s_nop 0                                                         // 000000000188: 
0xE0501040, 0x80032207, // buffer_load_dword v34, v7, s[12:15], 0 offen offset:64          // 00000000018C: 
0xD1FD002C, 0x0411031B, // v_lshl_add_u32 v44, v27, 1, v4                                  // 000000000194: 
0xBF800000,             // s_nop 0                                                         // 00000000019C: 
0xE0541000, 0x80020206, // buffer_load_dwordx2 v[2:3], v6, s[8:11], 0 offen                // 0000000001A0: 
0xE0541900, 0x80020406, // buffer_load_dwordx2 v[4:5], v6, s[8:11], 0 offen offset:2304    // 0000000001A8: 
0xE0541B00, 0x8002120A, // buffer_load_dwordx2 v[18:19], v10, s[8:11], 0 offen offset:2816 // 0000000001B0: 
0xE0541200, 0x8002140A, // buffer_load_dwordx2 v[20:21], v10, s[8:11], 0 offen offset:512  // 0000000001B8: 
0xBF800000,             // s_nop 0                                                         // 0000000001C0: 
0xE0501080, 0x80032107, // buffer_load_dword v33, v7, s[12:15], 0 offen offset:128         // 0000000001C4: 
0x685A4E07,             // v_add_u32_e32 v45, s7, v39                                      // 0000000001CC: 
0xBF800000,             // s_nop 0                                                         // 0000000001D0: 
0xE0541000, 0x80020E20, // buffer_load_dwordx2 v[14:15], v32, s[8:11], 0 offen             // 0000000001D4: 
0xE0541900, 0x80021020, // buffer_load_dwordx2 v[16:17], v32, s[8:11], 0 offen offset:2304 // 0000000001DC: 
0xE0541B00, 0x80020A2D, // buffer_load_dwordx2 v[10:11], v45, s[8:11], 0 offen offset:2816 // 0000000001E4: 
0xE0541200, 0x80020C2D, // buffer_load_dwordx2 v[12:13], v45, s[8:11], 0 offen offset:512  // 0000000001EC: 
0xBF800000,             // s_nop 0                                                         // 0000000001F4: 
0xE05010C0, 0x80032007, // buffer_load_dword v32, v7, s[12:15], 0 offen offset:192         // 0000000001F8: 
0xB0120430,             // s_movk_i32 s18, 0x430                                           // 000000000200: 
0xBE9000FF, 0x00036000, // s_mov_b32 s16, 0x36000                                          // 000000000204: 
0xD3D94004, 0x18000080, // v_accvgpr_write_b32 a4, 0                                       // 00000000020C: 
0x104A4AFF, 0x00002800, // v_mul_u32_u24_e32 v37, 0x2800, v37                              // 000000000214: 
0x244C4C84,             // v_lshlrev_b32_e32 v38, 4, v38                                   // 00000000021C: 
0x24484882,             // v_lshlrev_b32_e32 v36, 2, v36                                   // 000000000220: 
0x7E0AA504,             // v_accvgpr_mov_b32 a5, a4                                        // 000000000224: 
0xB0132400,             // s_movk_i32 s19, 0x2400                                          // 000000000228: 
0x7E0CA504,             // v_accvgpr_mov_b32 a6, a4                                        // 00000000022C: 
0x7E0EA504,             // v_accvgpr_mov_b32 a7, a4                                        // 000000000230: 
0xD3D94000, 0x18000080, // v_accvgpr_write_b32 a0, 0                                       // 000000000234: 
0xBF8C0F7F,             // s_waitcnt vmcnt(15)                                             // 00000000023C: 
0xD81A0000, 0x00001F1E, // ds_write_b32 v30, v31                                           // 000000000240: 
0xD1FE001F, 0x0206532B, // v_add_lshl_u32 v31, v43, v41, 1                                 // 000000000248: 
0xBF8C0F7D,             // s_waitcnt vmcnt(13)                                             // 000000000250: 
0xD2A00006, 0x00022D08, // v_pack_b32_f16 v6, v8, v22                                      // 000000000254: 
0xD2A01807, 0x00022D08, // v_pack_b32_f16 v7, v8, v22 op_sel:[1,1,0]                       // 00000000025C: 
0xD2A00008, 0x00022F09, // v_pack_b32_f16 v8, v9, v23                                      // 000000000264: 
0xD2A01809, 0x00022F09, // v_pack_b32_f16 v9, v9, v23 op_sel:[1,1,0]                       // 00000000026C: 
0xBF8C0F7B,             // s_waitcnt vmcnt(11)                                             // 000000000274: 
0xD2A00016, 0x0002311C, // v_pack_b32_f16 v22, v28, v24                                    // 000000000278: 
0xD2A01817, 0x0002311C, // v_pack_b32_f16 v23, v28, v24 op_sel:[1,1,0]                     // 000000000280: 
0xD2A00018, 0x0002331D, // v_pack_b32_f16 v24, v29, v25                                    // 000000000288: 
0xD2A01819, 0x0002331D, // v_pack_b32_f16 v25, v29, v25 op_sel:[1,1,0]                     // 000000000290: 
0xD1FE001D, 0x02065528, // v_add_lshl_u32 v29, v40, v42, 1                                 // 000000000298: 
0xD9BE0650, 0x0000161D, // ds_write_b128 v29, v[22:25] offset:1616                         // 0000000002A0: 
0xD9BE0430, 0x0000061D, // ds_write_b128 v29, v[6:9] offset:1072                           // 0000000002A8: 
0x68383A12,             // v_add_u32_e32 v28, s18, v29                                     // 0000000002B0: 
0x682C4E10,             // v_add_u32_e32 v22, s16, v39                                     // 0000000002B4: 
0x683A4EFF, 0x00037000, // v_add_u32_e32 v29, 0x37000, v39                                 // 0000000002B8: 
0xE0541000, 0x80020616, // buffer_load_dwordx2 v[6:7], v22, s[8:11], 0 offen               // 0000000002C0: 
0xE0541900, 0x80020816, // buffer_load_dwordx2 v[8:9], v22, s[8:11], 0 offen offset:2304   // 0000000002C8: 
0xBF800000,             // s_nop 0                                                         // 0000000002D0: 
0xE0541B00, 0x8002161D, // buffer_load_dwordx2 v[22:23], v29, s[8:11], 0 offen offset:2816 // 0000000002D4: 
0xE0541200, 0x8002181D, // buffer_load_dwordx2 v[24:25], v29, s[8:11], 0 offen offset:512  // 0000000002DC: 
0xD1FD001D, 0x0049032C, // v_lshl_add_u32 v29, v44, 1, s18                                 // 0000000002E4: 
0x9212FF11, 0x00168000, // s_mul_i32 s18, s17, 0x168000                                    // 0000000002EC: 
0xB7910500,             // s_mulk_i32 s17, 0x500                                           // 0000000002F4: 
0xD1FF0025, 0x04982325, // v_add3_u32 v37, v37, s17, v38                                   // 0000000002F8: 
0xB0110140,             // s_movk_i32 s17, 0x140                                           // 000000000300: 
0x7E4E0212,             // v_mov_b32_e32 v39, s18                                          // 000000000304: 
0xD1FF0024, 0x00464925, // v_add3_u32 v36, v37, v36, s17                                   // 000000000308: 
0xD3D84025, 0x18000105, // v_accvgpr_read_b32 v37, a5                                      // 000000000310: 
0xD3D94001, 0x18000125, // v_accvgpr_write_b32 a1, v37                                     // 000000000318: 
0xD3D84025, 0x18000106, // v_accvgpr_read_b32 v37, a6                                      // 000000000320: 
0xD1C30023, 0x049C2723, // v_mad_u32_u24 v35, v35, s19, v39                                // 000000000328: 
0x8E128102,             // s_lshl_b32 s18, s2, 1                                           // 000000000330: 
0xD1FF0023, 0x004A5523, // v_add3_u32 v35, v35, v42, s18                                   // 000000000334: 
0xD3D94002, 0x18000125, // v_accvgpr_write_b32 a2, v37                                     // 00000000033C: 
0xD3D84025, 0x18000107, // v_accvgpr_read_b32 v37, a7                                      // 000000000344: 
0x684646FF, 0x00048000, // v_add_u32_e32 v35, 0x48000, v35                                 // 00000000034C: 
0xBE9100C3,             // s_mov_b32 s17, -3                                               // 000000000354: 
0xD3D94003, 0x18000125, // v_accvgpr_write_b32 a3, v37                                     // 000000000358: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0) // 360 <BB0_1>                             // 000000000360: 
0xBF8A0000,             // s_barrier                                                       // 000000000364: 
0xD9FE0000, 0x2600001F, // ds_read_b128 v[38:41], v31                                      // 000000000368: 
0xD8700100, 0x2A00001D, // ds_read2st64_b32 v[42:43], v29 offset1:1                        // 000000000370: 
0xD86EC888, 0x2C00001D, // ds_read2_b32 v[44:45], v29 offset0:136 offset1:200              // 000000000378: 
0x68683AFF, 0x00000060, // v_add_u32_e32 v52, 0x60, v29                                    // 000000000380: 
0x684A3AC0,             // v_add_u32_e32 v37, 64, v29                                      // 000000000388: 
0xD8700706, 0x30000034, // ds_read2st64_b32 v[48:49], v52 offset0:6 offset1:7              // 00000000038C: 
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                            // 000000000394: 
0x7E5C032A,             // v_mov_b32_e32 v46, v42                                          // 000000000398: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 00000000039C: 
0x7E5E032C,             // v_mov_b32_e32 v47, v44                                          // 0000000003A0: 
0x7E58032B,             // v_mov_b32_e32 v44, v43                                          // 0000000003A4: 
0xBF800000,             // s_nop 0                                                         // 0000000003A8: 
0xD3CD8004, 0x04125D26, // v_mfma_f32_16x16x16f16 a[4:7], v[38:39], v[46:47], a[4:7]       // 0000000003AC: 
0xD8700504, 0x2E000025, // ds_read2st64_b32 v[46:47], v37 offset0:4 offset1:5              // 0000000003B4: 
0x81118311,             // s_add_i32 s17, s17, 3                                           // 0000000003BC: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 0000000003C0: 
0x7E660330,             // v_mov_b32_e32 v51, v48                                          // 0000000003C4: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 0000000003C8: 
0xBF8A0000,             // s_barrier                                                       // 0000000003CC: 
0xBF8C0F7E,             // s_waitcnt vmcnt(14)                                             // 0000000003D0: 
0xD81A0000, 0x0000221E, // ds_write_b32 v30, v34                                           // 0000000003D4: 
0x6C4448C0,             // v_subrev_u32_e32 v34, 64, v36                                   // 0000000003DC: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 0000000003E0: 
0x7E64032E,             // v_mov_b32_e32 v50, v46                                          // 0000000003E4: 
0x7E60032F,             // v_mov_b32_e32 v48, v47                                          // 0000000003E8: 
0xBF0A8B11,             // s_cmp_lt_u32 s17, 11                                            // 0000000003EC: 
0xD3CD8000, 0x04025926, // v_mfma_f32_16x16x16f16 a[0:3], v[38:39], v[44:45], a[0:3]       // 0000000003F0: 
0xE0501000, 0x80032222, // buffer_load_dword v34, v34, s[12:15], 0 offen                   // 0000000003F8: 
0xBF8C0F7D,             // s_waitcnt vmcnt(13)                                             // 000000000400: 
0xD2A00026, 0x00020902, // v_pack_b32_f16 v38, v2, v4                                      // 000000000404: 
0xD2A01827, 0x00020902, // v_pack_b32_f16 v39, v2, v4 op_sel:[1,1,0]                       // 00000000040C: 
0xD3CD8004, 0x04126528, // v_mfma_f32_16x16x16f16 a[4:7], v[40:41], v[50:51], a[4:7]       // 000000000414: 
0xD3CD8000, 0x04026128, // v_mfma_f32_16x16x16f16 a[0:3], v[40:41], v[48:49], a[0:3]       // 00000000041C: 
0xD2A00028, 0x00020B03, // v_pack_b32_f16 v40, v3, v5                                      // 000000000424: 
0xD2A01829, 0x00020B03, // v_pack_b32_f16 v41, v3, v5 op_sel:[1,1,0]                       // 00000000042C: 
0xBF8C0F7B,             // s_waitcnt vmcnt(11)                                             // 000000000434: 
0xD2A00002, 0x00022514, // v_pack_b32_f16 v2, v20, v18                                     // 000000000438: 
0xD2A01803, 0x00022514, // v_pack_b32_f16 v3, v20, v18 op_sel:[1,1,0]                      // 000000000440: 
0xD2A00004, 0x00022715, // v_pack_b32_f16 v4, v21, v19                                     // 000000000448: 
0xD2A01805, 0x00022715, // v_pack_b32_f16 v5, v21, v19 op_sel:[1,1,0]                      // 000000000450: 
0xD9BE0220, 0x0000021C, // ds_write_b128 v28, v[2:5] offset:544                            // 000000000458: 
0xD9BE0000, 0x0000261C, // ds_write_b128 v28, v[38:41]                                     // 000000000460: 
0x684C4603,             // v_add_u32_e32 v38, s3, v35                                      // 000000000468: 
0xE0541000, 0x80020223, // buffer_load_dwordx2 v[2:3], v35, s[8:11], 0 offen               // 00000000046C: 
0xE0541900, 0x80020423, // buffer_load_dwordx2 v[4:5], v35, s[8:11], 0 offen offset:2304   // 000000000474: 
0xE0541B00, 0x80021226, // buffer_load_dwordx2 v[18:19], v38, s[8:11], 0 offen offset:2816 // 00000000047C: 
0xE0541200, 0x80021426, // buffer_load_dwordx2 v[20:21], v38, s[8:11], 0 offen offset:512  // 000000000484: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 00000000048C: 
0xBF8A0000,             // s_barrier                                                       // 000000000490: 
0xD9FE0000, 0x2600001F, // ds_read_b128 v[38:41], v31                                      // 000000000494: 
0xD8700100, 0x2A00001D, // ds_read2st64_b32 v[42:43], v29 offset1:1                        // 00000000049C: 
0xD86EC888, 0x2C00001D, // ds_read2_b32 v[44:45], v29 offset0:136 offset1:200              // 0000000004A4: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 0000000004AC: 
0x7E5C032A,             // v_mov_b32_e32 v46, v42                                          // 0000000004B0: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 0000000004B4: 
0x7E5E032C,             // v_mov_b32_e32 v47, v44                                          // 0000000004B8: 
0x7E58032B,             // v_mov_b32_e32 v44, v43                                          // 0000000004BC: 
0xBF800000,             // s_nop 0                                                         // 0000000004C0: 
0xD3CD8004, 0x04125D26, // v_mfma_f32_16x16x16f16 a[4:7], v[38:39], v[46:47], a[4:7]       // 0000000004C4: 
0xD8700504, 0x2E000025, // ds_read2st64_b32 v[46:47], v37 offset0:4 offset1:5              // 0000000004CC: 
0xD8700706, 0x30000034, // ds_read2st64_b32 v[48:49], v52 offset0:6 offset1:7              // 0000000004D4: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 0000000004DC: 
0xBF8A0000,             // s_barrier                                                       // 0000000004E0: 
0xBF8C0F7E,             // s_waitcnt vmcnt(14)                                             // 0000000004E4: 
0xD81A0000, 0x0000211E, // ds_write_b32 v30, v33                                           // 0000000004E8: 
0xE0501000, 0x80032124, // buffer_load_dword v33, v36, s[12:15], 0 offen                   // 0000000004F0: 
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                            // 0000000004F8: 
0x7E64032E,             // v_mov_b32_e32 v50, v46                                          // 0000000004FC: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 000000000500: 
0x7E660330,             // v_mov_b32_e32 v51, v48                                          // 000000000504: 
0x7E60032F,             // v_mov_b32_e32 v48, v47                                          // 000000000508: 
0xD3CD8000, 0x04025926, // v_mfma_f32_16x16x16f16 a[0:3], v[38:39], v[44:45], a[0:3]       // 00000000050C: 
0xBF8C0F7D,             // s_waitcnt vmcnt(13)                                             // 000000000514: 
0xD2A00026, 0x0002210E, // v_pack_b32_f16 v38, v14, v16                                    // 000000000518: 
0xD2A01827, 0x0002210E, // v_pack_b32_f16 v39, v14, v16 op_sel:[1,1,0]                     // 000000000520: 
0xD3CD8004, 0x04126528, // v_mfma_f32_16x16x16f16 a[4:7], v[40:41], v[50:51], a[4:7]       // 000000000528: 
0xD3CD8000, 0x04026128, // v_mfma_f32_16x16x16f16 a[0:3], v[40:41], v[48:49], a[0:3]       // 000000000530: 
0xD2A00028, 0x0002230F, // v_pack_b32_f16 v40, v15, v17                                    // 000000000538: 
0xD2A01829, 0x0002230F, // v_pack_b32_f16 v41, v15, v17 op_sel:[1,1,0]                     // 000000000540: 
0xBF8C0F7B,             // s_waitcnt vmcnt(11)                                             // 000000000548: 
0xD2A0000E, 0x0002150C, // v_pack_b32_f16 v14, v12, v10                                    // 00000000054C: 
0xD2A0180F, 0x0002150C, // v_pack_b32_f16 v15, v12, v10 op_sel:[1,1,0]                     // 000000000554: 
0xD2A00010, 0x0002170D, // v_pack_b32_f16 v16, v13, v11                                    // 00000000055C: 
0xD2A01811, 0x0002170D, // v_pack_b32_f16 v17, v13, v11 op_sel:[1,1,0]                     // 000000000564: 
0xD9BE0220, 0x00000E1C, // ds_write_b128 v28, v[14:17] offset:544                          // 00000000056C: 
0xD9BE0000, 0x0000261C, // ds_write_b128 v28, v[38:41]                                     // 000000000574: 
0x68144604,             // v_add_u32_e32 v10, s4, v35                                      // 00000000057C: 
0x684C4605,             // v_add_u32_e32 v38, s5, v35                                      // 000000000580: 
0xE0541000, 0x80020E0A, // buffer_load_dwordx2 v[14:15], v10, s[8:11], 0 offen             // 000000000584: 
0xE0541900, 0x8002100A, // buffer_load_dwordx2 v[16:17], v10, s[8:11], 0 offen offset:2304 // 00000000058C: 
0xBF800000,             // s_nop 0                                                         // 000000000594: 
0xE0541B00, 0x80020A26, // buffer_load_dwordx2 v[10:11], v38, s[8:11], 0 offen offset:2816 // 000000000598: 
0xE0541200, 0x80020C26, // buffer_load_dwordx2 v[12:13], v38, s[8:11], 0 offen offset:512  // 0000000005A0: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 0000000005A8: 
0xBF8A0000,             // s_barrier                                                       // 0000000005AC: 
0xD9FE0000, 0x2600001F, // ds_read_b128 v[38:41], v31                                      // 0000000005B0: 
0xD8700100, 0x2A00001D, // ds_read2st64_b32 v[42:43], v29 offset1:1                        // 0000000005B8: 
0xD86EC888, 0x2C00001D, // ds_read2_b32 v[44:45], v29 offset0:136 offset1:200              // 0000000005C0: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 0000000005C8: 
0x7E5C032A,             // v_mov_b32_e32 v46, v42                                          // 0000000005CC: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 0000000005D0: 
0x7E5E032C,             // v_mov_b32_e32 v47, v44                                          // 0000000005D4: 
0x7E58032B,             // v_mov_b32_e32 v44, v43                                          // 0000000005D8: 
0xBF800000,             // s_nop 0                                                         // 0000000005DC: 
0xD3CD8004, 0x04125D26, // v_mfma_f32_16x16x16f16 a[4:7], v[38:39], v[46:47], a[4:7]       // 0000000005E0: 
0xD8700504, 0x2E000025, // ds_read2st64_b32 v[46:47], v37 offset0:4 offset1:5              // 0000000005E8: 
0xD8700706, 0x30000034, // ds_read2st64_b32 v[48:49], v52 offset0:6 offset1:7              // 0000000005F0: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 0000000005F8: 
0xBF8A0000,             // s_barrier                                                       // 0000000005FC: 
0xBF8C0F7E,             // s_waitcnt vmcnt(14)                                             // 000000000600: 
0xD81A0000, 0x0000201E, // ds_write_b32 v30, v32                                           // 000000000604: 
0xE0501040, 0x80032024, // buffer_load_dword v32, v36, s[12:15], 0 offen offset:64         // 00000000060C: 
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                            // 000000000614: 
0x7E64032E,             // v_mov_b32_e32 v50, v46                                          // 000000000618: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 00000000061C: 
0x7E660330,             // v_mov_b32_e32 v51, v48                                          // 000000000620: 
0x7E60032F,             // v_mov_b32_e32 v48, v47                                          // 000000000624: 
0x684A4607,             // v_add_u32_e32 v37, s7, v35                                      // 000000000628: 
0xD3CD8000, 0x04025926, // v_mfma_f32_16x16x16f16 a[0:3], v[38:39], v[44:45], a[0:3]       // 00000000062C: 
0xBF8C0F7D,             // s_waitcnt vmcnt(13)                                             // 000000000634: 
0xD2A00026, 0x00021106, // v_pack_b32_f16 v38, v6, v8                                      // 000000000638: 
0xD2A01827, 0x00021106, // v_pack_b32_f16 v39, v6, v8 op_sel:[1,1,0]                       // 000000000640: 
0x684848FF, 0x000000C0, // v_add_u32_e32 v36, 0xc0, v36                                    // 000000000648: 
0xD3CD8004, 0x04126528, // v_mfma_f32_16x16x16f16 a[4:7], v[40:41], v[50:51], a[4:7]       // 000000000650: 
0xD3CD8000, 0x04026128, // v_mfma_f32_16x16x16f16 a[0:3], v[40:41], v[48:49], a[0:3]       // 000000000658: 
0xD2A00028, 0x00021307, // v_pack_b32_f16 v40, v7, v9                                      // 000000000660: 
0xD2A01829, 0x00021307, // v_pack_b32_f16 v41, v7, v9 op_sel:[1,1,0]                       // 000000000668: 
0xBF8C0F7B,             // s_waitcnt vmcnt(11)                                             // 000000000670: 
0xD2A00006, 0x00022D18, // v_pack_b32_f16 v6, v24, v22                                     // 000000000674: 
0xD2A01807, 0x00022D18, // v_pack_b32_f16 v7, v24, v22 op_sel:[1,1,0]                      // 00000000067C: 
0xD2A00008, 0x00022F19, // v_pack_b32_f16 v8, v25, v23                                     // 000000000684: 
0xD2A01809, 0x00022F19, // v_pack_b32_f16 v9, v25, v23 op_sel:[1,1,0]                      // 00000000068C: 
0x682C4606,             // v_add_u32_e32 v22, s6, v35                                      // 000000000694: 
0xD9BE0220, 0x0000061C, // ds_write_b128 v28, v[6:9] offset:544                            // 000000000698: 
0xD9BE0000, 0x0000261C, // ds_write_b128 v28, v[38:41]                                     // 0000000006A0: 
0xE0541000, 0x80020616, // buffer_load_dwordx2 v[6:7], v22, s[8:11], 0 offen               // 0000000006A8: 
0xE0541900, 0x80020816, // buffer_load_dwordx2 v[8:9], v22, s[8:11], 0 offen offset:2304   // 0000000006B0: 
0xBF800000,             // s_nop 0                                                         // 0000000006B8: 
0xE0541B00, 0x80021625, // buffer_load_dwordx2 v[22:23], v37, s[8:11], 0 offen offset:2816 // 0000000006BC: 
0xE0541200, 0x80021825, // buffer_load_dwordx2 v[24:25], v37, s[8:11], 0 offen offset:512  // 0000000006C4: 
0x68464610,             // v_add_u32_e32 v35, s16, v35                                     // 0000000006CC: 
0xBF85FF23,             // s_cbranch_scc1 BB0_1                                            // 0000000006D0: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 0000000006D4: 
0xBF8A0000,             // s_barrier                                                       // 0000000006D8: 
0xD9FE0000, 0x2600001F, // ds_read_b128 v[38:41], v31                                      // 0000000006DC: 
0xD8700100, 0x2A00001D, // ds_read2st64_b32 v[42:43], v29 offset1:1                        // 0000000006E4: 
0xD86EC888, 0x2C00001D, // ds_read2_b32 v[44:45], v29 offset0:136 offset1:200              // 0000000006EC: 
0x68683AC0,             // v_add_u32_e32 v52, 64, v29                                      // 0000000006F4: 
0x686A3AFF, 0x00000060, // v_add_u32_e32 v53, 0x60, v29                                    // 0000000006F8: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 000000000700: 
0x7E5C032A,             // v_mov_b32_e32 v46, v42                                          // 000000000704: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000000708: 
0x7E5E032C,             // v_mov_b32_e32 v47, v44                                          // 00000000070C: 
0x7E58032B,             // v_mov_b32_e32 v44, v43                                          // 000000000710: 
0xBF800000,             // s_nop 0                                                         // 000000000714: 
0xD3CD8004, 0x04125D26, // v_mfma_f32_16x16x16f16 a[4:7], v[38:39], v[46:47], a[4:7]       // 000000000718: 
0xD8700504, 0x2E000034, // ds_read2st64_b32 v[46:47], v52 offset0:4 offset1:5              // 000000000720: 
0xD8700706, 0x30000035, // ds_read2st64_b32 v[48:49], v53 offset0:6 offset1:7              // 000000000728: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000000730: 
0xBF8A0000,             // s_barrier                                                       // 000000000734: 
0xBF8C0F7E,             // s_waitcnt vmcnt(14)                                             // 000000000738: 
0xD81A0000, 0x0000221E, // ds_write_b32 v30, v34                                           // 00000000073C: 
0x6C4448C0,             // v_subrev_u32_e32 v34, 64, v36                                   // 000000000744: 
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                            // 000000000748: 
0x7E64032E,             // v_mov_b32_e32 v50, v46                                          // 00000000074C: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 000000000750: 
0x7E660330,             // v_mov_b32_e32 v51, v48                                          // 000000000754: 
0x7E60032F,             // v_mov_b32_e32 v48, v47                                          // 000000000758: 
0xB0041000,             // s_movk_i32 s4, 0x1000                                           // 00000000075C: 
0xD3CD8000, 0x04025926, // v_mfma_f32_16x16x16f16 a[0:3], v[38:39], v[44:45], a[0:3]       // 000000000760: 
0xB0031200,             // s_movk_i32 s3, 0x1200                                           // 000000000768: 
0xD3CD8004, 0x04126528, // v_mfma_f32_16x16x16f16 a[4:7], v[40:41], v[50:51], a[4:7]       // 00000000076C: 
0xE0501000, 0x80033222, // buffer_load_dword v50, v34, s[12:15], 0 offen                   // 000000000774: 
0xBF8C0F7D,             // s_waitcnt vmcnt(13)                                             // 00000000077C: 
0xD2A00024, 0x00020902, // v_pack_b32_f16 v36, v2, v4                                      // 000000000780: 
0xD2A01825, 0x00020902, // v_pack_b32_f16 v37, v2, v4 op_sel:[1,1,0]                       // 000000000788: 
0xD2A00026, 0x00020B03, // v_pack_b32_f16 v38, v3, v5                                      // 000000000790: 
0xD2A01827, 0x00020B03, // v_pack_b32_f16 v39, v3, v5 op_sel:[1,1,0]                       // 000000000798: 
0xBF8C0F7B,             // s_waitcnt vmcnt(11)                                             // 0000000007A0: 
0xD2A00002, 0x00022514, // v_pack_b32_f16 v2, v20, v18                                     // 0000000007A4: 
0xD2A01803, 0x00022514, // v_pack_b32_f16 v3, v20, v18 op_sel:[1,1,0]                      // 0000000007AC: 
0xD2A00004, 0x00022715, // v_pack_b32_f16 v4, v21, v19                                     // 0000000007B4: 
0xD2A01805, 0x00022715, // v_pack_b32_f16 v5, v21, v19 op_sel:[1,1,0]                      // 0000000007BC: 
0xD9BE0220, 0x0000021C, // ds_write_b128 v28, v[2:5] offset:544                            // 0000000007C4: 
0xD9BE0000, 0x0000241C, // ds_write_b128 v28, v[36:39]                                     // 0000000007CC: 
0xE0541900, 0x80022423, // buffer_load_dwordx2 v[36:37], v35, s[8:11], 0 offen offset:2304 // 0000000007D4: 
0x68044604,             // v_add_u32_e32 v2, s4, v35                                       // 0000000007DC: 
0xD3CD8000, 0x04026128, // v_mfma_f32_16x16x16f16 a[0:3], v[40:41], v[48:49], a[0:3]       // 0000000007E0: 
0xE0541000, 0x80022623, // buffer_load_dwordx2 v[38:39], v35, s[8:11], 0 offen             // 0000000007E8: 
0xE0541B00, 0x80022802, // buffer_load_dwordx2 v[40:41], v2, s[8:11], 0 offen offset:2816  // 0000000007F0: 
0xE0541200, 0x80022A02, // buffer_load_dwordx2 v[42:43], v2, s[8:11], 0 offen offset:512   // 0000000007F8: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000000800: 
0xBF8A0000,             // s_barrier                                                       // 000000000804: 
0xD9FE0000, 0x0200001F, // ds_read_b128 v[2:5], v31                                        // 000000000808: 
0xD8700100, 0x1200001D, // ds_read2st64_b32 v[18:19], v29 offset1:1                        // 000000000810: 
0xD86EC888, 0x1400001D, // ds_read2_b32 v[20:21], v29 offset0:136 offset1:200              // 000000000818: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 000000000820: 
0x7E440312,             // v_mov_b32_e32 v34, v18                                          // 000000000824: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000000828: 
0x7E460314,             // v_mov_b32_e32 v35, v20                                          // 00000000082C: 
0x7E280313,             // v_mov_b32_e32 v20, v19                                          // 000000000830: 
0xBF800000,             // s_nop 0                                                         // 000000000834: 
0xD3CD8004, 0x04124502, // v_mfma_f32_16x16x16f16 a[4:7], v[2:3], v[34:35], a[4:7]         // 000000000838: 
0xD8700504, 0x22000034, // ds_read2st64_b32 v[34:35], v52 offset0:4 offset1:5              // 000000000840: 
0xD8700706, 0x2C000035, // ds_read2st64_b32 v[44:45], v53 offset0:6 offset1:7              // 000000000848: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000000850: 
0xBF8A0000,             // s_barrier                                                       // 000000000854: 
0xBF8C0F7E,             // s_waitcnt vmcnt(14)                                             // 000000000858: 
0xD81A0000, 0x0000211E, // ds_write_b32 v30, v33                                           // 00000000085C: 
0xBF8C0F7C,             // s_waitcnt vmcnt(12)                                             // 000000000864: 
0xD2A00012, 0x0002210E, // v_pack_b32_f16 v18, v14, v16                                    // 000000000868: 
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                            // 000000000870: 
0x7E5C0322,             // v_mov_b32_e32 v46, v34                                          // 000000000874: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 000000000878: 
0x7E5E032C,             // v_mov_b32_e32 v47, v44                                          // 00000000087C: 
0x7E580323,             // v_mov_b32_e32 v44, v35                                          // 000000000880: 
0xD2A01813, 0x0002210E, // v_pack_b32_f16 v19, v14, v16 op_sel:[1,1,0]                     // 000000000884: 
0xD3CD8000, 0x04022902, // v_mfma_f32_16x16x16f16 a[0:3], v[2:3], v[20:21], a[0:3]         // 00000000088C: 
0xD2A00014, 0x0002230F, // v_pack_b32_f16 v20, v15, v17                                    // 000000000894: 
0xD2A01815, 0x0002230F, // v_pack_b32_f16 v21, v15, v17 op_sel:[1,1,0]                     // 00000000089C: 
0xBF8C0F7A,             // s_waitcnt vmcnt(10)                                             // 0000000008A4: 
0xD2A0000E, 0x0002150C, // v_pack_b32_f16 v14, v12, v10                                    // 0000000008A8: 
0xD2A0180F, 0x0002150C, // v_pack_b32_f16 v15, v12, v10 op_sel:[1,1,0]                     // 0000000008B0: 
0xD2A00010, 0x0002170D, // v_pack_b32_f16 v16, v13, v11                                    // 0000000008B8: 
0xD2A01811, 0x0002170D, // v_pack_b32_f16 v17, v13, v11 op_sel:[1,1,0]                     // 0000000008C0: 
0xD9BE0220, 0x00000E1C, // ds_write_b128 v28, v[14:17] offset:544                          // 0000000008C8: 
0xD9BE0000, 0x0000121C, // ds_write_b128 v28, v[18:21]                                     // 0000000008D0: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 0000000008D8: 
0xBF8A0000,             // s_barrier                                                       // 0000000008DC: 
0xD3CD8004, 0x04125D04, // v_mfma_f32_16x16x16f16 a[4:7], v[4:5], v[46:47], a[4:7]         // 0000000008E0: 
0xD3CD8000, 0x04025904, // v_mfma_f32_16x16x16f16 a[0:3], v[4:5], v[44:45], a[0:3]         // 0000000008E8: 
0xD9FE0000, 0x0200001F, // ds_read_b128 v[2:5], v31                                        // 0000000008F0: 
0xD8700100, 0x0A00001D, // ds_read2st64_b32 v[10:11], v29 offset1:1                        // 0000000008F8: 
0xD86EC888, 0x0E00001D, // ds_read2_b32 v[14:15], v29 offset0:136 offset1:200              // 000000000900: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 000000000908: 
0x7E18030A,             // v_mov_b32_e32 v12, v10                                          // 00000000090C: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000000910: 
0x7E1A030E,             // v_mov_b32_e32 v13, v14                                          // 000000000914: 
0x7E1C030B,             // v_mov_b32_e32 v14, v11                                          // 000000000918: 
0xBF800000,             // s_nop 0                                                         // 00000000091C: 
0xD3CD8004, 0x04121902, // v_mfma_f32_16x16x16f16 a[4:7], v[2:3], v[12:13], a[4:7]         // 000000000920: 
0xD8700504, 0x0C000034, // ds_read2st64_b32 v[12:13], v52 offset0:4 offset1:5              // 000000000928: 
0xD8700706, 0x10000035, // ds_read2st64_b32 v[16:17], v53 offset0:6 offset1:7              // 000000000930: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000000938: 
0xBF8A0000,             // s_barrier                                                       // 00000000093C: 
0xBF8C0F79,             // s_waitcnt vmcnt(9)                                              // 000000000940: 
0xD81A0000, 0x0000201E, // ds_write_b32 v30, v32                                           // 000000000944: 
0xBF8C0F77,             // s_waitcnt vmcnt(7)                                              // 00000000094C: 
0xD2A0000A, 0x00021106, // v_pack_b32_f16 v10, v6, v8                                      // 000000000950: 
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                            // 000000000958: 
0x7E24030C,             // v_mov_b32_e32 v18, v12                                          // 00000000095C: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 000000000960: 
0x7E260310,             // v_mov_b32_e32 v19, v16                                          // 000000000964: 
0xD2A0180B, 0x00021106, // v_pack_b32_f16 v11, v6, v8 op_sel:[1,1,0]                       // 000000000968: 
0x7E20030D,             // v_mov_b32_e32 v16, v13                                          // 000000000970: 
0xD2A0000C, 0x00021307, // v_pack_b32_f16 v12, v7, v9                                      // 000000000974: 
0xD2A0180D, 0x00021307, // v_pack_b32_f16 v13, v7, v9 op_sel:[1,1,0]                       // 00000000097C: 
0xBF8C0F75,             // s_waitcnt vmcnt(5)                                              // 000000000984: 
0xD2A00006, 0x00022D18, // v_pack_b32_f16 v6, v24, v22                                     // 000000000988: 
0xD2A01807, 0x00022D18, // v_pack_b32_f16 v7, v24, v22 op_sel:[1,1,0]                      // 000000000990: 
0xD2A00008, 0x00022F19, // v_pack_b32_f16 v8, v25, v23                                     // 000000000998: 
0xD2A01809, 0x00022F19, // v_pack_b32_f16 v9, v25, v23 op_sel:[1,1,0]                      // 0000000009A0: 
0xD9BE0220, 0x0000061C, // ds_write_b128 v28, v[6:9] offset:544                            // 0000000009A8: 
0xD9BE0000, 0x00000A1C, // ds_write_b128 v28, v[10:13]                                     // 0000000009B0: 
0xD3CD8000, 0x04021D02, // v_mfma_f32_16x16x16f16 a[0:3], v[2:3], v[14:15], a[0:3]         // 0000000009B8: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 0000000009C0: 
0xBF8A0000,             // s_barrier                                                       // 0000000009C4: 
0xD3CD8004, 0x04122504, // v_mfma_f32_16x16x16f16 a[4:7], v[4:5], v[18:19], a[4:7]         // 0000000009C8: 
0xD3CD8000, 0x04022104, // v_mfma_f32_16x16x16f16 a[0:3], v[4:5], v[16:17], a[0:3]         // 0000000009D0: 
0xD9FE0000, 0x0200001F, // ds_read_b128 v[2:5], v31                                        // 0000000009D8: 
0xD8700100, 0x0600001D, // ds_read2st64_b32 v[6:7], v29 offset1:1                          // 0000000009E0: 
0xD86EC888, 0x0E00001D, // ds_read2_b32 v[14:15], v29 offset0:136 offset1:200              // 0000000009E8: 
0xD8700504, 0x0A000034, // ds_read2st64_b32 v[10:11], v52 offset0:4 offset1:5              // 0000000009F0: 
0xD8700706, 0x10000035, // ds_read2st64_b32 v[16:17], v53 offset0:6 offset1:7              // 0000000009F8: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000000A00: 
0xBF8A0000,             // s_barrier                                                       // 000000000A04: 
0xBF8C0F74,             // s_waitcnt vmcnt(4)                                              // 000000000A08: 
0xD81A0000, 0x0000321E, // ds_write_b32 v30, v50                                           // 000000000A0C: 
0xBF8CC47F,             // s_waitcnt lgkmcnt(4)                                            // 000000000A14: 
0x7E100306,             // v_mov_b32_e32 v8, v6                                            // 000000000A18: 
0xBF8CC37F,             // s_waitcnt lgkmcnt(3)                                            // 000000000A1C: 
0x7E12030E,             // v_mov_b32_e32 v9, v14                                           // 000000000A20: 
0x7E1C0307,             // v_mov_b32_e32 v14, v7                                           // 000000000A24: 
0xBF800000,             // s_nop 0                                                         // 000000000A28: 
0xD3CD8004, 0x04121102, // v_mfma_f32_16x16x16f16 a[4:7], v[2:3], v[8:9], a[4:7]           // 000000000A2C: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 000000000A34: 
0x7E120310,             // v_mov_b32_e32 v9, v16                                           // 000000000A38: 
0x7E10030A,             // v_mov_b32_e32 v8, v10                                           // 000000000A3C: 
0x7E20030B,             // v_mov_b32_e32 v16, v11                                          // 000000000A40: 
0xD3CD8000, 0x04021D02, // v_mfma_f32_16x16x16f16 a[0:3], v[2:3], v[14:15], a[0:3]         // 000000000A44: 
0xBF8C0F72,             // s_waitcnt vmcnt(2)                                              // 000000000A4C: 
0xD2A00006, 0x00024926, // v_pack_b32_f16 v6, v38, v36                                     // 000000000A50: 
0xD2A01807, 0x00024926, // v_pack_b32_f16 v7, v38, v36 op_sel:[1,1,0]                      // 000000000A58: 
0xD3CD8004, 0x04121104, // v_mfma_f32_16x16x16f16 a[4:7], v[4:5], v[8:9], a[4:7]           // 000000000A60: 
0xD2A00008, 0x00024B27, // v_pack_b32_f16 v8, v39, v37                                     // 000000000A68: 
0xD2A01809, 0x00024B27, // v_pack_b32_f16 v9, v39, v37 op_sel:[1,1,0]                      // 000000000A70: 
0xBF8C0F70,             // s_waitcnt vmcnt(0)                                              // 000000000A78: 
0xD2A0000A, 0x0002512A, // v_pack_b32_f16 v10, v42, v40                                    // 000000000A7C: 
0xD2A0180B, 0x0002512A, // v_pack_b32_f16 v11, v42, v40 op_sel:[1,1,0]                     // 000000000A84: 
0xD2A0000C, 0x0002532B, // v_pack_b32_f16 v12, v43, v41                                    // 000000000A8C: 
0xD2A0180D, 0x0002532B, // v_pack_b32_f16 v13, v43, v41 op_sel:[1,1,0]                     // 000000000A94: 
0xD9BE0220, 0x00000A1C, // ds_write_b128 v28, v[10:13] offset:544                          // 000000000A9C: 
0xD9BE0000, 0x0000061C, // ds_write_b128 v28, v[6:9]                                       // 000000000AA4: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000000AAC: 
0xBF8A0000,             // s_barrier                                                       // 000000000AB0: 
0xD9FE0000, 0x0600001F, // ds_read_b128 v[6:9], v31                                        // 000000000AB4: 
0xD8700100, 0x0200001D, // ds_read2st64_b32 v[2:3], v29 offset1:1                          // 000000000ABC: 
0xD86EC888, 0x0A00001D, // ds_read2_b32 v[10:11], v29 offset0:136 offset1:200              // 000000000AC4: 
0xD3CD8000, 0x04022104, // v_mfma_f32_16x16x16f16 a[0:3], v[4:5], v[16:17], a[0:3]         // 000000000ACC: 
0xD8700504, 0x04000034, // ds_read2st64_b32 v[4:5], v52 offset0:4 offset1:5                // 000000000AD4: 
0xD8700706, 0x0E000035, // ds_read2st64_b32 v[14:15], v53 offset0:6 offset1:7              // 000000000ADC: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000000AE4: 
0xBF8A0000,             // s_barrier                                                       // 000000000AE8: 
0xBF8CC37F,             // s_waitcnt lgkmcnt(3)                                            // 000000000AEC: 
0x7E180302,             // v_mov_b32_e32 v12, v2                                           // 000000000AF0: 
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                            // 000000000AF4: 
0x7E1A030A,             // v_mov_b32_e32 v13, v10                                          // 000000000AF8: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 000000000AFC: 
0x7E200304,             // v_mov_b32_e32 v16, v4                                           // 000000000B00: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000000B04: 
0x7E22030E,             // v_mov_b32_e32 v17, v14                                          // 000000000B08: 
0x7E140303,             // v_mov_b32_e32 v10, v3                                           // 000000000B0C: 
0x260436FF, 0x7FFFFFC0, // v_and_b32_e32 v2, 0x7fffffc0, v27                               // 000000000B10: 
0xD3CD8004, 0x04121906, // v_mfma_f32_16x16x16f16 a[4:7], v[6:7], v[12:13], a[4:7]         // 000000000B18: 
0xD1FD0003, 0x046D0F1A, // v_lshl_add_u32 v3, v26, 7, v27                                  // 000000000B20: 
0x6A040503,             // v_sub_u32_e32 v2, v3, v2                                        // 000000000B28: 
0x0C0602FF, 0xFFFFFFC0, // v_mul_i32_i24_e32 v3, 0xffffffc0, v1                            // 000000000B2C: 
0x7E080202,             // v_mov_b32_e32 v4, s2                                            // 000000000B34: 
0xD1FE0000, 0x02060103, // v_add_lshl_u32 v0, v3, v0, 1                                    // 000000000B38: 
0x24060289,             // v_lshlrev_b32_e32 v3, 9, v1                                     // 000000000B40: 
0xD1C30001, 0x04100701, // v_mad_u32_u24 v1, v1, s3, v4                                    // 000000000B44: 
0x7E1C0305,             // v_mov_b32_e32 v14, v5                                           // 000000000B4C: 
0xD3CD8004, 0x04122108, // v_mfma_f32_16x16x16f16 a[4:7], v[8:9], v[16:17], a[4:7]         // 000000000B50: 
0x24040481,             // v_lshlrev_b32_e32 v2, 1, v2                                     // 000000000B58: 
0xBE8300FF, 0x00020000, // s_mov_b32 s3, 0x20000                                           // 000000000B5C: 
0xBE8200FF, 0x00009000, // s_mov_b32 s2, 0x9000                                            // 000000000B64: 
0xD3CD8000, 0x04021506, // v_mfma_f32_16x16x16f16 a[0:3], v[6:7], v[10:11], a[0:3]         // 000000000B6C: 
0xBF800006,             // s_nop 6                                                         // 000000000B74: 
0xD3D84004, 0x18000104, // v_accvgpr_read_b32 v4, a4                                       // 000000000B78: 
0xD3D84005, 0x18000105, // v_accvgpr_read_b32 v5, a5                                       // 000000000B80: 
0x7E081504,             // v_cvt_f16_f32_e32 v4, v4                                        // 000000000B88: 
0xD3CD8000, 0x04021D08, // v_mfma_f32_16x16x16f16 a[0:3], v[8:9], v[14:15], a[0:3]         // 000000000B8C: 
0x7E0A1505,             // v_cvt_f16_f32_e32 v5, v5                                        // 000000000B94: 
0xD83E0000, 0x00000402, // ds_write_b16 v2, v4                                             // 000000000B98: 
0xD83E0100, 0x00000502, // ds_write_b16 v2, v5 offset:256                                  // 000000000BA0: 
0xD3D84004, 0x18000106, // v_accvgpr_read_b32 v4, a6                                       // 000000000BA8: 
0xD3D84005, 0x18000107, // v_accvgpr_read_b32 v5, a7                                       // 000000000BB0: 
0x7E081504,             // v_cvt_f16_f32_e32 v4, v4                                        // 000000000BB8: 
0x7E0A1505,             // v_cvt_f16_f32_e32 v5, v5                                        // 000000000BBC: 
0xD83E0200, 0x00000402, // ds_write_b16 v2, v4 offset:512                                  // 000000000BC0: 
0xD83E0300, 0x00000502, // ds_write_b16 v2, v5 offset:768                                  // 000000000BC8: 
0xBF800001,             // s_nop 1                                                         // 000000000BD0: 
0xD3D84004, 0x18000103, // v_accvgpr_read_b32 v4, a3                                       // 000000000BD4: 
0xD3D84005, 0x18000102, // v_accvgpr_read_b32 v5, a2                                       // 000000000BDC: 
0x7E081504,             // v_cvt_f16_f32_e32 v4, v4                                        // 000000000BE4: 
0xD3D84006, 0x18000101, // v_accvgpr_read_b32 v6, a1                                       // 000000000BE8: 
0x7E0A1505,             // v_cvt_f16_f32_e32 v5, v5                                        // 000000000BF0: 
0xD3D84007, 0x18000100, // v_accvgpr_read_b32 v7, a0                                       // 000000000BF4: 
0x7E0C1506,             // v_cvt_f16_f32_e32 v6, v6                                        // 000000000BFC: 
0x7E0E1507,             // v_cvt_f16_f32_e32 v7, v7                                        // 000000000C00: 
0xD83E0380, 0x00000402, // ds_write_b16 v2, v4 offset:896                                  // 000000000C04: 
0xD83E0280, 0x00000502, // ds_write_b16 v2, v5 offset:640                                  // 000000000C0C: 
0xD83E0180, 0x00000602, // ds_write_b16 v2, v6 offset:384                                  // 000000000C14: 
0xD83E0080, 0x00000702, // ds_write_b16 v2, v7 offset:128                                  // 000000000C1C: 
0xD1FE0002, 0x02060700, // v_add_lshl_u32 v2, v0, v3, 1                                    // 000000000C24: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000000C2C: 
0xBF8A0000,             // s_barrier                                                       // 000000000C30: 
0xD86C0000, 0x03000002, // ds_read_b32 v3, v2                                              // 000000000C34: 
0xD1FE0000, 0x02060101, // v_add_lshl_u32 v0, v1, v0, 1                                    // 000000000C3C: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000000C44: 
0xE1381000, 0x80000300, // buffer_atomic_pk_add_f16 v3, v0, s[0:3], 0 offen                // 000000000C48: 
0xD86C0100, 0x01000002, // ds_read_b32 v1, v2 offset:256                                   // 000000000C50: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000000C58: 
0xE1381900, 0x80000100, // buffer_atomic_pk_add_f16 v1, v0, s[0:3], 0 offen offset:2304    // 000000000C5C: 
0xD86C0200, 0x01000002, // ds_read_b32 v1, v2 offset:512                                   // 000000000C64: 
0x68000004,             // v_add_u32_e32 v0, s4, v0                                        // 000000000C6C: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000000C70: 
0xE1381200, 0x80000100, // buffer_atomic_pk_add_f16 v1, v0, s[0:3], 0 offen offset:512     // 000000000C74: 
0xD86C0300, 0x01000002, // ds_read_b32 v1, v2 offset:768                                   // 000000000C7C: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000000C84: 
0xE1381B00, 0x80000100, // buffer_atomic_pk_add_f16 v1, v0, s[0:3], 0 offen offset:2816    // 000000000C88: 
0xBF810000              // s_endpgm                                                        // 000000000C90: 
};

const uint32_t GEMM_CK_ISA_16_5120_384[] = {
// Original ISA:
// s4-s5 : kernarg pointer
// s6 : workgroup_id_x

// Hacked ISA
// s0-s1: matrix C pointer
// s2-s3: matrix A pointer
// s4-s5: matrix B pointer
// s6 : workgroup_id_x
0xBE880002,             // s_mov_b32 s8, s2
0xBE890003,             // s_mov_b32 s9, s3
0xBE8C0004,             // s_mov_b32 s12, s4
0xBE8D0005,             // s_mov_b32 s13, s5
0xBF800000,             // s_nop 0
0xBF800000,             // s_nop 0

0x9682FF06, 0x66666667, // s_mul_hi_i32 s2, s6, 0x66666667                                 // 000000000D00: 
0x20020082,             // v_lshrrev_b32_e32 v1, 2, v0                                     // 000000000D08: 
0x20060084,             // v_lshrrev_b32_e32 v3, 4, v0                                     // 000000000D0C: 

//0xC0060202, 0x00000000, // s_load_dwordx2 s[8:9], s[4:5], 0x0                              // 000000000D10: 
//0xC0060302, 0x00000008, // s_load_dwordx2 s[12:13], s[4:5], 0x8                            // 000000000D18: 
//0xC0060002, 0x00000010, // s_load_dwordx2 s[0:1], s[4:5], 0x10                             // 000000000D20: 

0x8F039F02,             // s_lshr_b32 s3, s2, 31                                           // 000000000D28: 
0x90028402,             // s_ashr_i32 s2, s2, 4                                            // 000000000D2C: 
0xD1C20004, 0x04058903, // v_mad_i32_i24 v4, v3, -4, v1                                    // 000000000D30: 
0xB0050088,             // s_movk_i32 s5, 0x88                                             // 000000000D38: 
0x0C0402C4,             // v_mul_i32_i24_e32 v2, -4, v1                                    // 000000000D3C: 
0x81030302,             // s_add_i32 s3, s2, s3                                            // 000000000D40: 
0x240A0883,             // v_lshlrev_b32_e32 v5, 3, v4                                     // 000000000D44: 
0xD2850004, 0x00000B04, // v_mul_lo_u32 v4, v4, s5                                         // 000000000D48: 
0x9202A803,             // s_mul_i32 s2, s3, 40                                            // 000000000D50: 
0x100C06FF, 0x00000180, // v_mul_u32_u24_e32 v6, 0x180, v3                                 // 000000000D54: 
0xD1FD0003, 0x04110703, // v_lshl_add_u32 v3, v3, 3, v4                                    // 000000000D5C: 
0xD1FE0002, 0x02060102, // v_add_lshl_u32 v2, v2, v0, 1                                    // 000000000D64: 
0xB7830060,             // s_mulk_i32 s3, 0x60                                             // 000000000D6C: 
0x20080085,             // v_lshrrev_b32_e32 v4, 5, v0                                     // 000000000D70: 
0xD1FF0005, 0x04080705, // v_add3_u32 v5, v5, s3, v2                                       // 000000000D74: 
0xB004FFE0,             // s_movk_i32 s4, 0xffe0                                           // 000000000D7C: 
0xD2000008, 0x000D0504, // v_lshl_or_b32 v8, v4, 2, s3                                     // 000000000D80: 
0xB0031400,             // s_movk_i32 s3, 0x1400                                           // 000000000D88: 
0xD1C20007, 0x04000904, // v_mad_i32_i24 v7, v4, s4, v0                                    // 000000000D8C: 
0xD2850008, 0x00000708, // v_mul_lo_u32 v8, v8, s3                                         // 000000000D94: 
0x81820206,             // s_sub_i32 s2, s6, s2                                            // 000000000D9C: 
0x101808FF, 0x00000220, // v_mul_u32_u24_e32 v12, 0x220, v4                                // 000000000DA0: 
0x241A0E83,             // v_lshlrev_b32_e32 v13, 3, v7                                    // 000000000DA8: 
0xD1FD0008, 0x04210507, // v_lshl_add_u32 v8, v7, 2, v8                                    // 000000000DAC: 
0x260800BF,             // v_and_b32_e32 v4, 63, v0                                        // 000000000DB4: 
0x260E00B0,             // v_and_b32_e32 v7, 48, v0                                        // 000000000DB8: 
0x6A080F04,             // v_sub_u32_e32 v4, v4, v7                                        // 000000000DBC: 
0x8E028702,             // s_lshl_b32 s2, s2, 7                                            // 000000000DC0: 
0xD1C80007, 0x02090900, // v_bfe_u32 v7, v0, 4, 2                                          // 000000000DC4: 
0xBE8B00FF, 0x00020000, // s_mov_b32 s11, 0x20000                                          // 000000000DCC: 
0xD1FE0017, 0x02040508, // v_add_lshl_u32 v23, v8, s2, 1                                   // 000000000DD4: 
0x101C0E05,             // v_mul_u32_u24_e32 v14, s5, v7                                   // 000000000DDC: 
0x20280086,             // v_lshrrev_b32_e32 v20, 6, v0                                    // 000000000DE0: 
0xB0042000,             // s_movk_i32 s4, 0x2000                                           // 000000000DE4: 
0xB0057000,             // s_movk_i32 s5, 0x7000                                           // 000000000DE8: 
0xB0065000,             // s_movk_i32 s6, 0x5000                                           // 000000000DEC: 
0xBE8E00FF, 0x003C0000, // s_mov_b32 s14, 0x3c0000                                         // 000000000DF0: 
0xBE8F000B,             // s_mov_b32 s15, s11                                              // 000000000DF8: 
0x241E0883,             // v_lshlrev_b32_e32 v15, 3, v4                                    // 000000000DFC: 
0xD1FD0015, 0x04110914, // v_lshl_add_u32 v21, v20, 4, v4                                  // 000000000E00: 
0xB00A3000,             // s_movk_i32 s10, 0x3000                                          // 000000000E08: 
0xD1FE0016, 0x02060D05, // v_add_lshl_u32 v22, v5, v6, 1                                   // 000000000E0C: 
0x68222E04,             // v_add_u32_e32 v17, s4, v23                                      // 000000000E14: 
0x68242E05,             // v_add_u32_e32 v18, s5, v23                                      // 000000000E18: 
0x68262E06,             // v_add_u32_e32 v19, s6, v23                                      // 000000000E1C: 

//0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000000E20: 
0xBF800000,             // s_nop 0

0xE0541000, 0x80030417, // buffer_load_dwordx2 v[4:5], v23, s[12:15], 0 offen              // 000000000E24: 
0xE0541800, 0x80030611, // buffer_load_dwordx2 v[6:7], v17, s[12:15], 0 offen offset:2048  // 000000000E2C: 
0xE0541800, 0x80030812, // buffer_load_dwordx2 v[8:9], v18, s[12:15], 0 offen offset:2048  // 000000000E34: 
0xE0541000, 0x80030A13, // buffer_load_dwordx2 v[10:11], v19, s[12:15], 0 offen            // 000000000E3C: 
0xD1FE0018, 0x02060503, // v_add_lshl_u32 v24, v3, v2, 1                                   // 000000000E44: 
0xE0501000, 0x80020216, // buffer_load_dword v2, v22, s[8:11], 0 offen                     // 000000000E4C: 
0xE0501040, 0x80021D16, // buffer_load_dword v29, v22, s[8:11], 0 offen offset:64          // 000000000E54: 
0x2602028C,             // v_and_b32_e32 v1, 12, v1                                        // 000000000E5C: 
0x100202FF, 0x00000110, // v_mul_u32_u24_e32 v1, 0x110, v1                                 // 000000000E60: 
0x24202A81,             // v_lshlrev_b32_e32 v16, 1, v21                                   // 000000000E68: 
0x68242EFF, 0x00050000, // v_add_u32_e32 v18, 0x50000, v23                                 // 000000000E6C: 
0xD1FE0001, 0x02060310, // v_add_lshl_u32 v1, v16, v1, 1                                   // 000000000E74: 
0xD1FE0019, 0x02061B0C, // v_add_lshl_u32 v25, v12, v13, 1                                 // 000000000E7C: 
0xD1FE001A, 0x02061D0F, // v_add_lshl_u32 v26, v15, v14, 1                                 // 000000000E84: 
0x68262EFF, 0x00052000, // v_add_u32_e32 v19, 0x52000, v23                                 // 000000000E8C: 
0x683C2EFF, 0x00057000, // v_add_u32_e32 v30, 0x57000, v23                                 // 000000000E94: 
0x683E2EFF, 0x00055000, // v_add_u32_e32 v31, 0x55000, v23                                 // 000000000E9C: 
0x683602B0,             // v_add_u32_e32 v27, 48, v1                                       // 000000000EA4: 
0x683802FF, 0x00000050, // v_add_u32_e32 v28, 0x50, v1                                     // 000000000EA8: 
0xBF8C0F71,             // s_waitcnt vmcnt(1)                                              // 000000000EB0: 
0xD81A0000, 0x00000218, // ds_write_b32 v24, v2                                            // 000000000EB4: 
0xD2A00002, 0x00020D04, // v_pack_b32_f16 v2, v4, v6                                       // 000000000EBC: 
0xD2A01803, 0x00020D04, // v_pack_b32_f16 v3, v4, v6 op_sel:[1,1,0]                        // 000000000EC4: 
0xD2A00004, 0x00020F05, // v_pack_b32_f16 v4, v5, v7                                       // 000000000ECC: 
0xD2A01805, 0x00020F05, // v_pack_b32_f16 v5, v5, v7 op_sel:[1,1,0]                        // 000000000ED4: 
0xD2A00006, 0x0002110A, // v_pack_b32_f16 v6, v10, v8                                      // 000000000EDC: 
0xD2A01807, 0x0002110A, // v_pack_b32_f16 v7, v10, v8 op_sel:[1,1,0]                       // 000000000EE4: 
0xD2A00008, 0x0002130B, // v_pack_b32_f16 v8, v11, v9                                      // 000000000EEC: 
0xD2A01809, 0x0002130B, // v_pack_b32_f16 v9, v11, v9 op_sel:[1,1,0]                       // 000000000EF4: 
0xE0541000, 0x80030A12, // buffer_load_dwordx2 v[10:11], v18, s[12:15], 0 offen            // 000000000EFC: 
0xE0541800, 0x80030C13, // buffer_load_dwordx2 v[12:13], v19, s[12:15], 0 offen offset:2048// 000000000F04: 
0xE0541800, 0x80030E1E, // buffer_load_dwordx2 v[14:15], v30, s[12:15], 0 offen offset:2048// 000000000F0C: 
0xE0541000, 0x8003101F, // buffer_load_dwordx2 v[16:17], v31, s[12:15], 0 offen            // 000000000F14: 
0xD9BE0650, 0x00000619, // ds_write_b128 v25, v[6:9] offset:1616                           // 000000000F1C: 
0xD9BE0430, 0x00000219, // ds_write_b128 v25, v[2:5] offset:1072                           // 000000000F24: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000000F2C: 
0xBF8A0000,             // s_barrier                                                       // 000000000F30: 
0xD9FE0000, 0x0200001A, // ds_read_b128 v[2:5], v26                                        // 000000000F34: 
0xD8700504, 0x0600001B, // ds_read2st64_b32 v[6:7], v27 offset0:4 offset1:5                // 000000000F3C: 
0xD8700706, 0x0800001C, // ds_read2st64_b32 v[8:9], v28 offset0:6 offset1:7                // 000000000F44: 
0x683C02FF, 0x00000070, // v_add_u32_e32 v30, 0x70, v1                                     // 000000000F4C: 
0x683E02FF, 0x00000090, // v_add_u32_e32 v31, 0x90, v1                                     // 000000000F54: 
0x68022EFF, 0x000A0000, // v_add_u32_e32 v1, 0xa0000, v23                                  // 000000000F5C: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 000000000F64: 
0x7E240306,             // v_mov_b32_e32 v18, v6                                           // 000000000F68: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000000F6C: 
0x7E260308,             // v_mov_b32_e32 v19, v8                                           // 000000000F70: 
0x7E100307,             // v_mov_b32_e32 v8, v7                                            // 000000000F74: 
0xBF800000,             // s_nop 0                                                         // 000000000F78: 
0xD3CD8000, 0x02022502, // v_mfma_f32_16x16x16f16 a[0:3], v[2:3], v[18:19], 0              // 000000000F7C: 
0xBE83000B,             // s_mov_b32 s3, s11                                               // 000000000F84: 
0x68242EFF, 0x000A2000, // v_add_u32_e32 v18, 0xa2000, v23                                 // 000000000F88: 
0x68262EFF, 0x000A7000, // v_add_u32_e32 v19, 0xa7000, v23                                 // 000000000F90: 
0x682E2EFF, 0x000A5000, // v_add_u32_e32 v23, 0xa5000, v23                                 // 000000000F98: 
0xD3CD8004, 0x02021102, // v_mfma_f32_16x16x16f16 a[4:7], v[2:3], v[8:9], 0                // 000000000FA0: 
0xD8700908, 0x0200001E, // ds_read2st64_b32 v[2:3], v30 offset0:8 offset1:9                // 000000000FA8: 
0xD8700B0A, 0x0600001F, // ds_read2st64_b32 v[6:7], v31 offset0:10 offset1:11              // 000000000FB0: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000000FB8: 
0xBF8A0000,             // s_barrier                                                       // 000000000FBC: 
0xBF8C0F74,             // s_waitcnt vmcnt(4)                                              // 000000000FC0: 
0xD81A0000, 0x00001D18, // ds_write_b32 v24, v29                                           // 000000000FC4: 
0xE0501080, 0x80021616, // buffer_load_dword v22, v22, s[8:11], 0 offen offset:128         // 000000000FCC: 
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                            // 000000000FD4: 
0x7E100302,             // v_mov_b32_e32 v8, v2                                            // 000000000FD8: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 000000000FDC: 
0x7E120306,             // v_mov_b32_e32 v9, v6                                            // 000000000FE0: 
0x7E0C0303,             // v_mov_b32_e32 v6, v3                                            // 000000000FE4: 
0xBF800000,             // s_nop 0                                                         // 000000000FE8: 
0xD3CD8000, 0x04021104, // v_mfma_f32_16x16x16f16 a[0:3], v[4:5], v[8:9], a[0:3]           // 000000000FEC: 
0xD3CD8004, 0x04120D04, // v_mfma_f32_16x16x16f16 a[4:7], v[4:5], v[6:7], a[4:7]           // 000000000FF4: 
0xBF8C0F73,             // s_waitcnt vmcnt(3)                                              // 000000000FFC: 
0xD2A00002, 0x0002190A, // v_pack_b32_f16 v2, v10, v12                                     // 000000001000: 
0xD2A01803, 0x0002190A, // v_pack_b32_f16 v3, v10, v12 op_sel:[1,1,0]                      // 000000001008: 
0xD2A00004, 0x00021B0B, // v_pack_b32_f16 v4, v11, v13                                     // 000000001010: 
0xD2A01805, 0x00021B0B, // v_pack_b32_f16 v5, v11, v13 op_sel:[1,1,0]                      // 000000001018: 
0xBF8C0F71,             // s_waitcnt vmcnt(1)                                              // 000000001020: 
0xD2A00006, 0x00021D10, // v_pack_b32_f16 v6, v16, v14                                     // 000000001024: 
0xD2A01807, 0x00021D10, // v_pack_b32_f16 v7, v16, v14 op_sel:[1,1,0]                      // 00000000102C: 
0xD2A00008, 0x00021F11, // v_pack_b32_f16 v8, v17, v15                                     // 000000001034: 
0xD2A01809, 0x00021F11, // v_pack_b32_f16 v9, v17, v15 op_sel:[1,1,0]                      // 00000000103C: 
0xE0541000, 0x80030A01, // buffer_load_dwordx2 v[10:11], v1, s[12:15], 0 offen             // 000000001044: 
0xE0541800, 0x80030C12, // buffer_load_dwordx2 v[12:13], v18, s[12:15], 0 offen offset:2048// 00000000104C: 
0xE0541800, 0x80030E13, // buffer_load_dwordx2 v[14:15], v19, s[12:15], 0 offen offset:2048// 000000001054: 
0xE0541000, 0x80031017, // buffer_load_dwordx2 v[16:17], v23, s[12:15], 0 offen            // 00000000105C: 
0xD9BE0650, 0x00000619, // ds_write_b128 v25, v[6:9] offset:1616                           // 000000001064: 
0xD9BE0430, 0x00000219, // ds_write_b128 v25, v[2:5] offset:1072                           // 00000000106C: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000001074: 
0xBF8A0000,             // s_barrier                                                       // 000000001078: 
0xD9FE0000, 0x0200001A, // ds_read_b128 v[2:5], v26                                        // 00000000107C: 
0xD8700504, 0x0600001B, // ds_read2st64_b32 v[6:7], v27 offset0:4 offset1:5                // 000000001084: 
0xD8700706, 0x0800001C, // ds_read2st64_b32 v[8:9], v28 offset0:6 offset1:7                // 00000000108C: 
0x24020085,             // v_lshlrev_b32_e32 v1, 5, v0                                     // 000000001094: 
0x260202FF, 0x00000600, // v_and_b32_e32 v1, 0x600, v1                                     // 000000001098: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 0000000010A0: 
0x7E240306,             // v_mov_b32_e32 v18, v6                                           // 0000000010A4: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 0000000010A8: 
0x7E260308,             // v_mov_b32_e32 v19, v8                                           // 0000000010AC: 
0x7E100307,             // v_mov_b32_e32 v8, v7                                            // 0000000010B0: 
0xBF800000,             // s_nop 0                                                         // 0000000010B4: 
0xD3CD8000, 0x04022502, // v_mfma_f32_16x16x16f16 a[0:3], v[2:3], v[18:19], a[0:3]         // 0000000010B8: 
0xD3CD8004, 0x04121102, // v_mfma_f32_16x16x16f16 a[4:7], v[2:3], v[8:9], a[4:7]           // 0000000010C0: 
0xD8700908, 0x0200001E, // ds_read2st64_b32 v[2:3], v30 offset0:8 offset1:9                // 0000000010C8: 
0xD8700B0A, 0x0600001F, // ds_read2st64_b32 v[6:7], v31 offset0:10 offset1:11              // 0000000010D0: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 0000000010D8: 
0xBF8A0000,             // s_barrier                                                       // 0000000010DC: 
0xBF8C0F74,             // s_waitcnt vmcnt(4)                                              // 0000000010E0: 
0xD81A0000, 0x00001618, // ds_write_b32 v24, v22                                           // 0000000010E4: 
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                            // 0000000010EC: 
0x7E100302,             // v_mov_b32_e32 v8, v2                                            // 0000000010F0: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 0000000010F4: 
0x7E120306,             // v_mov_b32_e32 v9, v6                                            // 0000000010F8: 
0x7E0C0303,             // v_mov_b32_e32 v6, v3                                            // 0000000010FC: 
0xBF800000,             // s_nop 0                                                         // 000000001100: 
0xD3CD8000, 0x04021104, // v_mfma_f32_16x16x16f16 a[0:3], v[4:5], v[8:9], a[0:3]           // 000000001104: 
0xD3CD8004, 0x04120D04, // v_mfma_f32_16x16x16f16 a[4:7], v[4:5], v[6:7], a[4:7]           // 00000000110C: 
0xBF8C0F72,             // s_waitcnt vmcnt(2)                                              // 000000001114: 
0xD2A00002, 0x0002190A, // v_pack_b32_f16 v2, v10, v12                                     // 000000001118: 
0xD2A01803, 0x0002190A, // v_pack_b32_f16 v3, v10, v12 op_sel:[1,1,0]                      // 000000001120: 
0xD2A00004, 0x00021B0B, // v_pack_b32_f16 v4, v11, v13                                     // 000000001128: 
0xD2A01805, 0x00021B0B, // v_pack_b32_f16 v5, v11, v13 op_sel:[1,1,0]                      // 000000001130: 
0xBF8C0F70,             // s_waitcnt vmcnt(0)                                              // 000000001138: 
0xD2A00006, 0x00021D10, // v_pack_b32_f16 v6, v16, v14                                     // 00000000113C: 
0xD2A01807, 0x00021D10, // v_pack_b32_f16 v7, v16, v14 op_sel:[1,1,0]                      // 000000001144: 
0xD2A00008, 0x00021F11, // v_pack_b32_f16 v8, v17, v15                                     // 00000000114C: 
0xD2A01809, 0x00021F11, // v_pack_b32_f16 v9, v17, v15 op_sel:[1,1,0]                      // 000000001154: 
0xD9BE0650, 0x00000619, // ds_write_b128 v25, v[6:9] offset:1616                           // 00000000115C: 
0xD9BE0430, 0x00000219, // ds_write_b128 v25, v[2:5] offset:1072                           // 000000001164: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 00000000116C: 
0xBF8A0000,             // s_barrier                                                       // 000000001170: 
0xD9FE0000, 0x0200001A, // ds_read_b128 v[2:5], v26                                        // 000000001174: 
0xD8700504, 0x0600001B, // ds_read2st64_b32 v[6:7], v27 offset0:4 offset1:5                // 00000000117C: 
0xD8700706, 0x0800001C, // ds_read2st64_b32 v[8:9], v28 offset0:6 offset1:7                // 000000001184: 
0x68182B01,             // v_add_u32_e32 v12, v1, v21                                      // 00000000118C: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 000000001190: 
0x7E140306,             // v_mov_b32_e32 v10, v6                                           // 000000001194: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000001198: 
0x7E160308,             // v_mov_b32_e32 v11, v8                                           // 00000000119C: 
0x7E100307,             // v_mov_b32_e32 v8, v7                                            // 0000000011A0: 
0x0C0C28FF, 0xFFFFFFC0, // v_mul_i32_i24_e32 v6, 0xffffffc0, v20                           // 0000000011A4: 
0xD3CD8000, 0x04021502, // v_mfma_f32_16x16x16f16 a[0:3], v[2:3], v[10:11], a[0:3]         // 0000000011AC: 
0xD1FE000B, 0x02060106, // v_add_lshl_u32 v11, v6, v0, 1                                   // 0000000011B4: 
0xD8700908, 0x0000001E, // ds_read2st64_b32 v[0:1], v30 offset0:8 offset1:9                // 0000000011BC: 
0x260C2AFF, 0x7FFFFFC0, // v_and_b32_e32 v6, 0x7fffffc0, v21                               // 0000000011C4: 
0x24142889,             // v_lshlrev_b32_e32 v10, 9, v20                                   // 0000000011CC: 
0xD3CD8004, 0x04121102, // v_mfma_f32_16x16x16f16 a[4:7], v[2:3], v[8:9], a[4:7]           // 0000000011D0: 
0xD8700B0A, 0x0200001F, // ds_read2st64_b32 v[2:3], v31 offset0:10 offset1:11              // 0000000011D8: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 0000000011E0: 
0xBF8A0000,             // s_barrier                                                       // 0000000011E4: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 0000000011E8: 
0x7E0E0302,             // v_mov_b32_e32 v7, v2                                            // 0000000011EC: 
0x7E040301,             // v_mov_b32_e32 v2, v1                                            // 0000000011F0: 
0xD1FE0001, 0x0206150B, // v_add_lshl_u32 v1, v11, v10, 1                                  // 0000000011F4: 
0xBF800000,             // s_nop 0                                                         // 0000000011FC: 
0xD3CD8004, 0x04120504, // v_mfma_f32_16x16x16f16 a[4:7], v[4:5], v[2:3], a[4:7]           // 000000001200: 
0x6A040D0C,             // v_sub_u32_e32 v2, v12, v6                                       // 000000001208: 
0x7E0C0300,             // v_mov_b32_e32 v6, v0                                            // 00000000120C: 
0x24040481,             // v_lshlrev_b32_e32 v2, 1, v2                                     // 000000001210: 
0x7E060202,             // v_mov_b32_e32 v3, s2                                            // 000000001214: 
0xD1C30003, 0x040C0D14, // v_mad_u32_u24 v3, v20, s6, v3                                   // 000000001218: 
0xBE8200FF, 0x00028000, // s_mov_b32 s2, 0x28000                                           // 000000001220: 
0xD3CD8000, 0x04020D04, // v_mfma_f32_16x16x16f16 a[0:3], v[4:5], v[6:7], a[0:3]           // 000000001228: 
0xBF800003,             // s_nop 3                                                         // 000000001230: 
0xD3D84007, 0x18000107, // v_accvgpr_read_b32 v7, a7                                       // 000000001234: 
0xD3D84008, 0x18000106, // v_accvgpr_read_b32 v8, a6                                       // 00000000123C: 
0xD3D84009, 0x18000105, // v_accvgpr_read_b32 v9, a5                                       // 000000001244: 
0xD3D8400A, 0x18000104, // v_accvgpr_read_b32 v10, a4                                      // 00000000124C: 
0x7E0E1507,             // v_cvt_f16_f32_e32 v7, v7                                        // 000000001254: 
0x7E101508,             // v_cvt_f16_f32_e32 v8, v8                                        // 000000001258: 
0x7E121509,             // v_cvt_f16_f32_e32 v9, v9                                        // 00000000125C: 
0xD3D84000, 0x18000100, // v_accvgpr_read_b32 v0, a0                                       // 000000001260: 
0xD3D84004, 0x18000101, // v_accvgpr_read_b32 v4, a1                                       // 000000001268: 
0x7E001500,             // v_cvt_f16_f32_e32 v0, v0                                        // 000000001270: 
0xD3D84005, 0x18000102, // v_accvgpr_read_b32 v5, a2                                       // 000000001274: 
0x7E081504,             // v_cvt_f16_f32_e32 v4, v4                                        // 00000000127C: 
0xD3D84006, 0x18000103, // v_accvgpr_read_b32 v6, a3                                       // 000000001280: 
0x7E0A1505,             // v_cvt_f16_f32_e32 v5, v5                                        // 000000001288: 
0x7E0C1506,             // v_cvt_f16_f32_e32 v6, v6                                        // 00000000128C: 
0x7E14150A,             // v_cvt_f16_f32_e32 v10, v10                                      // 000000001290: 
0xD83E0000, 0x00000002, // ds_write_b16 v2, v0                                             // 000000001294: 
0xD83E0100, 0x00000402, // ds_write_b16 v2, v4 offset:256                                  // 00000000129C: 
0xD83E0200, 0x00000502, // ds_write_b16 v2, v5 offset:512                                  // 0000000012A4: 
0xD83E0300, 0x00000602, // ds_write_b16 v2, v6 offset:768                                  // 0000000012AC: 
0xD83E0380, 0x00000702, // ds_write_b16 v2, v7 offset:896                                  // 0000000012B4: 
0xD83E0280, 0x00000802, // ds_write_b16 v2, v8 offset:640                                  // 0000000012BC: 
0xD83E0180, 0x00000902, // ds_write_b16 v2, v9 offset:384                                  // 0000000012C4: 
0xD83E0080, 0x00000A02, // ds_write_b16 v2, v10 offset:128                                 // 0000000012CC: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 0000000012D4: 
0xBF8A0000,             // s_barrier                                                       // 0000000012D8: 
0xD86C0000, 0x00000001, // ds_read_b32 v0, v1                                              // 0000000012DC: 
0xD1FE0002, 0x02061703, // v_add_lshl_u32 v2, v3, v11, 1                                   // 0000000012E4: 
0x68060404,             // v_add_u32_e32 v3, s4, v2                                        // 0000000012EC: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 0000000012F0: 
0xE1381000, 0x80000002, // buffer_atomic_pk_add_f16 v0, v2, s[0:3], 0 offen                // 0000000012F4: 
0xD86C0100, 0x00000001, // ds_read_b32 v0, v1 offset:256                                   // 0000000012FC: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000001304: 
0xE1381800, 0x80000003, // buffer_atomic_pk_add_f16 v0, v3, s[0:3], 0 offen offset:2048    // 000000001308: 
0xD86C0200, 0x00000001, // ds_read_b32 v0, v1 offset:512                                   // 000000001310: 
0x68060406,             // v_add_u32_e32 v3, s6, v2                                        // 000000001318: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 00000000131C: 
0xE1381000, 0x80000003, // buffer_atomic_pk_add_f16 v0, v3, s[0:3], 0 offen                // 000000001320: 
0xD86C0300, 0x00000001, // ds_read_b32 v0, v1 offset:768                                   // 000000001328: 
0x68020405,             // v_add_u32_e32 v1, s5, v2                                        // 000000001330: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000001334: 
0xE1381800, 0x80000001, // buffer_atomic_pk_add_f16 v0, v1, s[0:3], 0 offen offset:2048    // 000000001338: 
0xBF810000              // s_endpgm                                                        // 000000001340: 
};

const uint32_t GEMM_CK_ISA_16_1280_5120[] = {
// Original ISA:
// s4-s5 : kernarg pointer
// s6 : workgroup_id_x

// Hacked ISA
// s0-s1: matrix C pointer
// s2-s3: matrix A pointer
// s4-s5: matrix B pointer
// s6 : workgroup_id_x
0xBE8C0002,             // s_mov_b32 s12, s2
0xBE8D0003,             // s_mov_b32 s13, s3
0xBE880004,             // s_mov_b32 s8, s4
0xBE890005,             // s_mov_b32 s9, s5
0xBF800000,             // s_nop 0
0xBF800000,             // s_nop 0

0x9682FF06, 0x66666667, // s_mul_hi_i32 s2, s6, 0x66666667                                 // 000000001400: 
0x8F039F02,             // s_lshr_b32 s3, s2, 31                                           // 000000001408: 
0x90028202,             // s_ashr_i32 s2, s2, 2                                            // 00000000140C: 
0x81100302,             // s_add_i32 s16, s2, s3                                           // 000000001410: 
0x20460085,             // v_lshrrev_b32_e32 v35, 5, v0                                    // 000000001414: 
0x9203FF10, 0x00000280, // s_mul_i32 s3, s16, 0x280                                        // 000000001418: 

//0xC0060302, 0x00000000, // s_load_dwordx2 s[12:13], s[4:5], 0x0                            // 000000001420: 
//0xC0060202, 0x00000008, // s_load_dwordx2 s[8:9], s[4:5], 0x8                              // 000000001428: 
//0xC0060002, 0x00000010, // s_load_dwordx2 s[0:1], s[4:5], 0x10                             // 000000001430: 

0x92028A10,             // s_mul_i32 s2, s16, 10                                           // 000000001438: 
0x20040082,             // v_lshrrev_b32_e32 v2, 2, v0                                     // 00000000143C: 
0x204A0084,             // v_lshrrev_b32_e32 v37, 4, v0                                    // 000000001440: 
0xB004FFE0,             // s_movk_i32 s4, 0xffe0                                           // 000000001444: 
0xD2000006, 0x000D0523, // v_lshl_or_b32 v6, v35, 2, s3                                    // 000000001448: 
0xB0110500,             // s_movk_i32 s17, 0x500                                           // 000000001450: 
0x81820206,             // s_sub_i32 s2, s6, s2                                            // 000000001454: 
0xD1C20005, 0x04000923, // v_mad_i32_i24 v5, v35, s4, v0                                   // 000000001458: 
0xD2850006, 0x00002306, // v_mul_lo_u32 v6, v6, s17                                        // 000000001460: 
0xD1C20024, 0x04018902, // v_mad_i32_i24 v36, v2, -4, v0                                   // 000000001468: 
0xD1C20026, 0x04098925, // v_mad_i32_i24 v38, v37, -4, v2                                  // 000000001470: 
0x24064881,             // v_lshlrev_b32_e32 v3, 1, v36                                    // 000000001478: 
0x24024C83,             // v_lshlrev_b32_e32 v1, 3, v38                                    // 00000000147C: 
0x8E028702,             // s_lshl_b32 s2, s2, 7                                            // 000000001480: 
0xD1FD0006, 0x04190505, // v_lshl_add_u32 v6, v5, 2, v6                                    // 000000001484: 
0xD1FF0001, 0x040C0701, // v_add3_u32 v1, v1, s3, v3                                       // 00000000148C: 
0x10084AFF, 0x00001400, // v_mul_u32_u24_e32 v4, 0x1400, v37                               // 000000001494: 
0xBE8F00FF, 0x00020000, // s_mov_b32 s15, 0x20000                                          // 00000000149C: 
0xD1FE0027, 0x02040506, // v_add_lshl_u32 v39, v6, s2, 1                                   // 0000000014A4: 
0xB0031000,             // s_movk_i32 s3, 0x1000                                           // 0000000014AC: 
0xD1FE0007, 0x02060901, // v_add_lshl_u32 v7, v1, v4, 1                                    // 0000000014B0: 
0xBE8E00FF, 0x00028000, // s_mov_b32 s14, 0x28000                                          // 0000000014B8: 
0xBE8A00FF, 0x00C80000, // s_mov_b32 s10, 0xc80000                                         // 0000000014C0: 
0xBE8B000F,             // s_mov_b32 s11, s15                                              // 0000000014C8: 
0x68024E03,             // v_add_u32_e32 v1, s3, v39                                       // 0000000014CC: 

//0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 0000000014D0: 
0xBF800000,             // s_nop 0

0xE0501000, 0x80031F07, // buffer_load_dword v31, v7, s[12:15], 0 offen                    // 0000000014D4: 
0xBF800000,             // s_nop 0                                                         // 0000000014DC: 
0xE0541000, 0x80020827, // buffer_load_dwordx2 v[8:9], v39, s[8:11], 0 offen               // 0000000014E0: 
0xE0541A00, 0x80021627, // buffer_load_dwordx2 v[22:23], v39, s[8:11], 0 offen offset:2560 // 0000000014E8: 
0xE0541E00, 0x80021801, // buffer_load_dwordx2 v[24:25], v1, s[8:11], 0 offen offset:3584  // 0000000014F0: 
0xE0541400, 0x80021C01, // buffer_load_dwordx2 v[28:29], v1, s[8:11], 0 offen offset:1024  // 0000000014F8: 
0xB0070088,             // s_movk_i32 s7, 0x88                                             // 000000001500: 
0x2634048C,             // v_and_b32_e32 v26, 12, v2                                       // 000000001504: 
0xD2850002, 0x00000F26, // v_mul_lo_u32 v2, v38, s7                                        // 000000001508: 
0x260800BF,             // v_and_b32_e32 v4, 63, v0                                        // 000000001510: 
0x260C00B0,             // v_and_b32_e32 v6, 48, v0                                        // 000000001514: 
0xD1C8000A, 0x02090900, // v_bfe_u32 v10, v0, 4, 2                                         // 000000001518: 
0x6A080D04,             // v_sub_u32_e32 v4, v4, v6                                        // 000000001520: 
0xBE8400FF, 0x00014000, // s_mov_b32 s4, 0x14000                                           // 000000001524: 
0x20020086,             // v_lshrrev_b32_e32 v1, 6, v0                                     // 00000000152C: 
0xD1FD0002, 0x04090725, // v_lshl_add_u32 v2, v37, 3, v2                                   // 000000001530: 
0xBE8500FF, 0x00015000, // s_mov_b32 s5, 0x15000                                           // 000000001538: 
0x10521407,             // v_mul_u32_u24_e32 v41, s7, v10                                  // 000000001540: 
0xBE8600FF, 0x00029000, // s_mov_b32 s6, 0x29000                                           // 000000001544: 
0x105046FF, 0x00000220, // v_mul_u32_u24_e32 v40, 0x220, v35                               // 00000000154C: 
0x24540A83,             // v_lshlrev_b32_e32 v42, 3, v5                                    // 000000001554: 
0x24560883,             // v_lshlrev_b32_e32 v43, 3, v4                                    // 000000001558: 
0xD1FD001B, 0x04110901, // v_lshl_add_u32 v27, v1, 4, v4                                   // 00000000155C: 
0x100834FF, 0x00000110, // v_mul_u32_u24_e32 v4, 0x110, v26                                // 000000001564: 
0xD1FE001E, 0x02060702, // v_add_lshl_u32 v30, v2, v3, 1                                   // 00000000156C: 
0x680C4E04,             // v_add_u32_e32 v6, s4, v39                                       // 000000001574: 
0x68144E05,             // v_add_u32_e32 v10, s5, v39                                      // 000000001578: 
0x68404E0E,             // v_add_u32_e32 v32, s14, v39                                     // 00000000157C: 
0xBF800000,             // s_nop 0                                                         // 000000001580: 
0xE0501040, 0x80032207, // buffer_load_dword v34, v7, s[12:15], 0 offen offset:64          // 000000001584: 
0xD1FD002C, 0x0411031B, // v_lshl_add_u32 v44, v27, 1, v4                                  // 00000000158C: 
0xBF800000,             // s_nop 0                                                         // 000000001594: 
0xE0541000, 0x80020206, // buffer_load_dwordx2 v[2:3], v6, s[8:11], 0 offen                // 000000001598: 
0xE0541A00, 0x80020406, // buffer_load_dwordx2 v[4:5], v6, s[8:11], 0 offen offset:2560    // 0000000015A0: 
0xE0541E00, 0x8002120A, // buffer_load_dwordx2 v[18:19], v10, s[8:11], 0 offen offset:3584 // 0000000015A8: 
0xE0541400, 0x8002140A, // buffer_load_dwordx2 v[20:21], v10, s[8:11], 0 offen offset:1024 // 0000000015B0: 
0xBF800000,             // s_nop 0                                                         // 0000000015B8: 
0xE0501080, 0x80032107, // buffer_load_dword v33, v7, s[12:15], 0 offen offset:128         // 0000000015BC: 
0x685A4E06,             // v_add_u32_e32 v45, s6, v39                                      // 0000000015C4: 
0xBF800000,             // s_nop 0                                                         // 0000000015C8: 
0xE0541000, 0x80020E20, // buffer_load_dwordx2 v[14:15], v32, s[8:11], 0 offen             // 0000000015CC: 
0xE0541A00, 0x80021020, // buffer_load_dwordx2 v[16:17], v32, s[8:11], 0 offen offset:2560 // 0000000015D4: 
0xE0541E00, 0x80020A2D, // buffer_load_dwordx2 v[10:11], v45, s[8:11], 0 offen offset:3584 // 0000000015DC: 
0xE0541400, 0x80020C2D, // buffer_load_dwordx2 v[12:13], v45, s[8:11], 0 offen offset:1024 // 0000000015E4: 
0xBF800000,             // s_nop 0                                                         // 0000000015EC: 
0xE05010C0, 0x80032007, // buffer_load_dword v32, v7, s[12:15], 0 offen offset:192         // 0000000015F0: 
0xB0120430,             // s_movk_i32 s18, 0x430                                           // 0000000015F8: 
0xBE8700FF, 0x0003C000, // s_mov_b32 s7, 0x3c000                                           // 0000000015FC: 
0xB0132800,             // s_movk_i32 s19, 0x2800                                          // 000000001604: 
0xD3D94004, 0x18000080, // v_accvgpr_write_b32 a4, 0                                       // 000000001608: 
0x104A4A13,             // v_mul_u32_u24_e32 v37, s19, v37                                 // 000000001610: 
0x244C4C84,             // v_lshlrev_b32_e32 v38, 4, v38                                   // 000000001614: 
0x24484882,             // v_lshlrev_b32_e32 v36, 2, v36                                   // 000000001618: 
0x7E0AA504,             // v_accvgpr_mov_b32 a5, a4                                        // 00000000161C: 
0x7E0CA504,             // v_accvgpr_mov_b32 a6, a4                                        // 000000001620: 
0x7E0EA504,             // v_accvgpr_mov_b32 a7, a4                                        // 000000001624: 
0xD3D94000, 0x18000080, // v_accvgpr_write_b32 a0, 0                                       // 000000001628: 
0xBF8C0F7F,             // s_waitcnt vmcnt(15)                                             // 000000001630: 
0xD81A0000, 0x00001F1E, // ds_write_b32 v30, v31                                           // 000000001634: 
0xD1FE001F, 0x0206532B, // v_add_lshl_u32 v31, v43, v41, 1                                 // 00000000163C: 
0xBF8C0F7D,             // s_waitcnt vmcnt(13)                                             // 000000001644: 
0xD2A00006, 0x00022D08, // v_pack_b32_f16 v6, v8, v22                                      // 000000001648: 
0xD2A01807, 0x00022D08, // v_pack_b32_f16 v7, v8, v22 op_sel:[1,1,0]                       // 000000001650: 
0xD2A00008, 0x00022F09, // v_pack_b32_f16 v8, v9, v23                                      // 000000001658: 
0xD2A01809, 0x00022F09, // v_pack_b32_f16 v9, v9, v23 op_sel:[1,1,0]                       // 000000001660: 
0xBF8C0F7B,             // s_waitcnt vmcnt(11)                                             // 000000001668: 
0xD2A00016, 0x0002311C, // v_pack_b32_f16 v22, v28, v24                                    // 00000000166C: 
0xD2A01817, 0x0002311C, // v_pack_b32_f16 v23, v28, v24 op_sel:[1,1,0]                     // 000000001674: 
0xD2A00018, 0x0002331D, // v_pack_b32_f16 v24, v29, v25                                    // 00000000167C: 
0xD2A01819, 0x0002331D, // v_pack_b32_f16 v25, v29, v25 op_sel:[1,1,0]                     // 000000001684: 
0xD1FE001D, 0x02065528, // v_add_lshl_u32 v29, v40, v42, 1                                 // 00000000168C: 
0xD9BE0650, 0x0000161D, // ds_write_b128 v29, v[22:25] offset:1616                         // 000000001694: 
0xD9BE0430, 0x0000061D, // ds_write_b128 v29, v[6:9] offset:1072                           // 00000000169C: 
0x68383A12,             // v_add_u32_e32 v28, s18, v29                                     // 0000000016A4: 
0x682C4E07,             // v_add_u32_e32 v22, s7, v39                                      // 0000000016A8: 
0x683A4EFF, 0x0003D000, // v_add_u32_e32 v29, 0x3d000, v39                                 // 0000000016AC: 
0xE0541000, 0x80020616, // buffer_load_dwordx2 v[6:7], v22, s[8:11], 0 offen               // 0000000016B4: 
0xE0541A00, 0x80020816, // buffer_load_dwordx2 v[8:9], v22, s[8:11], 0 offen offset:2560   // 0000000016BC: 
0xBF800000,             // s_nop 0                                                         // 0000000016C4: 
0xE0541E00, 0x8002161D, // buffer_load_dwordx2 v[22:23], v29, s[8:11], 0 offen offset:3584 // 0000000016C8: 
0xE0541400, 0x8002181D, // buffer_load_dwordx2 v[24:25], v29, s[8:11], 0 offen offset:1024 // 0000000016D0: 
0xD1FD001D, 0x0049032C, // v_lshl_add_u32 v29, v44, 1, s18                                 // 0000000016D8: 
0x9212FF10, 0x00190000, // s_mul_i32 s18, s16, 0x190000                                    // 0000000016E0: 
0x92101110,             // s_mul_i32 s16, s16, s17                                         // 0000000016E8: 
0xD1FF0025, 0x04982125, // v_add3_u32 v37, v37, s16, v38                                   // 0000000016EC: 
0xB0100140,             // s_movk_i32 s16, 0x140                                           // 0000000016F4: 
0x7E4E0212,             // v_mov_b32_e32 v39, s18                                          // 0000000016F8: 
0xD1FF0024, 0x00424925, // v_add3_u32 v36, v37, v36, s16                                   // 0000000016FC: 
0xD3D84025, 0x18000105, // v_accvgpr_read_b32 v37, a5                                      // 000000001704: 
0xD3D94001, 0x18000125, // v_accvgpr_write_b32 a1, v37                                     // 00000000170C: 
0xD3D84025, 0x18000106, // v_accvgpr_read_b32 v37, a6                                      // 000000001714: 
0xD1C30023, 0x049C2723, // v_mad_u32_u24 v35, v35, s19, v39                                // 00000000171C: 
0x8E128102,             // s_lshl_b32 s18, s2, 1                                           // 000000001724: 
0xD1FF0023, 0x004A5523, // v_add3_u32 v35, v35, v42, s18                                   // 000000001728: 
0xD3D94002, 0x18000125, // v_accvgpr_write_b32 a2, v37                                     // 000000001730: 
0xD3D84025, 0x18000107, // v_accvgpr_read_b32 v37, a7                                      // 000000001738: 
0x684646FF, 0x00050000, // v_add_u32_e32 v35, 0x50000, v35                                 // 000000001740: 
0xBE9000C3,             // s_mov_b32 s16, -3                                               // 000000001748: 
0xD3D94003, 0x18000125, // v_accvgpr_write_b32 a3, v37                                     // 00000000174C: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)   // 1754 <BB2_1>                          // 000000001754: 
0xBF8A0000,             // s_barrier                                                       // 000000001758: 
0xD9FE0000, 0x2600001F, // ds_read_b128 v[38:41], v31                                      // 00000000175C: 
0xD8700100, 0x2A00001D, // ds_read2st64_b32 v[42:43], v29 offset1:1                        // 000000001764: 
0xD86EC888, 0x2C00001D, // ds_read2_b32 v[44:45], v29 offset0:136 offset1:200              // 00000000176C: 
0x68683AFF, 0x00000060, // v_add_u32_e32 v52, 0x60, v29                                    // 000000001774: 
0x684A3AC0,             // v_add_u32_e32 v37, 64, v29                                      // 00000000177C: 
0xD8700706, 0x30000034, // ds_read2st64_b32 v[48:49], v52 offset0:6 offset1:7              // 000000001780: 
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                            // 000000001788: 
0x7E5C032A,             // v_mov_b32_e32 v46, v42                                          // 00000000178C: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 000000001790: 
0x7E5E032C,             // v_mov_b32_e32 v47, v44                                          // 000000001794: 
0x7E58032B,             // v_mov_b32_e32 v44, v43                                          // 000000001798: 
0xBF800000,             // s_nop 0                                                         // 00000000179C: 
0xD3CD8004, 0x04125D26, // v_mfma_f32_16x16x16f16 a[4:7], v[38:39], v[46:47], a[4:7]       // 0000000017A0: 
0xD8700504, 0x2E000025, // ds_read2st64_b32 v[46:47], v37 offset0:4 offset1:5              // 0000000017A8: 
0x81108310,             // s_add_i32 s16, s16, 3                                           // 0000000017B0: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 0000000017B4: 
0x7E660330,             // v_mov_b32_e32 v51, v48                                          // 0000000017B8: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 0000000017BC: 
0xBF8A0000,             // s_barrier                                                       // 0000000017C0: 
0xBF8C0F7E,             // s_waitcnt vmcnt(14)                                             // 0000000017C4: 
0xD81A0000, 0x0000221E, // ds_write_b32 v30, v34                                           // 0000000017C8: 
0x6C4448C0,             // v_subrev_u32_e32 v34, 64, v36                                   // 0000000017D0: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 0000000017D4: 
0x7E64032E,             // v_mov_b32_e32 v50, v46                                          // 0000000017D8: 
0x7E60032F,             // v_mov_b32_e32 v48, v47                                          // 0000000017DC: 
0xBF0A8B10,             // s_cmp_lt_u32 s16, 11                                            // 0000000017E0: 
0xD3CD8000, 0x04025926, // v_mfma_f32_16x16x16f16 a[0:3], v[38:39], v[44:45], a[0:3]       // 0000000017E4: 
0xE0501000, 0x80032222, // buffer_load_dword v34, v34, s[12:15], 0 offen                   // 0000000017EC: 
0xBF8C0F7D,             // s_waitcnt vmcnt(13)                                             // 0000000017F4: 
0xD2A00026, 0x00020902, // v_pack_b32_f16 v38, v2, v4                                      // 0000000017F8: 
0xD2A01827, 0x00020902, // v_pack_b32_f16 v39, v2, v4 op_sel:[1,1,0]                       // 000000001800: 
0xD3CD8004, 0x04126528, // v_mfma_f32_16x16x16f16 a[4:7], v[40:41], v[50:51], a[4:7]       // 000000001808: 
0xD3CD8000, 0x04026128, // v_mfma_f32_16x16x16f16 a[0:3], v[40:41], v[48:49], a[0:3]       // 000000001810: 
0xD2A00028, 0x00020B03, // v_pack_b32_f16 v40, v3, v5                                      // 000000001818: 
0xD2A01829, 0x00020B03, // v_pack_b32_f16 v41, v3, v5 op_sel:[1,1,0]                       // 000000001820: 
0xBF8C0F7B,             // s_waitcnt vmcnt(11)                                             // 000000001828: 
0xD2A00002, 0x00022514, // v_pack_b32_f16 v2, v20, v18                                     // 00000000182C: 
0xD2A01803, 0x00022514, // v_pack_b32_f16 v3, v20, v18 op_sel:[1,1,0]                      // 000000001834: 
0xD2A00004, 0x00022715, // v_pack_b32_f16 v4, v21, v19                                     // 00000000183C: 
0xD2A01805, 0x00022715, // v_pack_b32_f16 v5, v21, v19 op_sel:[1,1,0]                      // 000000001844: 
0xD9BE0220, 0x0000021C, // ds_write_b128 v28, v[2:5] offset:544                            // 00000000184C: 
0xD9BE0000, 0x0000261C, // ds_write_b128 v28, v[38:41]                                     // 000000001854: 
0x684C4603,             // v_add_u32_e32 v38, s3, v35                                      // 00000000185C: 
0xE0541000, 0x80020223, // buffer_load_dwordx2 v[2:3], v35, s[8:11], 0 offen               // 000000001860: 
0xE0541A00, 0x80020423, // buffer_load_dwordx2 v[4:5], v35, s[8:11], 0 offen offset:2560   // 000000001868: 
0xE0541E00, 0x80021226, // buffer_load_dwordx2 v[18:19], v38, s[8:11], 0 offen offset:3584 // 000000001870: 
0xE0541400, 0x80021426, // buffer_load_dwordx2 v[20:21], v38, s[8:11], 0 offen offset:1024 // 000000001878: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000001880: 
0xBF8A0000,             // s_barrier                                                       // 000000001884: 
0xD9FE0000, 0x2600001F, // ds_read_b128 v[38:41], v31                                      // 000000001888: 
0xD8700100, 0x2A00001D, // ds_read2st64_b32 v[42:43], v29 offset1:1                        // 000000001890: 
0xD86EC888, 0x2C00001D, // ds_read2_b32 v[44:45], v29 offset0:136 offset1:200              // 000000001898: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 0000000018A0: 
0x7E5C032A,             // v_mov_b32_e32 v46, v42                                          // 0000000018A4: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 0000000018A8: 
0x7E5E032C,             // v_mov_b32_e32 v47, v44                                          // 0000000018AC: 
0x7E58032B,             // v_mov_b32_e32 v44, v43                                          // 0000000018B0: 
0xBF800000,             // s_nop 0                                                         // 0000000018B4: 
0xD3CD8004, 0x04125D26, // v_mfma_f32_16x16x16f16 a[4:7], v[38:39], v[46:47], a[4:7]       // 0000000018B8: 
0xD8700504, 0x2E000025, // ds_read2st64_b32 v[46:47], v37 offset0:4 offset1:5              // 0000000018C0: 
0xD8700706, 0x30000034, // ds_read2st64_b32 v[48:49], v52 offset0:6 offset1:7              // 0000000018C8: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 0000000018D0: 
0xBF8A0000,             // s_barrier                                                       // 0000000018D4: 
0xBF8C0F7E,             // s_waitcnt vmcnt(14)                                             // 0000000018D8: 
0xD81A0000, 0x0000211E, // ds_write_b32 v30, v33                                           // 0000000018DC: 
0xE0501000, 0x80032124, // buffer_load_dword v33, v36, s[12:15], 0 offen                   // 0000000018E4: 
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                            // 0000000018EC: 
0x7E64032E,             // v_mov_b32_e32 v50, v46                                          // 0000000018F0: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 0000000018F4: 
0x7E660330,             // v_mov_b32_e32 v51, v48                                          // 0000000018F8: 
0x7E60032F,             // v_mov_b32_e32 v48, v47                                          // 0000000018FC: 
0xD3CD8000, 0x04025926, // v_mfma_f32_16x16x16f16 a[0:3], v[38:39], v[44:45], a[0:3]       // 000000001900: 
0xBF8C0F7D,             // s_waitcnt vmcnt(13)                                             // 000000001908: 
0xD2A00026, 0x0002210E, // v_pack_b32_f16 v38, v14, v16                                    // 00000000190C: 
0xD2A01827, 0x0002210E, // v_pack_b32_f16 v39, v14, v16 op_sel:[1,1,0]                     // 000000001914: 
0xD3CD8004, 0x04126528, // v_mfma_f32_16x16x16f16 a[4:7], v[40:41], v[50:51], a[4:7]       // 00000000191C: 
0xD3CD8000, 0x04026128, // v_mfma_f32_16x16x16f16 a[0:3], v[40:41], v[48:49], a[0:3]       // 000000001924: 
0xD2A00028, 0x0002230F, // v_pack_b32_f16 v40, v15, v17                                    // 00000000192C: 
0xD2A01829, 0x0002230F, // v_pack_b32_f16 v41, v15, v17 op_sel:[1,1,0]                     // 000000001934: 
0xBF8C0F7B,             // s_waitcnt vmcnt(11)                                             // 00000000193C: 
0xD2A0000E, 0x0002150C, // v_pack_b32_f16 v14, v12, v10                                    // 000000001940: 
0xD2A0180F, 0x0002150C, // v_pack_b32_f16 v15, v12, v10 op_sel:[1,1,0]                     // 000000001948: 
0xD2A00010, 0x0002170D, // v_pack_b32_f16 v16, v13, v11                                    // 000000001950: 
0xD2A01811, 0x0002170D, // v_pack_b32_f16 v17, v13, v11 op_sel:[1,1,0]                     // 000000001958: 
0xD9BE0220, 0x00000E1C, // ds_write_b128 v28, v[14:17] offset:544                          // 000000001960: 
0xD9BE0000, 0x0000261C, // ds_write_b128 v28, v[38:41]                                     // 000000001968: 
0x68144604,             // v_add_u32_e32 v10, s4, v35                                      // 000000001970: 
0x684C4605,             // v_add_u32_e32 v38, s5, v35                                      // 000000001974: 
0xE0541000, 0x80020E0A, // buffer_load_dwordx2 v[14:15], v10, s[8:11], 0 offen             // 000000001978: 
0xE0541A00, 0x8002100A, // buffer_load_dwordx2 v[16:17], v10, s[8:11], 0 offen offset:2560 // 000000001980: 
0xBF800000,             // s_nop 0                                                         // 000000001988: 
0xE0541E00, 0x80020A26, // buffer_load_dwordx2 v[10:11], v38, s[8:11], 0 offen offset:3584 // 00000000198C: 
0xE0541400, 0x80020C26, // buffer_load_dwordx2 v[12:13], v38, s[8:11], 0 offen offset:1024 // 000000001994: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 00000000199C: 
0xBF8A0000,             // s_barrier                                                       // 0000000019A0: 
0xD9FE0000, 0x2600001F, // ds_read_b128 v[38:41], v31                                      // 0000000019A4: 
0xD8700100, 0x2A00001D, // ds_read2st64_b32 v[42:43], v29 offset1:1                        // 0000000019AC: 
0xD86EC888, 0x2C00001D, // ds_read2_b32 v[44:45], v29 offset0:136 offset1:200              // 0000000019B4: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 0000000019BC: 
0x7E5C032A,             // v_mov_b32_e32 v46, v42                                          // 0000000019C0: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 0000000019C4: 
0x7E5E032C,             // v_mov_b32_e32 v47, v44                                          // 0000000019C8: 
0x7E58032B,             // v_mov_b32_e32 v44, v43                                          // 0000000019CC: 
0xBF800000,             // s_nop 0                                                         // 0000000019D0: 
0xD3CD8004, 0x04125D26, // v_mfma_f32_16x16x16f16 a[4:7], v[38:39], v[46:47], a[4:7]       // 0000000019D4: 
0xD8700504, 0x2E000025, // ds_read2st64_b32 v[46:47], v37 offset0:4 offset1:5              // 0000000019DC: 
0xD8700706, 0x30000034, // ds_read2st64_b32 v[48:49], v52 offset0:6 offset1:7              // 0000000019E4: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 0000000019EC: 
0xBF8A0000,             // s_barrier                                                       // 0000000019F0: 
0xBF8C0F7E,             // s_waitcnt vmcnt(14)                                             // 0000000019F4: 
0xD81A0000, 0x0000201E, // ds_write_b32 v30, v32                                           // 0000000019F8: 
0xE0501040, 0x80032024, // buffer_load_dword v32, v36, s[12:15], 0 offen offset:64         // 000000001A00: 
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                            // 000000001A08: 
0x7E64032E,             // v_mov_b32_e32 v50, v46                                          // 000000001A0C: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 000000001A10: 
0x7E660330,             // v_mov_b32_e32 v51, v48                                          // 000000001A14: 
0x7E60032F,             // v_mov_b32_e32 v48, v47                                          // 000000001A18: 
0x684A4606,             // v_add_u32_e32 v37, s6, v35                                      // 000000001A1C: 
0xD3CD8000, 0x04025926, // v_mfma_f32_16x16x16f16 a[0:3], v[38:39], v[44:45], a[0:3]       // 000000001A20: 
0xBF8C0F7D,             // s_waitcnt vmcnt(13)                                             // 000000001A28: 
0xD2A00026, 0x00021106, // v_pack_b32_f16 v38, v6, v8                                      // 000000001A2C: 
0xD2A01827, 0x00021106, // v_pack_b32_f16 v39, v6, v8 op_sel:[1,1,0]                       // 000000001A34: 
0x684848FF, 0x000000C0, // v_add_u32_e32 v36, 0xc0, v36                                    // 000000001A3C: 
0xD3CD8004, 0x04126528, // v_mfma_f32_16x16x16f16 a[4:7], v[40:41], v[50:51], a[4:7]       // 000000001A44: 
0xD3CD8000, 0x04026128, // v_mfma_f32_16x16x16f16 a[0:3], v[40:41], v[48:49], a[0:3]       // 000000001A4C: 
0xD2A00028, 0x00021307, // v_pack_b32_f16 v40, v7, v9                                      // 000000001A54: 
0xD2A01829, 0x00021307, // v_pack_b32_f16 v41, v7, v9 op_sel:[1,1,0]                       // 000000001A5C: 
0xBF8C0F7B,             // s_waitcnt vmcnt(11)                                             // 000000001A64: 
0xD2A00006, 0x00022D18, // v_pack_b32_f16 v6, v24, v22                                     // 000000001A68: 
0xD2A01807, 0x00022D18, // v_pack_b32_f16 v7, v24, v22 op_sel:[1,1,0]                      // 000000001A70: 
0xD2A00008, 0x00022F19, // v_pack_b32_f16 v8, v25, v23                                     // 000000001A78: 
0xD2A01809, 0x00022F19, // v_pack_b32_f16 v9, v25, v23 op_sel:[1,1,0]                      // 000000001A80: 
0x682C460E,             // v_add_u32_e32 v22, s14, v35                                     // 000000001A88: 
0xD9BE0220, 0x0000061C, // ds_write_b128 v28, v[6:9] offset:544                            // 000000001A8C: 
0xD9BE0000, 0x0000261C, // ds_write_b128 v28, v[38:41]                                     // 000000001A94: 
0xE0541000, 0x80020616, // buffer_load_dwordx2 v[6:7], v22, s[8:11], 0 offen               // 000000001A9C: 
0xE0541A00, 0x80020816, // buffer_load_dwordx2 v[8:9], v22, s[8:11], 0 offen offset:2560   // 000000001AA4: 
0xBF800000,             // s_nop 0                                                         // 000000001AAC: 
0xE0541E00, 0x80021625, // buffer_load_dwordx2 v[22:23], v37, s[8:11], 0 offen offset:3584 // 000000001AB0: 
0xE0541400, 0x80021825, // buffer_load_dwordx2 v[24:25], v37, s[8:11], 0 offen offset:1024 // 000000001AB8: 
0x68464607,             // v_add_u32_e32 v35, s7, v35                                      // 000000001AC0: 
0xBF85FF23,             // s_cbranch_scc1 BB2_1                                            // 000000001AC4: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000001AC8: 
0xBF8A0000,             // s_barrier                                                       // 000000001ACC: 
0xD9FE0000, 0x2600001F, // ds_read_b128 v[38:41], v31                                      // 000000001AD0: 
0xD8700100, 0x2A00001D, // ds_read2st64_b32 v[42:43], v29 offset1:1                        // 000000001AD8: 
0xD86EC888, 0x2C00001D, // ds_read2_b32 v[44:45], v29 offset0:136 offset1:200              // 000000001AE0: 
0x68683AC0,             // v_add_u32_e32 v52, 64, v29                                      // 000000001AE8: 
0x686A3AFF, 0x00000060, // v_add_u32_e32 v53, 0x60, v29                                    // 000000001AEC: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 000000001AF4: 
0x7E5C032A,             // v_mov_b32_e32 v46, v42                                          // 000000001AF8: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000001AFC: 
0x7E5E032C,             // v_mov_b32_e32 v47, v44                                          // 000000001B00: 
0x7E58032B,             // v_mov_b32_e32 v44, v43                                          // 000000001B04: 
0xBF800000,             // s_nop 0                                                         // 000000001B08: 
0xD3CD8004, 0x04125D26, // v_mfma_f32_16x16x16f16 a[4:7], v[38:39], v[46:47], a[4:7]       // 000000001B0C: 
0xD8700504, 0x2E000034, // ds_read2st64_b32 v[46:47], v52 offset0:4 offset1:5              // 000000001B14: 
0xD8700706, 0x30000035, // ds_read2st64_b32 v[48:49], v53 offset0:6 offset1:7              // 000000001B1C: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000001B24: 
0xBF8A0000,             // s_barrier                                                       // 000000001B28: 
0xBF8C0F7E,             // s_waitcnt vmcnt(14)                                             // 000000001B2C: 
0xD81A0000, 0x0000221E, // ds_write_b32 v30, v34                                           // 000000001B30: 
0x6C4448C0,             // v_subrev_u32_e32 v34, 64, v36                                   // 000000001B38: 
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                            // 000000001B3C: 
0x7E64032E,             // v_mov_b32_e32 v50, v46                                          // 000000001B40: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 000000001B44: 
0x7E660330,             // v_mov_b32_e32 v51, v48                                          // 000000001B48: 
0x7E60032F,             // v_mov_b32_e32 v48, v47                                          // 000000001B4C: 
0xB0041000,             // s_movk_i32 s4, 0x1000                                           // 000000001B50: 
0xD3CD8000, 0x04025926, // v_mfma_f32_16x16x16f16 a[0:3], v[38:39], v[44:45], a[0:3]       // 000000001B54: 
0xB0031400,             // s_movk_i32 s3, 0x1400                                           // 000000001B5C: 
0xD3CD8004, 0x04126528, // v_mfma_f32_16x16x16f16 a[4:7], v[40:41], v[50:51], a[4:7]       // 000000001B60: 
0xE0501000, 0x80033222, // buffer_load_dword v50, v34, s[12:15], 0 offen                   // 000000001B68: 
0xBF8C0F7D,             // s_waitcnt vmcnt(13)                                             // 000000001B70: 
0xD2A00024, 0x00020902, // v_pack_b32_f16 v36, v2, v4                                      // 000000001B74: 
0xD2A01825, 0x00020902, // v_pack_b32_f16 v37, v2, v4 op_sel:[1,1,0]                       // 000000001B7C: 
0xD2A00026, 0x00020B03, // v_pack_b32_f16 v38, v3, v5                                      // 000000001B84: 
0xD2A01827, 0x00020B03, // v_pack_b32_f16 v39, v3, v5 op_sel:[1,1,0]                       // 000000001B8C: 
0xBF8C0F7B,             // s_waitcnt vmcnt(11)                                             // 000000001B94: 
0xD2A00002, 0x00022514, // v_pack_b32_f16 v2, v20, v18                                     // 000000001B98: 
0xD2A01803, 0x00022514, // v_pack_b32_f16 v3, v20, v18 op_sel:[1,1,0]                      // 000000001BA0: 
0xD2A00004, 0x00022715, // v_pack_b32_f16 v4, v21, v19                                     // 000000001BA8: 
0xD2A01805, 0x00022715, // v_pack_b32_f16 v5, v21, v19 op_sel:[1,1,0]                      // 000000001BB0: 
0xD9BE0220, 0x0000021C, // ds_write_b128 v28, v[2:5] offset:544                            // 000000001BB8: 
0xD9BE0000, 0x0000241C, // ds_write_b128 v28, v[36:39]                                     // 000000001BC0: 
0xE0541A00, 0x80022423, // buffer_load_dwordx2 v[36:37], v35, s[8:11], 0 offen offset:2560 // 000000001BC8: 
0x68044604,             // v_add_u32_e32 v2, s4, v35                                       // 000000001BD0: 
0xD3CD8000, 0x04026128, // v_mfma_f32_16x16x16f16 a[0:3], v[40:41], v[48:49], a[0:3]       // 000000001BD4: 
0xE0541000, 0x80022623, // buffer_load_dwordx2 v[38:39], v35, s[8:11], 0 offen             // 000000001BDC: 
0xE0541E00, 0x80022802, // buffer_load_dwordx2 v[40:41], v2, s[8:11], 0 offen offset:3584  // 000000001BE4: 
0xE0541400, 0x80022A02, // buffer_load_dwordx2 v[42:43], v2, s[8:11], 0 offen offset:1024  // 000000001BEC: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000001BF4: 
0xBF8A0000,             // s_barrier                                                       // 000000001BF8: 
0xD9FE0000, 0x0200001F, // ds_read_b128 v[2:5], v31                                        // 000000001BFC: 
0xD8700100, 0x1200001D, // ds_read2st64_b32 v[18:19], v29 offset1:1                        // 000000001C04: 
0xD86EC888, 0x1400001D, // ds_read2_b32 v[20:21], v29 offset0:136 offset1:200              // 000000001C0C: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 000000001C14: 
0x7E440312,             // v_mov_b32_e32 v34, v18                                          // 000000001C18: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000001C1C: 
0x7E460314,             // v_mov_b32_e32 v35, v20                                          // 000000001C20: 
0x7E280313,             // v_mov_b32_e32 v20, v19                                          // 000000001C24: 
0xBF800000,             // s_nop 0                                                         // 000000001C28: 
0xD3CD8004, 0x04124502, // v_mfma_f32_16x16x16f16 a[4:7], v[2:3], v[34:35], a[4:7]         // 000000001C2C: 
0xD8700504, 0x22000034, // ds_read2st64_b32 v[34:35], v52 offset0:4 offset1:5              // 000000001C34: 
0xD8700706, 0x2C000035, // ds_read2st64_b32 v[44:45], v53 offset0:6 offset1:7              // 000000001C3C: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000001C44: 
0xBF8A0000,             // s_barrier                                                       // 000000001C48: 
0xBF8C0F7E,             // s_waitcnt vmcnt(14)                                             // 000000001C4C: 
0xD81A0000, 0x0000211E, // ds_write_b32 v30, v33                                           // 000000001C50: 
0xBF8C0F7C,             // s_waitcnt vmcnt(12)                                             // 000000001C58: 
0xD2A00012, 0x0002210E, // v_pack_b32_f16 v18, v14, v16                                    // 000000001C5C: 
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                            // 000000001C64: 
0x7E5C0322,             // v_mov_b32_e32 v46, v34                                          // 000000001C68: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 000000001C6C: 
0x7E5E032C,             // v_mov_b32_e32 v47, v44                                          // 000000001C70: 
0x7E580323,             // v_mov_b32_e32 v44, v35                                          // 000000001C74: 
0xD2A01813, 0x0002210E, // v_pack_b32_f16 v19, v14, v16 op_sel:[1,1,0]                     // 000000001C78: 
0xD3CD8000, 0x04022902, // v_mfma_f32_16x16x16f16 a[0:3], v[2:3], v[20:21], a[0:3]         // 000000001C80: 
0xD2A00014, 0x0002230F, // v_pack_b32_f16 v20, v15, v17                                    // 000000001C88: 
0xD2A01815, 0x0002230F, // v_pack_b32_f16 v21, v15, v17 op_sel:[1,1,0]                     // 000000001C90: 
0xBF8C0F7A,             // s_waitcnt vmcnt(10)                                             // 000000001C98: 
0xD2A0000E, 0x0002150C, // v_pack_b32_f16 v14, v12, v10                                    // 000000001C9C: 
0xD2A0180F, 0x0002150C, // v_pack_b32_f16 v15, v12, v10 op_sel:[1,1,0]                     // 000000001CA4: 
0xD2A00010, 0x0002170D, // v_pack_b32_f16 v16, v13, v11                                    // 000000001CAC: 
0xD2A01811, 0x0002170D, // v_pack_b32_f16 v17, v13, v11 op_sel:[1,1,0]                     // 000000001CB4: 
0xD9BE0220, 0x00000E1C, // ds_write_b128 v28, v[14:17] offset:544                          // 000000001CBC: 
0xD9BE0000, 0x0000121C, // ds_write_b128 v28, v[18:21]                                     // 000000001CC4: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000001CCC: 
0xBF8A0000,             // s_barrier                                                       // 000000001CD0: 
0xD3CD8004, 0x04125D04, // v_mfma_f32_16x16x16f16 a[4:7], v[4:5], v[46:47], a[4:7]         // 000000001CD4: 
0xD3CD8000, 0x04025904, // v_mfma_f32_16x16x16f16 a[0:3], v[4:5], v[44:45], a[0:3]         // 000000001CDC: 
0xD9FE0000, 0x0200001F, // ds_read_b128 v[2:5], v31                                        // 000000001CE4: 
0xD8700100, 0x0A00001D, // ds_read2st64_b32 v[10:11], v29 offset1:1                        // 000000001CEC: 
0xD86EC888, 0x0E00001D, // ds_read2_b32 v[14:15], v29 offset0:136 offset1:200              // 000000001CF4: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 000000001CFC: 
0x7E18030A,             // v_mov_b32_e32 v12, v10                                          // 000000001D00: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000001D04: 
0x7E1A030E,             // v_mov_b32_e32 v13, v14                                          // 000000001D08: 
0x7E1C030B,             // v_mov_b32_e32 v14, v11                                          // 000000001D0C: 
0xBF800000,             // s_nop 0                                                         // 000000001D10: 
0xD3CD8004, 0x04121902, // v_mfma_f32_16x16x16f16 a[4:7], v[2:3], v[12:13], a[4:7]         // 000000001D14: 
0xD8700504, 0x0C000034, // ds_read2st64_b32 v[12:13], v52 offset0:4 offset1:5              // 000000001D1C: 
0xD8700706, 0x10000035, // ds_read2st64_b32 v[16:17], v53 offset0:6 offset1:7              // 000000001D24: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000001D2C: 
0xBF8A0000,             // s_barrier                                                       // 000000001D30: 
0xBF8C0F79,             // s_waitcnt vmcnt(9)                                              // 000000001D34: 
0xD81A0000, 0x0000201E, // ds_write_b32 v30, v32                                           // 000000001D38: 
0xBF8C0F77,             // s_waitcnt vmcnt(7)                                              // 000000001D40: 
0xD2A0000A, 0x00021106, // v_pack_b32_f16 v10, v6, v8                                      // 000000001D44: 
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                            // 000000001D4C: 
0x7E24030C,             // v_mov_b32_e32 v18, v12                                          // 000000001D50: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 000000001D54: 
0x7E260310,             // v_mov_b32_e32 v19, v16                                          // 000000001D58: 
0xD2A0180B, 0x00021106, // v_pack_b32_f16 v11, v6, v8 op_sel:[1,1,0]                       // 000000001D5C: 
0x7E20030D,             // v_mov_b32_e32 v16, v13                                          // 000000001D64: 
0xD2A0000C, 0x00021307, // v_pack_b32_f16 v12, v7, v9                                      // 000000001D68: 
0xD2A0180D, 0x00021307, // v_pack_b32_f16 v13, v7, v9 op_sel:[1,1,0]                       // 000000001D70: 
0xBF8C0F75,             // s_waitcnt vmcnt(5)                                              // 000000001D78: 
0xD2A00006, 0x00022D18, // v_pack_b32_f16 v6, v24, v22                                     // 000000001D7C: 
0xD2A01807, 0x00022D18, // v_pack_b32_f16 v7, v24, v22 op_sel:[1,1,0]                      // 000000001D84: 
0xD2A00008, 0x00022F19, // v_pack_b32_f16 v8, v25, v23                                     // 000000001D8C: 
0xD2A01809, 0x00022F19, // v_pack_b32_f16 v9, v25, v23 op_sel:[1,1,0]                      // 000000001D94: 
0xD9BE0220, 0x0000061C, // ds_write_b128 v28, v[6:9] offset:544                            // 000000001D9C: 
0xD9BE0000, 0x00000A1C, // ds_write_b128 v28, v[10:13]                                     // 000000001DA4: 
0xD3CD8000, 0x04021D02, // v_mfma_f32_16x16x16f16 a[0:3], v[2:3], v[14:15], a[0:3]         // 000000001DAC: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000001DB4: 
0xBF8A0000,             // s_barrier                                                       // 000000001DB8: 
0xD3CD8004, 0x04122504, // v_mfma_f32_16x16x16f16 a[4:7], v[4:5], v[18:19], a[4:7]         // 000000001DBC: 
0xD3CD8000, 0x04022104, // v_mfma_f32_16x16x16f16 a[0:3], v[4:5], v[16:17], a[0:3]         // 000000001DC4: 
0xD9FE0000, 0x0200001F, // ds_read_b128 v[2:5], v31                                        // 000000001DCC: 
0xD8700100, 0x0600001D, // ds_read2st64_b32 v[6:7], v29 offset1:1                          // 000000001DD4: 
0xD86EC888, 0x0E00001D, // ds_read2_b32 v[14:15], v29 offset0:136 offset1:200              // 000000001DDC: 
0xD8700504, 0x0A000034, // ds_read2st64_b32 v[10:11], v52 offset0:4 offset1:5              // 000000001DE4: 
0xD8700706, 0x10000035, // ds_read2st64_b32 v[16:17], v53 offset0:6 offset1:7              // 000000001DEC: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000001DF4: 
0xBF8A0000,             // s_barrier                                                       // 000000001DF8: 
0xBF8C0F74,             // s_waitcnt vmcnt(4)                                              // 000000001DFC: 
0xD81A0000, 0x0000321E, // ds_write_b32 v30, v50                                           // 000000001E00: 
0xBF8CC47F,             // s_waitcnt lgkmcnt(4)                                            // 000000001E08: 
0x7E100306,             // v_mov_b32_e32 v8, v6                                            // 000000001E0C: 
0xBF8CC37F,             // s_waitcnt lgkmcnt(3)                                            // 000000001E10: 
0x7E12030E,             // v_mov_b32_e32 v9, v14                                           // 000000001E14: 
0x7E1C0307,             // v_mov_b32_e32 v14, v7                                           // 000000001E18: 
0xBF800000,             // s_nop 0                                                         // 000000001E1C: 
0xD3CD8004, 0x04121102, // v_mfma_f32_16x16x16f16 a[4:7], v[2:3], v[8:9], a[4:7]           // 000000001E20: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 000000001E28: 
0x7E120310,             // v_mov_b32_e32 v9, v16                                           // 000000001E2C: 
0x7E10030A,             // v_mov_b32_e32 v8, v10                                           // 000000001E30: 
0x7E20030B,             // v_mov_b32_e32 v16, v11                                          // 000000001E34: 
0xD3CD8000, 0x04021D02, // v_mfma_f32_16x16x16f16 a[0:3], v[2:3], v[14:15], a[0:3]         // 000000001E38: 
0xBF8C0F72,             // s_waitcnt vmcnt(2)                                              // 000000001E40: 
0xD2A00006, 0x00024926, // v_pack_b32_f16 v6, v38, v36                                     // 000000001E44: 
0xD2A01807, 0x00024926, // v_pack_b32_f16 v7, v38, v36 op_sel:[1,1,0]                      // 000000001E4C: 
0xD3CD8004, 0x04121104, // v_mfma_f32_16x16x16f16 a[4:7], v[4:5], v[8:9], a[4:7]           // 000000001E54: 
0xD2A00008, 0x00024B27, // v_pack_b32_f16 v8, v39, v37                                     // 000000001E5C: 
0xD2A01809, 0x00024B27, // v_pack_b32_f16 v9, v39, v37 op_sel:[1,1,0]                      // 000000001E64: 
0xBF8C0F70,             // s_waitcnt vmcnt(0)                                              // 000000001E6C: 
0xD2A0000A, 0x0002512A, // v_pack_b32_f16 v10, v42, v40                                    // 000000001E70: 
0xD2A0180B, 0x0002512A, // v_pack_b32_f16 v11, v42, v40 op_sel:[1,1,0]                     // 000000001E78: 
0xD2A0000C, 0x0002532B, // v_pack_b32_f16 v12, v43, v41                                    // 000000001E80: 
0xD2A0180D, 0x0002532B, // v_pack_b32_f16 v13, v43, v41 op_sel:[1,1,0]                     // 000000001E88: 
0xD9BE0220, 0x00000A1C, // ds_write_b128 v28, v[10:13] offset:544                          // 000000001E90: 
0xD9BE0000, 0x0000061C, // ds_write_b128 v28, v[6:9]                                       // 000000001E98: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000001EA0: 
0xBF8A0000,             // s_barrier                                                       // 000000001EA4: 
0xD9FE0000, 0x0600001F, // ds_read_b128 v[6:9], v31                                        // 000000001EA8: 
0xD8700100, 0x0200001D, // ds_read2st64_b32 v[2:3], v29 offset1:1                          // 000000001EB0: 
0xD86EC888, 0x0A00001D, // ds_read2_b32 v[10:11], v29 offset0:136 offset1:200              // 000000001EB8: 
0xD3CD8000, 0x04022104, // v_mfma_f32_16x16x16f16 a[0:3], v[4:5], v[16:17], a[0:3]         // 000000001EC0: 
0xD8700504, 0x04000034, // ds_read2st64_b32 v[4:5], v52 offset0:4 offset1:5                // 000000001EC8: 
0xD8700706, 0x0E000035, // ds_read2st64_b32 v[14:15], v53 offset0:6 offset1:7              // 000000001ED0: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000001ED8: 
0xBF8A0000,             // s_barrier                                                       // 000000001EDC: 
0xBF8CC37F,             // s_waitcnt lgkmcnt(3)                                            // 000000001EE0: 
0x7E180302,             // v_mov_b32_e32 v12, v2                                           // 000000001EE4: 
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                            // 000000001EE8: 
0x7E1A030A,             // v_mov_b32_e32 v13, v10                                          // 000000001EEC: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 000000001EF0: 
0x7E200304,             // v_mov_b32_e32 v16, v4                                           // 000000001EF4: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000001EF8: 
0x7E22030E,             // v_mov_b32_e32 v17, v14                                          // 000000001EFC: 
0x7E140303,             // v_mov_b32_e32 v10, v3                                           // 000000001F00: 
0x260436FF, 0x7FFFFFC0, // v_and_b32_e32 v2, 0x7fffffc0, v27                               // 000000001F04: 
0xD3CD8004, 0x04121906, // v_mfma_f32_16x16x16f16 a[4:7], v[6:7], v[12:13], a[4:7]         // 000000001F0C: 
0xD1FD0003, 0x046D0F1A, // v_lshl_add_u32 v3, v26, 7, v27                                  // 000000001F14: 
0x6A040503,             // v_sub_u32_e32 v2, v3, v2                                        // 000000001F1C: 
0x0C0602FF, 0xFFFFFFC0, // v_mul_i32_i24_e32 v3, 0xffffffc0, v1                            // 000000001F20: 
0x7E080202,             // v_mov_b32_e32 v4, s2                                            // 000000001F28: 
0xD1FE0000, 0x02060103, // v_add_lshl_u32 v0, v3, v0, 1                                    // 000000001F2C: 
0x24060289,             // v_lshlrev_b32_e32 v3, 9, v1                                     // 000000001F34: 
0xD1C30001, 0x04100701, // v_mad_u32_u24 v1, v1, s3, v4                                    // 000000001F38: 
0x7E1C0305,             // v_mov_b32_e32 v14, v5                                           // 000000001F40: 
0xD3CD8004, 0x04122108, // v_mfma_f32_16x16x16f16 a[4:7], v[8:9], v[16:17], a[4:7]         // 000000001F44: 
0x24040481,             // v_lshlrev_b32_e32 v2, 1, v2                                     // 000000001F4C: 
0xBE8300FF, 0x00020000, // s_mov_b32 s3, 0x20000                                           // 000000001F50: 
0xBE8200FF, 0x0000A000, // s_mov_b32 s2, 0xa000                                            // 000000001F58: 
0xD3CD8000, 0x04021506, // v_mfma_f32_16x16x16f16 a[0:3], v[6:7], v[10:11], a[0:3]         // 000000001F60: 
0xBF800006,             // s_nop 6                                                         // 000000001F68: 
0xD3D84004, 0x18000104, // v_accvgpr_read_b32 v4, a4                                       // 000000001F6C: 
0xD3D84005, 0x18000105, // v_accvgpr_read_b32 v5, a5                                       // 000000001F74: 
0x7E081504,             // v_cvt_f16_f32_e32 v4, v4                                        // 000000001F7C: 
0xD3CD8000, 0x04021D08, // v_mfma_f32_16x16x16f16 a[0:3], v[8:9], v[14:15], a[0:3]         // 000000001F80: 
0x7E0A1505,             // v_cvt_f16_f32_e32 v5, v5                                        // 000000001F88: 
0xD83E0000, 0x00000402, // ds_write_b16 v2, v4                                             // 000000001F8C: 
0xD83E0100, 0x00000502, // ds_write_b16 v2, v5 offset:256                                  // 000000001F94: 
0xD3D84004, 0x18000106, // v_accvgpr_read_b32 v4, a6                                       // 000000001F9C: 
0xD3D84005, 0x18000107, // v_accvgpr_read_b32 v5, a7                                       // 000000001FA4: 
0x7E081504,             // v_cvt_f16_f32_e32 v4, v4                                        // 000000001FAC: 
0x7E0A1505,             // v_cvt_f16_f32_e32 v5, v5                                        // 000000001FB0: 
0xD83E0200, 0x00000402, // ds_write_b16 v2, v4 offset:512                                  // 000000001FB4: 
0xD83E0300, 0x00000502, // ds_write_b16 v2, v5 offset:768                                  // 000000001FBC: 
0xBF800001,             // s_nop 1                                                         // 000000001FC4: 
0xD3D84004, 0x18000103, // v_accvgpr_read_b32 v4, a3                                       // 000000001FC8: 
0xD3D84005, 0x18000102, // v_accvgpr_read_b32 v5, a2                                       // 000000001FD0: 
0x7E081504,             // v_cvt_f16_f32_e32 v4, v4                                        // 000000001FD8: 
0xD3D84006, 0x18000101, // v_accvgpr_read_b32 v6, a1                                       // 000000001FDC: 
0x7E0A1505,             // v_cvt_f16_f32_e32 v5, v5                                        // 000000001FE4: 
0xD3D84007, 0x18000100, // v_accvgpr_read_b32 v7, a0                                       // 000000001FE8: 
0x7E0C1506,             // v_cvt_f16_f32_e32 v6, v6                                        // 000000001FF0: 
0x7E0E1507,             // v_cvt_f16_f32_e32 v7, v7                                        // 000000001FF4: 
0xD83E0380, 0x00000402, // ds_write_b16 v2, v4 offset:896                                  // 000000001FF8: 
0xD83E0280, 0x00000502, // ds_write_b16 v2, v5 offset:640                                  // 000000002000: 
0xD83E0180, 0x00000602, // ds_write_b16 v2, v6 offset:384                                  // 000000002008: 
0xD83E0080, 0x00000702, // ds_write_b16 v2, v7 offset:128                                  // 000000002010: 
0xD1FE0002, 0x02060700, // v_add_lshl_u32 v2, v0, v3, 1                                    // 000000002018: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000002020: 
0xBF8A0000,             // s_barrier                                                       // 000000002024: 
0xD86C0000, 0x03000002, // ds_read_b32 v3, v2                                              // 000000002028: 
0xD1FE0000, 0x02060101, // v_add_lshl_u32 v0, v1, v0, 1                                    // 000000002030: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000002038: 
0xE1381000, 0x80000300, // buffer_atomic_pk_add_f16 v3, v0, s[0:3], 0 offen                // 00000000203C: 
0xD86C0100, 0x01000002, // ds_read_b32 v1, v2 offset:256                                   // 000000002044: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 00000000204C: 
0xE1381A00, 0x80000100, // buffer_atomic_pk_add_f16 v1, v0, s[0:3], 0 offen offset:2560    // 000000002050: 
0xD86C0200, 0x01000002, // ds_read_b32 v1, v2 offset:512                                   // 000000002058: 
0x68000004,             // v_add_u32_e32 v0, s4, v0                                        // 000000002060: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000002064: 
0xE1381400, 0x80000100, // buffer_atomic_pk_add_f16 v1, v0, s[0:3], 0 offen offset:1024    // 000000002068: 
0xD86C0300, 0x01000002, // ds_read_b32 v1, v2 offset:768                                   // 000000002070: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000002078: 
0xE1381E00, 0x80000100, // buffer_atomic_pk_add_f16 v1, v0, s[0:3], 0 offen offset:3584    // 00000000207C: 
0xBF810000              // s_endpgm                                                        // 000000002084: 
};


const uint32_t GEMM_CK_ISA_16_5120_1280[] = {
// Original ISA:
// s4-s5 : kernarg pointer
// s6 : workgroup_id_x

// Hacked ISA
// s0-s1: matrix C pointer
// s2-s3: matrix A pointer
// s4-s5: matrix B pointer
// s6 : workgroup_id_x
0xBE880002,             // s_mov_b32 s8, s2
0xBE890003,             // s_mov_b32 s9, s3
0xBE8C0004,             // s_mov_b32 s12, s4
0xBE8D0005,             // s_mov_b32 s13, s5
0xBF800000,             // s_nop 0
0xBF800000,             // s_nop 0

0x9682FF06, 0x66666667, // s_mul_hi_i32 s2, s6, 0x66666667                                 // 000000002100: 
0x8F039F02,             // s_lshr_b32 s3, s2, 31                                           // 000000002108: 
0x90028402,             // s_ashr_i32 s2, s2, 4                                            // 00000000210C: 
0x20040082,             // v_lshrrev_b32_e32 v2, 2, v0                                     // 000000002110: 
0x20060084,             // v_lshrrev_b32_e32 v3, 4, v0                                     // 000000002114: 
0x81030302,             // s_add_i32 s3, s2, s3                                            // 000000002118: 
0x0C0204C4,             // v_mul_i32_i24_e32 v1, -4, v2                                    // 00000000211C: 
0xD1C20004, 0x04098903, // v_mad_i32_i24 v4, v3, -4, v2                                    // 000000002120: 
0x9202A803,             // s_mul_i32 s2, s3, 40                                            // 000000002128: 

//0xC0060202, 0x00000000, // s_load_dwordx2 s[8:9], s[4:5], 0x0                              // 00000000212C: 
//0xC0060302, 0x00000008, // s_load_dwordx2 s[12:13], s[4:5], 0x8                            // 000000002134: 
//0xC0060002, 0x00000010, // s_load_dwordx2 s[0:1], s[4:5], 0x10                             // 00000000213C: 

0xD1FE000C, 0x02060101, // v_add_lshl_u32 v12, v1, v0, 1                                   // 000000002144: 
0x8E038803,             // s_lshl_b32 s3, s3, 8                                            // 00000000214C: 
0x24020883,             // v_lshlrev_b32_e32 v1, 3, v4                                     // 000000002150: 
0xB0050088,             // s_movk_i32 s5, 0x88                                             // 000000002154: 
0xD1FF0005, 0x04300701, // v_add3_u32 v5, v1, s3, v12                                      // 000000002158: 
0xD2850001, 0x00000B04, // v_mul_lo_u32 v1, v4, s5                                         // 000000002160: 
0x100C06FF, 0x00000500, // v_mul_u32_u24_e32 v6, 0x500, v3                                 // 000000002168: 
0xD1FD0003, 0x04050703, // v_lshl_add_u32 v3, v3, 3, v1                                    // 000000002170: 
0x20020085,             // v_lshrrev_b32_e32 v1, 5, v0                                     // 000000002178: 
0xB004FFE0,             // s_movk_i32 s4, 0xffe0                                           // 00000000217C: 
0xD2000007, 0x000D0501, // v_lshl_or_b32 v7, v1, 2, s3                                     // 000000002180: 
0xB0031400,             // s_movk_i32 s3, 0x1400                                           // 000000002188: 
0xD1C20004, 0x04000901, // v_mad_i32_i24 v4, v1, s4, v0                                    // 00000000218C: 
0xD2850007, 0x00000707, // v_mul_lo_u32 v7, v7, s3                                         // 000000002194: 
0x81820206,             // s_sub_i32 s2, s6, s2                                            // 00000000219C: 
0x101A02FF, 0x00000220, // v_mul_u32_u24_e32 v13, 0x220, v1                                // 0000000021A0: 
0x241C0883,             // v_lshlrev_b32_e32 v14, 3, v4                                    // 0000000021A8: 
0xD1FD0007, 0x041D0504, // v_lshl_add_u32 v7, v4, 2, v7                                    // 0000000021AC: 
0x260200BF,             // v_and_b32_e32 v1, 63, v0                                        // 0000000021B4: 
0x260800B0,             // v_and_b32_e32 v4, 48, v0                                        // 0000000021B8: 
0x6A080901,             // v_sub_u32_e32 v4, v1, v4                                        // 0000000021BC: 
0x8E028702,             // s_lshl_b32 s2, s2, 7                                            // 0000000021C0: 
0xD1C80001, 0x02090900, // v_bfe_u32 v1, v0, 4, 2                                          // 0000000021C4: 
0xBE8B00FF, 0x00020000, // s_mov_b32 s11, 0x20000                                          // 0000000021CC: 
0xD1FE0028, 0x02040507, // v_add_lshl_u32 v40, v7, s2, 1                                   // 0000000021D4: 
0x10240205,             // v_mul_u32_u24_e32 v18, s5, v1                                   // 0000000021DC: 
0x20020086,             // v_lshrrev_b32_e32 v1, 6, v0                                     // 0000000021E0: 
0x2604048C,             // v_and_b32_e32 v2, 12, v2                                        // 0000000021E4: 
0xB0042000,             // s_movk_i32 s4, 0x2000                                           // 0000000021E8: 
0xB0057000,             // s_movk_i32 s5, 0x7000                                           // 0000000021EC: 
0xB0065000,             // s_movk_i32 s6, 0x5000                                           // 0000000021F0: 
0xBE8A00FF, 0x0000A000, // s_mov_b32 s10, 0xa000                                           // 0000000021F4: 
0xD1FE0027, 0x02060D05, // v_add_lshl_u32 v39, v5, v6, 1                                   // 0000000021FC: 
0xBE8E00FF, 0x00C80000, // s_mov_b32 s14, 0xc80000                                         // 000000002204: 
0xBE8F000B,             // s_mov_b32 s15, s11                                              // 00000000220C: 
0x24260883,             // v_lshlrev_b32_e32 v19, 3, v4                                    // 000000002210: 
0xD1FD0026, 0x04110901, // v_lshl_add_u32 v38, v1, 4, v4                                   // 000000002214: 
0x102804FF, 0x00000110, // v_mul_u32_u24_e32 v20, 0x110, v2                                // 00000000221C: 

//0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000002224: 
0xBF800000,             // s_nop 0

0xE0501000, 0x80020227, // buffer_load_dword v2, v39, s[8:11], 0 offen                     // 000000002228: 
0xBF800000,             // s_nop 0                                                         // 000000002230: 
0x681E5004,             // v_add_u32_e32 v15, s4, v40                                      // 000000002234: 
0x68205005,             // v_add_u32_e32 v16, s5, v40                                      // 000000002238: 
0x68225006,             // v_add_u32_e32 v17, s6, v40                                      // 00000000223C: 
0xE0541000, 0x80030428, // buffer_load_dwordx2 v[4:5], v40, s[12:15], 0 offen              // 000000002240: 
0xE0541800, 0x8003060F, // buffer_load_dwordx2 v[6:7], v15, s[12:15], 0 offen offset:2048  // 000000002248: 
0xE0541800, 0x80030810, // buffer_load_dwordx2 v[8:9], v16, s[12:15], 0 offen offset:2048  // 000000002250: 
0xE0541000, 0x80030A11, // buffer_load_dwordx2 v[10:11], v17, s[12:15], 0 offen            // 000000002258: 
0x681E50FF, 0x00050000, // v_add_u32_e32 v15, 0x50000, v40                                 // 000000002260: 
0xBF800000,             // s_nop 0                                                         // 000000002268: 
0xE0501040, 0x80021E27, // buffer_load_dword v30, v39, s[8:11], 0 offen offset:64          // 00000000226C: 
0xBF800000,             // s_nop 0                                                         // 000000002274: 
0x682050FF, 0x00052000, // v_add_u32_e32 v16, 0x52000, v40                                 // 000000002278: 
0x682250FF, 0x00057000, // v_add_u32_e32 v17, 0x57000, v40                                 // 000000002280: 
0x683E50FF, 0x00055000, // v_add_u32_e32 v31, 0x55000, v40                                 // 000000002288: 
0xE0541000, 0x8003160F, // buffer_load_dwordx2 v[22:23], v15, s[12:15], 0 offen            // 000000002290: 
0xE0541800, 0x80031810, // buffer_load_dwordx2 v[24:25], v16, s[12:15], 0 offen offset:2048// 000000002298: 
0xE0541800, 0x80031A11, // buffer_load_dwordx2 v[26:27], v17, s[12:15], 0 offen offset:2048// 0000000022A0: 
0xE0541000, 0x80031C1F, // buffer_load_dwordx2 v[28:29], v31, s[12:15], 0 offen            // 0000000022A8: 
0x684050FF, 0x000A0000, // v_add_u32_e32 v32, 0xa0000, v40                                 // 0000000022B0: 
0xBF800000,             // s_nop 0                                                         // 0000000022B8: 
0xE0501080, 0x80023227, // buffer_load_dword v50, v39, s[8:11], 0 offen offset:128         // 0000000022BC: 
0x684250FF, 0x000A2000, // v_add_u32_e32 v33, 0xa2000, v40                                 // 0000000022C4: 
0x684450FF, 0x000A7000, // v_add_u32_e32 v34, 0xa7000, v40                                 // 0000000022CC: 
0x684650FF, 0x000A5000, // v_add_u32_e32 v35, 0xa5000, v40                                 // 0000000022D4: 
0xBF800000,             // s_nop 0                                                         // 0000000022DC: 
0xE0541000, 0x80033420, // buffer_load_dwordx2 v[52:53], v32, s[12:15], 0 offen            // 0000000022E0: 
0xE0541800, 0x80033621, // buffer_load_dwordx2 v[54:55], v33, s[12:15], 0 offen offset:2048// 0000000022E8: 
0xE0541800, 0x80033822, // buffer_load_dwordx2 v[56:57], v34, s[12:15], 0 offen offset:2048// 0000000022F0: 
0xE0541000, 0x80033A23, // buffer_load_dwordx2 v[58:59], v35, s[12:15], 0 offen            // 0000000022F8: 
0xD1FE0029, 0x02061903, // v_add_lshl_u32 v41, v3, v12, 1                                  // 000000002300: 
0xBF800000,             // s_nop 0                                                         // 000000002308: 
0xE05010C0, 0x80023027, // buffer_load_dword v48, v39, s[8:11], 0 offen offset:192         // 00000000230C: 
0xD1FE002A, 0x02061D0D, // v_add_lshl_u32 v42, v13, v14, 1                                 // 000000002314: 
0xD1FE002B, 0x02062513, // v_add_lshl_u32 v43, v19, v18, 1                                 // 00000000231C: 
0x242A4C81,             // v_lshlrev_b32_e32 v21, 1, v38                                   // 000000002324: 
0xBF8C0F7F,             // s_waitcnt vmcnt(15)                                             // 000000002328: 
0xD81A0000, 0x00000229, // ds_write_b32 v41, v2                                            // 00000000232C: 
0xBF8C0F7D,             // s_waitcnt vmcnt(13)                                             // 000000002334: 
0xD2A00002, 0x00020D04, // v_pack_b32_f16 v2, v4, v6                                       // 000000002338: 
0xD2A01803, 0x00020D04, // v_pack_b32_f16 v3, v4, v6 op_sel:[1,1,0]                        // 000000002340: 
0xD2A00004, 0x00020F05, // v_pack_b32_f16 v4, v5, v7                                       // 000000002348: 
0xD2A01805, 0x00020F05, // v_pack_b32_f16 v5, v5, v7 op_sel:[1,1,0]                        // 000000002350: 
0xBF8C0F7B,             // s_waitcnt vmcnt(11)                                             // 000000002358: 
0xD2A00006, 0x0002110A, // v_pack_b32_f16 v6, v10, v8                                      // 00000000235C: 
0xD2A01807, 0x0002110A, // v_pack_b32_f16 v7, v10, v8 op_sel:[1,1,0]                       // 000000002364: 
0xD2A00008, 0x0002130B, // v_pack_b32_f16 v8, v11, v9                                      // 00000000236C: 
0xD2A01809, 0x0002130B, // v_pack_b32_f16 v9, v11, v9 op_sel:[1,1,0]                       // 000000002374: 
0xD9BE0650, 0x0000062A, // ds_write_b128 v42, v[6:9] offset:1616                           // 00000000237C: 
0xD9BE0430, 0x0000022A, // ds_write_b128 v42, v[2:5] offset:1072                           // 000000002384: 
0x680450FF, 0x000F0000, // v_add_u32_e32 v2, 0xf0000, v40                                  // 00000000238C: 
0x680650FF, 0x000F2000, // v_add_u32_e32 v3, 0xf2000, v40                                  // 000000002394: 
0x680850FF, 0x000F7000, // v_add_u32_e32 v4, 0xf7000, v40                                  // 00000000239C: 
0x680A50FF, 0x000F5000, // v_add_u32_e32 v5, 0xf5000, v40                                  // 0000000023A4: 
0xE0541000, 0x80030E02, // buffer_load_dwordx2 v[14:15], v2, s[12:15], 0 offen             // 0000000023AC: 
0xE0541800, 0x80031003, // buffer_load_dwordx2 v[16:17], v3, s[12:15], 0 offen offset:2048 // 0000000023B4: 
0xE0541800, 0x80030A04, // buffer_load_dwordx2 v[10:11], v4, s[12:15], 0 offen offset:2048 // 0000000023BC: 
0xE0541000, 0x80030C05, // buffer_load_dwordx2 v[12:13], v5, s[12:15], 0 offen             // 0000000023C4: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 0000000023CC: 
0xBF8A0000,             // s_barrier                                                       // 0000000023D0: 
0xD9FE0000, 0x0200002B, // ds_read_b128 v[2:5], v43                                        // 0000000023D4: 
0xD1FE0008, 0x02062915, // v_add_lshl_u32 v8, v21, v20, 1                                  // 0000000023DC: 
0x685810B0,             // v_add_u32_e32 v44, 48, v8                                       // 0000000023E4: 
0x685E10FF, 0x00000050, // v_add_u32_e32 v47, 0x50, v8                                     // 0000000023E8: 
0xD8700504, 0x3C00002C, // ds_read2st64_b32 v[60:61], v44 offset0:4 offset1:5              // 0000000023F0: 
0xD8700706, 0x1200002F, // ds_read2st64_b32 v[18:19], v47 offset0:6 offset1:7              // 0000000023F8: 
0x685A10FF, 0x00000070, // v_add_u32_e32 v45, 0x70, v8                                     // 000000002400: 
0x685C10FF, 0x00000090, // v_add_u32_e32 v46, 0x90, v8                                     // 000000002408: 
0xD8700908, 0x3E00002D, // ds_read2st64_b32 v[62:63], v45 offset0:8 offset1:9              // 000000002410: 
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                            // 000000002418: 
0x7E0C033C,             // v_mov_b32_e32 v6, v60                                           // 00000000241C: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 000000002420: 
0x7E0E0312,             // v_mov_b32_e32 v7, v18                                           // 000000002424: 
0xD8700B0A, 0x1400002E, // ds_read2st64_b32 v[20:21], v46 offset0:10 offset1:11            // 000000002428: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000002430: 
0xBF8A0000,             // s_barrier                                                       // 000000002434: 
0xBF8C0F7E,             // s_waitcnt vmcnt(14)                                             // 000000002438: 
0xD81A0000, 0x00001E29, // ds_write_b32 v41, v30                                           // 00000000243C: 
0xD3CD8000, 0x02020D02, // v_mfma_f32_16x16x16f16 a[0:3], v[2:3], v[6:7], 0                // 000000002444: 
0xBE83000B,             // s_mov_b32 s3, s11                                               // 00000000244C: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 000000002450: 
0x7E0E0314,             // v_mov_b32_e32 v7, v20                                           // 000000002454: 
0x7E0C033E,             // v_mov_b32_e32 v6, v62                                           // 000000002458: 
0xE0501100, 0x80023127, // buffer_load_dword v49, v39, s[8:11], 0 offen offset:256         // 00000000245C: 
0x7E24033D,             // v_mov_b32_e32 v18, v61                                          // 000000002464: 
0x7E28033F,             // v_mov_b32_e32 v20, v63                                          // 000000002468: 
0xD3CD8000, 0x04020D04, // v_mfma_f32_16x16x16f16 a[0:3], v[4:5], v[6:7], a[0:3]           // 00000000246C: 
0xBF8C0F7D,             // s_waitcnt vmcnt(13)                                             // 000000002474: 
0xD2A00006, 0x00023116, // v_pack_b32_f16 v6, v22, v24                                     // 000000002478: 
0xD2A01807, 0x00023116, // v_pack_b32_f16 v7, v22, v24 op_sel:[1,1,0]                      // 000000002480: 
0xD2A00008, 0x00023317, // v_pack_b32_f16 v8, v23, v25                                     // 000000002488: 
0xD2A01809, 0x00023317, // v_pack_b32_f16 v9, v23, v25 op_sel:[1,1,0]                      // 000000002490: 
0xBF8C0F7B,             // s_waitcnt vmcnt(11)                                             // 000000002498: 
0xD2A00016, 0x0002351C, // v_pack_b32_f16 v22, v28, v26                                    // 00000000249C: 
0xD2A01817, 0x0002351C, // v_pack_b32_f16 v23, v28, v26 op_sel:[1,1,0]                     // 0000000024A4: 
0xD2A00018, 0x0002371D, // v_pack_b32_f16 v24, v29, v27                                    // 0000000024AC: 
0xD2A01819, 0x0002371D, // v_pack_b32_f16 v25, v29, v27 op_sel:[1,1,0]                     // 0000000024B4: 
0xD9BE0650, 0x0000162A, // ds_write_b128 v42, v[22:25] offset:1616                         // 0000000024BC: 
0xD9BE0430, 0x0000062A, // ds_write_b128 v42, v[6:9] offset:1072                           // 0000000024C4: 
0x680C50FF, 0x00140000, // v_add_u32_e32 v6, 0x140000, v40                                 // 0000000024CC: 
0x680E50FF, 0x00142000, // v_add_u32_e32 v7, 0x142000, v40                                 // 0000000024D4: 
0x681050FF, 0x00147000, // v_add_u32_e32 v8, 0x147000, v40                                 // 0000000024DC: 
0x681250FF, 0x00145000, // v_add_u32_e32 v9, 0x145000, v40                                 // 0000000024E4: 
0xE0541000, 0x80031A06, // buffer_load_dwordx2 v[26:27], v6, s[12:15], 0 offen             // 0000000024EC: 
0xE0541800, 0x80031C07, // buffer_load_dwordx2 v[28:29], v7, s[12:15], 0 offen offset:2048 // 0000000024F4: 
0xE0541800, 0x80031608, // buffer_load_dwordx2 v[22:23], v8, s[12:15], 0 offen offset:2048 // 0000000024FC: 
0xE0541000, 0x80031809, // buffer_load_dwordx2 v[24:25], v9, s[12:15], 0 offen             // 000000002504: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 00000000250C: 
0xBF8A0000,             // s_barrier                                                       // 000000002510: 
0xD8700504, 0x1E00002C, // ds_read2st64_b32 v[30:31], v44 offset0:4 offset1:5              // 000000002514: 
0xD8700706, 0x2000002F, // ds_read2st64_b32 v[32:33], v47 offset0:6 offset1:7              // 00000000251C: 
0xD9FE0000, 0x0600002B, // ds_read_b128 v[6:9], v43                                        // 000000002524: 
0xD3CD8004, 0x02022502, // v_mfma_f32_16x16x16f16 a[4:7], v[2:3], v[18:19], 0              // 00000000252C: 
0x680450FF, 0x00195000, // v_add_u32_e32 v2, 0x195000, v40                                 // 000000002534: 
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                            // 00000000253C: 
0x7E44031E,             // v_mov_b32_e32 v34, v30                                          // 000000002540: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 000000002544: 
0x7E460320,             // v_mov_b32_e32 v35, v32                                          // 000000002548: 
0x683C50FF, 0x00190000, // v_add_u32_e32 v30, 0x190000, v40                                // 00000000254C: 
0x684050FF, 0x00192000, // v_add_u32_e32 v32, 0x192000, v40                                // 000000002554: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 00000000255C: 
0xD3CD8000, 0x04024506, // v_mfma_f32_16x16x16f16 a[0:3], v[6:7], v[34:35], a[0:3]         // 000000002560: 
0xD8700908, 0x2200002D, // ds_read2st64_b32 v[34:35], v45 offset0:8 offset1:9              // 000000002568: 
0xD8700B0A, 0x2400002E, // ds_read2st64_b32 v[36:37], v46 offset0:10 offset1:11            // 000000002570: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000002578: 
0xBF8A0000,             // s_barrier                                                       // 00000000257C: 
0xBF8C0F7E,             // s_waitcnt vmcnt(14)                                             // 000000002580: 
0xD81A0000, 0x00003229, // ds_write_b32 v41, v50                                           // 000000002584: 
0xE0501140, 0x80025227, // buffer_load_dword v82, v39, s[8:11], 0 offen offset:320         // 00000000258C: 
0xBF8C0F7D,             // s_waitcnt vmcnt(13)                                             // 000000002594: 
0xD2A00032, 0x00026D34, // v_pack_b32_f16 v50, v52, v54                                    // 000000002598: 
0xD2A01833, 0x00026D34, // v_pack_b32_f16 v51, v52, v54 op_sel:[1,1,0]                     // 0000000025A0: 
0xD2A00034, 0x00026F35, // v_pack_b32_f16 v52, v53, v55                                    // 0000000025A8: 
0xD2A01835, 0x00026F35, // v_pack_b32_f16 v53, v53, v55 op_sel:[1,1,0]                     // 0000000025B0: 
0xBF8C0F7B,             // s_waitcnt vmcnt(11)                                             // 0000000025B8: 
0xD2A00036, 0x0002713A, // v_pack_b32_f16 v54, v58, v56                                    // 0000000025BC: 
0xD2A01837, 0x0002713A, // v_pack_b32_f16 v55, v58, v56 op_sel:[1,1,0]                     // 0000000025C4: 
0xD2A00038, 0x0002733B, // v_pack_b32_f16 v56, v59, v57                                    // 0000000025CC: 
0xD2A01839, 0x0002733B, // v_pack_b32_f16 v57, v59, v57 op_sel:[1,1,0]                     // 0000000025D4: 
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                            // 0000000025DC: 
0x7E740322,             // v_mov_b32_e32 v58, v34                                          // 0000000025E0: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 0000000025E4: 
0x7E760324,             // v_mov_b32_e32 v59, v36                                          // 0000000025E8: 
0xD9BE0650, 0x0000362A, // ds_write_b128 v42, v[54:57] offset:1616                         // 0000000025EC: 
0xD9BE0430, 0x0000322A, // ds_write_b128 v42, v[50:53] offset:1072                         // 0000000025F4: 
0xD3CD8000, 0x04027508, // v_mfma_f32_16x16x16f16 a[0:3], v[8:9], v[58:59], a[0:3]         // 0000000025FC: 
0x684450FF, 0x00197000, // v_add_u32_e32 v34, 0x197000, v40                                // 000000002604: 
0xE0541000, 0x8003321E, // buffer_load_dwordx2 v[50:51], v30, s[12:15], 0 offen            // 00000000260C: 
0xE0541800, 0x80033420, // buffer_load_dwordx2 v[52:53], v32, s[12:15], 0 offen offset:2048// 000000002614: 
0xE0541800, 0x80033622, // buffer_load_dwordx2 v[54:55], v34, s[12:15], 0 offen offset:2048// 00000000261C: 
0xE0541000, 0x80033802, // buffer_load_dwordx2 v[56:57], v2, s[12:15], 0 offen             // 000000002624: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 00000000262C: 
0xBF8A0000,             // s_barrier                                                       // 000000002630: 
0xD8700504, 0x3A00002C, // ds_read2st64_b32 v[58:59], v44 offset0:4 offset1:5              // 000000002634: 
0x7E40031F,             // v_mov_b32_e32 v32, v31                                          // 00000000263C: 
0x7E480323,             // v_mov_b32_e32 v36, v35                                          // 000000002640: 
0xD3CD8004, 0x04122904, // v_mfma_f32_16x16x16f16 a[4:7], v[4:5], v[20:21], a[4:7]         // 000000002644: 
0xD8700706, 0x3C00002F, // ds_read2st64_b32 v[60:61], v47 offset0:6 offset1:7              // 00000000264C: 
0xD8700908, 0x3E00002D, // ds_read2st64_b32 v[62:63], v45 offset0:8 offset1:9              // 000000002654: 
0xD8700B0A, 0x4000002E, // ds_read2st64_b32 v[64:65], v46 offset0:10 offset1:11            // 00000000265C: 
0xD9FE0000, 0x0200002B, // ds_read_b128 v[2:5], v43                                        // 000000002664: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 00000000266C: 
0xBF8A0000,             // s_barrier                                                       // 000000002670: 
0xBF8C0F7E,             // s_waitcnt vmcnt(14)                                             // 000000002674: 
0xD81A0000, 0x00003029, // ds_write_b32 v41, v48                                           // 000000002678: 
0xE0501180, 0x80025327, // buffer_load_dword v83, v39, s[8:11], 0 offen offset:384         // 000000002680: 
0xBF8C0F7D,             // s_waitcnt vmcnt(13)                                             // 000000002688: 
0xD2A00012, 0x0002210E, // v_pack_b32_f16 v18, v14, v16                                    // 00000000268C: 
0xD2A01813, 0x0002210E, // v_pack_b32_f16 v19, v14, v16 op_sel:[1,1,0]                     // 000000002694: 
0xD2A00014, 0x0002230F, // v_pack_b32_f16 v20, v15, v17                                    // 00000000269C: 
0xD2A01815, 0x0002230F, // v_pack_b32_f16 v21, v15, v17 op_sel:[1,1,0]                     // 0000000026A4: 
0xBF8CC47F,             // s_waitcnt lgkmcnt(4)                                            // 0000000026AC: 
0x7E1E033C,             // v_mov_b32_e32 v15, v60                                          // 0000000026B0: 
0x7E1C033A,             // v_mov_b32_e32 v14, v58                                          // 0000000026B4: 
0xD3CD8004, 0x04124106, // v_mfma_f32_16x16x16f16 a[4:7], v[6:7], v[32:33], a[4:7]         // 0000000026B8: 
0x680C50FF, 0x00230000, // v_add_u32_e32 v6, 0x230000, v40                                 // 0000000026C0: 
0x680E50FF, 0x00232000, // v_add_u32_e32 v7, 0x232000, v40                                 // 0000000026C8: 
0x7E78033B,             // v_mov_b32_e32 v60, v59                                          // 0000000026D0: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 0000000026D4: 
0xD3CD8000, 0x04021D02, // v_mfma_f32_16x16x16f16 a[0:3], v[2:3], v[14:15], a[0:3]         // 0000000026D8: 
0xBF8C0F7B,             // s_waitcnt vmcnt(11)                                             // 0000000026E0: 
0xD2A0000E, 0x0002150C, // v_pack_b32_f16 v14, v12, v10                                    // 0000000026E4: 
0xD2A0180F, 0x0002150C, // v_pack_b32_f16 v15, v12, v10 op_sel:[1,1,0]                     // 0000000026EC: 
0xD2A00010, 0x0002170D, // v_pack_b32_f16 v16, v13, v11                                    // 0000000026F4: 
0xD2A01811, 0x0002170D, // v_pack_b32_f16 v17, v13, v11 op_sel:[1,1,0]                     // 0000000026FC: 
0x7E160340,             // v_mov_b32_e32 v11, v64                                          // 000000002704: 
0x7E14033E,             // v_mov_b32_e32 v10, v62                                          // 000000002708: 
0xD9BE0650, 0x00000E2A, // ds_write_b128 v42, v[14:17] offset:1616                         // 00000000270C: 
0xD9BE0430, 0x0000122A, // ds_write_b128 v42, v[18:21] offset:1072                         // 000000002714: 
0x681850FF, 0x001E7000, // v_add_u32_e32 v12, 0x1e7000, v40                                // 00000000271C: 
0x681A50FF, 0x001E5000, // v_add_u32_e32 v13, 0x1e5000, v40                                // 000000002724: 
0x7E80033F,             // v_mov_b32_e32 v64, v63                                          // 00000000272C: 
0xD3CD8000, 0x04021504, // v_mfma_f32_16x16x16f16 a[0:3], v[4:5], v[10:11], a[0:3]         // 000000002730: 
0x681450FF, 0x001E0000, // v_add_u32_e32 v10, 0x1e0000, v40                                // 000000002738: 
0x681650FF, 0x001E2000, // v_add_u32_e32 v11, 0x1e2000, v40                                // 000000002740: 
0xE0541000, 0x8003420A, // buffer_load_dwordx2 v[66:67], v10, s[12:15], 0 offen            // 000000002748: 
0xE0541800, 0x8003440B, // buffer_load_dwordx2 v[68:69], v11, s[12:15], 0 offen offset:2048// 000000002750: 
0xE0541800, 0x8003460C, // buffer_load_dwordx2 v[70:71], v12, s[12:15], 0 offen offset:2048// 000000002758: 
0xE0541000, 0x8003480D, // buffer_load_dwordx2 v[72:73], v13, s[12:15], 0 offen            // 000000002760: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000002768: 
0xBF8A0000,             // s_barrier                                                       // 00000000276C: 
0xD8700504, 0x4A00002C, // ds_read2st64_b32 v[74:75], v44 offset0:4 offset1:5              // 000000002770: 
0xD8700706, 0x4C00002F, // ds_read2st64_b32 v[76:77], v47 offset0:6 offset1:7              // 000000002778: 
0xD9FE0000, 0x0A00002B, // ds_read_b128 v[10:13], v43                                      // 000000002780: 
0xD8700908, 0x4E00002D, // ds_read2st64_b32 v[78:79], v45 offset0:8 offset1:9              // 000000002788: 
0xD8700B0A, 0x5000002E, // ds_read2st64_b32 v[80:81], v46 offset0:10 offset1:11            // 000000002790: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000002798: 
0xBF8A0000,             // s_barrier                                                       // 00000000279C: 
0xBF8CC47F,             // s_waitcnt lgkmcnt(4)                                            // 0000000027A0: 
0x7E1C034A,             // v_mov_b32_e32 v14, v74                                          // 0000000027A4: 
0xBF8CC37F,             // s_waitcnt lgkmcnt(3)                                            // 0000000027A8: 
0x7E1E034C,             // v_mov_b32_e32 v15, v76                                          // 0000000027AC: 
0xBF8C0F7E,             // s_waitcnt vmcnt(14)                                             // 0000000027B0: 
0xD81A0000, 0x00003129, // ds_write_b32 v41, v49                                           // 0000000027B4: 
0xE05011C0, 0x80022727, // buffer_load_dword v39, v39, s[8:11], 0 offen offset:448         // 0000000027BC: 
0xD3CD8004, 0x04124908, // v_mfma_f32_16x16x16f16 a[4:7], v[8:9], v[36:37], a[4:7]         // 0000000027C4: 
0x681050FF, 0x00237000, // v_add_u32_e32 v8, 0x237000, v40                                 // 0000000027CC: 
0x681250FF, 0x00235000, // v_add_u32_e32 v9, 0x235000, v40                                 // 0000000027D4: 
0x7E98034B,             // v_mov_b32_e32 v76, v75                                          // 0000000027DC: 
0xBF8CC37F,             // s_waitcnt lgkmcnt(3)                                            // 0000000027E0: 
0xD3CD8000, 0x04021D0A, // v_mfma_f32_16x16x16f16 a[0:3], v[10:11], v[14:15], a[0:3]       // 0000000027E4: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 0000000027EC: 
0x7E1E0350,             // v_mov_b32_e32 v15, v80                                          // 0000000027F0: 
0x7E1C034E,             // v_mov_b32_e32 v14, v78                                          // 0000000027F4: 
0x7EA0034F,             // v_mov_b32_e32 v80, v79                                          // 0000000027F8: 
0xD3CD8004, 0x04127902, // v_mfma_f32_16x16x16f16 a[4:7], v[2:3], v[60:61], a[4:7]         // 0000000027FC: 
0xD3CD8000, 0x04021D0C, // v_mfma_f32_16x16x16f16 a[0:3], v[12:13], v[14:15], a[0:3]       // 000000002804: 
0xBF8C0F7D,             // s_waitcnt vmcnt(13)                                             // 00000000280C: 
0xD2A0000E, 0x0002391A, // v_pack_b32_f16 v14, v26, v28                                    // 000000002810: 
0xD2A0180F, 0x0002391A, // v_pack_b32_f16 v15, v26, v28 op_sel:[1,1,0]                     // 000000002818: 
0xD2A00010, 0x00023B1B, // v_pack_b32_f16 v16, v27, v29                                    // 000000002820: 
0xD2A01811, 0x00023B1B, // v_pack_b32_f16 v17, v27, v29 op_sel:[1,1,0]                     // 000000002828: 
0xBF8C0F7B,             // s_waitcnt vmcnt(11)                                             // 000000002830: 
0xD2A00012, 0x00022D18, // v_pack_b32_f16 v18, v24, v22                                    // 000000002834: 
0xD2A01813, 0x00022D18, // v_pack_b32_f16 v19, v24, v22 op_sel:[1,1,0]                     // 00000000283C: 
0xD2A00014, 0x00022F19, // v_pack_b32_f16 v20, v25, v23                                    // 000000002844: 
0xD2A01815, 0x00022F19, // v_pack_b32_f16 v21, v25, v23 op_sel:[1,1,0]                     // 00000000284C: 
0xD9BE0650, 0x0000122A, // ds_write_b128 v42, v[18:21] offset:1616                         // 000000002854: 
0xD9BE0430, 0x00000E2A, // ds_write_b128 v42, v[14:17] offset:1072                         // 00000000285C: 
0xE0541000, 0x80031606, // buffer_load_dwordx2 v[22:23], v6, s[12:15], 0 offen             // 000000002864: 
0xE0541800, 0x80031807, // buffer_load_dwordx2 v[24:25], v7, s[12:15], 0 offen offset:2048 // 00000000286C: 
0xE0541800, 0x80031A08, // buffer_load_dwordx2 v[26:27], v8, s[12:15], 0 offen offset:2048 // 000000002874: 
0xE0541000, 0x80031C09, // buffer_load_dwordx2 v[28:29], v9, s[12:15], 0 offen             // 00000000287C: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000002884: 
0xBF8A0000,             // s_barrier                                                       // 000000002888: 
0xD8700504, 0x1E00002C, // ds_read2st64_b32 v[30:31], v44 offset0:4 offset1:5              // 00000000288C: 
0xD8700706, 0x2000002F, // ds_read2st64_b32 v[32:33], v47 offset0:6 offset1:7              // 000000002894: 
0xD3CD8004, 0x04128104, // v_mfma_f32_16x16x16f16 a[4:7], v[4:5], v[64:65], a[4:7]         // 00000000289C: 
0xD9FE0000, 0x0600002B, // ds_read_b128 v[6:9], v43                                        // 0000000028A4: 
0xD8700908, 0x2200002D, // ds_read2st64_b32 v[34:35], v45 offset0:8 offset1:9              // 0000000028AC: 
0xD8700B0A, 0x2400002E, // ds_read2st64_b32 v[36:37], v46 offset0:10 offset1:11            // 0000000028B4: 
0xBF8CC47F,             // s_waitcnt lgkmcnt(4)                                            // 0000000028BC: 
0x7E1C031E,             // v_mov_b32_e32 v14, v30                                          // 0000000028C0: 
0xBF8CC37F,             // s_waitcnt lgkmcnt(3)                                            // 0000000028C4: 
0x7E1E0320,             // v_mov_b32_e32 v15, v32                                          // 0000000028C8: 
0x7E40031F,             // v_mov_b32_e32 v32, v31                                          // 0000000028CC: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 0000000028D0: 
0xBF8A0000,             // s_barrier                                                       // 0000000028D4: 
0xBF8C0F7E,             // s_waitcnt vmcnt(14)                                             // 0000000028D8: 
0xD81A0000, 0x00005229, // ds_write_b32 v41, v82                                           // 0000000028DC: 
0xD3CD8004, 0x0412990A, // v_mfma_f32_16x16x16f16 a[4:7], v[10:11], v[76:77], a[4:7]       // 0000000028E4: 
0xBF8CC37F,             // s_waitcnt lgkmcnt(3)                                            // 0000000028EC: 
0xD3CD8000, 0x04021D06, // v_mfma_f32_16x16x16f16 a[0:3], v[6:7], v[14:15], a[0:3]         // 0000000028F0: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 0000000028F8: 
0x7E1E0324,             // v_mov_b32_e32 v15, v36                                          // 0000000028FC: 
0x7E1C0322,             // v_mov_b32_e32 v14, v34                                          // 000000002900: 
0x7E480323,             // v_mov_b32_e32 v36, v35                                          // 000000002904: 
0xD3CD8004, 0x0412A10C, // v_mfma_f32_16x16x16f16 a[4:7], v[12:13], v[80:81], a[4:7]       // 000000002908: 
0xD3CD8000, 0x04021D08, // v_mfma_f32_16x16x16f16 a[0:3], v[8:9], v[14:15], a[0:3]         // 000000002910: 
0xBF8C0F7C,             // s_waitcnt vmcnt(12)                                             // 000000002918: 
0xD2A0000E, 0x00026932, // v_pack_b32_f16 v14, v50, v52                                    // 00000000291C: 
0xD2A0180F, 0x00026932, // v_pack_b32_f16 v15, v50, v52 op_sel:[1,1,0]                     // 000000002924: 
0xD2A00010, 0x00026B33, // v_pack_b32_f16 v16, v51, v53                                    // 00000000292C: 
0xD2A01811, 0x00026B33, // v_pack_b32_f16 v17, v51, v53 op_sel:[1,1,0]                     // 000000002934: 
0xBF8C0F7A,             // s_waitcnt vmcnt(10)                                             // 00000000293C: 
0xD2A00012, 0x00026D38, // v_pack_b32_f16 v18, v56, v54                                    // 000000002940: 
0xD2A01813, 0x00026D38, // v_pack_b32_f16 v19, v56, v54 op_sel:[1,1,0]                     // 000000002948: 
0xD2A00014, 0x00026F39, // v_pack_b32_f16 v20, v57, v55                                    // 000000002950: 
0xD2A01815, 0x00026F39, // v_pack_b32_f16 v21, v57, v55 op_sel:[1,1,0]                     // 000000002958: 
0xD9BE0650, 0x0000122A, // ds_write_b128 v42, v[18:21] offset:1616                         // 000000002960: 
0xD9BE0430, 0x00000E2A, // ds_write_b128 v42, v[14:17] offset:1072                         // 000000002968: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000002970: 
0xBF8A0000,             // s_barrier                                                       // 000000002974: 
0xD8700504, 0x3000002C, // ds_read2st64_b32 v[48:49], v44 offset0:4 offset1:5              // 000000002978: 
0xD8700706, 0x3200002F, // ds_read2st64_b32 v[50:51], v47 offset0:6 offset1:7              // 000000002980: 
0xD8700908, 0x3400002D, // ds_read2st64_b32 v[52:53], v45 offset0:8 offset1:9              // 000000002988: 
0xD8700B0A, 0x3600002E, // ds_read2st64_b32 v[54:55], v46 offset0:10 offset1:11            // 000000002990: 
0xD9FE0000, 0x0200002B, // ds_read_b128 v[2:5], v43                                        // 000000002998: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 0000000029A0: 
0xBF8A0000,             // s_barrier                                                       // 0000000029A4: 
0xBF8CC37F,             // s_waitcnt lgkmcnt(3)                                            // 0000000029A8: 
0x7E260332,             // v_mov_b32_e32 v19, v50                                          // 0000000029AC: 
0xD3CD8004, 0x04124106, // v_mfma_f32_16x16x16f16 a[4:7], v[6:7], v[32:33], a[4:7]         // 0000000029B0: 
0x7E240330,             // v_mov_b32_e32 v18, v48                                          // 0000000029B8: 
0x7E640331,             // v_mov_b32_e32 v50, v49                                          // 0000000029BC: 
0xBF8C0F79,             // s_waitcnt vmcnt(9)                                              // 0000000029C0: 
0xD81A0000, 0x00005329, // ds_write_b32 v41, v83                                           // 0000000029C4: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 0000000029CC: 
0xD3CD8000, 0x04022502, // v_mfma_f32_16x16x16f16 a[0:3], v[2:3], v[18:19], a[0:3]         // 0000000029D0: 
0xD3CD8004, 0x04124908, // v_mfma_f32_16x16x16f16 a[4:7], v[8:9], v[36:37], a[4:7]         // 0000000029D8: 
0xBF8C0F77,             // s_waitcnt vmcnt(7)                                              // 0000000029E0: 
0xD2A0000E, 0x00028942, // v_pack_b32_f16 v14, v66, v68                                    // 0000000029E4: 
0xD2A0180F, 0x00028942, // v_pack_b32_f16 v15, v66, v68 op_sel:[1,1,0]                     // 0000000029EC: 
0xD2A00010, 0x00028B43, // v_pack_b32_f16 v16, v67, v69                                    // 0000000029F4: 
0xD2A01811, 0x00028B43, // v_pack_b32_f16 v17, v67, v69 op_sel:[1,1,0]                     // 0000000029FC: 
0xBF8C0F75,             // s_waitcnt vmcnt(5)                                              // 000000002A04: 
0xD2A00012, 0x00028D48, // v_pack_b32_f16 v18, v72, v70                                    // 000000002A08: 
0xD2A01813, 0x00028D48, // v_pack_b32_f16 v19, v72, v70 op_sel:[1,1,0]                     // 000000002A10: 
0xD2A00014, 0x00028F49, // v_pack_b32_f16 v20, v73, v71                                    // 000000002A18: 
0xD2A01815, 0x00028F49, // v_pack_b32_f16 v21, v73, v71 op_sel:[1,1,0]                     // 000000002A20: 
0xD9BE0650, 0x0000122A, // ds_write_b128 v42, v[18:21] offset:1616                         // 000000002A28: 
0x7E260336,             // v_mov_b32_e32 v19, v54                                          // 000000002A30: 
0x7E240334,             // v_mov_b32_e32 v18, v52                                          // 000000002A34: 
0xD9BE0430, 0x00000E2A, // ds_write_b128 v42, v[14:17] offset:1072                         // 000000002A38: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000002A40: 
0xBF8A0000,             // s_barrier                                                       // 000000002A44: 
0xD8700504, 0x3800002C, // ds_read2st64_b32 v[56:57], v44 offset0:4 offset1:5              // 000000002A48: 
0xD8700706, 0x3A00002F, // ds_read2st64_b32 v[58:59], v47 offset0:6 offset1:7              // 000000002A50: 
0xD8700908, 0x3C00002D, // ds_read2st64_b32 v[60:61], v45 offset0:8 offset1:9              // 000000002A58: 
0xD8700B0A, 0x3E00002E, // ds_read2st64_b32 v[62:63], v46 offset0:10 offset1:11            // 000000002A60: 
0xD9FE0000, 0x0E00002B, // ds_read_b128 v[14:17], v43                                      // 000000002A68: 
0xD3CD8004, 0x04126502, // v_mfma_f32_16x16x16f16 a[4:7], v[2:3], v[50:51], a[4:7]         // 000000002A70: 
0x7E6C0335,             // v_mov_b32_e32 v54, v53                                          // 000000002A78: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000002A7C: 
0xBF8A0000,             // s_barrier                                                       // 000000002A80: 
0xBF8C0F74,             // s_waitcnt vmcnt(4)                                              // 000000002A84: 
0xD81A0000, 0x00002729, // ds_write_b32 v41, v39                                           // 000000002A88: 
0xD3CD8000, 0x04022504, // v_mfma_f32_16x16x16f16 a[0:3], v[4:5], v[18:19], a[0:3]         // 000000002A90: 
0xD3CD8004, 0x04126D04, // v_mfma_f32_16x16x16f16 a[4:7], v[4:5], v[54:55], a[4:7]         // 000000002A98: 
0xBF8C0F72,             // s_waitcnt vmcnt(2)                                              // 000000002AA0: 
0xD2A00012, 0x00023116, // v_pack_b32_f16 v18, v22, v24                                    // 000000002AA4: 
0xD2A01813, 0x00023116, // v_pack_b32_f16 v19, v22, v24 op_sel:[1,1,0]                     // 000000002AAC: 
0xD2A00014, 0x00023317, // v_pack_b32_f16 v20, v23, v25                                    // 000000002AB4: 
0xD2A01815, 0x00023317, // v_pack_b32_f16 v21, v23, v25 op_sel:[1,1,0]                     // 000000002ABC: 
0xBF8CC47F,             // s_waitcnt lgkmcnt(4)                                            // 000000002AC4: 
0x7E2E033A,             // v_mov_b32_e32 v23, v58                                          // 000000002AC8: 
0x7E2C0338,             // v_mov_b32_e32 v22, v56                                          // 000000002ACC: 
0x7E740339,             // v_mov_b32_e32 v58, v57                                          // 000000002AD0: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 000000002AD4: 
0xD3CD8000, 0x04022D0E, // v_mfma_f32_16x16x16f16 a[0:3], v[14:15], v[22:23], a[0:3]       // 000000002AD8: 
0xBF8C0F70,             // s_waitcnt vmcnt(0)                                              // 000000002AE0: 
0xD2A00016, 0x0002351C, // v_pack_b32_f16 v22, v28, v26                                    // 000000002AE4: 
0xD2A01817, 0x0002351C, // v_pack_b32_f16 v23, v28, v26 op_sel:[1,1,0]                     // 000000002AEC: 
0xD2A00018, 0x0002371D, // v_pack_b32_f16 v24, v29, v27                                    // 000000002AF4: 
0xD2A01819, 0x0002371D, // v_pack_b32_f16 v25, v29, v27 op_sel:[1,1,0]                     // 000000002AFC: 
0xD9BE0650, 0x0000162A, // ds_write_b128 v42, v[22:25] offset:1616                         // 000000002B04: 
0xD9BE0430, 0x0000122A, // ds_write_b128 v42, v[18:21] offset:1072                         // 000000002B0C: 
0x7E26033E,             // v_mov_b32_e32 v19, v62                                          // 000000002B14: 
0x7E24033C,             // v_mov_b32_e32 v18, v60                                          // 000000002B18: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000002B1C: 
0xBF8A0000,             // s_barrier                                                       // 000000002B20: 
0xD8700504, 0x1600002C, // ds_read2st64_b32 v[22:23], v44 offset0:4 offset1:5              // 000000002B24: 
0xD8700706, 0x1800002F, // ds_read2st64_b32 v[24:25], v47 offset0:6 offset1:7              // 000000002B2C: 
0x7E7C033D,             // v_mov_b32_e32 v62, v61                                          // 000000002B34: 
0xD8700908, 0x0200002D, // ds_read2st64_b32 v[2:3], v45 offset0:8 offset1:9                // 000000002B38: 
0xD3CD8004, 0x0412750E, // v_mfma_f32_16x16x16f16 a[4:7], v[14:15], v[58:59], a[4:7]       // 000000002B40: 
0xBF8CC27F,             // s_waitcnt lgkmcnt(2)                                            // 000000002B48: 
0x7E340316,             // v_mov_b32_e32 v26, v22                                          // 000000002B4C: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 000000002B50: 
0x7E360318,             // v_mov_b32_e32 v27, v24                                          // 000000002B54: 
0xD8700B0A, 0x0400002E, // ds_read2st64_b32 v[4:5], v46 offset0:10 offset1:11              // 000000002B58: 
0x7E300317,             // v_mov_b32_e32 v24, v23                                          // 000000002B60: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 000000002B64: 
0x7E0C0302,             // v_mov_b32_e32 v6, v2                                            // 000000002B68: 
0x26044CFF, 0x7FFFFFC0, // v_and_b32_e32 v2, 0x7fffffc0, v38                               // 000000002B6C: 
0xD3CD8000, 0x04022510, // v_mfma_f32_16x16x16f16 a[0:3], v[16:17], v[18:19], a[0:3]       // 000000002B74: 
0xD9FE0000, 0x1200002B, // ds_read_b128 v[18:21], v43                                      // 000000002B7C: 
0xBF8CC17F,             // s_waitcnt lgkmcnt(1)                                            // 000000002B84: 
0x7E0E0304,             // v_mov_b32_e32 v7, v4                                            // 000000002B88: 
0x7E080303,             // v_mov_b32_e32 v4, v3                                            // 000000002B8C: 
0x7E060202,             // v_mov_b32_e32 v3, s2                                            // 000000002B90: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000002B94: 
0xBF8A0000,             // s_barrier                                                       // 000000002B98: 
0xBE8200FF, 0x00028000, // s_mov_b32 s2, 0x28000                                           // 000000002B9C: 
0xD3CD8004, 0x04127D10, // v_mfma_f32_16x16x16f16 a[4:7], v[16:17], v[62:63], a[4:7]       // 000000002BA4: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000002BAC: 
0xD3CD8000, 0x04023512, // v_mfma_f32_16x16x16f16 a[0:3], v[18:19], v[26:27], a[0:3]       // 000000002BB0: 
0xD3CD8004, 0x04123112, // v_mfma_f32_16x16x16f16 a[4:7], v[18:19], v[24:25], a[4:7]       // 000000002BB8: 
0xD3CD8000, 0x04020D14, // v_mfma_f32_16x16x16f16 a[0:3], v[20:21], v[6:7], a[0:3]         // 000000002BC0: 
0xD3CD8004, 0x04120914, // v_mfma_f32_16x16x16f16 a[4:7], v[20:21], v[4:5], a[4:7]         // 000000002BC8: 
0x24080085,             // v_lshlrev_b32_e32 v4, 5, v0                                     // 000000002BD0: 
0x260808FF, 0x00000600, // v_and_b32_e32 v4, 0x600, v4                                     // 000000002BD4: 
0x68084D04,             // v_add_u32_e32 v4, v4, v38                                       // 000000002BDC: 
0x6A040504,             // v_sub_u32_e32 v2, v4, v2                                        // 000000002BE0: 
0x0C0802FF, 0xFFFFFFC0, // v_mul_i32_i24_e32 v4, 0xffffffc0, v1                            // 000000002BE4: 
0xD1FE0000, 0x02060104, // v_add_lshl_u32 v0, v4, v0, 1                                    // 000000002BEC: 
0x24080289,             // v_lshlrev_b32_e32 v4, 9, v1                                     // 000000002BF4: 
0xD1C30001, 0x040C0D01, // v_mad_u32_u24 v1, v1, s6, v3                                    // 000000002BF8: 
0xBF800001,             // s_nop 1                                                         // 000000002C00: 
0xD3D84003, 0x18000100, // v_accvgpr_read_b32 v3, a0                                       // 000000002C04: 
0xD3D84005, 0x18000101, // v_accvgpr_read_b32 v5, a1                                       // 000000002C0C: 
0x7E061503,             // v_cvt_f16_f32_e32 v3, v3                                        // 000000002C14: 
0xD3D84006, 0x18000102, // v_accvgpr_read_b32 v6, a2                                       // 000000002C18: 
0x7E0A1505,             // v_cvt_f16_f32_e32 v5, v5                                        // 000000002C20: 
0xD3D84007, 0x18000103, // v_accvgpr_read_b32 v7, a3                                       // 000000002C24: 
0x7E0C1506,             // v_cvt_f16_f32_e32 v6, v6                                        // 000000002C2C: 
0x24040481,             // v_lshlrev_b32_e32 v2, 1, v2                                     // 000000002C30: 
0x7E0E1507,             // v_cvt_f16_f32_e32 v7, v7                                        // 000000002C34: 
0xD83E0000, 0x00000302, // ds_write_b16 v2, v3                                             // 000000002C38: 
0xD83E0100, 0x00000502, // ds_write_b16 v2, v5 offset:256                                  // 000000002C40: 
0xD83E0200, 0x00000602, // ds_write_b16 v2, v6 offset:512                                  // 000000002C48: 
0xD83E0300, 0x00000702, // ds_write_b16 v2, v7 offset:768                                  // 000000002C50: 
0xD3D84003, 0x18000107, // v_accvgpr_read_b32 v3, a7                                       // 000000002C58: 
0xD3D84005, 0x18000106, // v_accvgpr_read_b32 v5, a6                                       // 000000002C60: 
0x7E061503,             // v_cvt_f16_f32_e32 v3, v3                                        // 000000002C68: 
0xD3D84006, 0x18000105, // v_accvgpr_read_b32 v6, a5                                       // 000000002C6C: 
0x7E0A1505,             // v_cvt_f16_f32_e32 v5, v5                                        // 000000002C74: 
0xD3D84007, 0x18000104, // v_accvgpr_read_b32 v7, a4                                       // 000000002C78: 
0x7E0C1506,             // v_cvt_f16_f32_e32 v6, v6                                        // 000000002C80: 
0x7E0E1507,             // v_cvt_f16_f32_e32 v7, v7                                        // 000000002C84: 
0xD83E0380, 0x00000302, // ds_write_b16 v2, v3 offset:896                                  // 000000002C88: 
0xD83E0280, 0x00000502, // ds_write_b16 v2, v5 offset:640                                  // 000000002C90: 
0xD83E0180, 0x00000602, // ds_write_b16 v2, v6 offset:384                                  // 000000002C98: 
0xD83E0080, 0x00000702, // ds_write_b16 v2, v7 offset:128                                  // 000000002CA0: 
0xD1FE0002, 0x02060900, // v_add_lshl_u32 v2, v0, v4, 1                                    // 000000002CA8: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000002CB0: 
0xBF8A0000,             // s_barrier                                                       // 000000002CB4: 
0xD86C0000, 0x03000002, // ds_read_b32 v3, v2                                              // 000000002CB8: 
0xD1FE0000, 0x02060101, // v_add_lshl_u32 v0, v1, v0, 1                                    // 000000002CC0: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000002CC8: 
0xE1381000, 0x80000300, // buffer_atomic_pk_add_f16 v3, v0, s[0:3], 0 offen                // 000000002CCC: 
0xD86C0100, 0x01000002, // ds_read_b32 v1, v2 offset:256                                   // 000000002CD4: 
0x68060004,             // v_add_u32_e32 v3, s4, v0                                        // 000000002CDC: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000002CE0: 
0xE1381800, 0x80000103, // buffer_atomic_pk_add_f16 v1, v3, s[0:3], 0 offen offset:2048    // 000000002CE4: 
0xD86C0200, 0x01000002, // ds_read_b32 v1, v2 offset:512                                   // 000000002CEC: 
0x68060006,             // v_add_u32_e32 v3, s6, v0                                        // 000000002CF4: 
0x68000005,             // v_add_u32_e32 v0, s5, v0                                        // 000000002CF8: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000002CFC: 
0xE1381000, 0x80000103, // buffer_atomic_pk_add_f16 v1, v3, s[0:3], 0 offen                // 000000002D00: 
0xD86C0300, 0x01000002, // ds_read_b32 v1, v2 offset:768                                   // 000000002D08: 
0xBF8CC07F,             // s_waitcnt lgkmcnt(0)                                            // 000000002D10: 
0xE1381800, 0x80000100, // buffer_atomic_pk_add_f16 v1, v0, s[0:3], 0 offen offset:2048    // 000000002D14: 
0xBF810000              // s_endpgm                                                        // 000000002D1C: 
};

#endif  // _ISAGENERATOR_ALDEBARAN_H_
