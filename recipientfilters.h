#ifndef RECIPIENT_FILTERS_H
#define RECIPIENT_FILTERS_H

#include <irecipientfilter.h>

class ChatFilter : public IRecipientFilter
{
    private:
        int recipient;
    public:
        ChatFilter(int rec);
        virtual bool    IsReliable( void ) const;
        virtual bool    IsInitMessage( void ) const;
        virtual int     GetRecipientCount( void ) const;
        virtual int     GetRecipientIndex( int slot ) const;
};

#endif
