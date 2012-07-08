#ifndef OPERATORMODULE_H
#define OPERATORMODULE_H

#include "../Framework/Arithmetic/AbstractArithmetic.h"
#include "ParserModule.h"
#include "OperatorInterface.h"
#include <string>

class OperatorModule : public ParserModule
{
private:
    OperatorInterface interface;

public:
    OperatorModule(const ParserModule &parent_pkg, const OperatorInterface &interface) : ParserModule(parent_pkg), interface(interface) {};

    inline bool matches(std::string candidate) const { return interface.matches(candidate); };
    inline unsigned int priority() const { return interface.priority(); };
    inline std::unique_ptr<CAS::AbstractArithmetic> parse(std::unique_ptr<CAS::AbstractArithmetic> left, std::unique_ptr<CAS::AbstractArithmetic> right) const { return interface.parse(std::forward<std::unique_ptr<CAS::AbstractArithmetic>>(left), std::forward<std::unique_ptr<CAS::AbstractArithmetic>>(right)); };
};

#endif //OPERATORMODULE_H
