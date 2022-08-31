////////////////////////////////////////////////////////////////////////////////
//
// The University of Illinois/NCSA
// Open Source License (NCSA)
//
// Copyright (c) 2014-2020, Advanced Micro Devices, Inc. All rights reserved.
//
// Developed by:
//
//                 AMD Research and AMD HSA Software Development
//
//                 Advanced Micro Devices, Inc.
//
//                 www.amd.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal with the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
//  - Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimers.
//  - Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimers in
//    the documentation and/or other materials provided with the distribution.
//  - Neither the names of Advanced Micro Devices, Inc,
//    nor the names of its contributors may be used to endorse or promote
//    products derived from this Software without specific prior written
//    permission.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS WITH THE SOFTWARE.
//
////////////////////////////////////////////////////////////////////////////////

#include "core/inc/hsa_ven_amd_loader_impl.h"

#include "core/inc/amd_hsa_loader.hpp"
#include "core/inc/runtime.h"

#include "inc/hsa_ven_amd_aqlprofile.h"

#include "core/inc/amd_gpu_pm4.h"
#include "core/inc/amd_gpu_agent.h"
#include "core/inc/asic_reg/gfx_7_2_d.h"
#include "core/inc/asic_reg/gfx_7_2_sh_mask.h"
#include "core/runtime/pm4/PM4Packet.hpp"
#include "core/runtime/isa/isablobs.hpp"

#define PAGE_SIZE (0x1000)
#define PAGE_ALIGN (0x1000)

hsa_status_t HSA_API hsa_ven_amd_experiment_allocate_pm4_buffers(
  uint32_t m, uint32_t n, uint32_t k,
  void** pm4_a_buf,
  void** pm4_b_buf,
  void** pm4_c_buf,
  void** pm4_isa_buf,
  void** pm4_ib_buf) {
  //printf("hsa_ven_amd_experiment_allocate_pm4_buffers\n");

  using namespace rocr;
  using namespace rocr::core;
  if (!Runtime::runtime_singleton_->IsOpen()) {
    return HSA_STATUS_ERROR_NOT_INITIALIZED;
  }

  AMD::GpuAgent* gpu_agent = static_cast<AMD::GpuAgent*>(Runtime::runtime_singleton_->gpu_agents()[0]);

  uint32_t sizeofa = m * k * 2;
  uint32_t sizeofb = k * n * 2;
  uint32_t sizeofc = m * n * 2;

  *pm4_a_buf = gpu_agent->system_allocator()(sizeofa, PAGE_ALIGN, core::MemoryRegion::AllocateNoFlags);
  *pm4_b_buf = gpu_agent->system_allocator()(sizeofb, PAGE_ALIGN, core::MemoryRegion::AllocateNoFlags);
  *pm4_c_buf = gpu_agent->system_allocator()(sizeofc, PAGE_ALIGN, core::MemoryRegion::AllocateNoFlags);
  *pm4_isa_buf = gpu_agent->system_allocator()(PAGE_SIZE, PAGE_ALIGN, core::MemoryRegion::AllocateExecutable);
  *pm4_ib_buf = gpu_agent->system_allocator()(PAGE_SIZE, PAGE_ALIGN, core::MemoryRegion::AllocateExecutable);
  return HSA_STATUS_SUCCESS;
}

hsa_status_t HSA_API hsa_ven_amd_experiment_free_pm4_buffers(
  void* pm4_a_buf,
  void* pm4_b_buf,
  void* pm4_c_buf,
  void* pm4_isa_buf,
  void* pm4_ib_buf) {
  //printf("hsa_ven_amd_experiment_free_pm4_buffers\n");

  using namespace rocr;
  using namespace rocr::core;
  if (!Runtime::runtime_singleton_->IsOpen()) {
    return HSA_STATUS_ERROR_NOT_INITIALIZED;
  }

  AMD::GpuAgent* gpu_agent = static_cast<AMD::GpuAgent*>(Runtime::runtime_singleton_->gpu_agents()[0]);

  gpu_agent->system_deallocator()(pm4_a_buf);
  gpu_agent->system_deallocator()(pm4_b_buf);
  gpu_agent->system_deallocator()(pm4_c_buf);
  gpu_agent->system_deallocator()(pm4_isa_buf);
  gpu_agent->system_deallocator()(pm4_ib_buf);

  return HSA_STATUS_SUCCESS;
}

