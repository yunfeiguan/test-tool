#!/bin/bash
#usage ./test_controllers.sh
##################################################################################
#  Copyright (c) International Business Machines  Corp., 2007                    #
#                                                                                #
#  This program is free software;  you can redistribute it and/or modify         #
#  it under the terms of the GNU General Public License as published by          #
#  the Free Software Foundation; either version 2 of the License, or             #
#  (at your option) any later version.                                           #
#                                                                                #
#  This program is distributed in the hope that it will be useful,               #
#  but WITHOUT ANY WARRANTY;  without even the implied warranty of               #
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See                     #
#  the GNU General Public License for more details.                              #
#                                                                                #
#  You should have received a copy of the GNU General Public License             #
#  along with this program;  if not, write to the Free Software                  #
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA       #
#                                                                                #
##################################################################################
# Name Of File: test_controllers.sh                                              #
#                                                                                #
# Description:                                                                   #
#               This file runs the tests for resource management ie to test cpu  #
#               controller and memory controller. (for now cpu controller only)  #
#                                                                                #
# Author:       Sudhir Kumar    <sudhirkumarmalik@In.ibm.com>                    #
#                                                                                #
# History:                                                                       #
#                                                                                #
#  DATE        NAME            EMAIL                         DESC                #
#                                                                                #
#  20/12/07  Sudhir Kumar <sudhirkumarmalik@in.ibm.com>   Created this test      #
#  02/03/09  Miao Xie     <miaox@cn.fujitsu.com>          Add cpuset testset     #
#  07/07/09  Shi Weihua   <shiwh@cn.fujitsu.com>      Add cpu testset of Fujitsu #
#                                                                                #
##################################################################################

if [ -f /proc/cgroups ]
then
	CPU_CONTROLLER=`grep -w cpu /proc/cgroups | cut -f1`;
	MEM_CONTROLLER=`grep -w memory /proc/cgroups | cut -f1`;
	IOTHROTTLE_CONTROLLER=`grep -w blockio /proc/cgroups | cut -f1`;
	FREEZER=`grep -w freezer /proc/cgroups | cut -f1`;
	CPUSET_CONTROLLER=`grep -w cpuset /proc/cgroups | cut -f1`
	CPUACCOUNT_CONTROLLER=`grep -w cpuacct /proc/cgroups | cut -f1`

	if [ "$CPU_CONTROLLER" = "cpu" ]
	then
		$LTPROOT/testcases/bin/run_cpuctl_test.sh 1;
		$LTPROOT/testcases/bin/run_cpuctl_test.sh 3;
		$LTPROOT/testcases/bin/run_cpuctl_test.sh 4;
		$LTPROOT/testcases/bin/run_cpuctl_test.sh 5;
		$LTPROOT/testcases/bin/run_cpuctl_stress_test.sh 6;
		$LTPROOT/testcases/bin/run_cpuctl_stress_test.sh 7;
		$LTPROOT/testcases/bin/run_cpuctl_stress_test.sh 8;
		$LTPROOT/testcases/bin/run_cpuctl_stress_test.sh 9;
		$LTPROOT/testcases/bin/run_cpuctl_stress_test.sh 10;
		# Add the latency testcase to be run
		$LTPROOT/testcases/bin/run_cpuctl_latency_test.sh 1;
		$LTPROOT/testcases/bin/run_cpuctl_latency_test.sh 2;
		# Add the testcases from Fujitsu
		$LTPROOT/testcases/bin/run_cpuctl_test_fj.sh
	else
		echo "CONTROLLERS TESTCASES: WARNING";
		echo "Kernel does not support for cpu controller";
		echo "Skipping all cpu controller testcases....";
	fi;

	if [ "$MEM_CONTROLLER" = "memory" ]
	then
		$LTPROOT/testcases/bin/run_memctl_test.sh 1;
		$LTPROOT/testcases/bin/run_memctl_test.sh 2;
		$LTPROOT/testcases/bin/run_memctl_test.sh 3;
		$LTPROOT/testcases/bin/run_memctl_test.sh 4;
	else
		echo "CONTROLLERS TESTCASES: WARNING";
		echo "Kernel does not support for memory controller";
		echo "Skipping all memory controller testcases....";
	fi

	if [ "$IOTHROTTLE_CONTROLLER" = "blockio" ]
	then
		$LTPROOT/testcases/bin/run_io_throttle_test.sh;
	else
		echo "CONTROLLERS TESTCASES: WARNING";
		echo "Kernel does not support blockio controller";
		echo "Skipping all block device I/O throttling testcases....";
	fi

	if [ "$FREEZER" = "freezer" ]
	then
		"$LTPROOT/testcases/bin/run_freezer.sh"
	else
		echo "CONTROLLERS TESTCASES: WARNING";
		echo "Kernel does not support freezer controller";
		echo "Skipping all freezer testcases....";
	fi
	if [ "$CPUSET_CONTROLLER" = "cpuset" ]
	then
		$LTPROOT/testcases/bin/run_cpuset_test.sh 1;
		$LTPROOT/testcases/bin/run_cpuset_test.sh 2;
		$LTPROOT/testcases/bin/run_cpuset_test.sh 3;
		$LTPROOT/testcases/bin/run_cpuset_test.sh 4;
		$LTPROOT/testcases/bin/run_cpuset_test.sh 5;
		$LTPROOT/testcases/bin/run_cpuset_test.sh 6;
		$LTPROOT/testcases/bin/run_cpuset_test.sh 7;
		$LTPROOT/testcases/bin/run_cpuset_test.sh 8;
		$LTPROOT/testcases/bin/run_cpuset_test.sh 9;
		$LTPROOT/testcases/bin/run_cpuset_test.sh 10;
		$LTPROOT/testcases/bin/run_cpuset_test.sh 11;
	else
		echo "CONTROLLERS TESTCASES: WARNING";
		echo "Kernel does not support cpuset controller";
		echo "Skipping all cpuset controller testcases....";
	fi
	if [ "$CPUACCOUNT_CONTROLLER" = "cpuacct" ]
        then
                $LTPROOT/testcases/bin/run_cpuacct_test.sh 1;
                $LTPROOT/testcases/bin/run_cpuacct_test.sh 2;
        else
                echo "Could not start cpu accounting controller test";
                echo "usage: run_cpuacct_test.sh $TEST_NUM ";
                echo "Skipping the cpu accounting controller test...";
        fi
else
	echo "CONTROLLERS TESTCASES: WARNING"
	echo "Kernel does not support for control groups";
	echo "Skipping all controllers testcases....";
fi

exit 0;
