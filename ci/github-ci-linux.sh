set -e      # exit immediately if a command exits with a non-zero status.
set -x      # print all executed commands to the terminal.


announce_step() {
    echo "[New Step] $1"
}


print_error() {
    echo "[Error] $1"
}


terminate_if_failure() {
    error_code=$?
    if [ $error_code -ne 0 ] ; then
        if [ -z $1 ] ; then
            print_error $1
        else
            print_error "Non-zero exit code '$error_code' is detected."
        fi
        exit 1
    fi
}


run_cppcheck() {
    announce_step "Run CppCheck."

    sudo apt-get install -qq cppcheck
    cppcheck --inline-suppr --error-exitcode=1 --std=c++17 --inconclusive --enable=warning,performance,information,portability core control crwproc log
    
    terminate_if_failure "CppCheck static analysis failed."
}


run_continous_integration() {
    run_cppcheck
}


run_continous_integration
