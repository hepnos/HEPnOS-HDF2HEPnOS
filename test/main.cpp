#include <iostream>
#include "rec_vtx_elastic_fuzzyk_png2d.hpp"

void f(unsigned run, unsigned subrun, unsigned event, rec_vtx_elastic_fuzzyk_png2d& obj) {
    static unsigned x = 0;
    std::cout << "Read object " << x << std::endl;
    x += 1;
}

int main(int argc, char** argv) {

    rec_vtx_elastic_fuzzyk_png2d::from_hdf5("test.h5", f);

    return 0;
}
