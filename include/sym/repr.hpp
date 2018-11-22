/**
 * Copyright 
 * @file repr.hpp
 * @brief
 * @author Shogo Sawai
 * @date 2018-11-22 12:46:14
 */
#ifndef REPR_HPP_
#define REPR_HPP_

#include <vector>
#include <string>
#include <sstream>

namespace sym {

struct Repr {
    struct Item {
        Item(const char *v) : is_id(false), s(v), id(-1) {}
        Item(const std::string &v) : is_id(false), s(v), id(-1) {}
        Item(const int &v) : is_id(true), s(""), id(v) {}

        bool is_id;
        std::string s;
        int id;
    };
    std::string operator()(const std::vector<Repr> &id2repr) const {
        std::stringstream ss;
        for (auto &&item : items) {
            if (item.is_id) {
                if (id2repr[item.id].invalid) {
                    throw std::runtime_error("access invalid id");
                }
                ss << id2repr[item.id](id2repr);
            } else {
                ss << item.s;
            }
        }
        return ss.str();
    }
    bool invalid = false;
    std::vector<Item> items;
};

template<class ...T>
Repr _repr(const T&... args) {
    Repr r;
    r.items = {args...};
    return r;
}
}  // namespace sym

#endif  // REPR_HPP_
