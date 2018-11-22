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
#include "calculation_graph.hpp"
#include "cxx_code_printer.hpp"
#include "digraph.hpp"

namespace sym {

enum class IOTag {
    STATIC,
    DYNAMIC,
};

class Factory : public FactoryBase {
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
    Digraph digraph() const { return Digraph(rev_repr_map, depends_list, idMapping()); }
    CalculationGraph wholeGraph() const {
        std::unordered_map<int, std::string> input_nodes;
        std::unordered_map<int, std::string> output_nodes;
        for (auto &&[symbol, vlist] : static_inputs) {
            for (auto &&v : vlist) {
                input_nodes[v->id()] = rev_repr_map[v->id()](rev_repr_map);
            }
        }
        for (auto &&[symbol, vlist] : dynamic_inputs) {
            for (auto &&v : vlist) {
                input_nodes[v->id()] = rev_repr_map[v->id()](rev_repr_map);
            }
        }
        for (auto &&[symbol, ptr_vlist] : static_outputs) {
            for (size_t index = 0; index < ptr_vlist->size(); index++) {
                if (not ptr_vlist->at(index)) {
                    throw std::runtime_error("output variable is not set");
                }
                output_nodes[ptr_vlist->at(index)->id()] = symbol + "[" + std::to_string(index) + "]";
            }
        }
        for (auto &&[symbol, ptr_vlist] : dynamic_outputs) {
            for (size_t index = 0; index < ptr_vlist->size(); index++) {
                if (not ptr_vlist->at(index)) {
                    throw std::runtime_error("output variable is not set");
                }
                output_nodes[ptr_vlist->at(index)->id()] = symbol + "[" + std::to_string(index) + "]";
            }
        }
        return CalculationGraph(input_nodes, output_nodes, rev_repr_map, depends_list, idMapping());
    }

    void simplified() {
        for (auto &&[symbol, ptr_vlist] : static_outputs) {
            for (size_t index = 0; index < ptr_vlist->size(); index++) {
                if (not ptr_vlist->at(index)) {
                    throw std::runtime_error("output variable is not set");
                }
                ptr_vlist->at(index)->simplified();
            }
        }
        for (auto &&[symbol, ptr_vlist] : dynamic_outputs) {
            for (size_t index = 0; index < ptr_vlist->size(); index++) {
                if (not ptr_vlist->at(index)) {
                    throw std::runtime_error("output variable is not set");
                }
                ptr_vlist->at(index)->simplified();
            }
        }
    }

    CxxCodePrinter cxxCodePrinter(const std::string &ns, const std::string &class_name) {
        CxxCodePrinter printer(ns, class_name);
        std::vector<std::string> static_variables, dynamic_variables;
        for (auto &&[symbol, vlist] : static_inputs) {
            static_variables.push_back(symbol);
        }
        for (auto &&[symbol, vlist] : dynamic_inputs) {
            dynamic_variables.push_back(symbol);
        }
        for (auto &&[symbol, vlist] : static_outputs) {
            static_variables.push_back(symbol);
        }
        for (auto &&[symbol, vlist] : dynamic_outputs) {
            dynamic_variables.push_back(symbol);
        }

        std::vector<bool> dynamic_nodes(repr_map.size(), false);
        std::vector<bool> intermediate_nodes(repr_map.size(), false);
        for (size_t i = 0; i < whole_depends_list.size(); i++) {
            for (auto &&[symbol, vlist] : dynamic_inputs) {
                for (auto &&v : vlist) {
                    if (whole_depends_list[i].find(v->id()) != whole_depends_list[i].end()) {
                        dynamic_nodes[i] = true;
                        break;
                    }
                }
            }
        }
        for (size_t i = 0; i < dynamic_nodes.size(); i++) {
            if (not dynamic_nodes[i]) {
                continue;
            }
            for (auto &&d : depends_list[i]) {
                if (dynamic_nodes[d]) {
                    continue;
                }
                intermediate_nodes[d] = true;
            }
        }
        for (auto &&[symbol, vlist] : dynamic_outputs) {
            for (auto &&v : *vlist) {
                if (dynamic_nodes[v->id()]) {
                    continue;
                }
                intermediate_nodes[v->id()] = true;
            }
        }

        std::unordered_map<int, std::string> static_input_nodes, dynamic_input_nodes;
        std::unordered_map<int, std::string> static_output_nodes, dynamic_output_nodes;

        for (auto &&[symbol, vlist] : static_inputs) {
            for (auto &&v : vlist) {
                static_input_nodes[v->id()] = rev_repr_map[v->id()](rev_repr_map);
            }
        }
        for (auto &&[symbol, vlist] : dynamic_inputs) {
            for (auto &&v : vlist) {
                dynamic_input_nodes[v->id()] = rev_repr_map[v->id()](rev_repr_map);
            }
        }
        for (auto &&[symbol, ptr_vlist] : static_outputs) {
            for (size_t index = 0; index < ptr_vlist->size(); index++) {
                if (not ptr_vlist->at(index)) {
                    throw std::runtime_error("output variable is not set");
                }
                int id = ptr_vlist->at(index)->id();
                if (dynamic_nodes[id]) {
                    dynamic_output_nodes[id] = symbol + "[" + std::to_string(index) + "]";
                } else {
                    static_output_nodes[id] = symbol + "[" + std::to_string(index) + "]";
                }
            }
        }
        for (auto &&[symbol, ptr_vlist] : dynamic_outputs) {
            for (size_t index = 0; index < ptr_vlist->size(); index++) {
                if (not ptr_vlist->at(index)) {
                    throw std::runtime_error("output variable is not set");
                }
                dynamic_output_nodes[ptr_vlist->at(index)->id()] = symbol + "[" + std::to_string(index) + "]";
            }
        }

        int num_intermediates = 0;
        for (size_t i = 0; i < intermediate_nodes.size(); i++) {
            if (not intermediate_nodes[i]) {
                continue;
            }
            if (dynamic_input_nodes.find(i) != dynamic_input_nodes.end()) {
                continue;
            }
            static_output_nodes[i] = "_i[" + std::to_string(num_intermediates) + "]";
            dynamic_input_nodes[i] = "_i[" + std::to_string(num_intermediates) + "]";
            num_intermediates++;
        }

        CalculationGraph static_dag(static_input_nodes, static_output_nodes, rev_repr_map, depends_list, idMapping());
        CalculationGraph dynamic_dag(dynamic_input_nodes, dynamic_output_nodes, rev_repr_map, depends_list, idMapping());
        std::stringstream sd, dd;
        sd << static_dag;
        dd << dynamic_dag;

        printer.setStaticVariables(static_variables, num_intermediates, sd.str());
        printer.setDynamicVariables(dynamic_variables, dd.str());

        return printer;
    }

 protected:
    

 protected:
    std::vector<std::tuple<std::string, std::vector<std::shared_ptr<Variable>>>> static_inputs, dynamic_inputs;
    std::vector<std::tuple<std::string, std::vector<Function::shared>*>> static_outputs, dynamic_outputs;
};

} // namespace sym

#endif  // FACTORY_HPP_
