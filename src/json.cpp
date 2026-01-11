#include "json.h"

#include <charconv>
#include <cmath>
#include <iomanip>

#include "quoted.h"

namespace jsondb {

std::ostream& operator<< (std::ostream& os, const number& n) {
    if (n.is_int ()) {
        os << n.get_int ();
    } else {
        double d = n.get_double ();
        if (std::isnan (d) || std::isinf (d)) {
            os << "null";
        } else {
            auto org_flag = os.flags () & (std::ios_base::showpoint | std::ios_base::floatfield);
            os << std::showpoint << std::defaultfloat << d;
            os.setf (org_flag);
        }
    }

    return os;
}

std::istream& operator>> (std::istream& is, number& n) {
    is >> std::ws;

    string i_part;
    for (char c = is.peek (); '0' <= c && c <= '9' || c == '-'; c = is.peek ()) {
        i_part += is.get ();
    }

    n = static_cast<int64_t> (0);
    if (std::from_chars (i_part.data (), i_part.data () + i_part.length (), n.get_int ()).ec ==
        std::errc::result_out_of_range) {
        double nd = 0;

        for (int i = (i_part[0] == '-' ? 1 : 0); i < i_part.length (); ++i) {
            nd = nd * 10 + i_part[i] - '0';
        }

        n = (i_part[0] == '-' ? -nd : nd);
    }

    if (is.peek () == '.') {
        double nd = n.to_double ();
        is.get ();
        if (is.peek () < '0' || is.peek () > '9') throw std::invalid_argument ("format error, missing decimal");

        double nw = 1;
        while ('0' <= is.peek () && is.peek () <= '9') {
            nw *= .1;
            if (std::signbit (nd)) {
                nd -= (is.get () - '0') * nw;
            } else {
                nd += (is.get () - '0') * nw;
            }
        }

        n = nd;
    }

    if (is.peek () == 'e' || is.peek () == 'E') {
        is.get ();

        double index = 0;
        bool sign = is.peek () == '-';
        if (is.peek () == '+' || is.peek () == '-') is.get ();
        if (is.peek () < '0' || is.peek () > '9') throw std::invalid_argument ("format error, missing exponent");

        for (char c = is.peek (); '0' <= c && c <= '9'; is.get (), c = is.peek ()) {
            index = index * 10 + c - '0';
        }

        n = n.to_double () * std::pow (10, (sign ? -index : index));
    }

    return is;
}

namespace {
void print_json (std::ostream& os, const json& j, size_t w, size_t d = 0) {
    switch (j.get_type ()) {
        case t_obj: {
            os << '{';

            size_t i = 0;
            for (const auto& [key, value] : j.get_object ()) {
                if (i++) os << ',';
                if (os.fill () == '\n') {
                    os << '\n';
                    for (size_t i = 0; i < w * (d + 1); ++i) os << ' ';
                }

                os << quoted (key) << ':';
                if (os.fill () == '\n') os << ' ';

                print_json (os, value, w, d + 1);
            }

            if (os.fill () == '\n') {
                os << '\n';
                for (size_t i = 0; i < w * d; ++i) os << ' ';
            }
            os << '}';
        } break;

        case t_arr: {
            os << '[';

            size_t i = 0;
            for (const auto& value : j.get_array ()) {
                if (i++) os << ',';
                if (os.fill () == '\n') {
                    os << '\n';
                    for (size_t i = 0; i < w * (d + 1); ++i) os << ' ';
                }

                print_json (os, value, w, d + 1);
            }

            if (os.fill () == '\n') {
                os << '\n';
                for (size_t i = 0; i < w * d; ++i) os << ' ';
            }
            os << ']';
        } break;

        case t_str: {
            os << quoted (j.get_string ());
        } break;

        case t_num: {
            os << j.get_number ();
        } break;

        case t_bool: {
            os << (j.get_bool () ? "true" : "false");
        } break;

        case t_null: {
            os << "null";
        } break;
    }
}
}  // namespace
std::ostream& operator<< (std::ostream& os, const json& j) {
    size_t w = os.width ();
    os << std::setw (0);
    print_json (os, j, w);
    return os;
}

std::istream& operator>> (std::istream& is, json& j) {
    is >> std::ws;

    switch (char c = is.peek ()) {
        case '{': {
            is.get ();
            is >> std::ws;

            object o;
            for (size_t i = 0; is.peek () != '}'; ++i) {
                if (i) is >> fixed (',');

                string key;
                json value;
                is >> quoted (key) >> std::ws >> fixed (':') >> value >> std::ws;

                if (auto it = o.find (key); it != o.end ()) {
                    throw std::invalid_argument ("duplicated key");
                }
                o[key] = value;
            }
            is.get ();

            j = std::move (o);
        } break;
        case '[': {
            is.get ();
            is >> std::ws;

            array a;
            size_t i = 0;
            for (size_t i = 0; is.peek () != ']'; ++i) {
                if (i) is >> fixed (',');

                json element;
                is >> element >> std::ws;
                a.push_back (std::move (element));
            }
            is.get ();

            j = std::move (a);
        } break;
        case '"': {
            j = "";
            is >> quoted (j.get_string ());
        } break;
        case 't': {
            is >> fixed ("true");
            j = true;
        } break;
        case 'f': {
            is >> fixed ("false");
            j = false;
        } break;
        case 'n': {
            is >> fixed ("null");
            j = null;
        } break;
        default: {
            if ('0' <= c && c <= '9' || c == '-') {
                j = number ();
                is >> j.get_number ();
            } else {
                throw std::invalid_argument ("format error, unexpected character");
            }
        }
    }

    return is;
}

}  // namespace jsondb
