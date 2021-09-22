"""!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

"""
import subprocess
from subprocess import Popen, PIPE, STDOUT

from robot.api import logger


def run_application(app_name, parameters=''):
    application = Popen([app_name, parameters], stdout=subprocess.DEVNULL, stdin=PIPE, stderr=STDOUT,
                        bufsize=1, universal_newlines=True)

    logger.info("Application '%s' was run." % application)

    return application


def clear_stdout(application):
    for line in iter(application.stdout.readline, b''):
        application.stdout.flush()


def send_command(application, command):
    logger.info("Command to execute '%s' on the application '%s'." % (str.encode(command), application))

    if command[-1] != "\n":
        command += "\n"

    application.stdin.write(command)
    application.stdin.flush()

    logger.info("Command '%s' was sent to the application '%s'." % (str.encode(command), application))


def is_running(application):
    return application.poll() is not None


def wait_for_exit(application):
    logger.info("Wait for termination of the application '%s'." % application)
    application.wait(1)


def get_exit_code(application):
    logger.info("Exit code '%s' of the application '%s'." % (application.returncode, application))
    return application.returncode
