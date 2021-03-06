/*  (c) Copyright Hewlett-Packard Development Company, L.P., 2007
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of version 2 the GNU General Public License as
 *  published by the Free Software Foundation.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "includes.h"
#include <selinux/selinux.h>

int main(int argc, char **argv)
{
    int dir_fd;
    int exitval, result;

    if (argc < 3 || argc > 4) {
	fprintf(stderr, "Usage:\n%s <directory> <path> [context]\n", argv[0]);
	return TEST_ERROR;
    }

    dir_fd = open(argv[1], O_DIRECTORY);
    if (dir_fd < 0)
	    return TEST_ERROR;
    if (argc == 4 && setfscreatecon(argv[3]) < 0) {
	perror("do_mknodat: setfscreatecon");
	return TEST_ERROR;
    }

    errno = 0;
    exitval = mknodat(dir_fd, argv[2], S_IRWXU, S_IFBLK);
    result = exitval < 0;

    printf("%d %d %d\n", result, result ? errno : exitval, getpid());
    return result;
}