hsa_status_t HSA_API hsa_ven_amd_experiment_get_pm4(
  uint32_t m, uint32_t n, uint32_t k,
  hsa_ext_amd_aql_pm4_packet_t* aql_packet,
  void* pm4_a_buf,
  void* pm4_b_buf,
  void* pm4_c_buf,
  void* pm4_isa_buf,
  void* pm4_ib_buf) {
  //printf("hsa_ven_amd_experiment_get_pm4\n");

  using namespace rocr;
  using namespace rocr::core;
  if (!Runtime::runtime_singleton_->IsOpen()) {
    return HSA_STATUS_ERROR_NOT_INITIALIZED;
  }

  uint32_t sizeofa = m * k * 2;
  uint32_t sizeofb = k * n * 2;
  uint32_t sizeofc = m * n * 2;

  memset(pm4_a_buf, 0, sizeofa);
  memset(pm4_b_buf, 0, sizeofb);
  memset(pm4_c_buf, 0, sizeofc);

  for (uint32_t i = 0; i < sizeofa / sizeof(uint32_t); ++i) {
    reinterpret_cast<uint32_t*>(pm4_a_buf)[i] = 0x40004000; // 2.0 (half) / 2.0 (half)
  }
  for (uint32_t i = 0; i < sizeofb / sizeof(uint32_t); ++i) {
    reinterpret_cast<uint32_t*>(pm4_b_buf)[i] = 0x3C003C00; // 1.0 (half) / 1.0 (half)
  }

  uint32_t grid_size_x = 1;
  uint32_t block_size_x = 1;
  if ((m == 16) && (n == 1152) && (k == 5120)) {
    memcpy(pm4_isa_buf, GEMM_ISA_16_1152_5120, sizeof(GEMM_ISA_16_1152_5120));
    grid_size_x = 9 * 256;
    block_size_x = 256;
  } else if ((m == 16) && (n == 5120) && (k == 384)) {
    memcpy(pm4_isa_buf, GEMM_ISA_16_5120_384, sizeof(GEMM_ISA_16_5120_384));
    grid_size_x = 40 * 256;
    block_size_x = 256;
  } else if ((m == 16) && (n == 1280) && (k == 5120)) {
    memcpy(pm4_isa_buf, GEMM_ISA_16_1280_5120, sizeof(GEMM_ISA_16_1280_5120));
    grid_size_x = 10 * 256;
    block_size_x = 256;
  } else if ((m == 16) && (n == 5120) && (k == 1280)) {
    memcpy(pm4_isa_buf, GEMM_ISA_16_5120_1280, sizeof(GEMM_ISA_16_5120_1280));
    grid_size_x = 40 * 256;
    block_size_x = 256;
  } else {
    memcpy(pm4_isa_buf, CUSTOM_SGPR_ISA, sizeof(CUSTOM_SGPR_ISA));
  }
 
  // Parameters need to be set:
  // - ISA address.
  // - Kernel arguments: A/B/C
  // - Block size: X/Y/Z
  uint64_t shiftedIsaAddr = reinterpret_cast<uint64_t>(pm4_isa_buf) >> 8;
  uint32_t arg0 = reinterpret_cast<uint64_t>(pm4_c_buf) & 0xFFFFFFFF;
  uint32_t arg1 = reinterpret_cast<uint64_t>(pm4_c_buf) >> 32;
  uint32_t arg2 = reinterpret_cast<uint64_t>(pm4_a_buf) & 0xFFFFFFFF;
  uint32_t arg3 = reinterpret_cast<uint64_t>(pm4_a_buf) >> 32;
  uint32_t arg4 = reinterpret_cast<uint64_t>(pm4_b_buf) & 0xFFFFFFFF;
  uint32_t arg5 = reinterpret_cast<uint64_t>(pm4_b_buf) >> 32;
  uint32_t m_DimX = grid_size_x;
  uint32_t m_DimY = 1;
  uint32_t m_DimZ = 1;
  uint32_t m_BlockX = block_size_x;
  uint32_t m_BlockY = 1;
  uint32_t m_BlockZ = 1;

  // Starts at COMPUTE_START_X
  unsigned int COMPUTE_DISPATCH_DIMS_VALUES[] = {
      0,      // START_X
      0,      // START_Y
      0,      // START_Z
      1,      // NUM_THREADS_X - this is actually the number of threads in a thread group
      1,      // NUM_THREADS_Y
      1,      // NUM_THREADS_Z
      0,      // COMPUTE_PIPELINESTAT_ENABLE
      0,      // COMPUTE_PERFCOUNT_ENABLE
  };
  COMPUTE_DISPATCH_DIMS_VALUES[3] = m_BlockX;
  COMPUTE_DISPATCH_DIMS_VALUES[4] = m_BlockY;
  COMPUTE_DISPATCH_DIMS_VALUES[5] = m_BlockZ;

  unsigned int pgmRsrc2 = 0;
  pgmRsrc2 |= (0 << COMPUTE_PGM_RSRC2__SCRATCH_EN__SHIFT)
          & COMPUTE_PGM_RSRC2__SCRATCH_EN_MASK;
  pgmRsrc2 |= (6 << COMPUTE_PGM_RSRC2__USER_SGPR__SHIFT)
          & COMPUTE_PGM_RSRC2__USER_SGPR_MASK;
  pgmRsrc2 |= (0 << COMPUTE_PGM_RSRC2__TRAP_PRESENT__SHIFT)
          & COMPUTE_PGM_RSRC2__TRAP_PRESENT_MASK;
  pgmRsrc2 |= (1 << COMPUTE_PGM_RSRC2__TGID_X_EN__SHIFT)
          & COMPUTE_PGM_RSRC2__TGID_X_EN_MASK;
  pgmRsrc2 |= (1 << COMPUTE_PGM_RSRC2__TGID_Y_EN__SHIFT)
          & COMPUTE_PGM_RSRC2__TGID_Y_EN_MASK;
  pgmRsrc2 |= (1 << COMPUTE_PGM_RSRC2__TGID_Z_EN__SHIFT)
          & COMPUTE_PGM_RSRC2__TGID_Z_EN_MASK;
  pgmRsrc2 |= (0 << COMPUTE_PGM_RSRC2__TG_SIZE_EN__SHIFT)
          & COMPUTE_PGM_RSRC2__TG_SIZE_EN_MASK;
  pgmRsrc2 |= (2 << COMPUTE_PGM_RSRC2__TIDIG_COMP_CNT__SHIFT)
          & COMPUTE_PGM_RSRC2__TIDIG_COMP_CNT_MASK;
  pgmRsrc2 |= (0 << COMPUTE_PGM_RSRC2__EXCP_EN__SHIFT)
          & COMPUTE_PGM_RSRC2__EXCP_EN_MASK;
  pgmRsrc2 |= (0x13 << COMPUTE_PGM_RSRC2__LDS_SIZE__SHIFT)
          & COMPUTE_PGM_RSRC2__LDS_SIZE_MASK;
  pgmRsrc2 |= (0 << COMPUTE_PGM_RSRC2__EXCP_EN_MSB__SHIFT)
          & COMPUTE_PGM_RSRC2__EXCP_EN_MSB_MASK;

  const unsigned int COMPUTE_PGM_RSRC[] = {
      // PGM_RSRC1 = { VGPRS: 52 (44+8) SGPRS: 32 PRIORITY: 0 FLOAT_MODE: 240 PRIV: 0
      // DX10_CLAMP: 0 DEBUG_MODE: 0 IEEE_MODE: 1 BULKY: 0 CDBG_USER: 0 }
      0x004f808d,
      pgmRsrc2
  };

  // Starts at COMPUTE_PGM_LO
  const unsigned int COMPUTE_PGM_VALUES_GFX9[] = {
      static_cast<uint32_t>(shiftedIsaAddr),                  // PGM_LO
      static_cast<uint32_t>(shiftedIsaAddr >> 32),            // PGM_HI
      0,
      0,
      0,
      0
  };

  // Starts at COMPUTE_USER_DATA_0
  const unsigned int COMPUTE_USER_DATA_VALUES[] = {
                  // Reg name
      arg0,       // COMPUTE_USER_DATA_0
      arg1,       // COMPUTE_USER_DATA_1
      arg2,       // COMPUTE_USER_DATA_2
      arg3,       // COMPUTE_USER_DATA_3
      arg4,       // COMPUTE_USER_DATA_4
      arg5,       // COMPUTE_USER_DATA_5
      0,          // COMPUTE_USER_DATA_6
      0,          // COMPUTE_USER_DATA_7
      0,          // COMPUTE_USER_DATA_8
      0,          // COMPUTE_USER_DATA_9
      0,          // COMPUTE_USER_DATA_10
      0,          // COMPUTE_USER_DATA_11
      0,          // COMPUTE_USER_DATA_12
      0,          // COMPUTE_USER_DATA_13
      0,          // COMPUTE_USER_DATA_14
      0xCAFEBABE, // COMPUTE_USER_DATA_15
  };

  const unsigned int DISPATCH_INIT_VALUE = 0x00000021;
  // {COMPUTE_SHADER_EN=1, PARTIAL_TG_EN=0, FORCE_START_AT_000=0, ORDERED_APPEND_ENBL=0,
  // ORDERED_APPEND_MODE=0, USE_THREAD_DIMENSIONS=1, ORDER_MODE=0, DISPATCH_CACHE_CNTL=0,
  // SCALAR_L1_INV_VOL=0, VECTOR_L1_INV_VOL=0, DATA_ATC=?, RESTORE=0}

  uint32_t cmd_size_b = 0;

  PM4AcquireMemoryPacket p0(FAMILY_AL);
  memcpy(pm4_ib_buf, p0.GetPacket(), p0.SizeInBytes());
  cmd_size_b += p0.SizeInBytes();

  PM4SetShaderRegPacket p1(mmCOMPUTE_START_X, COMPUTE_DISPATCH_DIMS_VALUES, ARRAY_SIZE(COMPUTE_DISPATCH_DIMS_VALUES));
  memcpy(pm4_ib_buf + cmd_size_b, p1.GetPacket(), p1.SizeInBytes());
  cmd_size_b += p1.SizeInBytes();

  PM4SetShaderRegPacket p2(mmCOMPUTE_PGM_LO, COMPUTE_PGM_VALUES_GFX9, ARRAY_SIZE(COMPUTE_PGM_VALUES_GFX9));
  memcpy(pm4_ib_buf + cmd_size_b, p2.GetPacket(), p2.SizeInBytes());
  cmd_size_b += p2.SizeInBytes();

  PM4SetShaderRegPacket p3(mmCOMPUTE_PGM_RSRC1, COMPUTE_PGM_RSRC, ARRAY_SIZE(COMPUTE_PGM_RSRC));
  memcpy(pm4_ib_buf + cmd_size_b, p3.GetPacket(), p3.SizeInBytes());
  cmd_size_b += p3.SizeInBytes();

  #define COMPUTE_PGM_RSRC3__ACCUM_OFFSET__SHIFT 0x0
  #define COMPUTE_PGM_RSRC3__TG_SPLIT__SHIFT     0x10
  #define COMPUTE_PGM_RSRC3__ACCUM_OFFSET_MASK   0x0000003FL
  #define COMPUTE_PGM_RSRC3__TG_SPLIT__MASK      0x00010000L
  //; COMPUTE_PGM_RSRC3_GFX90A:ACCUM_OFFSET: 10
  //; COMPUTE_PGM_RSRC3_GFX90A:TG_SPLIT: 0
  const unsigned int COMPUTE_PGM_RSRC3[] = {10};
  PM4SetShaderRegPacket p4(mmCOMPUTE_PGM_RSRC3, COMPUTE_PGM_RSRC3, ARRAY_SIZE(COMPUTE_PGM_RSRC3));
  memcpy(pm4_ib_buf + cmd_size_b, p4.GetPacket(), p4.SizeInBytes());
  cmd_size_b += p4.SizeInBytes();

  PM4SetShaderRegPacket p5(mmCOMPUTE_USER_DATA_0, COMPUTE_USER_DATA_VALUES, ARRAY_SIZE(COMPUTE_USER_DATA_VALUES));
  memcpy(pm4_ib_buf + cmd_size_b, p5.GetPacket(), p5.SizeInBytes());
  cmd_size_b += p5.SizeInBytes();

  PM4DispatchDirectPacket p6(m_DimX, m_DimY, m_DimZ, DISPATCH_INIT_VALUE);
  memcpy(pm4_ib_buf + cmd_size_b, p6.GetPacket(), p6.SizeInBytes());
  cmd_size_b += p6.SizeInBytes();

  PM4PartialFlushPacket p7;
  memcpy(pm4_ib_buf + cmd_size_b, p7.GetPacket(), p7.SizeInBytes());
  cmd_size_b += p7.SizeInBytes();

  // // Construct four NOP PM4 command.
  // constexpr uint32_t pm4_nop_size_dw = 1;
  // uint32_t pm4_nop_cmd[pm4_nop_size_dw] = { PM4_HDR(PM4_HDR_IT_OPCODE_NOP, pm4_nop_size_dw, /*gfxip_ver=*/9) };

  // memcpy(pm4_ib_buf, pm4_nop_cmd, pm4_nop_size_dw * sizeof(uint32_t));
  // memcpy(pm4_ib_buf + pm4_nop_size_dw * sizeof(uint32_t), pm4_nop_cmd, pm4_nop_size_dw * sizeof(uint32_t));
  // memcpy(pm4_ib_buf + pm4_nop_size_dw * sizeof(uint32_t) * 2, pm4_nop_cmd, pm4_nop_size_dw * sizeof(uint32_t));
  // memcpy(pm4_ib_buf + pm4_nop_size_dw * sizeof(uint32_t) * 3, pm4_nop_cmd, pm4_nop_size_dw * sizeof(uint32_t));
  // uint32_t cmd_size_b = pm4_nop_size_dw * sizeof(uint32_t) * 4;
 
  // Construct a PM4 command to execute the IB.
  constexpr uint32_t ib_jump_size_dw = 4;

  uint32_t ib_jump_cmd[ib_jump_size_dw] = {
      PM4_HDR(PM4_HDR_IT_OPCODE_INDIRECT_BUFFER, ib_jump_size_dw, /*gfxip_ver=*/9),
      PM4_INDIRECT_BUFFER_DW1_IB_BASE_LO(uint32_t(uintptr_t(pm4_ib_buf) >> 2)),
      PM4_INDIRECT_BUFFER_DW2_IB_BASE_HI(uint32_t(uintptr_t(pm4_ib_buf) >> 32)),
      (PM4_INDIRECT_BUFFER_DW3_IB_SIZE(uint32_t(cmd_size_b / sizeof(uint32_t))) |
       PM4_INDIRECT_BUFFER_DW3_IB_VALID(1))};

  // Construct an AQL packet to jump to the PM4 IB.
  struct amd_aql_pm4_ib {
    uint16_t header;
    uint16_t ven_hdr;
    uint32_t ib_jump_cmd[4];
    uint32_t dw_cnt_remain;
    uint32_t reserved[8];
    hsa_signal_t completion_signal;
  };

  constexpr uint32_t AMD_AQL_FORMAT_PM4_IB = 0x1;

  amd_aql_pm4_ib aql_pm4_ib{};
  aql_pm4_ib.header = HSA_PACKET_TYPE_VENDOR_SPECIFIC << HSA_PACKET_HEADER_TYPE;
  aql_pm4_ib.ven_hdr = AMD_AQL_FORMAT_PM4_IB;
  aql_pm4_ib.ib_jump_cmd[0] = ib_jump_cmd[0];
  aql_pm4_ib.ib_jump_cmd[1] = ib_jump_cmd[1];
  aql_pm4_ib.ib_jump_cmd[2] = ib_jump_cmd[2];
  aql_pm4_ib.ib_jump_cmd[3] = ib_jump_cmd[3];
  aql_pm4_ib.dw_cnt_remain = 0xA;

  memcpy(aql_packet, &aql_pm4_ib, sizeof(amd_aql_pm4_ib));

  return HSA_STATUS_SUCCESS;
}

