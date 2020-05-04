#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char*, char*);

int main(int argc, char *argv[]) {
	int i;
	if (argc < 2) {
		fprintf(2, "find: invalid number of arguments\n");
		exit(1);
	}
	char* path = argv[1];
	for (i = 2; i < argc; i++) {
		find(path, argv[i]);
	}
	exit(0);
}

void find(char* path, char* query) {
	char buf[512], *p;
	int fd;
	struct dirent de;
	struct stat st;

	if ((fd = open(path, 0)) < 0) {
		fprintf(2, "find: cannot open %s\n", path);
		return;
	}
	if (fstat(fd, &st) < 0) {
		fprintf(2, "find: cannot stat %s\n", path);
		close(fd);
		return;
	}

	switch (st.type) {
		case T_FILE:	
		    printf("%s\n", fmtname(path));
		    break;
		case T_DIR:
	}
	close(fd);
}