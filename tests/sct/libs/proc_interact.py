"""!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

"""

import time
from subprocess import Popen, PIPE, STDOUT

from robot.api import logger


def run_application(app_name, parameters=''):
    stdout_file = open("stdout.txt", "w")
    application = Popen([app_name, parameters], stdout=stdout_file, stdin=PIPE, stderr=STDOUT,
                        bufsize=1, universal_newlines=True)

    logger.info("Application '%s' was run." % application)

    return application


def clear_stdout(application):
    for line in iter(application.stdout.readline, b''):
        application.stdout.flush()


def send_command(application, command, wait_stdout_ms=0.05):
    command_string = str(command)
    logger.info("Command to execute '%s' on the application '%s'." % (str.encode(command_string), application))

    if command_string[-1] != "\n":
        command_string += "\n"

    application.stdin.write(command_string)
    application.stdin.flush()

    time.sleep(wait_stdout_ms)  # 

    logger.info("Command '%s' was sent to the application '%s'." % (str.encode(command_string), application))


def is_running(application):
    return application.poll() is not None


def wait_for_exit(application):
    logger.info("Wait for termination of the application '%s'." % application)
    application.wait(1)


def get_exit_code(application):
    logger.info("Exit code '%s' of the application '%s'." % (application.returncode, application))
    return application.returncode


def get_pid(application):
    logger.info("PID of the application '%s' is '%d'." % (application, application.pid))
    return application.pid


def kill_application(application):
    logger.info("Kill application '%s'." % application)
    application.kill()
