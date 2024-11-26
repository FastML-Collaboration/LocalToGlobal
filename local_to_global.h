#ifndef LOCAL_TO_GLOBAL_H_
#define LOCAL_TO_GLOBAL_H_

#include "ap_fixed.h"
#include "ap_int.h"

using x_t = ap_fixed<16, 4, AP_RND>;
using y_t = ap_fixed<16, 4, AP_RND>;
using z_t = ap_fixed<16, 6, AP_RND>;
using r_t = ap_ufixed<16, 3, AP_RND>;
using sincos_t = ap_fixed<16, 2, AP_RND>;

struct coords_t {
  x_t x;
  y_t y;
  z_t z;
  sincos_t cosphi;
  sincos_t sinphi;
};

struct out_t {
  x_t x;
  y_t y;
  z_t z;
  ap_uint<4> nConst_or_chipid;
  ap_uint<1> bcid;
};

void getCoords(ap_uint<1> bcid, ap_uint<2> layer, ap_uint<5> stave, ap_uint<4> chip,
               ap_ufixed<11, 10> col, ap_ufixed<10,9> row, ap_uint<4> nConstituents, out_t& outval);
#endif
