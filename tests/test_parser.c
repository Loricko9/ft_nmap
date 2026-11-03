/*
 * Unit tests for the parsing part: check_flags, port bitmap, ft_split,
 * read_host_file. Run with `make test`.
 *
 * check_flags() calls exit() on bad input, so each parser case runs in a
 * forked child and the test looks at how the child ended.
 *
 * KNOWN_BUG cases describe what the subject requires but the parser does not
 * do yet. They report XFAIL and do not fail the suite. Once fixed they report
 * XPASS and fail the suite: flip KNOWN_BUG to OK so the fix stays covered.
 */

#include "ft_nmap.h"
#include <fcntl.h>
#include <sys/wait.h>

#define ARGS(...) ((char *[]){__VA_ARGS__, NULL})
#define OK false
#define KNOWN_BUG true

typedef enum e_outcome
{
	ACCEPTED,		/* check_flags returned and the check passed */
	WRONG_VALUES,	/* check_flags returned but the check failed */
	REJECTED,		/* exit(1) */
	EXITED_0,		/* exit(0), e.g. --help */
	CRASHED,		/* killed by a signal */
	OTHER_EXIT
} t_outcome;

static const char *outcome_names[] = {
	"accepted", "accepted with wrong values", "rejected", "exit(0)", "crash", "unexpected exit code"
};

typedef bool (*t_check)(const t_info *info);

static int g_pass, g_fail, g_xfail, g_xpass;

static void report(const char *name, bool ok, bool known_bug, const char *detail)
{
	if (ok && !known_bug)
		g_pass++;
	else if (!ok && known_bug) {
		g_xfail++;
		printf("XFAIL %s (known bug: %s)\n", name, detail);
	}
	else if (ok && known_bug) {
		g_xpass++;
		printf("XPASS %s: bug is fixed, change KNOWN_BUG to OK\n", name);
	}
	else {
		g_fail++;
		printf("FAIL  %s: %s\n", name, detail);
	}
}

/* ---------- check_flags ---------- */

static t_outcome run_parser(char **args, t_check check)
{
	fflush(stdout);
	pid_t pid = fork();
	if (pid == -1) {
		perror("fork");
		exit(2);
	}
	if (pid == 0) {
		int devnull = open("/dev/null", O_WRONLY);
		dup2(devnull, STDOUT_FILENO);
		dup2(devnull, STDERR_FILENO);

		char *av[64] = {"ft_nmap"};
		int ac = 1;
		while (args[ac - 1] && ac < 63) {
			av[ac] = args[ac - 1];
			ac++;
		}
		av[ac] = NULL;

		t_info info;
		ft_init_info(&info);
		check_flags(ac, av, &info);
		_exit(check && !check(&info) ? 101 : 100);
	}
	int status;
	if (waitpid(pid, &status, 0) == -1) {
		perror("waitpid");
		exit(2);
	}
	if (WIFSIGNALED(status))
		return CRASHED;
	switch (WEXITSTATUS(status)) {
		case 0: return EXITED_0;
		case 1: return REJECTED;
		case 100: return ACCEPTED;
		case 101: return WRONG_VALUES;
		default: return OTHER_EXIT;
	}
}

static void expect(const char *name, char **args, t_check check, t_outcome want, bool known_bug)
{
	char detail[128];
	t_outcome got = run_parser(args, check);

	snprintf(detail, sizeof(detail), "expected %s, got %s", outcome_names[want], outcome_names[got]);
	report(name, got == want, known_bug, detail);
}

static int count_ports(const t_info *info)
{
	int n = 0;
	for (int port = 0; port < MAX_PORTS; port++)
		n += is_port_set(info->port_bitmap, port);
	return n;
}

static bool only_types(const t_info *info, const bool want[SCAN_COUNT])
{
	for (int i = 0; i < SCAN_COUNT; i++)
		if (info->types[i] != want[i])
			return false;
	return true;
}

static bool check_defaults(const t_info *info)
{
	const bool all[SCAN_COUNT] = {true, true, true, true, true, true};
	return count_ports(info) == 1024 && is_port_set(info->port_bitmap, 1)
		&& is_port_set(info->port_bitmap, 1024) && !is_port_set(info->port_bitmap, 1025)
		&& only_types(info, all) && info->nb_threads == 0
		&& info->ip && strcmp(info->ip, "127.0.0.1") == 0 && !info->file;
}

static bool check_port_80(const t_info *info)
{
	return count_ports(info) == 1 && is_port_set(info->port_bitmap, 80);
}

