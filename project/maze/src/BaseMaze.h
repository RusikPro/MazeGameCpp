#ifndef BASEMAZE_H
#define BASEMAZE_H

/*----------------------------------------------------------------------------*/

#include "maze/IMaze.h"

/*----------------------------------------------------------------------------*/

namespace maze {

/*----------------------------------------------------------------------------*/

class BaseMaze : public IMaze
{
public:

    BaseMaze ( int _size );

    Grid const & getGrid () const override;
    int getSize () const override;
    const Room& getRoom ( int x, int y ) const override;

    void reset () override;
    void save ( std::string const & _filename ) const override;
    void load ( std::string const & _filename ) override;

private:

    void validateSize () const;

protected:

    int m_size;
    Grid m_grid;
};

/*----------------------------------------------------------------------------*/

} // namespace maze

/*----------------------------------------------------------------------------*/

#endif // BASEMAZE_H
