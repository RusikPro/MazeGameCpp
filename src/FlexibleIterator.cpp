#include "FlexibleIterator.h"

#include <iostream>

/*----------------------------------------------------------------------------*/

FlexibleIterator::FlexibleIterator ( int _size, bool _forward )
    :   m_current( _forward ? 0 : _size - 1 )
    ,   m_end( _forward ? _size : 0 )
    ,   m_step( _forward ? 1 : -1 )
{}

/*----------------------------------------------------------------------------*/

FlexibleIterator::FlexibleIterator ( int _start, int _end, bool _forward )
    :   m_current( _start )
    ,   m_end( _end )
    ,   m_step( _forward ? 1 : -1 )
{
}

/*----------------------------------------------------------------------------*/

bool FlexibleIterator::hasNext () const
{
    return m_step > 0 ? m_current < m_end : m_current >= m_end;
}

/*----------------------------------------------------------------------------*/

int FlexibleIterator::next ()
{
    int value = m_current;
    m_current += m_step;
    return value;
}

/*----------------------------------------------------------------------------*/