static bool check_port_65535(const t_info *info)
{
	return count_ports(info) == 1 && is_port_set(info->port_bitmap, 65535);
}

static bool check_list_and_range(const t_info *info)
{
	/* 1,5-15 */
	return count_ports(info) == 12 && is_port_set(info->port_bitmap, 1)
		&& !is_port_set(info->port_bitmap, 4) && is_port_set(info->port_bitmap, 5)
		&& is_port_set(info->port_bitmap, 15) && !is_port_set(info->port_bitmap, 16);
}

static bool check_full_1024(const t_info *info)
{
	return count_ports(info) == 1024;
}

static bool check_syn_only(const t_info *info)
{
	const bool want[SCAN_COUNT] = {[SCAN_SYN] = true};
	return only_types(info, want);
}

static bool check_syn_ack_udp(const t_info *info)
{
	const bool want[SCAN_COUNT] = {[SCAN_SYN] = true, [SCAN_ACK] = true, [SCAN_UDP] = true};
	return only_types(info, want);
}

static bool check_speedup_0(const t_info *info) { return info->nb_threads == 0; }
static bool check_speedup_1(const t_info *info) { return info->nb_threads == 1; }
static bool check_speedup_250(const t_info *info) { return info->nb_threads == 250; }

static bool check_file(const t_info *info)
{
	return info->file && strcmp(info->file, "hosts.txt") == 0 && !info->ip;
}

