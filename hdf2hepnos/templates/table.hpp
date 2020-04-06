/*
 * (C) 2019 The University of Chicago
 * 
 * See COPYRIGHT in top-level directory.
 */
#ifndef __{{ table.name|upper|as_cpp_identifier }}_HPP_
#define __{{ table.name|upper|as_cpp_identifier }}_HPP_

#include <tuple>
#include <string>
#include <vector>
#include <cstdint>
#include <hdf5.h>

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
    static void from_hdf5(hid_t file, F&& callback) {
        {{ table.name|as_cpp_identifier }} current;
        hid_t dataset;
        hid_t dataspace;
        hsize_t dims[2];
        herr_t err;
        int ndims;

        std::vector<unsigned> col_run;
        std::vector<unsigned> col_subrun;
        std::vector<unsigned> col_evt;
        _read_indices(file, col_run, col_subrun, col_evt);

        {% for member in table.members -%}
        std::vector<{{ member.cpptype }}> col_{{ member.name|as_cpp_identifier }}; /* {{ member.name }} column */
        dataset = H5Dopen(file, "{{ table.name }}/{{ member.name }}", H5P_DEFAULT);
        dataspace = H5Dget_space(dataset);
        ndims = H5Sget_simple_extent_dims(dataspace, dims, NULL);
        col_{{ member.name|as_cpp_identifier }}.resize(dims[0]);
        err = H5Dread(dataset, {{ member.h5type }}, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                static_cast<void*>(col_{{ member.name|as_cpp_identifier }}.data()));
        H5Sclose(dataspace);
        H5Dclose(dataset);
        {% endfor %}

        for(uint64_t i = 0; i < dims[0]; i++) {
            {% for member in table.members -%}
            current.{{ member.name|as_cpp_identifier }} = col_{{ member.name|as_cpp_identifier }}[i];
            {% endfor %}
            callback(col_run[i], col_subrun[i], col_evt[i], current);
        }
    }

    template<typename F>
    static void from_hdf5(const std::string& filename, F&& callback) {
        hid_t file_id = H5Fopen(filename.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
        from_hdf5(file_id, std::forward<F>(callback));
    }

    static std::tuple<
            std::vector<unsigned>,
            std::vector<unsigned>,
            std::vector<unsigned>,
            std::vector<{{ table.name|as_cpp_identifier }}>
           > from_hdf5(hid_t file) {
        hid_t dataset;
        hid_t dataspace;
        hsize_t dims[2];
        herr_t err;
        int ndims;
        std::vector<{{ table.name|as_cpp_identifier }}> content;
        std::vector<unsigned> col_run;
        std::vector<unsigned> col_subrun;
        std::vector<unsigned> col_evt;

        _read_indices(file, col_run, col_subrun, col_evt);

        content.resize(col_run.size());

        {% for member in table.members -%}
        std::vector<{{ member.cpptype }}> col_{{ member.name|as_cpp_identifier }}; /* {{ member.name }} column */
        dataset = H5Dopen(file, "{{ table.name }}/{{ member.name }}", H5P_DEFAULT);
        dataspace = H5Dget_space(dataset);
        ndims = H5Sget_simple_extent_dims(dataspace, dims, NULL);
        col_{{ member.name|as_cpp_identifier }}.resize(dims[0]);
        err = H5Dread(dataset, {{ member.h5type }}, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                static_cast<void*>(col_{{ member.name|as_cpp_identifier }}.data()));
        H5Sclose(dataspace);
        H5Dclose(dataset);
        {% endfor %}

        for(uint64_t i = 0; i < dims[0]; i++) {
            {% for member in table.members -%}
            content[i].{{ member.name|as_cpp_identifier }} = col_{{ member.name|as_cpp_identifier }}[i];
            {% endfor %}
        }

        return { col_run, col_subrun, col_evt, content };
    }

    static std::tuple<
            std::vector<unsigned>,
            std::vector<unsigned>,
            std::vector<unsigned>,
            std::vector<{{ table.name|as_cpp_identifier }}>
           > from_hdf5(const std::string& filename) {
        hid_t file_id = H5Fopen(filename.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
        return from_hdf5(file_id);
    }

    private:

    static void _read_indices(hid_t file,
                              std::vector<unsigned>& runs,
                              std::vector<unsigned>& subruns,
                              std::vector<unsigned>& events)
    {
        hid_t dataset;
        hid_t dataspace;
        hsize_t dims[2];
        herr_t err;
        int ndims;
        /* column for run indices */
        dataset = H5Dopen(file, "{{ table.name }}/run", H5P_DEFAULT);
        dataspace = H5Dget_space(dataset);
        ndims = H5Sget_simple_extent_dims(dataspace, dims, NULL);
        runs.resize(dims[0]);
        err = H5Dread(dataset, H5T_NATIVE_UINT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                static_cast<void*>(runs.data()));
        H5Sclose(dataspace);
        H5Dclose(dataset);
        /* column for subrun indices */
        dataset = H5Dopen(file, "{{ table.name }}/subrun", H5P_DEFAULT);
        dataspace = H5Dget_space(dataset);
        ndims = H5Sget_simple_extent_dims(dataspace, dims, NULL);
        subruns.resize(dims[0]);
        err = H5Dread(dataset, H5T_NATIVE_UINT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                static_cast<void*>(subruns.data()));
        H5Sclose(dataspace);
        H5Dclose(dataset);
        /* column for event indices */
        dataset = H5Dopen(file, "{{ table.name }}/evt", H5P_DEFAULT);
        dataspace = H5Dget_space(dataset);
        ndims = H5Sget_simple_extent_dims(dataspace, dims, NULL);
        events.resize(dims[0]);
        err = H5Dread(dataset, H5T_NATIVE_UINT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                static_cast<void*>(events.data()));
        H5Sclose(dataspace);
        H5Dclose(dataset);
    }
};

{% if namespace|length -%}
}
{%- endif %}
#endif
