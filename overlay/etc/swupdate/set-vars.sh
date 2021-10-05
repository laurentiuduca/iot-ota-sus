export ROOT_NAME="RPi0w"
export BOARD_ID=`cat /proc/cpuinfo | grep Serial | awk -F': ' '{print $2}'`
export TARGET_ID="${ROOT_NAME}${BOARD_ID}"
#export HAWKBIT_URL="http://10.10.10.100:8080"
export HAWKBIT_URL="https://laurPC-100:8443"
export TARGET_TOKEN="3bc13b476cb3962a0c63a5c92beacfh7"
export CERTIFICATE_LOCATION="/etc/swupdate/priv-cachain.pem"
