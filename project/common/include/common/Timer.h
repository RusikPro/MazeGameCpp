#ifndef TIMER_H
#define TIMER_H

/*----------------------------------------------------------------------------*/

#include <chrono>
#include <iostream>
#include <string>

/*----------------------------------------------------------------------------*/

namespace {

/*----------------------------------------------------------------------------*/

template < typename _RatioT >
struct ratio_repr
{
    static constexpr auto value = "unknown";
};

/*----------------------------------------------------------------------------*/

template <>
struct ratio_repr< std::nano >
{
    static constexpr auto value = "nanoseconds";
};

/*----------------------------------------------------------------------------*/

template <>
struct ratio_repr< std::milli >
{
    static constexpr auto value = "milliseconds";
};

/*----------------------------------------------------------------------------*/

template <>
struct ratio_repr< std::micro >
{
    static constexpr auto value = "microseconds";
};

/*----------------------------------------------------------------------------*/

} // namespace

/*----------------------------------------------------------------------------*/

template < typename _RatioT = std::micro >
class Timer
{

public:

    using Clock = std::chrono::high_resolution_clock;
    using CurrentClock = decltype( std::chrono::high_resolution_clock::now() );

    Timer ( std::string const & _title, std::ostream & _os = std::cout )
        :   m_title( _title )
        ,   m_start( Clock::now() )
        ,   m_os( _os )
    {
    }

    ~Timer ()
    {
        std::chrono::duration< double, _RatioT > duration =
            Clock::now() - m_start
        ;
        m_os
            <<  m_title << " took " << duration.count() << " "
            <<  ratio_repr< _RatioT >::value
            <<  std::endl
        ;
    }

private:

    const std::string m_title;

    const CurrentClock m_start;

    std::ostream & m_os;
};

/*----------------------------------------------------------------------------*/

#endif  //  TIMER_H
