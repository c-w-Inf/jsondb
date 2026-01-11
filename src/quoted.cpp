#include "quoted.h"

#include <iomanip>

namespace jsondb {

std::ostream& operator<< (std::ostream& os, const quoted_const_string& s) {
    os << std::setw (0);
    std::ios stat (nullptr);
    stat.copyfmt (os);

    os << '\"' << std::hex << std::noshowbase << std::setfill ('0');

    for (char c : s.s) {
        switch (c) {
            case '"': {
                os << "\\\"";
            } break;
            case '\\': {
                os << "\\\\";
            } break;
            case '\b': {
                os << "\\b";
            } break;
            case '\f': {
                os << "\\f";
            } break;
            case '\n': {
                os << "\\n";
            } break;
            case '\r': {
                os << "\\r";
            } break;
            case '\t': {
                os << "\\t";
            } break;
            default: {
                if (0 <= c && c < 0x20 || c == 0x7f)
                    os << "\\u" << std::setw (4) << static_cast<int> (c);
                else
                    os << c;
            }
        }
    }

    os << '\"';
    os.copyfmt (stat);
    return os;
}

std::istream& operator>> (std::istream& is, const quoted_string& s) {
    is >> std::ws >> fixed ('\"');
    s.s.clear ();

    for (int c;;) {
        switch (c = is.get ()) {
            case '\"': {
                return is;
            } break;
            case '\\': {
                switch (c = is.get ()) {
                    case '"': {
                        s.s += '\"';
                    } break;
                    case '\\': {
                        s.s += '\\';
                    } break;
                    case 'b': {
                        s.s += '\b';
                    } break;
                    case 'f': {
                        s.s += '\f';
                    } break;
                    case 'n': {
                        s.s += '\n';
                    } break;
                    case 'r': {
                        s.s += '\r';
                    } break;
                    case 't': {
                        s.s += '\t';
                    } break;
                    case 'u': {
                        int cp = 0;
                        for (size_t i = 0; i < 4; ++i) {
                            char c = is.get ();
                            if ('0' <= c && c <= '9') {
                                cp = cp * 0x10 + c - '0';
                            } else if ('a' <= c && c <= 'f') {
                                cp = cp * 0x10 + c - 'a' + 0xa;
                            } else if ('A' <= c && c <= 'F') {
                                cp = cp * 0x10 + c - 'A' + 0xa;
                            } else {
                                if (is.eof ())
                                    throw std::invalid_argument ("format error, unexpected eof");
                                else
                                    throw std::invalid_argument ("format error, expecting hex number");
                            }
                        }

                        if (cp < 0x80) {
                            s.s += static_cast<char> (cp);
                        } else if (cp < 0x800) {
                            s.s += static_cast<char> (0xc0 | ((cp >> 6) & 0x1f));
                            s.s += static_cast<char> (0x80 | (cp & 0x3f));
                        } else {
                            s.s += static_cast<char> (0xe0 | ((cp >> 12) & 0x0f));
                            s.s += static_cast<char> (0x80 | ((cp >> 6) & 0x3f));
                            s.s += static_cast<char> (0x80 | (cp & 0x3f));
                        }
                    } break;
                    default: {
                        if (is.eof ()) throw std::invalid_argument ("format error, unexpected eof");
                        throw std::invalid_argument ("format error, unknown escape");
                    }
                }
            } break;
            default: {
                if (is.eof ()) throw std::invalid_argument ("format error, unexpected eof");
                if (0 <= c && c < 0x20 || c == 0x7f)
                    throw std::invalid_argument ("format error, unexpected " + std::to_string (c));
                s.s += c;
            }
        }
    }
}

quoted_string quoted (std::string& s) { return quoted_string (s); }
quoted_const_string quoted (const std::string& s) { return quoted_const_string (s); }

std::istream& operator>> (std::istream& is, const fixed_string& s) {
    for (char c : s.s) {
        if (is.peek () != c) throw std::invalid_argument ("format error, expecting " + std::to_string (c));
        is.get ();
    }

    return is;
}
std::istream& operator>> (std::istream& is, const fixed_char& c) {
    if (is.peek () != c.c) throw std::invalid_argument ("format error, expecting " + std::to_string (c.c));
    is.get ();

    return is;
}

fixed_string fixed (const std::string& s) { return fixed_string (s); }
fixed_char fixed (char c) { return fixed_char (c); }

}  // namespace jsondb
