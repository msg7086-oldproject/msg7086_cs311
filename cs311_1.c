#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <malloc.h>

const char *opt_string = "ob:h?";

int main(int argc, char **argv)
{
	char *src_file, *dst_file;
	int src_fd, dst_fd;
	int buffer_length = 8192;
	bool overwrite = false;
	char opt;
	opt = getopt(argc, argv, opt_string);
	while (opt != -1) {
		switch (opt) {
			case 'b':
				buffer_length = atoi(optarg);
				break;

			case 'o':
				overwrite = true;
				break;

			case 'h':
			case '?':
				display_usage();
				return 0;
				break;

			default:
				break;
		}
		opt = getopt(argc, argv, opt_string);
	}

	if (optind > argc - 2) {
		display_usage();
		return -1;
	}
	src_file = argv[optind++];
	dst_file = argv[optind++];

	if (buffer_length == 0)
		buffer_length = 8192;

	/* Check if source file exists */
	src_fd = open(src_file, O_RDONLY);
	if (src_fd == -1) {
		puts("Unable to open source file");
		return -1;
	}

	/* Check if dest file exists */
	if (!overwrite) {
		dst_fd = open(dst_file, O_RDONLY);
		if (dst_fd != -1) {
			puts("Destination file exists. Specify -o to overwrite.");
			return -1;
		}
		close(dst_fd);
	}

	/* TODO: posix_fallocate */
	dst_fd = open(dst_file, O_CREAT | O_WRONLY | O_TRUNC, 0664);
	if (dst_fd == -1) {
		puts("Unable to open destination file");
		return -1;
	}

	int num_read = 0, num_write;
	char *buffer = (char *)malloc(buffer_length * sizeof(char));
	if (buffer == NULL) {
		puts("Unable to allocate memory");
		return -1;
	}
	while ((num_read = read(src_fd, buffer, buffer_length)) > 0) {
		num_write = write(dst_fd, buffer, num_read);
		if (num_write != num_read) {
			puts("Unable to write buffer into destination file");
			return -1;
		}
	}
	if (num_read == -1) {
		puts("Unable to read from source file");
		return -1;
	}

	if (close(src_fd) == -1)
		puts("Unable to close source file");
	if (close(dst_fd) == -1)
		puts("Unable to close destination file");

	exit(EXIT_SUCCESS);
}