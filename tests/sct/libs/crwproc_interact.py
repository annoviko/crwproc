"""!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

"""

import proc_interact


def crwproc_run(path):
    return proc_interact.run_application(path + "crwproc.exe")


def crwproc_exit(application):
    if application is not None:
        proc_interact.send_command(application, "\\exit")
        proc_interact.wait_for_exit(application)
        return proc_interact.get_exit_code(application)

    return 0


def crwproc_press_any_key(application):
    proc_interact.send_command(application, "\n")


def crwproc_press_enter_key(application):
    proc_interact.send_command(application, "\n")
