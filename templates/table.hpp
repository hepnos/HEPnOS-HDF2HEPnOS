/*
 * (C) 2019 The University of Chicago
 * 
 * See COPYRIGHT in top-level directory.
 */
#ifndef __{{ table.name|upper|as_cpp_identifier }}_HPP_
#define __{{ table.name|upper|as_cpp_identifier }}_HPP_

#include <string>
#include <vector>
#include <cstdint>
#include <H5Cpp.h>

{% if namespace|length -%}
namespace {{ namespace }} {
{%- endif %}

/**
 * Auto-generated {{ table.name|as_cpp_identifier }} structure for the 
 * "{{ table.name }}" table in corresponding HDF5 files.
 */
struct {{ table.name|as_cpp_identifier }} {

    {% for member in table.members -%}
    {{ member.cpptype }} {{ member.name|as_cpp_identifier }}; /* {{ member.name }} column */
    {% endfor %}
    /**
     * Serialization function for Boost
     * @tparam A Archive type
     * @param ar Archive (input or output)
     */
    template<typename A>
    void serialize(A& ar, const unsigned int version) {

        {% for member in table.members -%}
        ar & {{ member.name|as_cpp_identifier }};
        {% endfor %}
    }

    /**
     * Extract objects of this type from an HDF5 file
     * and call the callback on each object. The callback
     * must have the following signature:
     *    (?) f(uint64_t run, uint64_t subrun, uint64_t event, const {{ table.name|as_cpp_identifier }}& value)
     *
     * @tparam F type of callback
     * @param file HDF5 from which to extract the objects
     * @param callback callback to call on each object
     */
    template<typename F>
    static void from_hdf5(H5::H5File& file, F&& callback) {
        {{ table.name|as_cpp_identifier }} current;
        H5::DataSet dataset;
        H5::DataSpace dataspace;
        hsize_t dims[2];
        int ndims;

        {% for member in table.members -%}
        std::vector<{{ member.cpptype }}> col_{{ member.name|as_cpp_identifier }}; /* {{ member.name }} column */
        dataset = file.openDataSet("{{ table.name }}/{{ member.name }}");
        dataspace = dataset.getSpace();
        ndims = dataspace.getSimpleExtentDims(dims, NULL);
        col_{{ member.name|as_cpp_identifier }}.resize(dims[0]);
        dataset.read(static_cast<void*>(col_{{ member.name|as_cpp_identifier }}.data()), {{ member.h5type }});
        {% endfor %}

        /* column for run indices */
        std::vector<unsigned> col_run;
        dataset = file.openDataSet("{{ table.name }}/run");
        dataspace = dataset.getSpace();
        ndims = dataspace.getSimpleExtentDims(dims, NULL);
        col_run.resize(dims[0]);
        dataset.read(static_cast<void*>(col_run.data()), H5::PredType::NATIVE_UINT);
        /* column for subrun indices */
        std::vector<unsigned> col_subrun;
        dataset = file.openDataSet("{{ table.name }}/subrun");
        dataspace = dataset.getSpace();
        ndims = dataspace.getSimpleExtentDims(dims, NULL);
        col_subrun.resize(dims[0]);
        dataset.read(static_cast<void*>(col_subrun.data()), H5::PredType::NATIVE_UINT);
        /* column for event indices */
        std::vector<unsigned> col_evt;
        dataset = file.openDataSet("{{ table.name }}/evt");
        dataspace = dataset.getSpace();
        ndims = dataspace.getSimpleExtentDims(dims, NULL);
        col_evt.resize(dims[0]);
        dataset.read(static_cast<void*>(col_evt.data()), H5::PredType::NATIVE_UINT);

        for(uint64_t i = 0; i < dims[0]; i++) {
            {% for member in table.members -%}
            current.{{ member.name|as_cpp_identifier }} = col_{{ member.name|as_cpp_identifier }}[i];
            {% endfor %}
            callback(col_run[i], col_subrun[i], col_evt[i], current);
        }
    }

    template<typename F>
    static void from_hdf5(const std::string& filename, F&& callback) {
        H5::H5File file(filename, H5F_ACC_RDONLY);
        from_hdf5(file, std::forward<F>(callback));
    }
};

{% if namespace|length -%}
}
{%- endif %}
#endif
