#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>
#include <pwd.h>
#include <shadow.h>
#include <crypt.h>

#define APPLICATION_NAME "uexec"

void usage() {
	printf("Usage: %s USERNAME PASSWORD COMMAND [PARAMETER [...]]\n", APPLICATION_NAME);
}

int check_pass(const char *user, const char *pass) {
	struct spwd *shadow;
	char *crypted;
	int result;

	if ((shadow = getspnam(user)) == NULL)
		return 0;

	crypted = crypt(pass, shadow->sp_pwdp);
	result = strcmp(crypted, shadow->sp_pwdp) == 0;
    free(crypted);

	return result;
}

int main(int argc, char *argv[]) {
	if (argc <= 3) {
		usage();
		exit(2);
	}

	int i;
	int arg = 3;
	char *user = argv[1];
	char *pass = argv[2];
	char *acmd = argv[3];
	char *cmd;
	struct passwd *p;

	if (atoi(user) > 0 && (p = getpwuid(atoi(user))) != NULL) {
		user = p->pw_name;
	}

	if (!check_pass(user, pass)) {
		openlog(APPLICATION_NAME, LOG_PID, LOG_AUTH);
		syslog(LOG_WARNING, "Incorrect password for '%s'", user);
		closelog();

		sleep(2);
		fputs("Login incorrect\n", stderr);
		exit(1);
	}

	for (i = 0; arg < argc; i ++) {
		if (acmd[i] == '\0') {
			if (arg < argc - 1)
				acmd[i] = ' ';
			arg ++;
		}
	}

	cmd = (char*)malloc(strlen(user) + i + 12);
	sprintf(cmd, "sudo -u %s -- %s", user, acmd);
	i = system(cmd);
	free(cmd);

	return i;
}
