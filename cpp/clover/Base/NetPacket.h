#pragma once

#include "Common.h"
#include "ByteBuffer.h"


class NetPacket
{
public:
    
    NetPacket(uint16 opcode, size_t res=256): m_opcode(opcode), m_buf(res) { }
    NetPacket(size_t res) : m_opcode(0), m_buf(res) { }
    NetPacket(const NetPacket& packet) : m_opcode(packet.m_opcode), m_buf(packet.m_buf) { }

    ~NetPacket() {}

    void Initialize(uint16 opcode, size_t newres = 256)
    {
        m_buf.Clear();
        m_buf.Reserve(newres);
        m_opcode = opcode;
    }

    const uint8* Contents()
    {
        return m_buf.Contents();
    }

    uint8* RawData()
    {
        return m_buf.RawData();
    }

    uint16 Opcode() const { return m_opcode; }
    void Opcode(uint16 opcode) { m_opcode = opcode; }

    size_t Size() const { return m_buf.Size(); }
    void Resize(size_t size) { m_buf.Resize(size);}

    void AppendTo(ByteBuffer& buf) const
    {
        buf << m_opcode;
        buf << (uint16)Size();
        buf.Append(m_buf);
    }

    template<class T> NetPacket & operator << (const T& value){ m_buf << value; return *this; }
    template<class T> NetPacket & operator >> (T& value) { m_buf >> value; return *this; }

private:

    uint16 m_opcode;
    ByteBuffer m_buf;

};