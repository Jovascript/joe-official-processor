//
// Created by Joseph on 08/10/2018.
//

#include <utility>
#include "compilation_helpers.h"

CompilerError::CompilerError(std::string message) : message_(std::move(message)) {}


AddressAllocation::AddressAllocation(const std::string &label, bool resolved, jop::dtype address) : label(label),
                                                                                                       resolved(
                                                                                                               resolved),
                                                                                                       address(address) {}
