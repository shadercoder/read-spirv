#pragma once
#include <string>
#include <vector>
#include <optional>
#include <memory>
#include <variant>

enum class Dim {
    Dim1D = 0,
    Dim2D = 1,
    Dim3D = 2,
    Cube = 3,
    Rect = 4,
    Buffer = 5,
    SubpassData = 6,
};

enum class AccessQualifier {
    ReadOnly = 0,
    WriteOnly = 1,
    ReadWrite = 2,
    Max = 0x7fffffff,
};

struct type;

struct numeric_type
{
    enum element_kind { float_, int_, uint_ };
    element_kind elem_kind; 
    size_t elem_width;
    size_t row_count, column_count;
    size_t row_stride, column_stride; // TODO: Should these be optional?
};

struct array_type
{
    std::unique_ptr<type> elem_type;
    size_t elem_count;
    std::optional<size_t> stride;
};

struct struct_member;
struct struct_type
{
    std::string name;
    std::vector<struct_member> members;
};

struct sampler_type
{
    numeric_type type;
    Dim dim;
    bool is_multisampled;
    bool is_array;
    bool is_shadow;
    std::optional<AccessQualifier> access;
};

struct type : std::variant<numeric_type, array_type, struct_type, sampler_type>
{
     using variant::variant;
};

struct struct_member
{
    std::string name;
    type member_type;
    std::optional<size_t> offset;
};


struct uniform_info
{
    std::string name;
    uint32_t set, binding;
    type uniform_type;
};

struct interface_info
{
    std::string name;
    uint32_t location;
    type interface_type;
};

struct entry_point_info
{
    std::string name;
    std::vector<interface_info> inputs;
    std::vector<interface_info> outputs;
};

struct module_interface
{
    std::vector<uniform_info> uniforms;
    std::vector<entry_point_info> entry_points;
};

module_interface get_module_interface(const uint32_t * words, size_t word_count);