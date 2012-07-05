#include "ExpressionParser.h"
#include "DemoModules.h"

ExpressionParser::ExpressionParser()
{
    terminals.append(std::unique_ptr<AbstractTerminal>(new NumberExpression)); //placeholder for dynamic class loading

    operators.append(std::unique_ptr<AbstractOperator>(new AdditionExpression));
}

std::unique_ptr<CAS::AbstractArithmetic> ExpressionParser::parse(std::string input)
{
    int level;
    bool deleted;
    do {
        if (input.empty()) return nullptr;
        deleted = false;
        if (input.at(0) == '(' && input.at(input.length() - 1) == ')') {
            level = 0;
            for (const auto i : input) {
                if (i == '(') level++;
                else if (i == ')' && --level == -1) break;
            }
            if (level != -1) {
                input.erase(0, 1);
                input.erase(input.length() - 1, 1);
                deleted = true;
            }
        }
    } while (deleted);

    std::unique_ptr<CAS::AbstractArithmetic> result;
    for (const auto &terminal : modules.terminals) {
        result = terminal->parse(input);
        if (result) return result;
    }

    level = 0;
    unsigned int op_pos = 0;
    std::unique_ptr<AbstractOperator> best_op_match = nullptr;

    for (auto i = input.rbegin(); i != input.rend(); ++i) {
        if (*i == '(' || *i == '[' || *i == '{')  level--;
        else if (*i == ')' || *i == ']' || *i == '}') level++;
        else if (level == 0) {
            for (const auto &it_op : modules.operators) {
                if (it_op->matches(std::string(1, *i))) {
                    if (it_op->priority() == 0) return it_op->parse(parse(input.substr(0, input.rend() - i - 1)), parse(input.substr(input.rend() - i, i - input.rbegin())));
                    else if (best_op_match == nullptr || it_op->priority() < best_op_match->priority()){
                        best_op_match.reset(it_op.get());
                        op_pos = input.rend() - i - 1;
                    }
                }
            }
        }
    }
    if (best_op_match != nullptr)
        return best_op_match->parse(parse(input.substr(0, op_pos)), parse(input.substr(op_pos + 1, input.length() - op_pos - 1)));

    unsigned int pos_parenthesis = input.find_first_of('(');
    std::string identifier = input.substr(0, pos_parenthesis);
    for (const auto &it_func : modules.functions)
        if (it_func->matches(identifier)) return it_func->parse(parse(input.substr(pos_parenthesis + 1, input.length() - pos_parenthesis - 1)));
    return nullptr;
}