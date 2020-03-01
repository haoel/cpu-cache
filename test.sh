#!/bin/bash

set -e

pushd `dirname $0` > /dev/null
SCRIPT_PATH=`pwd -P`
popd > /dev/null
SCRIPT_FILE=`basename $0`

COLOR_NONE='\033[0m'
COLOR_INFO='\033[0;36m'
COLOR_ERROR='\033[1;31m'

function compile(){
    CC=g++
    OPT="-std=c++11"
    case $3 in
        1) OPT="${OPT} -g";; 
        2) OPT="${OPT} -O2";;
        3) OPT="${OPT} -march=native";;
        *) ;;
    esac
    echo "compile program with \"${OPT}\""
    ${CC} ${OPT} -o $2 $1
}

function compile_name() {
    NAME=${1}
    SOURCE=${SCRIPT_PATH}/${NAME}.cpp
    TARGET=${SCRIPT_PATH}/${NAME}
    compile ${SOURCE} ${TARGET} $2 
}

function cache_line_fetch() {
    NAME="cacheline"
    compile_name ${NAME} 2

    step=1
    echo " steps : microseconds"
    for i in {1..20};do
        ${TARGET} ${step}
        step=$(expr ${step} \* 2)
    done
    rm ${TARGET}
}

function cache_line_race() {
    NAME="cacheline.race"
    compile_name ${NAME} 2

    echo "cache line race "
    ${TARGET}
    echo "no cache line race "
    ${TARGET} false
    rm ${TARGET}
}

function cache_size() {
    NAME="cache.size"
    compile_name ${NAME} 2

    size=256
    echo " steps : microseconds"
    for i in {1..20};do
        ${TARGET} ${size}
        size=$(expr ${size} \* 2 )
    done
    rm ${TARGET}
}

function matrix_travel() {
    NAME="matrix.travel"
    compile_name ${NAME} 2
    ${TARGET} 
    rm ${TARGET} 
}

function false_sharing() {
    NAME="false.sharing"
    compile_name ${NAME} 

    thread=1
    echo "threads : scalable : non-scalable "
    for i in {1..32};do
        t1=$(${TARGET} ${thread})
        t2=$(${TARGET} ${thread} xx)
        printf "  %3d : %5d : %5d\n" $thread $t1 $t2 
        thread=$(expr ${thread} + 1 )
    done
    thread=1

    rm ${TARGET} 
}

function object_member() {
    NAME="object.member"
    compile_name ${NAME} 2 
    ${TARGET} 
    rm ${TARGET} 
}

function cache_hierarchy_bandwidth() {
    NAME="cache.bandwidth"
    compile_name ${NAME} 2

    size="1024 2048 4096 8192 16384 32768 49152 65536 98304 \
          131072 196608 262144 524288 786432 1048576 1572864 \
          2097152 3145728 4194304 6291456 8388608 12582912 16777216 \
          25165824 33554432 50331648 67108864 100663296"
    for i in $size; do
       t=$(${TARGET} ${i})
       printf "%10d : %.2f\n" $i $t
    done

    rm ${TARGET}
}

function cache_conflicts() {
    NAME="cache.conflicts"
    compile_name ${NAME} 2
    echo "| count |   4    |   64  | 2048  | 4096  |"
    echo "------------------------------------------"
    for count in {1..18}; do
        printf "| %5d | " ${count}
        for inc in 4 64 2048 4096; do
            time=$(${TARGET} ${count} ${inc})
            printf "%6d |" ${time}
        done
        printf "\n"
    done

    rm ${TARGET}
}

TEST1="Cache Line Fetch"
TEST2="Cache Line Race"
TEST3="Cache Size"
TEST4="Cache Hierarchy Bandwidth"
TEST5="Matrix Travel"
TEST6="False Sharing Thread"
TEST7="Object Member"
TEST8="Cache Conflicts"


function select_test()
{
    while true; do
        echo  " "
        PS3='Please enter your choice: '
        options=("${TEST1}" "${TEST2}" "${TEST3}" "${TEST4}" "${TEST5}" "${TEST6}" "${TEST7}" "${TEST8}" "Quit")
        COLUMNS=12
        select opt in "${options[@]}"
        do
            case $opt in
                "${TEST1}")
                    cache_line_fetch; break
                    ;;
                "$TEST2")
                    cache_line_race; break;
                    ;;
                "${TEST3}")
                    cache_size; break;
                    ;;
                "${TEST4}")
                    cache_hierarchy_bandwidth; break;
                    ;;
                "${TEST5}")
                    matrix_travel; break;
                    ;;
                "${TEST6}")
                    false_sharing; break;
                    ;;
                "${TEST7}")
                    object_member; break;
                    ;;
                "${TEST8}")
                    cache_conflicts; break;
                    ;;
                "Quit")
                    exit 0;
                    ;;
                *) echo "invalid option $REPLY";;
            esac
        done
    done
}

select_test
