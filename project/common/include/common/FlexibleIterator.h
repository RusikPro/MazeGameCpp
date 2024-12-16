#ifndef FLEXIBLEITERATOR_H
#define FLEXIBLEITERATOR_H

/*----------------------------------------------------------------------------*/

class FlexibleIterator
{
public:
    FlexibleIterator ( int _size, bool _forward );

    FlexibleIterator ( int _start, int _end, bool _forward );

    bool hasNext () const;

    int next ();

private:

    int m_current;
    int m_end;
    int m_step;
};

/*----------------------------------------------------------------------------*/

#endif // FLEXIBLEITERATOR_H
