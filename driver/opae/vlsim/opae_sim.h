#pragma once

#include "Vvortex_afu_shim.h"
#include "Vvortex_afu_shim__Syms.h"
#include "verilated.h"

#ifdef VCD_OUTPUT
#include <verilated_vcd_c.h>
#endif

#include <VX_config.h>
#include "ram.h"

#include <ostream>
#include <future>
#include <vector>

class opae_sim {
public:
  
  opae_sim();
  virtual ~opae_sim();

  void prepare_buffer(uint64_t len, void **buf_addr, uint64_t *wsid, int flags);

  void release_buffer(uint64_t wsid);

  void get_io_address(uint64_t wsid, uint64_t *ioaddr);

  void write_mmio64(uint32_t mmio_num, uint64_t offset, uint64_t value);

  void read_mmio64(uint32_t mmio_num, uint64_t offset, uint64_t *value);

private: 

  typedef struct {
    int cycles_left;  
    std::array<uint8_t, GLOBAL_BLOCK_SIZE> block;
    unsigned tag;
  } dram_rd_req_t;

  typedef struct {
    int cycles_left;  
    std::array<uint8_t, GLOBAL_BLOCK_SIZE> block;
    unsigned mdata;
  } cci_rd_req_t;

  typedef struct {
    int cycles_left;  
    std::array<uint8_t, GLOBAL_BLOCK_SIZE> block;
    unsigned mdata;
  } cci_wr_req_t;

  typedef struct {    
    uint8_t* data;
    size_t size;  
  } host_alloc_t;

  void reset();

  void eval();

  void step();

  void sRxPort_bus();
  void sTxPort_bus();
  void avs_bus();

  uint8_t* find_host_ptr(uint64_t addr);

  std::future<void> future_;
  bool stop_;

  std::vector<host_alloc_t> host_allocs_;

  std::vector<dram_rd_req_t> dram_reads_;

  std::vector<cci_rd_req_t> cci_reads_;

  std::vector<cci_wr_req_t> cci_writes_;

  RAM ram_;
  Vvortex_afu_shim *vortex_afu_;
#ifdef VCD_OUTPUT
  VerilatedVcdC *trace_;
#endif
};