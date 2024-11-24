#include<iostream>
#include<fstream>
#include<sstream>
#include<tuple>
#include<map>
#include<array>
#include<cmath>

struct coord_t {
    float x;
    float y;
    float z;
    float r;
    float phi;
};

struct simple_coord_t {
    float x;
    float y;
    float z;
};

// row and column are in steps of 0.5
simple_coord_t correctCoords(coord_t centers, float tilt, float col, float row) {
    const float pix_phi = 0.002688;
    const float pix_z = 0.002924;

    simple_coord_t rv;
    rv.z = centers.z + row * pix_z;
    rv.y = centers.y + col * std::cos(centers.phi + tilt);
    rv.x = centers.x - col * std::sin(centers.phi + tilt);

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

            coord_t coord{x, y, z, r, phi};
            positionMap[std::make_tuple(layer, stave, chip)] = coord;
        }
    }

    // the tilts (indexed by layer number)
    std::array<float, 3> phitilt{{0.23255, 0.29539, 0.29731}};

    std::ifstream testVectors("test_vec.txt");
    if (!testVectors.is_open()) {
        throw std::runtime_error("Failed to open test_vectors file");
    }


    for (std::string iline; std::getline(testVectors, iline);) {
        std::stringstream ssline(iline);

        int layer, stave, chip;
        float col, row;
        ssline >> layer >> stave >> chip >> col >> row;

        // std::cout << layer << ", " << stave << ", " << chip << ", "
        //     << x << ", " << y << ", " << z << ", " << r << ", " << phi << std::endl;

        auto center_coord = positionMap[std::make_tuple(layer, stave, chip)];
        auto corCoord = correctCoords(center_coord, phitilt[layer], col, row);
        std::cout << corCoord.x << ", " << corCoord.y << ", " << corCoord.z << std::endl;
    }
}