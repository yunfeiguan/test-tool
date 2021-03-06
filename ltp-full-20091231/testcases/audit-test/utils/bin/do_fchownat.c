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
#include <pwd.h>

int main(int argc, char **argv)
{
    int exitval, result;
    struct passwd *pw;
    int dir_fd;

    if (argc != 4) {
	fprintf(stderr, "Usage:\n%s <directory> <path> <owner>\n",
		argv[0]);
	return TEST_ERROR;
    }

    dir_fd = open(argv[1], O_DIRECTORY);
    if (dir_fd == -1) {
	perror("do_fchownat: open dir fd");
	return TEST_ERROR;
    }

    pw = getpwnam(argv[3]);
    if (!pw) {
	perror("do_fchownat: getpwnam");
	return TEST_ERROR;
    }

    errno = 0;
    exitval = fchownat(dir_fd, argv[2], pw->pw_uid, -1, 0);
    result = exitval < 0;

    printf("%d %d %d\n", result, result ? errno : exitval, getpid());
    return result;
}
