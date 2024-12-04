#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <backend/net.h>

int main(int argc, char *argv[]) {
	(void)argc;
	puts("WiiBot Node v" NODE_VER " is starting up...");
	// fork to start a new thread for handling the networking
	pid_t pid = fork();

	// head into the networking code if we are that thread
	if (pid == -1) {
		perror("fork");
		return 1;
	}

	else if (pid == 0) {
		startNet();
		return 0;
	}

	printf("Network thread started with pid %d\n", pid);

	// if we're the parent, chill out
	// we should monitor the network thread to make sure that it doesn't die
	// also, if we get a signal to quit (e.g. SIGINT, SIGTERM, etc),
	// make sure that we clean up the network thread too
	while (1) {
		sleep(1);

		// check if the network thread is still running
		int status;
		pid_t result = waitpid(pid, &status, WNOHANG);
		if (result == 0) {
			// network thread is still running
			continue;
		} else if (result == -1) {
			// error
			perror("waitpid");
			return 1;
		} else {
			// network thread is dead
			puts("\e[1;31mNetwork thread died.\e[0m");
			// restart ourselves
			execv(argv[0], argv);

			// exec failed, something is very wrong
			puts("\e[1;31mfailed to restart.\e[0m");
			return 1;
		}
	}

}
