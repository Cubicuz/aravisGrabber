#pragma once
#include <arv.h>

namespace aravisgrabber {

// interface for classes handling persistency
class BufferReceiver
{
public:
    virtual void receive(ArvBuffer* buffer) = 0;
};

}