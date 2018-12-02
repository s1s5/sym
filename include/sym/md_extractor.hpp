/**
 * Copyright Shogo Sawai
 * @file md_extractor.hpp
 * @brief
 * @author Shogo Sawai
 * @date 2018-12-01 21:59:24
 */
#ifndef MD_EXTRACTOR_HPP_
#define MD_EXTRACTOR_HPP_

#include "function.hpp"
#include "unary_function.hpp"
#include "binary_function.hpp"

namespace sym {

class MDExtractor {
 public:
    void addMul(Symbol s) {
        if (s->is<MulFunction>()) {
            std::shared_ptr<MulFunction> p = s->ptr<MulFunction>();
            addMul(p->arg0);
            addMul(p->arg1);
        } else if (s->is<DivFunction>()) {
            std::shared_ptr<DivFunction> p = s->ptr<DivFunction>();
            addDiv(p->arg1);
        } else if (s->is<NegFunction>()) {
            is_negative = not is_negative;
            std::shared_ptr<NegFunction> p = s->ptr<NegFunction>();
            addMul(p->arg);
        } else if (s->is<Constant>()) {
            constants.push_back(s);
        } else {
            mul_terms.push_back(s);
        }
    }

    void addDiv(Symbol s) {
        if (s->is<MulFunction>()) {
            std::shared_ptr<MulFunction> p = s->ptr<MulFunction>();
            addDiv(p->arg0);
            addDiv(p->arg1);
        } else if (s->is<DivFunction>()) {
            std::shared_ptr<DivFunction> p = s->ptr<DivFunction>();
            addMul(p->arg1);
        } else if (s->is<NegFunction>()) {
            is_negative = not is_negative;
            std::shared_ptr<NegFunction> p = s->ptr<NegFunction>();
            addDiv(p->arg);
        } else if (s->is<Constant>()) {
            inv_constants.push_back(s);
        } else {
            div_terms.push_back(s);
        }
    }

    bool canBeSimplified() const {
        for (auto &&p : mul_terms) {
            for (auto &&n : div_terms) {
                if (p == n) {
                    return true;
                }
            }
        }
        return (constants.size() + inv_constants.size()) > 1;
    }

    Symbol simplified() const {
        std::vector<bool> used(div_terms.size(), false);
        double v = is_negative ? -1 : 1;
        for (auto &&c : constants) {
            v *= c.eval();
        }
        for (auto &&c : inv_constants) {
            v /= c.eval();
        }
        Symbol s(v), ds(1);
        for (auto &&p : mul_terms) {
            bool found = false;
            for (size_t i = 0; i < div_terms.size(); i++) {
                if (used[i]) {
                    continue;
                }
                if (p == div_terms[i]) {
                    used[i] = true;
                    found = true;
                    break;
                }
            }
            if (found) {
                continue;
            }
            s *= p;
        }
        for (size_t i = 0; i < div_terms.size(); i++) {
            if (used[i]) {
                continue;
            }
            ds *= div_terms[i];
        }
        return s / ds;
    }


    bool is_negative = false;
    std::vector<Symbol> mul_terms, div_terms;
    std::vector<Symbol> constants, inv_constants;
};

}  // namespace sym


#endif  // MD_EXTRACTOR_HPP_
