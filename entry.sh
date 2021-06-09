#!/bin/bash
set -m

GREEN='\033[0;32m'
echo -e "${GREEN}Systemd init system enabled."

# systemd causes a POLLHUP for console FD to occur
# on startup once all other process have stopped.
# We need this sleep to ensure this doesn't occur, else
# logging to the console will not work.
sleep infinity &
for var in $(compgen -e); do
	printf '%q=%q\n' "$var" "${!var}"
done > /etc/docker.env
exec /lib/systemd/systemd