#include<iostream>
#include<fstream>
#include<sstream>
#include<tuple>
#include<map>
#include<array>
#include<cmath>
#include "local_to_global.h"

struct coord_t {
    float z;
    float r;
    float phi;
};

using simple_coord_t = coord_t;


// row and column are in steps of 0.5
simple_coord_t correctCoords(coord_t centers, float tilt, float col, float row) {
    const float pix_phi = 0.002688;
    const float pix_z = 0.002924;

    simple_coord_t rv;
    rv.z = centers.z + (col - 511) * pix_z;

    auto x = centers.r * std::cos(tilt);
    auto y = centers.r * std::sin(tilt) -  (row - 255) * pix_phi;

    rv.r = std::hypot(x, y);
    rv.phi = centers.phi + tilt - std::atan2(y, x);

    return rv;
}

int main() {
    // first read in the positions file:

    std::ifstream positionsFile("mvtxPositions.txt");
    if (!positionsFile.is_open()) {
        throw std::runtime_error("Failed to open positions file");
    }

    std::map<std::tuple<int, int, int>, coord_t> positionMap;

    for (std::string iline; std::getline(positionsFile, iline);) {
        if (iline.find("Layer") == std::string::npos) {
            // skipped the header
            std::stringstream ssline(iline);

            int layer, stave, chip;
            float x, y, z, r, phi;
            ssline >> layer >> stave >> chip >> x >> y >> z >> r >> phi;

            // std::cout << layer << ", " << stave << ", " << chip << ", "
            //     << x << ", " << y << ", " << z << ", " << r << ", " << phi << std::endl;

            coord_t coord{z, r, phi};
            positionMap[std::make_tuple(layer, stave, chip)] = coord;
        }
    }

    // the tilts (indexed by layer number)
    std::array<float, 3> phitilt{{0.23255, 0.29539, 0.29731}};

    std::ifstream testVectors("test_vec.txt");
    if (!testVectors.is_open()) {
        throw std::runtime_error("Failed to open test_vectors file");
    }


    int exit_code = 0;

    for (std::string iline; std::getline(testVectors, iline);) {
        std::stringstream ssline(iline);

        int layer, stave, chip;
        float col, row;
        ssline >> layer >> stave >> chip >> col >> row;

        auto center_coord = positionMap[std::make_tuple(layer, stave, chip)];

        std::cout << layer << ", " << stave << ", " << chip << ", row = " << row << ", col = " << col
            << "\nCenters: " << center_coord.r << ", " << center_coord.phi << ", " << center_coord.z  << std::endl;

        auto corCoord = correctCoords(center_coord, phitilt[layer], col, row);
        out_t outval;  // these are the
        getCoords(0, layer, stave, chip, col, row, 3, outval);
        std::cout << "Emulated: " << corCoord.r << ", " << corCoord.phi << ", " << corCoord.z
            << "\nHLS:      " << outval.r << ", " << outval.phi << ", " << outval.z  << std::endl << std::endl;
        if (std::abs(corCoord.r - outval.r.to_float()) > 0.003 ||
            std::abs(corCoord.phi - outval.phi.to_float()) > 0.003 ||
            std::abs(corCoord.z - outval.z.to_float()) > 0.03) {

            std::cerr << "MISMATCH FOUND" << std::endl;
            exit_code = 1;
        }
    }
    return exit_code;
}
