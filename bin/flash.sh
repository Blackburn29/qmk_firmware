#!/bin/bash
QMK_HOME="$HOME/qmk_firmware"

usage() {
    echo "Usage: $0 -f <firmware-file> [-hc]"
}

help() {
    usage
    echo ""
    cat << EOF
Utility for flashing and/or compiling the QMK firmware for your keyboard.
    -f      Flashes the specified firmware file your your keyboard. Will wait for a USB device in DFU mode.
    -c      Optionally, compile firmware before flashing.
    -h      Show this help message.
EOF
}

c=0
while getopts ":hcf:" o; do
    case "${o}" in
        c)
            c=1;;
        f)
            if [ -z "${OPTARG}" ]; then
                usage
                exit 1
            fi
            f=$(realpath ${OPTARG})
            ;;
        h)
            help
            exit 0
            ;;
        *)
            usage
            exit 1
            ;;
    esac
done
shift $((OPTIND-1))

if [ $c -eq 1 ]; then
    echo "Compiling firmware with docker..."
    $QMK_HOME/util/docker_build.sh gmmk/pro/rev1/ansi:blackburn29
fi

if [ -z "$f" ]; then
    echo "No firmware file specified. Skipping flash process."
    exit 0
fi

echo "Preparing to flash: $f"
echo "*** Waiting for device. Please enter bootloader mode now! (Fn + \\) ***"

DEVICE_STATUS=1
i=1
sp="/-\|"
while true; do
    dfu-util --list | grep -iq "found dfu"
    DEVICE_STATUS=$?
    printf "\b${sp:i++%${#sp}:1}"

    if [ $DEVICE_STATUS -eq 0 ]; then
        break;
    fi
    sleep 0.2
done
echo "Flashing now! ($f)"
dfu-util -a 0 -s 0x8000000:leave -D "$f"
