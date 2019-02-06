// #include "/tools/Xilinx/Vivado/2018.3/include/gmp.h"
// Proposed solution to Vivado HLS gmp bug

// https://github.com/Xilinx/HLx_Examples/tree/master/DSP/fir_example

#include "ap_int.h"
#include "moving_average.h"

void moving_average(hls::stream<din_t> &din,  hls::stream<cfg_t> &cfg, hls::stream<data_t> &dout) {
#pragma HLS interface ap_ctrl_none port=return
#pragma HLS data_pack variable=din
#pragma HLS data_pack variable=cfg
#pragma HLS INTERFACE ap_hs port=cfg
#pragma HLS INTERFACE ap_hs port=din
#pragma HLS INTERFACE ap_hs port=dout
// #pragma HLS resource -core RAM_S2P_LUTRAM "moving_average" shift_reg

  data_t shift_reg[MAX_SIZE];
  ap_uint<16> window;
  data_t coef;
  cfg_t cfg_s;
  din_t din_s;

  data_t tmp;
  data_t accum = 0;

  data_t data;
  ap_uint<1> eot;

  cfg.read(cfg_s);

  window = cfg_s.average_window;
  coef = cfg_s.average_coef;

  do {
    din.read(din_s);
    data = din_s.data;
    eot = din_s.eot;

    tmp = coef * data;
    accum = 0;
  accum_loop: for (int i = window-1; i >= 0; i--) {
      if (i == 0) {
        shift_reg[i] = tmp;
      } else {
        shift_reg[i] = shift_reg[i-1];
      }
      accum += shift_reg[i];
      // *dout = accum;
    }
    dout.write(accum);
  } while (! eot);
}

// set_directive_resource -core RAM_S2P_LUTRAM "moving_average" shift_reg
// set_directive_interface -mode ap_hs "moving_average" din
// set_directive_interface -mode ap_hs "moving_average" cfg
// set_directive_data_pack "moving_average" din
// set_directive_data_pack "moving_average" cfg
// set_directive_interface -mode ap_hs "moving_average" dout
