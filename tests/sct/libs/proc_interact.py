"""!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

"""

import ntpath
import re
import os
import time

from datetime import datetime
from subprocess import Popen, PIPE

import robot.api.logger
import robot.utils.asserts


__proc_file_mapping = {}
__proc_line_cursor_mapping = {}


def run_application(app_name, parameters=[''], stdout_filename=None):
    if stdout_filename is None:
        output_filename = ntpath.basename(app_name) + "_output.txt"
    else:
        output_filename = stdout_filename

    output_fstream = open(output_filename, "w")

    command_line = [app_name] + parameters
    application = Popen(command_line, stdout=output_fstream, stdin=PIPE, stderr=output_fstream,
                        bufsize=1, universal_newlines=True)

    __proc_file_mapping[application] = output_filename
    __proc_line_cursor_mapping[application] = 0

    if application is not None:
        robot.api.logger.info("Application '%s' (PID: '%s') was run." % (application, application.pid))
    else:
        robot.api.logger.info("Application '%s' was run." % application)

    return application


def check_log_correctness(application, test_name):
    time.sleep(0.1)

    date = datetime.today()
    expected_date_postfix = date.strftime("%Y-%m-%d_%H-%M-%S")
    
    expected_name_postfix = test_name.replace(" ", "_").lower()
    
    folder_files = os.listdir(".")
    
    expression = ".*" + expected_name_postfix + ".*" + expected_date_postfix + ".log"
    robot.api.logger.info("Regex expression to find log file '%s' amoung files '%s'." % (expression, str(folder_files)))
    for filename in folder_files:
        if re.match(expression, filename):
            return True
    
    robot.utils.asserts.fail("Log file for the application is not found.")


def clean_output_stream(application):
    robot.api.logger.info("Clean out stream '%s' by skipping lines." % __proc_file_mapping[application])
    
    time.sleep(0.1)
    output_fstream = open(__proc_file_mapping[application], "r")

    amount_lines = 0
    for _ in output_fstream.readlines():
        amount_lines += 1
    
    robot.api.logger.info("The amount lines to skip: %d." % amount_lines)
    __proc_line_cursor_mapping[application] = amount_lines


def output_stream_contains(application, expression):
    robot.api.logger.info("Process (contains) output stream '%s' (stdout: '%s')." % (__proc_file_mapping[application], application.stdout))

    result = False
    lines_to_analyse = ""
    line_number = 1
    skip_until = __proc_line_cursor_mapping[application]

    robot.api.logger.info("Consider output after line %d." % skip_until)
    stream = open(__proc_file_mapping[application], "r")
    for line in stream:
        if line_number >= skip_until:
            lines_to_analyse += line

            if re.match(expression, line):
                result = True
                break

        line_number += 1

    robot.api.logger.info("Analysed content using expression %s:\n%s" % (expression, lines_to_analyse))

    return result


def output_stream_match(application, expression):
    robot.api.logger.info("Process (match) output stream '%s' (stdout: '%s')." % (__proc_file_mapping[application], application.stdout))

    lines_to_analyse = ""
    line_number = 0
    skip_until = __proc_line_cursor_mapping[application]

    robot.api.logger.info("Consider output after line %d." % skip_until)
    stream = open(__proc_file_mapping[application], "r")
    for line in stream:
        if line_number >= skip_until:
            lines_to_analyse += line

            match_result = re.match(expression, line)
            if match_result:
                return list(match_result.groups())

        line_number += 1

    robot.api.logger.info("Analysed content using expression %s:\n%s" % (expression, lines_to_analyse))

    return []


def wait_for_output_stream_content(application, expression, timeout=1):
    robot.api.logger.info("Wait for '%s' for '%d' seconds (stdout: '%s')." % (expression, timeout, application.stdout))
    time_start = datetime.now()

    result = output_stream_contains(application, expression)
    time_current = datetime.now()
    
    while ((result is False) and ((time_current - time_start).seconds < timeout)):
        result = output_stream_contains(application, expression)
        time_current = datetime.now()
        time.sleep(0.05)

    return result


def send_command(application, command, wait_app_ms=0.025):
    command_string = str(command)
    robot.api.logger.info("Command to execute '%s' on the application '%s'." % (str.encode(command_string), application))

    if command_string[-1] != "\n":
        command_string += "\n"

    application.stdin.write(command_string)
    application.stdin.flush()

    time.sleep(wait_app_ms)  # give some time to the process to finish input command

    robot.api.logger.info("Command '%s' was sent to the application '%s'." % (str.encode(command_string), application))


def is_running(application):
    return application.poll() is not None


def wait_for_exit(application):
    robot.api.logger.info("Wait for termination of the application '%s'." % application)
    application.wait(1)


def get_exit_code(application):
    robot.api.logger.info("Exit code '%s' of the application '%s'." % (application.returncode, application))
    return application.returncode


def get_pid(application):
    robot.api.logger.info("PID of the application '%s' is '%d'." % (application, application.pid))
    return application.pid


def kill_application(application):
    robot.api.logger.info("Kill application '%s'." % application)
    application.kill()
    application.communicate()
