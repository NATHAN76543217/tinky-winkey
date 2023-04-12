#ifndef SERVICE_HPP
# define SERVICE_HPP

# include <iostream>
# include <string>

#define SERVICE_NAME  _T("My Sample Service")    

class Service
{

	public:

		Service();
		Service( Service const & src );
		~Service();

		void WINAPI ServiceMain (DWORD argc, LPTSR *argv);
		void WINAPI ServiceCtrlHandler (DWORD);
		DWORD WINAPI ServiceWorkerThread (LPVOID lpParam);

	private:
		SERVICE_STATUS			ServiceStatus;
		SERVICE_STATUS_HANDLE	StatusHandle;
		HANDLE					ServiceStopEvent;


};

#endif /* ********************************************************* SERVICE_H */