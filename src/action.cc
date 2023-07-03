#include "action.hh"
#include "steppingAction.hh"

ActionInitialization::ActionInitialization() {}

ActionInitialization::~ActionInitialization() {}

void ActionInitialization::Build() const {
    auto *generator = new PrimaryGenerator();
    SetUserAction(generator);

    auto *steppingAction = new SteppingAction();
    steppingAction->file = std::ofstream(file);
    SetUserAction(steppingAction);
}