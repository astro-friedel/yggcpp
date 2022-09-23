#pragma once

#include "MetaschemaType.hpp"
#include "utils/tools.hpp"

namespace communication {
namespace datatypes {
namespace Metaschema {

class SchemaMetaschemaType : public MetaschemaType {
public:
    SchemaMetaschemaType() = delete;

    /*!
  @brief Constructor for SchemaMetaschemaType.
  @param[in] use_generic bool If true, serialized/deserialized
  objects will be expected to be YggGeneric classes.
 */
    SchemaMetaschemaType(const bool use_generic=true) :
    // Always generic
            MetaschemaType("schema", true) {}
    /*!
      @brief Constructor for SchemaMetaschemaType from a JSON type defintion.
      @param[in] type_doc rapidjson::Value rapidjson object containing
      the type definition from a JSON encoded header.
      @param[in] use_generic bool If true, serialized/deserialized
      objects will be expected to be YggGeneric classes.
     */
    SchemaMetaschemaType(const rapidjson::Value &type_doc,
                         const bool use_generic=true) :
    // Always generic
            MetaschemaType(type_doc, true) {}
    /*!
      @brief Constructor for SchemaMetaschemaType from Python dictionary.
      @param[in] pyobj PyObject* Python object.
      @param[in] use_generic bool If true, serialized/deserialized
      objects will be expected to be YggGeneric classes.
     */
    SchemaMetaschemaType(PyObject* pyobj, const bool use_generic=true) :
    // Always generic
            MetaschemaType(pyobj, true) {}
    /*!
      @brief Copy constructor.
      @param[in] other SchemaMetaschemaType* Instance to copy.
     */
    SchemaMetaschemaType(const SchemaMetaschemaType &other) :
            SchemaMetaschemaType(other.use_generic()) {}
    /*!
      @brief Create a copy of the type.
      @returns pointer to new SchemaMetaschemaType instance with the same data.
     */
    SchemaMetaschemaType* copy() const override { return (new SchemaMetaschemaType(use_generic())); }
    /*!
      @brief Copy data wrapped in YggGeneric class.
      @param[in] data YggGeneric* Pointer to generic object.
      @param[in] orig_data Pointer to data that should be copied if the provided
      structure should nto be used.
      @returns void* Pointer to copy of data.
     */
    void* copy_generic(const YggGeneric* data, void* orig_data=nullptr) const override;

    /*!
      @brief Free data wrapped in YggGeneric class.
      @param[in] data YggGeneric* Pointer to generic object.
     */
    void free_generic(YggGeneric* data) const override;

    /*!
      @brief Display data.
      @param[in] data YggGeneric* Pointer to generic object.
      @param[in] indent char* Indentation to add to display output.
     */
    void display_generic(const YggGeneric* data, const char* indent) const override;

    /*!
      @brief Update the type object with info from provided variable arguments for serialization.
      @param[in,out] nargs size_t Number of arguments contained in ap. On output
      the number of unused arguments will be assigned to this address.
      @param[in] ap va_list_t Variable argument list.
      @returns size_t Number of arguments in ap consumed.
     */
    size_t update_from_serialization_args(size_t *nargs, struct va_list_t &ap) override;

    /*!
      @brief Get the item size.
      @returns size_t Size of item in bytes.
     */
    const size_t nbytes() const override {
        return sizeof(dtype_t);
    }
    /*!
      @brief Get the number of arguments expected to be filled/used by the type.
      @returns size_t Number of arguments.
     */
    size_t nargs_exp() const override {
        return 1;
    }
    /*!
      @brief Skip arguments that make of this type.
      @param[in, out] nargs Pointer to number of arguments in ap.
      @param[in, out] ap va_list_t Variable argument list.
     */
    void skip_va_elements_core(size_t *nargs, struct va_list_t *ap) const override {
        va_arg(ap->va, dtype_t*);
        (*nargs)--;
    }
    /*!
      @brief Convert a Python representation to a C representation.
      @param[in] pyobj PyObject* Pointer to Python object.
      @returns YggGeneric* Pointer to C object.
     */
    YggGeneric* python2c(PyObject* pyobj) const override;

    /*!
      @brief Convert a C representation to a Python representation.
      @param[in] cobj YggGeneric* Pointer to C object.
      @returns PyObject* Pointer to Python object.
     */
    PyObject* c2python(YggGeneric *cobj) const override;

    // Encoding
    /*!
      @brief Encode arguments describine an instance of this type into a JSON string.
      @param[in] writer rapidjson::Writer<rapidjson::StringBuffer> rapidjson writer.
      @param[in,out] nargs size_t * Pointer to the number of arguments contained in
      ap. On return it will be set to the number of arguments used.
      @param[in] ap va_list_t Variable number of arguments that should be encoded
      as a JSON string.
      @returns bool true if the encoding was successful, false otherwise.
     */
    bool encode_data(rapidjson::Writer<rapidjson::StringBuffer> *writer,
                     size_t *nargs, struct va_list_t &ap) const override;

    /*!
      @brief Encode arguments describine an instance of this type into a JSON string.
      @param[in] writer rapidjson::Writer<rapidjson::StringBuffer> rapidjson writer.
      @param[in] x YggGeneric* Pointer to generic wrapper for data.
      @returns bool true if the encoding was successful, false otherwise.
     */
    bool encode_data(rapidjson::Writer<rapidjson::StringBuffer> *writer,
                     YggGeneric* x) const override;

    // Decoding
    /*!
      @brief Decode variables from a JSON string.
      @param[in] data rapidjson::Value Reference to entry in JSON string.
      @param[in] allow_realloc int If 1, the passed variables will be reallocated
      to contain the deserialized data.
      @param[in,out] nargs size_t Number of arguments contained in ap. On return,
      the number of arguments assigned from the deserialized data will be assigned
      to this address.
      @param[out] ap va_list_t Reference to variable argument list containing
      address where deserialized data should be assigned.
      @returns bool true if the data was successfully decoded, false otherwise.
     */
    bool decode_data(rapidjson::Value &data, const int allow_realloc,
                     size_t *nargs, struct va_list_t &ap) const override;

};
}
}
}