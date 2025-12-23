#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/netlink.h>
#include <sys/socket.h>
#include <sys/stat.h>

#define BUFFER_SIZE 4096

int main() {
    int sock;
    struct sockaddr_nl addr;
    char buffer[BUFFER_SIZE];
    char file_path[1024];

    // --- USER INPUT ---
    printf("Enter full path of file to protect: ");
    if (!fgets(file_path, sizeof(file_path), stdin)) {
        perror("fgets");
        return -1;
    }
    // Remove trailing newline
    file_path[strcspn(file_path, "\n")] = '\0';

    // Initial protection
    if (chmod(file_path, 0) < 0) {
        perror("chmod initial");
        return -1;
    }

    // Create Netlink socket
    sock = socket(AF_NETLINK, SOCK_RAW, NETLINK_KOBJECT_UEVENT);
    if (sock < 0) {
        perror("socket");
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.nl_family = AF_NETLINK;
    addr.nl_groups = -1; // listen to all kernel uevents

    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return -1;
    }

    printf("\nListening for kernel device events...\n");

    while (1) {
        int len = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (len < 0) {
            perror("recv");
            continue;
        }
        buffer[len] = '\0';

        // --- FILTER RESULTS HERE ---
        // Filter for USB block devices only
        // Keywords: "block" (device type) and "sdb" (USB device)
        if (strstr(buffer, "block") && strstr(buffer, "sdb")) {

            // --- ACTION PERFORMING ---
            // Detect add/remove events
            char *action = strtok(buffer, "@"); // first token before '@'
            if (action) {
                printf("\n--- Kernel Event ---\n");
                printf("%s\n", buffer);             // full uevent path
                printf("Block device %s\n", action); // show add/remove

                // Change file permissions based on USB event
                if (strcmp(action, "add") == 0) {
                    if (chmod(file_path, 0600) == 0)
                        printf("File %s permissions set to 600 (access allowed)\n", file_path);
                    else
                        perror("chmod add");
                } else if (strcmp(action, "remove") == 0) {
                    if (chmod(file_path, 0000) == 0)
                        printf("File %s permissions set to 000 (access denied)\n", file_path);
                    else
                        perror("chmod remove");
                }
            }
        }
    }

    close(sock);
    return 0;
}
