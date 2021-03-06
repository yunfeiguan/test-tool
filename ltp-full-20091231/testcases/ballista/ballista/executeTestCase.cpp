// executeTestCase.cpp: executes the Ballista test case  
// Copyright (C) 1998-2001  Carnegie Mellon University
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include "executeTestCase.h"
#include <stdio.h>
#include <iostream>
#include <signal.h>
#include "unmarshal.h"

/* This prevents mangling of execute_test_case for dynamic loading */
#ifdef __cplusplus
extern "C" {
#endif

void setup_catch_signal (int signal_number)
{

   FILE* logFile = NULL;

   if ((logFile = fopen ("/tmp/kobeyTest.txt","a+")) == NULL)
   {
     cout << "CAUGHT SETUP SIGNAL" << endl;
   }
   fprintf(logFile, "caught setup HANDLE signal\n");
   fclose(logFile);

   //sleep(60);   
   exit(99);
}

int execute_test_case (char *marshaled_parameters)
{
  char *se;
  int ie;
  errno=0;
  void **ref[MAXP];      //dereferenced pointer for return
  int unmarshal_return;  // return value of unmarshal()
  struct sigaction setup_action;

  MARSHAL_DATA_TYPE params; //dial parameter setting array
  
  unmarshal_return = unmarshal(params , marshaled_parameters);

  if (unmarshal_return != 0) {
    return unmarshal_return;
  }

//inlining variable instantiations
int	rval;
b_ptr_sigset_t param1;
b_ptr_siginfo_t param2;
b_ptr_timespec param3;
b_ptr_timespec param4;

  
#ifdef     B_HANDLE_EXCEPTIONS
  try{
#endif //  B_HANDLE_EXCEPTIONS


  //sigaction added to differentiate between setup robustness failures and 
  //robustness failures related to the mut
  sigemptyset(&setup_action.sa_mask);
  setup_action.sa_handler = setup_catch_signal;  //setup_catch_signal is the signal handler
  (void) sigaction (SIGINT, &setup_action, NULL);
  (void) sigaction (SIGQUIT, &setup_action, NULL);
  (void) sigaction (SIGTERM, &setup_action, NULL);
  (void) sigaction (SIGABRT, &setup_action, NULL);
  (void) sigaction (SIGBUS, &setup_action, NULL);
  (void) sigaction (SIGSEGV, &setup_action, NULL);

//inlining userSetup, callInclude, and userShutdown

#ifdef     B_HANDLE_EXCEPTIONS
  }
  catch (char *se)
    {
      cerr<<"Exception String: "<<se<<endl;
    }
  catch (int ie)
    {
      cerr<<"Exception Integer: "<<ie<<endl;
    }      
  catch (...)
    {
      cerr<<"Caught unknown exception at lowest level.\n";
    }
#endif //  B_HANDLE_EXCEPTIONS

  return (errno);

}

#ifdef __cplusplus
}
#endif

