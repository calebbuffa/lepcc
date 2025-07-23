#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "../lepcc/src/include/lepcc_c_api.h"

namespace py = pybind11;

class Context {
public:
    Context() : ctx(lepcc_createContext()) {}
    ~Context() { lepcc_deleteContext(&ctx); }
    lepcc_ContextHdl get() const { return ctx; }
private:
    lepcc_ContextHdl ctx;
};

py::bytes decode_xyz(const Context& ctx, py::bytes buffer) {
    std::string buf = buffer;
    const unsigned char* packed = reinterpret_cast<const unsigned char*>(buf.data());
    int bufferSize = static_cast<int>(buf.size());

    unsigned int nPts = 0;
    lepcc_getPointCount(ctx.get(), packed, bufferSize, &nPts);

    std::vector<double> xyz(nPts * 3);
    const unsigned char* packed_ptr = packed;
    lepcc_decodeXYZ(ctx.get(), &packed_ptr, bufferSize, &nPts, xyz.data());

    return py::bytes(reinterpret_cast<const char*>(xyz.data()), xyz.size() * sizeof(double));
}

py::bytes decode_rgb(const Context& ctx, py::bytes buffer) {
    std::string buf = buffer;
    const unsigned char* packed = reinterpret_cast<const unsigned char*>(buf.data());
    int bufferSize = static_cast<int>(buf.size());

    unsigned int nRGB = 0;
    lepcc_getRGBCount(ctx.get(), packed, bufferSize, &nRGB);

    std::vector<unsigned char> rgb(nRGB * 3);
    const unsigned char* packed_ptr = packed;
    lepcc_decodeRGB(ctx.get(), &packed_ptr, bufferSize, &nRGB, rgb.data());

    return py::bytes(reinterpret_cast<const char*>(rgb.data()), rgb.size() * sizeof(unsigned char));
}

PYBIND11_MODULE(lepcc, m) {
    py::class_<Context>(m, "Context")
        .def(py::init<>());

    m.def("decode_xyz", &decode_xyz, "Decode LEPCC XYZ into float64 bytes with context",
          py::arg("ctx"), py::arg("buffer"));
    m.def("decode_rgb", &decode_rgb, "Decode LEPCC RGB into uint8 bytes with context",
          py::arg("ctx"), py::arg("buffer"));
}