static void test_check_flags(void)
{
	/* valid input */
	/* only --ip given: ports 1-1024, all six scans, speedup 0, no file
	 * (known bug: ft_init_info memsets bits 0..1023, so port 1024 is missing) */
	expect("defaults (ports 1-1024, all scans, speedup 0)", ARGS("--ip", "127.0.0.1"), check_defaults, ACCEPTED, KNOWN_BUG);
	/* one port selects exactly that port */
	expect("single port", ARGS("--ports", "80", "--ip", "127.0.0.1"), check_port_80, ACCEPTED, OK);
	/* 65535, the highest valid port, is accepted */
	expect("highest port", ARGS("--ports", "65535", "--ip", "127.0.0.1"), check_port_65535, ACCEPTED, OK);
	/* a list mixing a single port and a range selects both, bounds included */
	expect("list + range", ARGS("--ports", "1,5-15", "--ip", "127.0.0.1"), check_list_and_range, ACCEPTED, OK);
	/* exactly 1024 ports (the subject's limit) is accepted */
	expect("1024 ports is the max", ARGS("--ports", "1001-2024", "--ip", "127.0.0.1"), check_full_1024, ACCEPTED, OK);
	/* one scan type enables only that type */
	expect("scan SYN", ARGS("--scan", "SYN", "--ip", "127.0.0.1"), check_syn_only, ACCEPTED, OK);
	/* several scan types separated by '/' or ',' enable exactly those */
	expect("scan SYN/ACK,UDP", ARGS("--scan", "SYN/ACK,UDP", "--ip", "127.0.0.1"), check_syn_ack_udp, ACCEPTED, OK);
	/* lowest allowed thread count */
	expect("speedup 1", ARGS("--speedup", "1", "--ip", "127.0.0.1"), check_speedup_1, ACCEPTED, OK);
	/* highest allowed thread count */
	expect("speedup 250", ARGS("--speedup", "250", "--ip", "127.0.0.1"), check_speedup_250, ACCEPTED, OK);
	/* --file stores the path and leaves ip unset */
	expect("file", ARGS("--file", "hosts.txt"), check_file, ACCEPTED, OK);
	/* flags work in any order, not just the usage order */
	expect("flags in any order", ARGS("--ip", "127.0.0.1", "--scan", "SYN", "--ports", "80"), check_syn_only, ACCEPTED, OK);
	/* --help prints the help screen and exits with status 0 */
	expect("help", ARGS("--help"), NULL, EXITED_0, OK);

	/* invalid input */
	/* a flag with no value after it */
	expect("missing value", ARGS("--ip"), NULL, REJECTED, OK);
	/* the last flag on the line has no value */
	expect("missing value after other flag", ARGS("--ip", "127.0.0.1", "--ports"), NULL, REJECTED, OK);
	/* a flag that does not exist */
	expect("unknown flag", ARGS("--bogus", "1", "--ip", "127.0.0.1"), NULL, REJECTED, OK);
	/* a word that belongs to no flag */
	expect("stray argument", ARGS("--ip", "127.0.0.1", "extra", "x"), NULL, REJECTED, OK);
	/* a port that is not a number */
	expect("port letters", ARGS("--ports", "abc", "--ip", "127.0.0.1"), NULL, REJECTED, OK);
	/* port 0 is not a valid port */
	expect("port 0", ARGS("--ports", "0", "--ip", "127.0.0.1"), NULL, REJECTED, OK);
	/* one above the highest port */
	expect("port 65536", ARGS("--ports", "65536", "--ip", "127.0.0.1"), NULL, REJECTED, OK);
	/* a negative port (a leading '-' is not a range) */
	expect("port negative", ARGS("--ports", "-1", "--ip", "127.0.0.1"), NULL, REJECTED, OK);
	/* a range missing its end */
	expect("port trailing dash", ARGS("--ports", "1-", "--ip", "127.0.0.1"), NULL, REJECTED, OK);
	/* a list starting with an empty item */
	expect("port leading comma", ARGS("--ports", ",80", "--ip", "127.0.0.1"), NULL, REJECTED, OK);
	/* a range with three parts */
	expect("port double range", ARGS("--ports", "1-2-3", "--ip", "127.0.0.1"), NULL, REJECTED, OK);
	/* a scan type that does not exist */
	expect("scan unknown", ARGS("--scan", "FOO", "--ip", "127.0.0.1"), NULL, REJECTED, OK);
	/* one thread above the maximum of 250 */
	expect("speedup 251", ARGS("--speedup", "251", "--ip", "127.0.0.1"), NULL, REJECTED, OK);
	/* a negative thread count */
	expect("speedup negative", ARGS("--speedup", "-1", "--ip", "127.0.0.1"), NULL, REJECTED, OK);

	/* subject requirements the parser does not meet yet (tmp/parser_issue.txt) */
	/* 0 threads is the subject's default, so asking for it explicitly must work */
	expect("speedup 0 is valid (subject default)", ARGS("--speedup", "0", "--ip", "127.0.0.1"), check_speedup_0, ACCEPTED, KNOWN_BUG);
	/* another flag must not be taken as --ip's value */
	expect("flag used as a value", ARGS("--ip", "--help"), NULL, REJECTED, KNOWN_BUG);
	/* the subject caps a scan at 1024 ports */
	expect("more than 1024 ports", ARGS("--ports", "1-1025", "--ip", "127.0.0.1"), NULL, REJECTED, KNOWN_BUG);
	/* a range whose start is above its end selects nothing, so it must be refused */
	expect("descending range", ARGS("--ports", "100-1", "--ip", "127.0.0.1"), NULL, REJECTED, KNOWN_BUG);
	/* an empty --ports value selects nothing, so it must be refused */
	expect("empty port list", ARGS("--ports", "", "--ip", "127.0.0.1"), NULL, REJECTED, KNOWN_BUG);
	/* a huge number must not wrap around to a small valid port (2^32 + 1 becomes 1) */
	expect("port overflowing int", ARGS("--ports", "4294967297", "--ip", "127.0.0.1"), NULL, REJECTED, KNOWN_BUG);
	/* trailing characters after the number must be refused, not ignored */
	expect("speedup with junk", ARGS("--speedup", "2junk", "--ip", "127.0.0.1"), NULL, REJECTED, KNOWN_BUG);
}

/* ---------- port bitmap ---------- */

static void test_bitmap(void)
{
	uint8_t bitmap[BITMAP_SIZE] = {0};

	/* port 0 cannot be stored */
	report("set_port(0) refused", !set_port(bitmap, 0), OK, "returned true");
	/* a port past the end of the bitmap cannot be stored */
	report("set_port(65536) refused", !set_port(bitmap, 65536), OK, "returned true");
	/* a negative port cannot be stored */
	report("set_port(-1) refused", !set_port(bitmap, -1), OK, "returned true");
	/* the lowest valid port can be stored and read back */
	report("set_port(1)", set_port(bitmap, 1) && is_port_set(bitmap, 1), OK, "port 1 not set");
	/* the highest valid port can be stored and read back */
	report("set_port(65535)", set_port(bitmap, 65535) && is_port_set(bitmap, 65535), OK, "port 65535 not set");
	/* setting a port does not also set the ports next to it */
	report("neighbours untouched", !is_port_set(bitmap, 2) && !is_port_set(bitmap, 65534), OK, "extra bit set");
	/* reading an out-of-range port returns false instead of reading past the bitmap */
	report("is_port_set out of range", !is_port_set(bitmap, 0) && !is_port_set(bitmap, MAX_PORTS), OK, "returned true");
}

