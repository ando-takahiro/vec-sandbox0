#ifndef SYSTEM_TIMER_H
#define SYSTEM_TIMER_H


//This class is NOT threading timer!


#ifdef _WIN32
#	include <windows.h>
#	include <mmsystem.h>
#pragma comment( lib, "winmm.lib" )
#else
#if defined(__unix__) || defined(__APPLE__)
#		include <sys/time.h>
#	else
#		include <ctime>
#	endif
#endif
	
		
	class timer{
	public:
#ifdef _WIN32
		typedef DWORD time_t;
		
		timer() {::timeBeginPeriod( 1 );}
		~timer(){::timeEndPeriod(1);}
			
		void start(){t_[0] = ::timeGetTime();}
		void end()  {t_[1] = ::timeGetTime();}
				
		time_t sec() {return (time_t)( (t_[1]-t_[0]) / 1000 );}
		time_t msec(){return (time_t)( (t_[1]-t_[0]) );}
		time_t usec(){return (time_t)( (t_[1]-t_[0]) * 1000 );}
		
#else 
#if defined(__unix__) || defined(__APPLE__)
		typedef unsigned long int time_t;

		
		void start(){::gettimeofday(tv+0, &tz);}
		void end()  {::gettimeofday(tv+1, &tz);}
		
		time_t sec() {return (time_t)(tv[1].tv_sec-tv[0].tv_sec);}
		time_t msec(){return this->sec()*1000 + (time_t)((tv[1].tv_usec-tv[0].tv_usec)/1000);}
		time_t usec(){return this->sec()*1000000 + (time_t)(tv[1].tv_usec-tv[0].tv_usec);}

#else //C timer
		//using namespace std;
		typedef clock_t time_t;
		
		void start(){t_[0] = clock();}
		void end()  {t_[1] = clock();}
		
		time_t sec() {return (time_t)( (t_[1]-t_[0]) / CLOCKS_PER_SEC );}
		time_t msec(){return (time_t)( (t_[1]-t_[0]) * 1000 / CLOCKS_PER_SEC );}
		time_t usec(){return (time_t)( (t_[1]-t_[0]) * 1000000 / CLOCKS_PER_SEC );}
	
#endif
#endif
		
	private:
		
#ifdef _WIN32
		DWORD t_[2];
#else
#if defined(__unix__) || defined(__APPLE__)
		struct timeval tv[2];
		struct timezone tz;
#else
		time_t t_[2];
#endif
#endif
		
	};
	

#endif

