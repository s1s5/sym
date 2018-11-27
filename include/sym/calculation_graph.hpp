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

namespace sym {

class CalculationGraph {
 public:
    CalculationGraph(const std::unordered_map<int, std::string> &input_nodes_,
                     const std::unordered_map<std::string, int> &output_nodes_,
                     const std::vector<Repr> &repr_list_,
                     const std::vector<std::unordered_set<int>> &depends_list,
                     const std::vector<int> &id_mapping) : input_nodes(input_nodes_), repr_list(repr_list_) {
        std::vector<int> depths(repr_list_.size(), -1);
        std::vector<std::tuple<int, int>> stack;
        
        for (auto &&[key, value] : input_nodes_) {
            repr_list[key] = _repr(value);
        }
        for (size_t i = 0; i < id_mapping.size(); i++) {
            if ((int)i != id_mapping[i]) {
                repr_list[i] = repr_list[id_mapping[i]];
            }
        }

        for (auto &&[value, key] : output_nodes_) {
            // std::cout << "[output] key=" << id_mapping[key] << ", value = " << value << std::endl;
            output_nodes[value] = id_mapping[key];
            stack.emplace_back(0, id_mapping[key]);
        }

        std::vector<int> counts(repr_list_.size(), 0);
        for (auto &&dl : depends_list) {
            for (auto &&d : dl) {
                counts[d]++;
            }
        }

        while (stack.size()) {
            auto [depth, key] = stack.back();
            depths[key] = std::max<int>(depths[key], depth);

            stack.pop_back();

            depth += 1;
            for (auto &&dkey_ : depends_list[key]) {
                int dkey = id_mapping[dkey_];
                // std::cout << key << "(" << FactoryBase::repr(key) << ")" << " -> " << dkey << "(" << FactoryBase::repr(dkey) << ")" << std::endl;
                if (depths[dkey] >= depth) {
                    continue;
                }

                if (input_nodes.find(dkey) != input_nodes.end()) {
                    continue;
                }
                stack.emplace_back(depth, dkey);
            }
        }
        std::vector<std::tuple<int, int>> test_order;
        for (size_t i = 0; i < depths.size(); i++) {
            // std::cout << FactoryBase::repr(i) << ", key=" << i << ", depth=" << depths[i] << std::endl;
            if (depths[i] == -1) {
                continue;
            }
            // if (output_nodes.find(i) == output_nodes.end() and input_nodes.find(i) != input_nodes.end()) {
            if (input_nodes.find(i) != input_nodes.end()) {
                continue;
            }
            test_order.emplace_back(depths[i], i);
        }
        std::sort(test_order.begin(), test_order.end());
        for (auto &&[depth, key] : test_order) {
            if (FactoryBase::is<Constant>(key)) {
                continue;
            }
            // std::cout << "key=" << key << ", depth=" << depth << std::endl;
            // if (output_nodes.find(key) == output_nodes.end()) {
                // if (counts[key] <= 1) {
                //     continue;
                // }
                output_order.emplace_back(tvar_type, tvar + std::to_string(key), key);
            // } else {
            //     output_order.emplace_back("", output_nodes[key], key);
            // }
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
        for (auto &&[value, key] : g.output_nodes) {
            os << g.line_prefix << value << " = " << cur_repr_list[key](cur_repr_list) << ";" << std::endl;
        }
        return os;
    }

 protected:
    std::string tvar_type{"IntermediateScalar"}, tvar{"_t"}, line_prefix{"        "};
    std::unordered_map<int, std::string> input_nodes;
    std::unordered_map<std::string, int> output_nodes;
    std::vector<Repr> repr_list;
    std::vector<std::tuple<std::string, std::string, int>> output_order;
};

}  // namespace sym

#endif  // CALCULATION_GRAPH_HPP_
