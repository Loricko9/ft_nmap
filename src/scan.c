#include "ft_nmap.h"

void	create_threads(const t_thread_data *data, const t_info *info)
{
	pthread_t	*threads = malloc(sizeof(pthread_t) * info->nb_threads);
	if (!threads) {
		printf("ft_nmap: Memory allocation error\n");
		exit(1);
	}
	for (size_t i = 0; i < info->nb_threads; i++) {
		pthread_create(&threads[i], NULL, preview_scan_jobs, (void *)data);
	}
	for (size_t i = 0; i < info->nb_threads; i++) {
		pthread_join(threads[i], NULL);
	}
	free(threads);
}

bool preview_target(const char *target, const t_info *info)
{
    struct in_addr source;
    char src_text[INET_ADDRSTRLEN];
    char target_text[INET_ADDRSTRLEN];
    struct sockaddr_in destination = {0};

    if (!prepare_ipv4_target(target, &destination.sin_addr))
    {
        fprintf(stderr, "ft_nmap: Cannot prepare IPv4 target: %s\n",
            target);
        return false;
    }

    printf("Prepared IPv4 target: %s\n", target);
    destination.sin_family = AF_INET;

    if (inet_ntop(AF_INET, &destination.sin_addr,
            target_text, sizeof(target_text)) == NULL)
    {
        perror("ft_nmap: inet_ntop");
        return false;
    }

    if (!find_source_ipv4(destination.sin_addr, &source))
        return false;

    if (inet_ntop(AF_INET, &source,
            src_text, sizeof(src_text)) == NULL)
    {
        perror("ft_nmap: source inet_ntop");
        return false;
    }

    printf("Source IPv4: %s\n", src_text);
    t_thread_data data = {
        .target = target,
        .source = source,
        .port_bitmap = info->port_bitmap,
        .types = info->types
    };

    if (info->nb_threads == 0)
        preview_scan_jobs(&data);
    else
        create_threads(&data, info);

    return true;
}
