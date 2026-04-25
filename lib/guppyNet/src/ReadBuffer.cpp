//
// Created by guppy on 4/19/2026.
//

#include "network/ReadBuffer.hpp"

namespace gp::network
{
    Packet ReadBuffer::collectPacket()
    {
        // TODO: error checking   update: is this done in the packet constructor? idk
        Packet packet(_header, _body);

        _headerLength = 0;
        _body.clear();

        return packet;
    }
}
