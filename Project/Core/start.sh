#!/bin/sh

(crontab -l ; echo \"50 23 * * * /home/pedrosmv/Documents/TCC/Project/Core/runstart.sh $1 >/dev/null 2>&1\") | crontab -
