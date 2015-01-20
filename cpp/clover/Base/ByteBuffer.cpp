#include "ByteBuffer.h"

ByteBuffer & ByteBuffer::operator<<(const ::google::protobuf::Message & message)
{
    uint16 msgSize = message.ByteSize();
    if (msgSize > 1024 * 16)
    {
        throw std::runtime_error("Google Protobuf Message size is more than 16KB");
    }

    Append<uint16>((uint16)messageSize);

    if (msgSize > 0)
    {
        if (_storage.size() < _wpos + messageSize)
        {
            _storage.resize(_wpos + messageSize);
        }

        if (!message.SerializeToArray(&_storage[_wpos], messageSize))
        {
            throw ByteBufferException();
        }
        _wpos += messageSize;
    }

    return *this;
}

ByteBuffer & ByteBuffer::operator>>(::google::protobuf::Message & message)
{
    uint16 msgSize = Read<uint16>();
    if (_rpos + msgSize > _storage.size())
    {
        assert(false);
    }

    if (msgSize > 0)
    {
        if (!message.ParseFromArray(&_storage[_rpos], messageSize);)
        {
            assert(false);
        }
        assert(msgSize == message.ByteSize());

        _rpos += msgSize;
    }
    return *this;
}

void ByteBuffer::AppendPackGUID(uint64 guid)
{
    size_t mask_position = WPos();
    *this << uint8(0);
    for(uint8 i = 0; i < 8; i++)
    {
        if(guid & 0xFF)
        {
            _storage[mask_position] |= (1 << i);
            *this << ((uint8)(guid & 0xFF));
        }

        guid >>= 8;
    }
}

uint64 ByteBuffer::UnpackGUID()
{
    uint64 guid = 0;
    uint8 mask;
    uint8 temp;
    *this >> mask;
    for(uint8 i = 0; i < 8; ++i)
    {
        if(mask & (1 << i))
        {
            *this >> temp;
            guid |= uint64(temp << uint64(i << 3));
        }
    }
    return guid;
}

void ByteBuffer::HexDump()
{
    uint32 j = 1, k = 1;
    printf("STORAGE_SIZE: %u\n", (unsigned int)Size());
    for(uint32 i = 0; i < Size(); i++)
    {
        if((i == (j * 8)) && ((i != (k * 16))))
        {
            if(Read<uint8>(i) <= 0x0F)
            {
                printf("| 0%X ", Read<uint8>(i));
            }
            else
            {
                printf("| %X ", Read<uint8>(i));
            }

            j++;
        }
        else if(i == (k * 16))
        {
            RPos(RPos() - 16);  // move Read pointer 16 places back
            printf(" | ");    // write split char

            for(int x = 0; x < 16; x++)
            {
                printf("%c", Read<uint8>(i - 16 + x));
            }

            if(Read<uint8>(i) <= 0x0F)
            {
                printf("\n0%X ", Read<uint8>(i));
            }
            else
            {
                printf("\n%X ", Read<uint8>(i));
            }

            k++;
            j++;
        }
        else
        {
            if(Read<uint8>(i) <= 0x0F)
            {
                printf("0%X ", Read<uint8>(i));
            }
            else
            {
                printf("%X ", Read<uint8>(i));
            }
        }
    }
    printf("\n");
}