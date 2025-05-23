function runTests
{
    local component="$1"
    echo "Starting test for $component"
    cd $component
    bash "./test.sh"
}

runTests "echo"
runTests "exit"
echo "All tests completed"
