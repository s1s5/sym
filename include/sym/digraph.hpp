/**
 * Copyright 
 * @file digraph.hpp
 * @brief
 * @author Shogo Sawai
 * @date 2018-11-22 10:03:39
 */
#ifndef DIGRAPH_HPP_
#define DIGRAPH_HPP_

#include "factory_base.hpp"

namespace sym {
    class Digraph {
     public:
        Digraph(const std::vector<Repr> &rev_repr_map_, const std::vector<std::unordered_set<int>> &depends_list_,
                const std::vector<int> &id_mapping_) : rev_repr_map(rev_repr_map_), depends_list(depends_list_), id_mapping(id_mapping_) {
        }

        void setLabel(int key, const std::string &label) {
            labels[key] = label;
        }
        void setShape(int key, const std::string &shape) {
            shapes[key] = shape;
        }

        friend std::ostream &operator<<(std::ostream &os, const Digraph &d) {
            std::vector<Repr> cur_rev_repr_map = d.rev_repr_map;
            os << "digraph graphname {" << std::endl;
            for (size_t i = 0; i < d.rev_repr_map.size(); i++) {
                std::string result = cur_rev_repr_map[i](cur_rev_repr_map);
                if (d.labels.find(i) != d.labels.end()) {
                    result = d.labels.find(i)->second;
                }
                os << "    n" << i << " [label=\"" << i << "=" << result << "\"";
                if (d.shapes.find(i) != d.shapes.end()) {
                    os << " shape=" << d.shapes.find(i)->second;
                }
                os << "];" << std::endl;
                cur_rev_repr_map[i] = _repr(std::to_string(i));
            }
            for (size_t i = 0; i < d.depends_list.size(); i++) {
                for (auto &&j : d.depends_list[i]) {
                    os << "    n" << j << " -> n" << i << ";" << std::endl;
                }
            }
            return os << "}" << std::endl;
        }
        
     protected:
        std::unordered_map<int, std::string> labels;
        std::unordered_map<int, std::string> shapes;
        std::vector<Repr> rev_repr_map;
        std::vector<std::unordered_set<int>> depends_list;
        std::vector<int> id_mapping;
    };

} // namespace sym

#endif  // DIGRAPH_HPP_
