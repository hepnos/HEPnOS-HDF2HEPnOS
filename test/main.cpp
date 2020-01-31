#include <iostream>
#include <chrono>
#include "rec_vtx_elastic_fuzzyk_png2d.hpp"

void f(unsigned run, unsigned subrun, unsigned event, rec_vtx_elastic_fuzzyk_png2d& obj) {
    static unsigned x = 0;
    std::cout << "Read object " << x << std::endl;
    x += 1;
}

int main(int argc, char** argv) {

    auto start = std::chrono::high_resolution_clock::now();
    rec_vtx_elastic_fuzzyk_png2d::from_hdf5("test.h5", f);
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "time: " << elapsed.count() << std::endl;
    start = std::chrono::high_resolution_clock::now();
    auto events = rec_vtx_elastic_fuzzyk_png2d::from_hdf5("test.h5");
    static unsigned x = 0;
    for(auto& t : std::get<3>(events)) {
        std::cout << "Read object " << x << std::endl;
        x += 1;
    }
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout << "time: " << elapsed.count() << std::endl;

    return 0;
}
