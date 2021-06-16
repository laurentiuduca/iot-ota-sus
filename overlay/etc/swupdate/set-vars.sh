ROOT_NAME="RPi0w"
BOARD_ID=`cat /proc/cpuinfo | grep Serial | awk -F': ' '{print $2}'`
TARGET_ID="${ROOT_NAME}${BOARD_ID}"
#HAWKBIT_URL="http://10.10.10.100:8080"
#date -s 2021.03.22-15:00
HAWKBIT_URL="https://laurPC-100:8443"
TARGET_TOKEN="3bc13b476cb3962a0c63a5c92beacfh7"
CERTIFICATE_LOCATION="/etc/swupdate/priv-cachain.pem"

