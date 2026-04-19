//
// Created by guppy on 4/19/2026.
//

#include "network/ReadBuffer.hpp"

namespace gp::network
{
    bool ReadBuffer::readData(const uint8_t* start, const uint32_t length)
    {
        uint32_t pos = 0;

        while (_headerLength < headerMaxLength) // header is not full, need to keep adding!
        {
            if (pos >= length) return false; // eof
            _header[_headerLength++] = *(start + pos++);
        }

        if (pos >= length) return false;

        // NOTE: assumes headers are full
        if (_body.size() < getTotalLength()) // basically if not all bytes in packet are stored, store bytes
        {
            const int bytesLeftToWrite = getTotalLength() - _body.size();
            const int bytesLeftToRead = length - pos;

            if (bytesLeftToRead < bytesLeftToWrite)
            {
                _body.insert(_body.end(), start + pos, start + length);
                return false;
            }

            // packet can be finished
            _body.insert(_body.end(), start + pos, start + pos + bytesLeftToWrite);
        }
        return true;
    }

    NetworkPacket ReadBuffer::collectPacket()
    {
        // TODO: error checking

        NetworkPacket packet = ParsePacket(getType(), getEventNameLength(), getBodyLength(), _body);

        _headerLength = 0;
        _body.clear();

        return packet;
    }
}
