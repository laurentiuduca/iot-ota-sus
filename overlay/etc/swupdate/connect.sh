#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2021 Laurentiu-Cristian Duca

set -x

. /etc/swupdate/set-vars.sh

# sleep until ntp sets current date
#sleep $1
let crt_year=`date +%Y`
while [[ ${crt_year} -eq 1970 ]]; do
    sleep 3
    let crt_year=`date +%Y`
done

# autoconnect if server restarts
while [[ 1 -eq 1 ]]; do
    /root/encipher-decipher.out 2
    echo "connect command returns $?"
#    curl -v --cacert "${CERTIFICATE_LOCATION}" --user 'admin:admin' "${HAWKBIT_URL}/rest/v1/targets" -i -X POST -H 'Content-Type: application/json;charset=UTF-8' -d "[ {\"securityToken\" : \"${TARGET_TOKEN}\", \"controllerId\" : \"${TARGET_ID}\",\"name\" : \"${TARGET_ID}\", \"description\" : \"Controller ${TARGET_ID}\"} ]"
    sleep 300
done

