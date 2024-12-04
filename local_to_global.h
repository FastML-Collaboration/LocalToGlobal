#ifndef LOCAL_TO_GLOBAL_H_
#define LOCAL_TO_GLOBAL_H_

#include "ap_fixed.h"
#include "ap_int.h"

using z_t = ap_fixed<16, 5, AP_RND>;
using r_t = ap_ufixed<16, 3, AP_RND>;
using phi_t = ap_fixed<16, 3, AP_RND>;
using r_inv_t = ap_ufixed<16, 0, AP_RND>;
using sincos_t = ap_fixed<16, 2, AP_RND>;

struct coords_t {
  z_t z;
  r_t r;
  phi_t phi;
  r_inv_t r_inv;
};

struct out_t {
  r_t r;
  phi_t phi;
  z_t z;
  ap_uint<4> nConst_or_chipid;
  ap_uint<1> bcid;
};

void getCoords(ap_uint<1> bcid, ap_uint<2> layer, ap_uint<5> stave, ap_uint<4> chip,
               ap_ufixed<11, 10> col, ap_ufixed<10,9> row, ap_uint<4> nConstituents, out_t& outval);
#endif
