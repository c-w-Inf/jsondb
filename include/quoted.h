#pragma once

#include <iostream>

namespace jsondb {

class quoted_const_string {
    const std::string& s;

 public:
    explicit quoted_const_string (const std::string& s): s (s) {}

    friend std::ostream& operator<< (std::ostream&, const quoted_const_string&);
};

class quoted_string {
    std::string& s;

 public:
    explicit quoted_string (std::string& s): s (s) {}

    operator quoted_const_string () const { return quoted_const_string (s); }

    friend std::ostream& operator<< (std::ostream&, const quoted_string&);
    friend std::istream& operator>> (std::istream&, const quoted_string&);
};

quoted_const_string quoted (const std::string&);
quoted_string quoted (std::string&);

class fixed_string {
    std::string s;

 public:
    explicit fixed_string (const std::string& s): s (s) {}

    friend std::istream& operator>> (std::istream&, const fixed_string&);
};

class fixed_char {
    char c;

 public:
    explicit fixed_char (char c): c (c) {}

    friend std::istream& operator>> (std::istream&, const fixed_char&);
};

fixed_string fixed (const std::string&);
fixed_char fixed (char);

}  // namespace jsondb
