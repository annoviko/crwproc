"""!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

"""

import ntpath
import re
import time

from subprocess import Popen, PIPE

from robot.api import logger


__proc_file_mapping = {}
__proc_line_cursor_mapping = {}


def run_application(app_name, parameters=''):
    output_filename = ntpath.basename(app_name) + "_output.txt"
    output_fstream = open(output_filename, "w")

    application = Popen([app_name, parameters], stdout=output_fstream, stdin=PIPE, stderr=output_fstream,
                        bufsize=1, universal_newlines=True)

    __proc_file_mapping[application] = output_filename
    __proc_line_cursor_mapping[application] = 0

    logger.info("Application '%s' was run." % application)

    return application


def clean_output_stream(application):
    logger.info("Clean out stream '%s' by skipping lines." % __proc_file_mapping[application])
    
    time.sleep(0.1)
    output_fstream = open(__proc_file_mapping[application], "r")

    amount_lines = 0
    for _ in output_fstream.readlines():
        amount_lines += 1
    
    logger.info("The amount lines to skip: %d." % amount_lines)
    __proc_line_cursor_mapping[application] = amount_lines


def output_stream_contains(application, expression):
    logger.info("Handle process output stream '%s' (stdout: '%s')." % (__proc_file_mapping[application], application.stdout))

    result = False
    lines_to_analyse = ""
    line_number = 0
    skip_until = __proc_line_cursor_mapping[application]

    logger.info("Consider output after line %d." % skip_until)
    stream = open(__proc_file_mapping[application], "r")
    for line in stream:
        if line_number >= skip_until:
            lines_to_analyse += line

            if re.match(expression, line):
                result = True
                break

        line_number += 1

    logger.info("Analysed content using expression %s:\n%s" % (expression, lines_to_analyse))

    return result


def send_command(application, command, wait_app_ms=0.1):
    command_string = str(command)
    logger.info("Command to execute '%s' on the application '%s'." % (str.encode(command_string), application))

    if command_string[-1] != "\n":
        command_string += "\n"

    application.stdin.write(command_string)
    application.stdin.flush()

    time.sleep(wait_app_ms)  # give some time to the process to finish input command

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
