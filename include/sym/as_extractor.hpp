/**
 * Copyright 
 * @file as_extractor.hpp
 * @brief
 * @author Shogo Sawai
 * @date 2018-11-22 15:46:58
 */
#ifndef AS_EXTRACTOR_HPP_
#define AS_EXTRACTOR_HPP_

#include "function.hpp"
#include "unary_function.hpp"
#include "binary_function.hpp"

namespace sym {

class ASExtractor {
 public:
    void addAdd(Symbol s) {
        if (s->is<AddFunction>()) {
            std::shared_ptr<AddFunction> p = s->ptr<AddFunction>();
            addAdd(p->arg0);
            addAdd(p->arg1);
        } else if (s->is<SubFunction>()) {
            std::shared_ptr<SubFunction> p = s->ptr<SubFunction>();
            addAdd(p->arg0);
            addSub(p->arg1);
        } else if (s->is<NegFunction>()) {
            std::shared_ptr<NegFunction> p = s->ptr<NegFunction>();
            addSub(p->arg);
        } else {
            positives.push_back(s);
        }
    }

    void addSub(Symbol s) {
        if (s->is<AddFunction>()) {
            std::shared_ptr<AddFunction> p = s->ptr<AddFunction>();
            addSub(p->arg0);
            addSub(p->arg1);
        } else if (s->is<SubFunction>()) {
            std::shared_ptr<SubFunction> p = s->ptr<SubFunction>();
            addSub(p->arg0);
            addAdd(p->arg1);
        } else if (s->is<NegFunction>()) {
            std::shared_ptr<NegFunction> p = s->ptr<NegFunction>();
            addAdd(p->arg);
        } else {
            negatives.push_back(s);
        }
    }

    bool camBeSimplified() const {
        for (auto &&p : positives) {
            for (auto &&n : negatives) {
                if (p == n) {
                    return true;
                }
            }
        }
        return false;
    }

    Symbol simplified() const {
        Symbol s(0);
        std::vector<bool> used(negatives.size(), false);
        for (auto &&p : positives) {
            bool found = false;
            for (size_t i = 0; i < negatives.size(); i++) {
                if (used[i]) {
                    continue;
                }
                if (p == negatives[i]) {
                    used[i] = true;
                    found = true;
                    break;
                }
            }
            if (found) {
                continue;
            }
            s += p;
        }
        for (size_t i = 0; i < negatives.size(); i++) {
            if (used[i]) {
                continue;
            }
            s -= negatives[i];
        }
        return s;
    }

    std::vector<Symbol> positives, negatives;
};

}  // namespace sym


#endif  // AS_EXTRACTOR_HPP_
