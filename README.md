# ft_nmap

A network port scanner written in C and inspired by `nmap`. This project uses raw sockets to craft, send, and analyze network packets at the IP, TCP, and UDP layers.

## Features

* **Scan Techniques:**
  * `SYN` (half-open stealth scan)
  * `NULL` (no TCP flags set)
  * `FIN` (FIN flag set)
  * `XMAS` (FIN, PSH, and URG flags set)
  * `ACK` (firewall filtering mapping)
  * `UDP` (connectionless service discovery via ICMP Port Unreachable inspection)
* **Target Resolution:** Supports both IPv4 addresses and domain names (automatic DNS resolution).
* **Port Selection:** SUpports single ports (`--ports 80`), comma-separated lists (`--ports 22,80,443`), and port ranges (`--ports 1-1024`).
* **Performance:** Uses a multithreaded architecture to send and receive packets in parallel.

## Execution

This project includes a `Makefile` that compiles the source files in the `src/` directory and generates the executable.
````sh
make
````
Run the program with the following command :
````sh
./ft_nmap [options] --ip <hots> [--file <file>]
````

**Note :** You cannot use `--ip` and `--file` in the same command.
You can specify either an IP address (e.g., 172.217.20.163) or a domain name (e.g., google.fr) as the target.
You can also specify the optional flags in any order. Run `./ft_nmap --help` for more details.

## Available Options

The program supports the following options :

| Option | Description |
| ---- | ----- |
| --ip | IP or hostname to analyse |
| --file | Path to a file containing a list of IP addresses or hostnames to scan. |
| --types | List of scan type to perform. **Default all scan perform** |
| --ports | Ports selection (e.g., `--ports 20-89,106`). **Default 1-1024** |
| --speedup | Number of threads to use to speed up the scan. **Default 0** |
| --help | Display the help page |
