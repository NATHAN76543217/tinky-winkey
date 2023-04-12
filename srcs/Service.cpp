#include "Service.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Service::Service() : ServiceStatus({0}), StatusHandle(NULL), ServiceStopEvent(INVALID_HANDLE_VALUE)
{
}

Service::Service( const Service & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Service::~Service()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/


/*
** --------------------------------- METHODS ----------------------------------
*/

void	WINAPI Service::ServiceMain( DWORD argc, LPSTR *argv)
{
	DWORD Status = E_FAIL;
 
    // Register our service control handler with the SCM
    this->StatusHandle = RegisterServiceCtrlHandler (SERVICE_NAME, this->ServiceCtrlHandler);
 
    if (this->StatusHandle == NULL) 
		return ;
 
    // Tell the service controller we are starting
    ZeroMemory (&(this->ServiceStatus), sizeof (this->ServiceStatus));
    this->ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    this->ServiceStatus.dwControlsAccepted = 0;
    this->ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    this->ServiceStatus.dwWin32ExitCode = 0;
    this->ServiceStatus.dwServiceSpecificExitCode = 0;
    this->ServiceStatus.dwCheckPoint = 0;
 
    if (SetServiceStatus (this->StatusHandle , &(this->ServiceStatus)) == FALSE)
    {
        OutputDebugString(_T(
          "My Sample Service: ServiceMain: SetServiceStatus returned error"));
    }
 
    /*
    	Perform tasks necessary to start the service here
    */

    // Create a service stop event to wait on later
    this->ServiceStopEvent = CreateEvent (NULL, TRUE, FALSE, NULL);
    if (this->ServiceStopEvent == NULL) 
    {   
        // Error creating event
        // Tell service controller we are stopped and exit
        g_ServiceStatus.dwControlsAccepted = 0;
        g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        g_ServiceStatus.dwWin32ExitCode = GetLastError();
        g_ServiceStatus.dwCheckPoint = 1;
 
        if (SetServiceStatus (this->StatusHandle, &(this->ServiceStatus)) == FALSE)
		{
	    	OutputDebugString(_T(
	    	  "My Sample Service: ServiceMain: SetServiceStatus returned error"));
		}
		return ;
	}    
    
    // Tell the service controller we are started
    this->ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    this->ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    this->ServiceStatus.dwWin32ExitCode = 0;
    this->ServiceStatus.dwCheckPoint = 0;
 
    if (SetServiceStatus (this->StatusHandle, &(this->ServiceStatus)) == FALSE)
    {
        OutputDebugString(_T(
          "My Sample Service: ServiceMain: SetServiceStatus returned error"));
    }
 
    // Start a thread that will perform the main task of the service
    HANDLE hThread = CreateThread (NULL, 0, this->ServiceWorkerThread, NULL, 0, NULL);
   
    // Wait until our worker thread exits signaling that the service needs to stop
    WaitForSingleObject (hThread, INFINITE);
   
    
    /*
     * Perform any cleanup tasks 
     */
 
    CloseHandle (this->ServiceStopEvent);
 
    // Tell the service controller we are stopped
    this->ServiceStatus.dwControlsAccepted = 0;
    this->ServiceStatus.dwCurrentState = SERVICE_STOPPED;
    this->ServiceStatus.dwWin32ExitCode = 0;
    this->ServiceStatus.dwCheckPoint = 3;
 
    if (SetServiceStatus (this->StatusHandle, &(this->ServiceStatus)) == FALSE)
    {
        OutputDebugString(_T(
          "My Sample Service: ServiceMain: SetServiceStatus returned error"));
    }
    
    return;
}


VOID WINAPI Service::ServiceCtrlHandler (DWORD CtrlCode)
{
    switch (CtrlCode) 
	{
    	case SERVICE_CONTROL_STOP :
 			if (this->ServiceStatus.dwCurrentState != SERVICE_RUNNING)
        break;
 
    /* 
     	Perform tasks necessary to stop the service here 
    */
        
        this->ServiceStatus.dwControlsAccepted = 0;
        this->ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
        this->ServiceStatus.dwWin32ExitCode = 0;
        this->ServiceStatus.dwCheckPoint = 4;
 
        if (SetServiceStatus (this->StatusHandle, &(this->ServiceStatus)) == FALSE)
        {
            OutputDebugString(_T(
              "My Sample Service: ServiceCtrlHandler: SetServiceStatus returned error"));
        }
 
        // This will signal the worker thread to start shutting down
        SetEvent (this->ServiceStopEvent);
 
        break;
 
     default:
         break;
    }
}  


DWORD WINAPI Service::ServiceWorkerThread (LPVOID lpParam)
{
    //  Periodically check if the service has been requested to stop
    while (WaitForSingleObject(this->ServiceStopEvent, 0) != WAIT_OBJECT_0)
    {        
    /* 
    	Perform main service function here
    */
 
        //  Simulate some work by sleeping
        Sleep(3000);
    }
 
    return ERROR_SUCCESS;
} 


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */