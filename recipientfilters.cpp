#include "recipientfilters.h"

ChatFilter::ChatFilter(int rec)
{
    recipient = rec;
}
bool ChatFilter::IsReliable( void ) const
{
    return true;
}
bool ChatFilter::IsInitMessage( void ) const
{
    return false;
}

int ChatFilter::GetRecipientCount( void ) const
{
    return 1;
}
int ChatFilter::GetRecipientIndex( int slot ) const
{
    return  recipient;
}
