#include "action.hh"

ActionInitialization::ActionInitialization() {}

ActionInitialization::~ActionInitialization() {}

void ActionInitialization::Build() const {
    auto *generator = new PrimaryGenerator();
    SetUserAction(generator);
}