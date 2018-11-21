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
#include <memory>
#include <unordered_set>
#include <unordered_map>

namespace sym {

class Function;

class FactoryBase {
 public:
    static FactoryBase *get() {
        return get_set(nullptr);
    }
    static int add(const std::string &repr, const std::unordered_set<int> &depends) {
        return get()->_add(repr, depends);
    }

    static const std::unordered_set<int> &depends(int id) { return get()->_depends(id); }
    static std::unordered_set<int> depends(int id0, int id1) {
        std::unordered_set<int> result = get()->_depends(id0);
        for (auto &&i : get()->_depends(id1)) {
            result.insert(i);
        }
        return result;
    }
    static const std::unordered_set<int> &wholeDepends(int id) { return get()->_wholeDepends(id); }
    static const std::string &repr(int id) {
        while (get()->aliases[id] >= 0) {
            id = get()->aliases[id];
        }
        return get()->rev_repr_map[id];
    }

    static void setAliasRepr(int id0, int id1) {
        get()->aliases[id0] = id1;
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

 protected:
    FactoryBase() : num_input_variables(0) {
        get_set(this);
    }

    int _add(const std::string &repr, const std::unordered_set<int> &depends) {
        auto iter = repr_map.find(repr);
        if (iter == repr_map.end()) {
            int index = repr_map.size();
            repr_map[repr] = index;
            rev_repr_map.push_back(repr);
            depends_list.push_back(depends);
            whole_depends_list.push_back(depends);
            for (auto &&d: depends) {
                for (auto &&j : whole_depends_list[d]) {
                    whole_depends_list[index].insert(j);
                }
            }
            aliases.push_back(-1);
            return index;
        }
        return iter->second;
    }

    const std::unordered_set<int> &_depends(int id) const { return depends_list[id]; }
    const std::unordered_set<int> &_wholeDepends(int id) const { return whole_depends_list[id]; }

 protected:
    int num_input_variables;
    std::unordered_map<std::string, int> repr_map;
    std::vector<std::string> rev_repr_map;
    std::vector<std::unordered_set<int>> depends_list, whole_depends_list;
    std::vector<int> aliases;
};
}  // namespace sym

#endif  // FACTORY_BASE_HPP_
