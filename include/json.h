#pragma once

#include <iostream>
#include <map>
#include <variant>
#include <vector>

namespace jsondb {

class number {
    std::variant<double, int64_t> v;

 public:
    number (): v (static_cast<int64_t> (0)) {}
    template <std::integral T>
    number (T v): v (static_cast<int64_t> (v)) {}  // NOLINT(runtime/explicit)
    template <std::floating_point T>
    number (T v): v (static_cast<double> (v)) {}  // NOLINT(runtime/explicit)

    double to_double () const {
        if (auto pd = std::get_if<double> (&v)) {
            return *pd;
        } else {
            return std::get<int64_t> (v);
        }
    }

    double& get_double () { return std::get<double> (v); }
    double get_double () const { return std::get<double> (v); }

    int64_t& get_int () { return std::get<int64_t> (v); }
    int64_t get_int () const { return std::get<int64_t> (v); }

    bool is_int () const { return std::holds_alternative<int64_t> (v); }

    friend std::ostream& operator<< (std::ostream&, const number&);
    friend std::istream& operator>> (std::istream&, number&);
};

typedef std::string string;
typedef std::monostate null_type;
constexpr null_type null;

class json;

typedef std::map<std::string, json> object;
typedef std::vector<json> array;

class json {
    std::variant<object, array, string, number, bool, null_type> v;

 public:
    json (): v (null) {}

    json (const object& v): v (v) {}         // NOLINT(runtime/explicit)
    json (object&& v): v (std::move (v)) {}  // NOLINT(runtime/explicit)

    json (const array& v): v (v) {}         // NOLINT(runtime/explicit)
    json (array&& v): v (std::move (v)) {}  // NOLINT(runtime/explicit)

    json (const string& v): v (v) {}                      // NOLINT(runtime/explicit)
    json (string&& v): v (std::move (v)) {}               // NOLINT(runtime/explicit)
    json (const char* v): v (static_cast<string> (v)) {}  // NOLINT(runtime/explicit)

    json (const number& v): v (v) {}         // NOLINT(runtime/explicit)
    json (number&& v): v (std::move (v)) {}  // NOLINT(runtime/explicit)
    template <std::integral T>
    json (T v): v (number (v)) {}  // NOLINT(runtime/explicit)
    template <std::floating_point T>
    json (T v): v (number (v)) {}  // NOLINT(runtime/explicit)

    json (bool v): v (v) {}  // NOLINT(runtime/explicit)

    json (null_type): v (null) {}  // NOLINT(runtime/explicit)

    json (const json&) = default;
    json (json&& o): v (std::move (o.v)) { o.v = null; }

    json& operator= (const json&) = default;
    json& operator= (json&& o) {
        v = std::move (o.v);
        o.v = null;
        return *this;
    }

    const object& get_object () const { return std::get<object> (v); }
    object& get_object () { return std::get<object> (v); }

    const array& get_array () const { return std::get<array> (v); }
    array& get_array () { return std::get<array> (v); }

    const std::string& get_string () const { return std::get<std::string> (v); }
    std::string& get_string () { return std::get<std::string> (v); }

    const number& get_number () const { return std::get<number> (v); }
    number& get_number () { return std::get<number> (v); }

    bool get_bool () const { return std::get<bool> (v); }
    bool& get_bool () { return std::get<bool> (v); }

    size_t get_type () const { return v.index (); }

    friend std::ostream& operator<< (std::ostream&, const json&);
    friend std::istream& operator>> (std::istream&, json&);
};

static constexpr size_t t_obj = 0;
static constexpr size_t t_arr = 1;
static constexpr size_t t_str = 2;
static constexpr size_t t_num = 3;
static constexpr size_t t_bool = 4;
static constexpr size_t t_null = 5;

}  // namespace jsondb
