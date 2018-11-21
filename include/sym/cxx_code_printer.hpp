/**
 * Copyright 
 * @file cxx_code_printer.hpp
 * @brief
 * @author Shogo Sawai
 * @date 2018-11-21 18:41:01
 */
#ifndef CXX_CODE_PRINTER_HPP_
#define CXX_CODE_PRINTER_HPP_

#include <ostream>
#include <vector>

namespace sym {

class CxxCodePrinter {
 public:
    class CxxFunction {
     public:
        CxxFunction() {}
        
        friend std::ostream &operator<<(std::ostream &os, const CxxFunction &f) {
            if (f.name == "") {
                return os;
            }
            std::string line_prefix = "    ";
            os << line_prefix << f.type << (f.type == "" ? "" : " ") << f.name << "(" << f.args << ") "
               << (f.initials == "" ? "" : ": ") << f.initials << (f.initials == "" ? "" : " ") << "{" << std::endl;
            os << f.contents;
            os << line_prefix << "}" << std::endl;
            return os;
        }
     
        std::string type, name, args, contents, initials;
    };

 public:
    CxxCodePrinter(const std::string &ns_, const std::string &class_name_) : ns(ns_), class_name(class_name_), function_list(2) {
    }

    void setStaticVariables(const std::vector<std::string> &variables, int num_intermediates, const std::string &refresh_contents) {
        constructor.name = class_name;
        constructor.args = "";
        constructor.initials = "";
        members.clear();
        for (auto &&s : variables) {
            constructor.args += std::string(constructor.args == "" ? "" : ", ") + std::string("ProbeScalar *" + s + "_");
            constructor.initials += (constructor.initials == "" ? "" : ", ") + s + "(" + s + "_)";
            members.push_back("ProbeScalar *" + s);
        }
        if (num_intermediates > 0) {
            members.push_back("IntermediateScalar _i[" + std::to_string(num_intermediates) + "]");
        }
        constructor.contents = "        refresh();\n";

        auto &f = function_list[0];
        f.type = "void";
        f.name = "refresh";
        f.args = "";
        f.contents = refresh_contents;
    }

    void setDynamicVariables(const std::vector<std::string> &variables, const std::string &contents) {
        auto &f = function_list[1];
        f.type = "void";
        f.name = "operator()";
        f.args = "";
        for (auto &&s : variables) {
            f.args += std::string(f.args == "" ? "" : ", ") + std::string("ProbeScalar *" + s);
        }
        f.contents = contents;
    }
    
    friend std::ostream &operator<<(std::ostream &os, const CxxCodePrinter &g) {
        os << "namespace " << g.ns << " {" << std::endl;
        os << "template<class ProbeScalar = double, class IntermediateScalar = double>" << std::endl;
        os << "class " << g.class_name << " {" << std::endl;
        os << " public:" << std::endl;
        os << g.constructor << std::endl;
        for (auto &&f : g.function_list) {
            os << f << std::endl;
        }
        for (auto &&m : g.members) {
            os << "    " << m << ";" << std::endl;
        }
        os << "};" << std::endl;
        os << "}  // namespace " << g.ns << std::endl;
        return os;
    }

 protected:
    std::string ns, class_name;
    CxxFunction constructor;
    std::vector<CxxFunction> function_list;
    std::vector<std::string> members;
};

}  // namespace sym

#endif  // CXX_CODE_PRINTER_HPP_
