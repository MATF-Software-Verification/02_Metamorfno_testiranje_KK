#!/usr/bin/env python3

import os
import re
import sys
import time
import random
import signal
import subprocess
from typing import List, Tuple

class Timeout:
    """
    Helper Class that stops program execution after X seconds if it has not finished.
    Used to filter programs with no lower chance of infinite loop (they do not last too long).
    """
    def __init__(self, seconds: int = 1, error_message: str ='TimeoutError'):
        self.seconds = seconds
        self.error_message = error_message

    def handle_timeout(self, signum: int, frame: object):
        raise TimeoutError(self.error_message)

    def __enter__(self):
        signal.signal(signal.SIGALRM, self.handle_timeout)
        signal.alarm(self.seconds)

    def __exit__(self, type, value, traceback):
        signal.alarm(0)

MAX_RUN_DURATION = 5

def trace(content: str, *args, **kwargs):
    print(f'[csmith-gen]: {content}', *args, **kwargs)

def get_csmith_include() -> str:
    """
    Finds relative path based on `CSMITH_PATH` environment variable value.
    
    CSmith output can be compiled with `<gcc/clang> <cfile> -I$CSMITH_PATH` but
    AST library does not support `-I` option. This is solved by replacing:

    `#include "csmith.h"`

    with 

    `include "[REL_PATH]/csmith.h"`

    :return: `include "[REL_PATH]/csmith.h"`
    """
    assert os.environ['CSMITH_PATH'] is not None, 'Please set "CSMITH_PATH" environment variable value!'
    csmith_abs_path = os.environ['CSMITH_PATH']
    script_abs_path = os.path.abspath('.')
    csmith_include_file = 'csmith.h'
    
    csmith_relative_path = os.path.relpath(csmith_abs_path, script_abs_path)
    csmith_include = f'{csmith_relative_path}/{csmith_include_file}'
    return csmith_include

def run_csmith(seed: int, args: List[str]) -> Tuple[str, int, List[str]]:
    """
    Generates random C file using CSmith tool.

    This script takes same arguments as CSmith tool (it's just a wrapper)

    Option '-s' is deleted from args after first run.

    :param seed: Seed
    :param args: Command line arguments without program name (first argument).
    :return: Output filename, new (old) seed, new (old) args 
    """
    csmith_args = []

    # If no seed is chosen with `-s` option then random seed is generated
    try:
        seed_index = args.index('-s')
        assert len(args) > seed_index+1, 'Missing seed number after "-s" option!'
        seed = int(args[seed_index+1])

        # Can only extract seed once
        del args[seed_index+1]
        del args[seed_index]
    except ValueError:
        seed = random.randrange(sys.maxsize)

    # csmith filters all arguments except `-s` and `-o`
    csmith_args.append('-s')
    csmith_args.append(str(seed))

    trace(f'Program seed is {seed}.')

    # if no output filename is chosen with `-o` option then name is same as seed number
    try:
        output_filename_index = args.index('-o')+1
        assert len(args) > output_filename_index, 'Missing output file name after "-o" option!'
        output_filename = args[output_filename_index]
    except ValueError:
        output_filename = f'{seed}.c'

    csmith_args.append('-o')
    csmith_args.append(output_filename)

    # Creating random C program
    args_line = ' '.join(csmith_args)
    command = f'csmith {args_line}'
    os.system(command)

    return output_filename, seed, args

def replace_csmith_include(output_filename: str, csmith_include: str) -> None:
    """
    Replacing `#include "csmith.h"` with `include "[csmith_include]"` in output_filename.
    Check `get_csmith_include()` function.

    :param output_filename: CSmith generated C file
    :param csmith_include: CSmith library relative path
    """
    result_lines = []

    with open(output_filename, 'r') as f:
        code = f.read()

    # Tuning include path and fixed-width types
    # CSmith generated variable types are not compatible with Clang library
    code = re.sub(r'#include "csmith.h"', f'#include "{csmith_include}"', code)
    code = re.sub(r'([(), ])int8_t([(), ])', r'\1signed char\2', code)
    code = re.sub(r'([(), ])int16_t([(), ])', r'\1short\2', code)
    code = re.sub(r'([(), ])int32_t([(), ])', r'\1long\2', code)
    code = re.sub(r'([(), ])int64_t([(), ])', r'\1long long\2', code)
    code = re.sub(r'([(), ])uint8_t([(), ])', r'\1unsigned char\2', code)
    code = re.sub(r'([(), ])uint16_t([(), ])', r'\1unsigned short\2', code)
    code = re.sub(r'([(), ])uint32_t([(), ])', r'\1unsigned long\2', code)
    code = re.sub(r'([(), ])uint64_t([(), ])', r'\1unsigned long long\2', code)

    with open(output_filename, 'w') as f:
        f.write(code)

def test_generated_c_code(compiler: str, output_filename: str, compiler_options: str, max_run_duration: int) -> bool:
    """
    CSmith can generate C programs with infinite loop.
    All files that need more than [MAX_RUN_DURATION] to finish are dumped.

    In `[seed].c` is program.
    In `[seed].checksum.txt` is program output.

    :param compiler: clang/gcc
    :param output_filename: CSmith generated C program
    :param compiler_options: Compiler options
    :param max_run_duration: All programs take mora than `max_run_duration` seconds to finish are considered
        to have infinite loop
    :return: True if program has not infinite loop and False instead.
    """
    compiled_file_name = 'csmith.out'
    # Option '-w' disables all warnings
    compile_command = f'{compiler} {output_filename} -o {compiled_file_name} -w {compiler_options}'
    os.system(compile_command)

    run_command = f'./{compiled_file_name}'

    filename = output_filename[:-2]
    warn_filename = filename + '.warn.txt'
    checksum_file = filename + '.checksum.txt'
    with Timeout(seconds=max_run_duration, error_message='Program took too long to run!'):
        try:
            process = subprocess.Popen(f'{run_command} > {checksum_file}', shell=True)
            process.communicate()
        except TimeoutError:
            trace('Generated program timed out...')
            # Making sure he is dead...
            os.kill(process.pid, signal.SIGKILL)
            # Removing files from dumped program
            for file in [output_filename, checksum_file, warn_filename]:
                if os.path.exists(file):
                    os.remove(file)
            return False

    trace('New program is generated!')
    # cleanup
    os.remove(compiled_file_name)
    return True

def cleanup():
    zombie_filename = 'zombies.txt'
    list_zombies_command = f'ps -ef | grep csmith.out > {zombie_filename}'
    subprocess.run(list_zombies_command, shell=True)
    with open(zombie_filename, 'r') as f:
        lines = f.readlines()
        for line in lines:
            content = line.split(' ')
            content_without_spaces = list(filter(lambda w: w.strip() != '', content))
            pid = int(content_without_spaces[1])
            try:
                os.kill(pid, signal.SIGKILL)
            except ProcessLookupError:
                pass
    os.remove(zombie_filename)

def run(seed: int = None, compiler: str = 'gcc', compiler_options: str = '', max_run_duration: int = None) -> int:
    passed_test = False
    args = sys.argv[1:]
    if max_run_duration is None:
        max_run_duration = MAX_RUN_DURATION

    while not passed_test:
        csmith_include = get_csmith_include()
        output_filename, seed, args = run_csmith(seed, args)
        replace_csmith_include(output_filename, csmith_include)
        passed_test = test_generated_c_code(compiler, output_filename, compiler_options, max_run_duration)

    cleanup()
    return seed

if __name__ == '__main__':
    run()

