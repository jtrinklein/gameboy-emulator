i=0
j=0
report="---- "
d=$( cd "$( dirname "${BASH_SOURCE[0]}" )/../" && pwd )
file="${d}/opcode.h"
while (( i++ < 255 )); do {
    if (( j++ == 15 ))
    then
        j=0
        report="${report}\n"
    fi

    code=$(printf "0x%.2x" $i)
    search="OP(${code})"
    result=$(grep -i $search $file)
    if [[ "x${result}x" == "xx" ]]
    then
        report="${report}${code} "
    else 
        report="${report}---- "
    fi
}; done

if [[ "z${report}z" != "zz" ]]
then
    echo "Missing OpCodes:"
    echo $report
fi
