function compareOutput
{
    local command="$1"
    local expectedOut="$2"
    local expectedRet="$3"
    local actual1=$(eval "../../src/42sh < $command" 2>/dev/null)
    local val1=$?
    local actual2=$(eval "../../src/42sh $command" 2>/dev/null)
    local val2=$?
    if [ "$actual1" != "$expectedOut" ] || [  $val1 != $expectedRet ] || [ "$actual2" != "$expectedOut" ] || [ $val2 != $expectedRet ] ;
    then
        echo "Test Failed....command:$command"
        echo "Expected: $expectedOut val: $expectedRet"
        echo "Actual1: $actual1 val1: $val1"
        echo "Actual2: $actual2 val2: $val2"
        exit 1
    else
        echo "Test Passed $command"
    fi
}

command="test1"
bash $command
expectedOut="exit"
expectedRet=$?
compareOutput "$command" "$expectedOut" "$expectedRet"

command="test2"
bash $command
expectedOut="exit"
expectedRet=$?
compareOutput "$command" "$expectedOut" "$expectedRet"

command="test3"
bash $command
expectedOut="exit"
expectedRet=$?
compareOutput "$command" "$expectedOut" "$expectedRet"

command="test4"
bash $command 2>/dev/null
expectedOut="exit
42sh: exit: too many arguments"
expectedRet=$?
compareOutput "$command" "$expectedOut" "$expectedRet"

command="test5"
bash $command 2>/dev/null
expectedOut="exit
42sh: exit: too many arguments"
expectedRet=$?
compareOutput "$command" "$expectedOut" "$expectedRet"

command="test6"
bash $command 2>/dev/null
expectedOut="exit"
expectedRet=$?
compareOutput "$command" "$expectedOut" "$expectedRet"

command="test7"
bash $command 2>/dev/null
expectedout="exit
42sh: exit: too many arguments"
expectedret=$?
compareOutput "$command" "$expectedout" "$expectedret"

command="test8"
bash $command 2>/dev/null
expectedout="exit"
expectedret=$?
compareOutput "$command" "$expectedout" "$expectedret"

command="test9"
bash $command 2>/dev/null
expectedout="exit"
expectedret=$?
compareOutput "$command" "$expectedout" "$expectedret"

command="test10"
bash $command 2>/dev/null
expectedout="exit"
expectedret=$?
compareOutput "$command" "$expectedout" "$expectedret"

command="test11"
bash $command 2>/dev/null
expectedout="exit"
expectedret=$?
compareOutput "$command" "$expectedout" "$expectedret"

command="test12"
bash $command 2>/dev/null
expectedout="exit"
expectedret=$?
compareOutput "$command" "$expectedout" "$expectedret"

command="test13"
bash $command 2>/dev/null
expectedout="exit"
expectedret=$?
compareOutput "$command" "$expectedout" "$expectedret"

echo "Test Finished for exit"