namespace rocr {

using namespace amd::hsa;
using namespace core;

using loader::CodeObjectReaderImpl;
using loader::Executable;
using loader::LoadedCodeObject;
using loader::Loader;

namespace AMD {

hsa_status_t handleException();

}   // namespace amd

hsa_status_t hsa_ven_amd_loader_query_host_address(
  const void *device_address,
  const void **host_address) {
  try {
    if (!Runtime::runtime_singleton_->IsOpen()) {
      return HSA_STATUS_ERROR_NOT_INITIALIZED;
    }
    if (nullptr == device_address) {
      return HSA_STATUS_ERROR_INVALID_ARGUMENT;
    }
    if (nullptr == host_address) {
      return HSA_STATUS_ERROR_INVALID_ARGUMENT;
    }

    uintptr_t udaddr = reinterpret_cast<uintptr_t>(device_address);
    uintptr_t uhaddr = Runtime::runtime_singleton_->loader()->FindHostAddress(udaddr);
    if (0 == uhaddr) {
      return HSA_STATUS_ERROR_INVALID_ARGUMENT;
    }

    *host_address = reinterpret_cast<void*>(uhaddr);
    return HSA_STATUS_SUCCESS;
  } catch(...) { return AMD::handleException(); }
}

hsa_status_t hsa_ven_amd_loader_query_segment_descriptors(
  hsa_ven_amd_loader_segment_descriptor_t *segment_descriptors,
  size_t *num_segment_descriptors) {
  try {
    if (!Runtime::runtime_singleton_->IsOpen()) {
      return HSA_STATUS_ERROR_NOT_INITIALIZED;
    }

    // Arguments are checked by the loader.
    return Runtime::runtime_singleton_->loader()->QuerySegmentDescriptors(segment_descriptors, num_segment_descriptors);
  } catch(...) { return AMD::handleException(); }
}

hsa_status_t hsa_ven_amd_loader_query_executable(
  const void *device_address,
  hsa_executable_t *executable) {
  try {
    if (!Runtime::runtime_singleton_->IsOpen()) {
      return HSA_STATUS_ERROR_NOT_INITIALIZED;
    }
    if ((nullptr == device_address) || (nullptr == executable)) {
      return HSA_STATUS_ERROR_INVALID_ARGUMENT;
    }

    uintptr_t udaddr = reinterpret_cast<uintptr_t>(device_address);
    hsa_executable_t exec = Runtime::runtime_singleton_->loader()->FindExecutable(udaddr);
    if (0 == exec.handle) {
      return HSA_STATUS_ERROR_INVALID_ARGUMENT;
    }

    *executable = exec;
    return HSA_STATUS_SUCCESS;
  } catch(...) { return AMD::handleException(); }
}

hsa_status_t hsa_ven_amd_loader_executable_iterate_loaded_code_objects(
  hsa_executable_t executable,
  hsa_status_t (*callback)(
    hsa_executable_t executable,
    hsa_loaded_code_object_t loaded_code_object,
    void *data),
  void *data) {
  try {
    if (!Runtime::runtime_singleton_->IsOpen()) {
      return HSA_STATUS_ERROR_NOT_INITIALIZED;
    }
    if (nullptr == callback) {
      return HSA_STATUS_ERROR_INVALID_ARGUMENT;
    }

    Executable *exec = Executable::Object(executable);
    if (!exec) {
      return HSA_STATUS_ERROR_INVALID_EXECUTABLE;
    }

    return exec->IterateLoadedCodeObjects(callback, data);
  } catch(...) { return AMD::handleException(); }
}

hsa_status_t hsa_ven_amd_loader_loaded_code_object_get_info(
  hsa_loaded_code_object_t loaded_code_object,
  hsa_ven_amd_loader_loaded_code_object_info_t attribute,
  void *value) {
  try {
    if (!Runtime::runtime_singleton_->IsOpen()) {
      return HSA_STATUS_ERROR_NOT_INITIALIZED;
    }
    if (nullptr == value) {
      return HSA_STATUS_ERROR_INVALID_ARGUMENT;
    }

    const LoadedCodeObject *lcobj = LoadedCodeObject::Object(loaded_code_object);
    if (!lcobj) {
      return HSA_STATUS_ERROR_INVALID_CODE_OBJECT;
    }

    switch (attribute) {
      case HSA_VEN_AMD_LOADER_LOADED_CODE_OBJECT_INFO_EXECUTABLE: {
        *((hsa_executable_t*)value) = lcobj->getExecutable();
        break;
      }
      case HSA_VEN_AMD_LOADER_LOADED_CODE_OBJECT_INFO_KIND: {
        *((uint32_t*)value) = lcobj->getAgent().handle == 0
            ? HSA_VEN_AMD_LOADER_LOADED_CODE_OBJECT_KIND_PROGRAM
            : HSA_VEN_AMD_LOADER_LOADED_CODE_OBJECT_KIND_AGENT;
        break;
      }
      case HSA_VEN_AMD_LOADER_LOADED_CODE_OBJECT_INFO_AGENT: {
        hsa_agent_t agent = lcobj->getAgent();
        if (agent.handle == 0) {
            return HSA_STATUS_ERROR_INVALID_ARGUMENT;
        }
        *((hsa_agent_t*)value) = agent;
        break;
      }
      case HSA_VEN_AMD_LOADER_LOADED_CODE_OBJECT_INFO_CODE_OBJECT_STORAGE_TYPE: {
        // TODO Update loader so it keeps track if code object was loaded from a
        // file or memory.
        *((uint32_t*)value) = HSA_VEN_AMD_LOADER_CODE_OBJECT_STORAGE_TYPE_MEMORY;
        break;
      }
      case HSA_VEN_AMD_LOADER_LOADED_CODE_OBJECT_INFO_CODE_OBJECT_STORAGE_MEMORY_BASE: {
        *((uint64_t*)value) = lcobj->getElfData();
        break;
      }
      case HSA_VEN_AMD_LOADER_LOADED_CODE_OBJECT_INFO_CODE_OBJECT_STORAGE_MEMORY_SIZE: {
        *((uint64_t*)value) = lcobj->getElfSize();
        break;
      }
      case HSA_VEN_AMD_LOADER_LOADED_CODE_OBJECT_INFO_CODE_OBJECT_STORAGE_FILE: {
        // TODO Update loader so it keeps track if code object was loaded from a
        // file or memory.
        return HSA_STATUS_ERROR_INVALID_ARGUMENT;
        break;
      }
      case HSA_VEN_AMD_LOADER_LOADED_CODE_OBJECT_INFO_LOAD_DELTA: {
        // TODO Check if executable is frozen.
        // This suggests this code should be moved into LoadedCodeObjectImpl::getinfo
        // as is done for other *_get_info methods. Currently LoadedCodeObject has a
        // GetInfo method which is likely not used.
        // Also should this have a *NOT_FROZEN ststus code added?
        // if (state_ != HSA_EXECUTABLE_STATE_FROZEN) {
        //   return HSA_STATUS_ERROR_INVALID_ARGUMENT;
        // }
        *((int64_t*)value) = lcobj->getDelta();
        break;
      }
      case HSA_VEN_AMD_LOADER_LOADED_CODE_OBJECT_INFO_LOAD_BASE: {
        // TODO Check if executable is frozen.
        *((uint64_t*)value) = lcobj->getLoadBase();
        break;
      }
      case HSA_VEN_AMD_LOADER_LOADED_CODE_OBJECT_INFO_LOAD_SIZE: {
        // TODO Check if executable is frozen.
        *((uint64_t*)value) = lcobj->getLoadSize();
        break;
      }
      case HSA_VEN_AMD_LOADER_LOADED_CODE_OBJECT_INFO_URI_LENGTH: {
        *(reinterpret_cast<uint32_t*>(value)) = lcobj->getUri().size();
        break;
      }
      case HSA_VEN_AMD_LOADER_LOADED_CODE_OBJECT_INFO_URI: {
        memcpy(value, lcobj->getUri().c_str(), lcobj->getUri().size());
        break;
      }
      default: {
        return HSA_STATUS_ERROR_INVALID_ARGUMENT;
      }
    }

    return HSA_STATUS_SUCCESS;
  } catch(...) { return AMD::handleException(); }
}

hsa_status_t
hsa_ven_amd_loader_code_object_reader_create_from_file_with_offset_size(
    hsa_file_t file,
    size_t offset,
    size_t size,
    hsa_code_object_reader_t *code_object_reader) {
  try {
    if (!Runtime::runtime_singleton_->IsOpen()) {
      return HSA_STATUS_ERROR_NOT_INITIALIZED;
    }
    if (nullptr == code_object_reader) {
      return HSA_STATUS_ERROR_INVALID_ARGUMENT;
    }

    if (size == 0) {
      return HSA_STATUS_ERROR_INVALID_CODE_OBJECT;
    }

    std::unique_ptr<CodeObjectReaderImpl> reader(
        new (std::nothrow) CodeObjectReaderImpl());
    if (!reader) {
      return HSA_STATUS_ERROR_OUT_OF_RESOURCES;
    }

    hsa_status_t status = reader->SetFile(file, offset, size);
    if (status != HSA_STATUS_SUCCESS) {
      return status;
    }

    *code_object_reader = CodeObjectReaderImpl::Handle(reader.release());
    return HSA_STATUS_SUCCESS;
  } catch(...) { return AMD::handleException(); }
}

namespace {

Loader *GetLoader() {
  return Runtime::runtime_singleton_->loader();
}

} // namespace anonymous

hsa_status_t
hsa_ven_amd_loader_iterate_executables(
    hsa_status_t (*callback)(
      hsa_executable_t executable,
      void *data),
    void *data) {
  try {
    if (!Runtime::runtime_singleton_->IsOpen()) {
      return HSA_STATUS_ERROR_NOT_INITIALIZED;
    }
    if (nullptr == callback) {
      return HSA_STATUS_ERROR_INVALID_ARGUMENT;
    }

    return GetLoader()->IterateExecutables(callback, data);
  } catch(...) { return AMD::handleException(); }
}

} // namespace rocr
