#include <gtk/gtk.h>
#include <sys/stat.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>
#include <unistd.h>

extern ssize_t readlink(const char *restrict path, char *restrict buf, size_t bufsize);
extern void maxwin(GtkWidget *window, gpointer erdp);

char* find_gladefile() {
	/*find where my executable is*/
	char path[PATH_MAX] = "\0";
	char *gladename = "/erdp.glade";
	size_t len;
	pid_t pid = getpid();
	snprintf(path, sizeof(path), "/proc/%d/exe", pid);
	if (readlink(path, path, PATH_MAX) == -1)
		perror("readlink");
	/*this is bad and hacky and I should feel bad*/
	len = strlen(path) - strlen("/erdp\0");
	for(;strncmp("/erdp", &path[len], strlen("/erdp")) != 0;len--){
		path[len+strlen("/erdp")-1] = '\0';
	}
	len = strlen(path);
	printf("Executable Path: %s\n", path);
	for(;strncmp("/erdp\0", &path[len], strlen("/erdp\0")) != 0;len--){
		path[len+strlen("/erdp\0")] = '\0';
	};

	/*if the gladefile exists in the same directory as my executable, use it*/
	if(access(g_strconcat(path, gladename, NULL), R_OK) != -1) {
		printf("Gladefile Path: %s%s\n", path, gladename);
		return g_strconcat(path, gladename, NULL);
	}

	/*if it doesn't exist there, check in my path plus share/ */
	if(access(g_strconcat(path, "/share", gladename, NULL), R_OK) != -1) {
		printf("Gladefile Path: %s/share%s\n", path, gladename);
		return g_strconcat(path, "/share", gladename, NULL);
	}

	/*if it doesn't exist there, check in /usr/share/erdp/ */
	if(access(g_strconcat("/usr/share/erdp", gladename, NULL), R_OK) != -1) {	
		printf("Gladefile Path: /usr/share/erdp%s\n", gladename);
		return g_strconcat("/usr/share/erdp", gladename, NULL);
	}

	/*maybe /usr/local/ */
	if(access(g_strconcat("/usr/local/share/erdp", gladename, NULL), R_OK) != -1) {
		printf("Gladefile Path: /usr/local/share/erdp%s\n", gladename);
		return g_strconcat("/usr/local/share/erdp", gladename, NULL);
	}

	/* I give up. Return NULL */
	return NULL;
}

