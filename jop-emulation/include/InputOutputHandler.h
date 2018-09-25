//
// Created by Joseph on 12/09/2018.
//

#ifndef JOP_INPUTOUTPUTHANDLER_H
#define JOP_INPUTOUTPUTHANDLER_H

#include "proc_types.h"

namespace jop {

    class InputOutputHandler {
    public:
        virtual dtype handle_input() = 0;

        virtual void handle_output(dtype x) = 0;
    };

    class StandardIOHandler : public InputOutputHandler {
        dtype handle_input() override;

        void handle_output(dtype x) override;

    };

}
#endif //JOP_INPUTOUTPUTHANDLER_H
