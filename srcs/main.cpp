# include "Service.hpp"
# include <stdlib.h>

int main(int ac, char **av)
{
	    SERVICE_TABLE_ENTRY ServiceTable[] = 
    {
        {SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION) ServiceMain},
        {NULL, NULL}
    };
 
    if (StartServiceCtrlDispatcher (ServiceTable) == FALSE)
    {
        return GetLastError ();
    }
 
    return 0;
    return 0;
}
