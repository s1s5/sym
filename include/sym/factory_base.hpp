/**
 * Copyright
 * @file factory_base.hpp
 * @brief
 * @author Shogo Sawai
 * @date 2018-11-21 14:30:12
 */
#ifndef FACTORY_BASE_HPP_
#define FACTORY_BASE_HPP_

#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include <unordered_set>
#include <unordered_map>

namespace sym {

class Function;

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

class FactoryBase {
 public:
    static FactoryBase *get() {
        return get_set(nullptr);
    }
    static int add(const Repr &repr, const std::unordered_set<int> &depends, Function * f) {
        return get()->_add(repr, depends, f);
    }
    
    static std::unordered_set<int> depends(int id) {
        std::unordered_set<int> result = get()->_depends(id);
        result.insert(id);
        return result;
    }
    static std::unordered_set<int> depends(int id0, int id1) {
        std::unordered_set<int> result = get()->_depends(id0);
        for (auto &&i : get()->_depends(id1)) {
            result.insert(i);
        }
        result.insert(id0);
        result.insert(id1);
        return result;
    }
    static const std::unordered_set<int> &wholeDepends(int id) { return get()->_wholeDepends(id); }
    static bool checkDepends(int my_id, int var_id) { auto &wdl = wholeDepends(my_id); return wdl.find(var_id) != wdl.end(); }
    static std::string repr(int id) {
        return get()->rev_repr_map[id](get()->rev_repr_map);
    }

    static void setAliasRepr(int id0, int id1) {
        get()->aliases[id0] = id1;
        get()->rev_repr_map[id0] = get()->rev_repr_map[id1];
        get()->depends_list[id0] = get()->depends_list[id1];
        get()->whole_depends_list[id0] = get()->whole_depends_list[id1];
    }

    static int alias(int id) {
        return get()->aliases[id] < 0 ? id : get()->aliases[id] ;
    }

    template<class T>
    static bool is(int id) {
        while (get()->aliases[id] >= 0) {
            id = get()->aliases[id];
        }
        return dynamic_cast<T*>(get()->functions[id]);
    }

 protected:
    static FactoryBase *get_set(FactoryBase *current) {
        static FactoryBase *factory = nullptr;
        if (current) {
            factory = current;
        }
        return factory;
    }

 public:
    virtual ~FactoryBase() {}
    std::vector<int> idMapping() const {
        std::vector<int> result(aliases.size());
        for (size_t i = 0; i < aliases.size(); i++) {
            int id = i;
            while (aliases[id] >= 0) {
                id = aliases[id];
            }
            result[i] = id;
        }
        return result;
    }

 protected:
    FactoryBase() : num_input_variables(0) {
        get_set(this);
    }

    int _add(const Repr &repr_obj, const std::unordered_set<int> &depends, Function *f ) {
        std::string repr = repr_obj(rev_repr_map);
        auto iter = repr_map.find(repr);
        if (iter == repr_map.end()) {
            int index = repr_map.size();
            repr_map[repr] = index;
            rev_repr_map.push_back(repr_obj);
            org_rev_repr_map.push_back(repr_obj);
            depends_list.push_back(depends);
            whole_depends_list.push_back(depends);
            for (auto &&d: depends) {
                for (auto &&j : whole_depends_list[d]) {
                    whole_depends_list[index].insert(j);
                }
            }
            aliases.push_back(-1);
            functions.push_back(f);
            return index;
        }
        return iter->second;
    }

    const std::unordered_set<int> &_depends(int id) const { return depends_list[id]; }
    const std::unordered_set<int> &_wholeDepends(int id) const { return whole_depends_list[id]; }

 protected:
    int num_input_variables;
    std::unordered_map<std::string, int> repr_map;
    std::vector<Repr> rev_repr_map, org_rev_repr_map;
    std::vector<std::unordered_set<int>> depends_list, whole_depends_list;
    std::vector<int> aliases;
    std::vector<Function*> functions;
};
}  // namespace sym

#endif  // FACTORY_BASE_HPP_
