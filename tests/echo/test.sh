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
expectedOut=$(bash $command)
expectedRet=$?
compareOutput "$command" "$expectedOut" "$expectedRet"

command="test2"
expectedOut=$(bash $command)
expectedRet=$?
compareOutput "$command" "$expectedOut" "$expectedRet"

command="test3"
expectedOut=$(bash $command)
expectedRet=$?
compareOutput "$command" "$expectedOut" "$expectedRet"

command="test4"
expectedOut=$(bash $command)
expectedRet=$?
compareOutput "$command" "$expectedOut" "$expectedRet"

command="test5"
expectedOut=$(bash $command)
expectedRet=$?
compareOutput "$command" "$expectedOut" "$expectedRet"

command="test6"
expectedOut=$(bash $command)
expectedRet=$?
compareOutput "$command" "$expectedOut" "$expectedRet"

command="test7"
expectedOut=$(bash $command)
expectedRet=$?
compareOutput "$command" "$expectedOut" "$expectedRet"

command="test8"
expectedOut=$(bash $command)
expectedRet=$?
compareOutput "$command" "$expectedOut" "$expectedRet"

command="test9"
expectedOut=$(bash $command)
expectedRet=$?
compareOutput "$command" "$expectedOut" "$expectedRet"

command="test10"
expectedOut=$(bash $command)
expectedRet=$?
compareOutput "$command" "$expectedOut" "$expectedRet"

command="test11"
expectedOut=$(bash $command)
expectedRet=$?
compareOutput "$command" "$expectedOut" "$expectedRet"

command="test12"
expectedOut=$(bash $command)
expectedRet=$?
compareOutput "$command" "$expectedOut" "$expectedRet"

command="test13"
expectedOut=$(bash $command)
expectedRet=$?
compareOutput "$command" "$expectedOut" "$expectedRet"

command="test14"
expectedOut=$(bash $command)
expectedRet=$?
compareOutput "$command" "$expectedOut" "$expectedRet"

command="test15"
expectedOut=$(bash $command)
expectedRet=$?
compareOutput "$command" "$expectedOut" "$expectedRet"

command="test16"
expectedOut=$(bash $command)
expectedRet=$?
compareOutput "$command" "$expectedOut" "$expectedRet"

command="test17"
expectedOut=$(bash $command)
expectedRet=$?
compareOutput "$command" "$expectedOut" "$expectedRet"

command="test18"
expectedOut=$(bash $command)
expectedRet=$?
compareOutput "$command" "$expectedOut" "$expectedRet"

command="test19"
expectedOut=$(bash $command)
expectedRet=$?
compareOutput "$command" "$expectedOut" "$expectedRet"

command="test20"
expectedOut=$(bash $command)
expectedRet=$?
compareOutput "$command" "$expectedOut" "$expectedRet"

command="test21"
expectedOut=$(bash $command)
expectedRet=$?
compareOutput "$command" "$expectedOut" "$expectedRet"

command="test22"
expectedOut=$(bash $command)
expectedRet=$?
compareOutput "$command" "$expectedOut" "$expectedRet"

command="test23"
expectedOut=$(bash $command)
expectedRet=$?
compareOutput "$command" "$expectedOut" "$expectedRet"

#command="test24"
#expectedOut=$(bash $command)
#expectedRet=$?
#compareOutput "$command" "$expectedOut" "$expectedRet"
echo "Test Finished for echo"
