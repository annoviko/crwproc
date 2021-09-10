"""!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

"""

from subprocess import Popen, PIPE, STDOUT


def run_application(app_name, parameters=''):
    application = Popen([app_name, parameters], stdout=PIPE, stdin=PIPE, stderr=STDOUT)
    application.stdout.flush()
    return application


def send_command(application, command):
    if command[len(command) - 2:] != "\r\n":
        command += "\r\n"

    command_bytes = str.encode(command)
    application.stdin.write(command_bytes)
    application.stdin.flush()


def is_running(application):
    return application.poll() is not None


def wait_for_exit(application):
    application.wait(1)


def get_exit_code(application):
    return application.returncode
