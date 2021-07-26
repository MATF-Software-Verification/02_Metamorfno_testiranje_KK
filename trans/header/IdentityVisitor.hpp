#ifndef IDENTITYVISITOR_HPP
#define IDENTITYVISITOR_HPP

#include "MTKConsumer.hpp"

/*
 * Identitet transformacija.
 *
 */

class IdentityVisitor : public MTKVisitor<IdentityVisitor> {
public:
    IdentityVisitor(Rewriter& r, ASTContext& ast)
        : MTKVisitor(r, ast) {}

};

#endif // IDENTITYVISITOR_HPP
