/**
 * Copyright
 * @file sym.hpp
 * @brief
 * @author Shogo Sawai
 * @date 2018-11-21 12:29:40
 */
#ifndef SYM_HPP_
#define SYM_HPP_

#include <vector>
#include <string>
#include <memory>
#include <unordered_set>
#include <unordered_map>

namespace sym {

class Factory {
 public:
    static Factory *get() {
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
    static Factory *get_set(Factory *current) {
        static Factory *factory = nullptr;
        if (current) {
            factory = current;
        }
        return factory;
    }

 protected:
    Factory() : num_input_variables(0) {
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

class Function : public std::enable_shared_from_this<Function> {
 public:
    using shared = std::shared_ptr<Function>;

 public:
    virtual ~Function() {}
    int id() const { return _id; }
    const std::string &repr() { return Factory::repr(_id); }
    virtual shared diff(shared v) const = 0;
    virtual void simplified() const = 0;

    template<class T>
    bool is() const { return dynamic_cast<const T*>(this); }

 protected:
    Function(const std::string &repr, const std::unordered_set<int> &depends) : _id(Factory::add(repr, depends)) {
    }
    
 private:
    int _id;
};

class Constant : public Function {
 public:
    Constant(double value) : Function(std::to_string(value), {}) {}
    virtual shared diff(shared v) const override { return std::make_shared<Constant>(0); }
    virtual void simplified() const override {}
};

inline bool is_zero(const Function::shared &f) {
    return f->repr() == "0";
}

inline bool is_one(const Function::shared &f) {
    return f->repr() == "1";
}

class Variable : public Function {
 public:
    Variable(const std::string &symbol) : Function(symbol, {}) {}
    virtual shared diff(shared v) const override {
        if (v->id() == id()) {
            return std::make_shared<Constant>(1);
        }
        return std::make_shared<Constant>(0);
    }
    virtual void simplified() const override {}
};

class BinaryFunction : public Function {
 public:
    BinaryFunction(const std::string &operator_, const Function::shared &arg0_, const Function::shared &arg1_) :
        Function("(" + arg0_->repr() + operator_ + arg1_->repr(), Factory::depends(arg0_->id(), arg1_->id())),
        arg0(arg0_), arg1(arg1_) {}
 protected:
    Function::shared arg0, arg1;
};

class AddFunction : public BinaryFunction {
 public:
    AddFunction(const Function::shared &arg0, const Function::shared &arg1) : BinaryFunction("+", arg0, arg1) {}
    virtual shared diff(shared v) const override {
        return std::make_shared<AddFunction>(arg0->diff(v), arg1->diff(v));
    }
    virtual void simplified() const override {
        arg0->simplified();
        arg1->simplified();
        if (is_zero(arg0)) {
            Factory::setAliasRepr(id(), arg1->id());
        } else if (is_zero(arg1)) {
            Factory::setAliasRepr(id(), arg0->id());
        }
    }
};

template<class V>
class VariableList {
 public:
    VariableList(const std::string &symbol, int num_variables) {
        // TODO: formatcheck symbol
        
    }

    V &operator [](int index) { return v[index]; }
    const V &operator [](int index) const { return v[index]; }

 protected:
    std::vector<V> v;
};

class Input {
    
};

class Output {
    
};

}  // namespace sym

#endif  // SYM_HPP_
