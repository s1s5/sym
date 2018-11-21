/**
 * Copyright 
 * @file factory.hpp
 * @brief
 * @author Shogo Sawai
 * @date 2018-11-21 14:34:15
 */
#ifndef FACTORY_HPP_
#define FACTORY_HPP_

#include <ostream>

#include "factory_base.hpp"
#include "function.hpp"

namespace sym {

enum class IOTag {
    STATIC,
    DYNAMIC,
};

class Factory : public FactoryBase {
 public:
    class Digraph {
     public:
        Digraph(const std::vector<std::string> &rev_repr_map_, const std::vector<std::unordered_set<int>> &depends_list_,
                const std::vector<int> &aliases_) : rev_repr_map(rev_repr_map_), depends_list(depends_list_), aliases(aliases_.size()) {
            for (size_t i = 0; i < aliases_.size(); i++) {
                int id = i;
                while (aliases_[id] >= 0) {
                    id = aliases_[id];
                }
                aliases[i] = id;
            }
        }

        friend std::ostream &operator<<(std::ostream &os, const Digraph &d) {
            os << "digraph graphname {" << std::endl;
            for (size_t i = 0; i < d.rev_repr_map.size(); i++) {
                os << "    n" << i << " [label=\"" << d.rev_repr_map[i] << "\"];" << std::endl;
            }
            for (size_t i = 0; i < d.depends_list.size(); i++) {
                for (auto &&j : d.depends_list[i]) {
                    os << "    n" << j << " -> n" << i << ";" << std::endl;
                }
            }
            return os << "}" << std::endl;
        }
        
     protected:
        std::vector<std::string> rev_repr_map;
        std::vector<std::unordered_set<int>> depends_list;
        std::vector<int> aliases;
    };

 public:
    static void addInput(IOTag tag, const std::string &symbol, const std::vector<std::shared_ptr<Variable>> &inputs_) {
        switch (tag) {
            case IOTag::STATIC:
                get()->static_inputs.emplace_back(symbol, inputs_);
                break;
            case IOTag::DYNAMIC:
                get()->dynamic_inputs.emplace_back(symbol, inputs_);
                break;
        }
    }

    static void addOutput(IOTag tag, const std::string &symbol, std::vector<Function::shared> *outputs_) {
        switch (tag) {
            case IOTag::STATIC:
                get()->static_outputs.emplace_back(symbol, outputs_);
                break;
            case IOTag::DYNAMIC:
                get()->dynamic_outputs.emplace_back(symbol, outputs_);
                break;
        }
    }

 protected:
    static Factory *get() {
        return dynamic_cast<Factory*>(FactoryBase::get());
    }

 public:
    virtual ~Factory() {}
    Function::shared diff(const Function::shared &func, const Function::shared &var) { return func->diff(var); }
    Digraph digraph() const { return Digraph(rev_repr_map, depends_list, aliases); }

 protected:
    

 protected:
    std::vector<std::tuple<std::string, std::vector<std::shared_ptr<Variable>>>> static_inputs, dynamic_inputs;
    std::vector<std::tuple<std::string, std::vector<Function::shared>*>> static_outputs, dynamic_outputs;
};

} // namespace sym

#endif  // FACTORY_HPP_
