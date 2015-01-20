#pragma once

class DoubleBuffer
{
public:
 
    DoubleBuffer() 
        :m_inIdx(0)
    {
    }

    ~DoubleBuffer()
    {
    }

    ByteBuffer & InBuff()
    {
        return m_buffs[m_inIdx];
    }

    ByteBuffer & OutBuff()
    {
        return m_buffs[1-m_inIdx];
    }

    void SwapInOut()
    {
        m_inIdx = 1 - m_inIdx;
    }

private:
    int         m_inIdx; 
    ByteBuffer  m_buffs[2];
};