/* ---------- ft_split ---------- */

static bool tab_equals(char **tab, const char **want)
{
	int i = 0;
	if (!tab)
		return false;
	for (; want[i]; i++)
		if (!tab[i] || strcmp(tab[i], want[i]) != 0)
			return false;
	return tab[i] == NULL;
}

static void expect_split(const char *name, char *str, char *sep, const char **want)
{
	char **tab = ft_split(str, sep);
	report(name, tab_equals(tab, want), OK, "wrong words");
	free_tab(tab);
}

static void test_split(void)
{
	/* splits words on a single separator */
	expect_split("split simple", "a,b,c", ",", (const char *[]){"a", "b", "c", NULL});
	/* repeated, leading and trailing separators produce no empty words */
	expect_split("split repeated/edge separators", ",,a,,b,", ",", (const char *[]){"a", "b", NULL});
	/* any character of the separator set splits (as --scan uses it) */
	expect_split("split several separators", "SYN/ACK, UDP", "/, ", (const char *[]){"SYN", "ACK", "UDP", NULL});
	/* an empty string gives an empty list */
	expect_split("split empty string", "", ",", (const char *[]){NULL});
	/* a string of only separators gives an empty list */
	expect_split("split only separators", "   ", " ", (const char *[]){NULL});
	/* a NULL string returns NULL instead of crashing */
	report("split NULL", ft_split(NULL, ",") == NULL, OK, "returned non-NULL");
}

/* ---------- read_host_file ---------- */

static char *write_temp(const char *content)
{
	static char path[64];
	strcpy(path, "/tmp/ft_nmap_test_XXXXXX");
	int fd = mkstemp(path);
	if (fd == -1 || write(fd, content, strlen(content)) != (ssize_t)strlen(content)) {
		perror("temp file");
		exit(2);
	}
	close(fd);
	return path;
}

static void expect_hosts(const char *name, const char *content, const char **want, bool known_bug)
{
	char *path = write_temp(content);
	char **hosts = read_host_file(path);

	if (want)
		report(name, tab_equals(hosts, want), known_bug, "wrong host list");
	else
		report(name, hosts == NULL, known_bug, "expected NULL (empty), got a list");
	free_tab(hosts);
	unlink(path);
}

static void child_read_missing(void)
{
	int devnull = open("/dev/null", O_WRONLY);
	dup2(devnull, STDOUT_FILENO);
	read_host_file("/nonexistent/ft_nmap_hosts");
	_exit(100);
}

static void test_host_file(void)
{
	/* one host per line with Unix line endings */
	expect_hosts("hosts LF", "127.0.0.1\ngoogle.com\n", (const char *[]){"127.0.0.1", "google.com", NULL}, OK);
	/* Windows line endings: no '\r' is left in the host names */
	expect_hosts("hosts CRLF", "127.0.0.1\r\ngoogle.com\r\n", (const char *[]){"127.0.0.1", "google.com", NULL}, OK);
	/* several hosts on one line, with extra spaces, give no empty hosts */
	expect_hosts("hosts several per line + spaces", "  a  b \nc   \n", (const char *[]){"a", "b", "c", NULL}, OK);
	/* the last line is read even without a final newline */
	expect_hosts("hosts no trailing newline", "a\nb", (const char *[]){"a", "b", NULL}, OK);
	/* empty and whitespace-only lines are skipped */
	expect_hosts("hosts blank lines skipped", "a\n\n   \nb\n", (const char *[]){"a", "b", NULL}, OK);
	/* an empty file returns NULL so main can report it */
	expect_hosts("hosts empty file", "", NULL, OK);
	/* a file of blank lines counts as empty
	 * (known bug: returns an empty list instead of NULL, so main scans nothing and says nothing) */
	expect_hosts("hosts blank-only file is empty", "\n  \n\n", NULL, KNOWN_BUG);

	/* a file that cannot be opened makes the program exit with status 1 */
	fflush(stdout);
	pid_t pid = fork();
	if (pid == 0)
		child_read_missing();
	int status;
	waitpid(pid, &status, 0);
	report("hosts missing file rejected", WIFEXITED(status) && WEXITSTATUS(status) == 1, OK, "did not exit(1)");
}

int main(void)
{
	test_check_flags();
	test_bitmap();
	test_split();
	test_host_file();

	printf("\n%d passed, %d failed, %d known bugs (xfail), %d fixed bugs to flip (xpass)\n",
		g_pass, g_fail, g_xfail, g_xpass);
	return (g_fail || g_xpass) ? 1 : 0;
}
