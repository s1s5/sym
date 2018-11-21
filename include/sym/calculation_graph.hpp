/**
 * Copyright 
 * @file calculation_graph.hpp
 * @brief
 * @author Shogo Sawai
 * @date 2018-11-21 17:42:44
 */
#ifndef CALCULATION_GRAPH_HPP_
#define CALCULATION_GRAPH_HPP_

#include <algorithm>
#include "factory_base.hpp"
#include <iostream>

namespace sym {

class CalculationGraph {
 public:
    CalculationGraph(const std::unordered_map<int, std::string> &input_nodes_,
                     const std::unordered_map<int, std::string> &output_nodes_,
                     const std::vector<Repr> &repr_list_,
                     const std::vector<std::unordered_set<int>> &depends_list,
                     const std::vector<int> &id_mapping) : input_nodes(input_nodes_), repr_list(repr_list_) {
        std::vector<int> depths(repr_list_.size(), 1<<30);
        std::vector<std::tuple<int, int>> stack;
        
        for (auto &&[key, value] : input_nodes_) {
            repr_list[key] = _repr(value);
        }
        for (size_t i = 0; i < id_mapping.size(); i++) {
            if ((int)i != id_mapping[i]) {
                repr_list[i] = repr_list[id_mapping[i]];
            }
        }

        for (auto &&[key, value] : output_nodes_) {
            output_nodes[id_mapping[key]] = value;
            stack.emplace_back(0, id_mapping[key]);
        }

        while (stack.size()) {
            auto [depth, key] = stack.back();
            depths[key] = depth;

            stack.pop_back();

            if (input_nodes.find(key) != input_nodes.end()) {
                continue;
            }

            depth += 1;
            for (auto &&dkey_ : depends_list[key]) {
                int dkey = id_mapping[dkey_];
                if (depths[dkey] <= depth) {
                    continue;
                }
                stack.emplace_back(depth, dkey);
            }
        }
        std::vector<std::tuple<int, int>> test_order;
        for (size_t i = 0; i < depths.size(); i++) {
            if (depths[i] == 1<<30) {
                continue;
            }
            if (input_nodes.find(i) != input_nodes.end()) {
                continue;
            }
            test_order.emplace_back(depths[i], i);
        }
        std::sort(test_order.begin(), test_order.end());
        for (auto &&[depth, key] : test_order) {
            if (output_nodes.find(key) == output_nodes.end()) {
                output_order.emplace_back(tvar_type, tvar + std::to_string(key), key);
            } else {
                output_order.emplace_back("", output_nodes[key], key);
            }
        }
        std::reverse(output_order.begin(), output_order.end());
    }

    friend std::ostream &operator<<(std::ostream &os, const CalculationGraph &g) {
        std::vector<Repr> cur_repr_list = g.repr_list;
        for (auto &&[dtype, dvar, key] : g.output_order) {
            std::string dv = dtype == "" ? dvar : dtype + " " + dvar;
            os << g.line_prefix << dv << " = " << cur_repr_list[key](cur_repr_list) << ";" << std::endl;
            cur_repr_list[key] = _repr(dvar);
        }
        return os;
    }

 protected:
    std::string tvar_type{"IntermediateScalar"}, tvar{"_t"}, line_prefix{""};
    std::unordered_map<int, std::string> input_nodes, output_nodes;
    std::vector<Repr> repr_list;
    std::vector<std::tuple<std::string, std::string, int>> output_order;
};

}  // namespace sym

#endif  // CALCULATION_GRAPH_HPP_
