#!/usr/bin/env python3

from csmith import csmith_gen
import os
import shutil
import sys
import filecmp
from pathlib import Path
import random
import argparse
import subprocess
from typing import List, Tuple
from functools import reduce
import traceback

def get_transformation_sequence(n: int = 3) -> List[str]:
    """
    Generates random sequence of transformations.

    :param n: Sequence length
    :return: Sequence of transformations
    """
    transformations = ['do', 'while', 'for', 'o', 'if', 'switch', 'iter', 'u']

    sequence = []
    hasO = False
    hasU = False

    for _ in range(n):
        t = random.choice(transformations)
        if t == 'o':
            r = random.randrange(2)
            t = f'{t}{r}'

            if hasO: continue
            hasO = True
        if t == 'u':
            r = random.randrange(10, 50)
            t = f'{t}{r}'

            if hasU: continue
            hasU = True
        sequence.append(t)
    return sequence

class Transformator:
    """
    Transformations wrapper.
    """
    def __init__(self, 
        verbosity: int, 
        compiler: str, 
        compiler_options: str,
        trans_seq_len: int,
        max_run_duration: int
    ):
        self.compiled_program_name = 'run.out'
        self.verbosity = verbosity
        self.compiler = compiler
        self.compiler_options = compiler_options
        self.trans_seq_len = trans_seq_len
        self.max_run_duration = max_run_duration

        self._initialize()

    def _initialize(self) -> None:
        """
        If transformations program is not compiled yet 
        then it compiles before future Transformator usage.
        """
        build_path = f'build'
        self._trace('Compilling transformator library!')
        if not os.path.exists(f'{build_path}/trans'):
            Path(build_path).mkdir(parents=True, exist_ok=True)

        owd = os.getcwd()
        os.chdir(build_path)

        build_command = 'cmake -G "Unix Makefiles" ../trans'
        os.system(build_command)

        os.system('make')

        os.chdir(owd)

    def _trace(self, content: str, verbosity: int = 0, *args, **kwargs) -> None:
        if verbosity <= self.verbosity:
            print(f'[verify-transformator]: {content}', *args, **kwargs)

    def transform(self, seed: int) -> Tuple[bool, List[str]]:
        """
        Transforms C file using random transformation.

        Steps:
        1. Transform C file using C++ compiled transformation program;
        2. Compiles transformed C file;
        3. Saves new output.

        :param seed: Seed
        :return: 
            1. True if transformed program does not have infinite loop and False instead.
            2. Sequence of transformations.
        """
        c_file = f'{seed}.c'
        c_file_duplicate = f'{seed}.dup.c'
        c_transformed_file = f'{seed}.transform.c'
        checksum_file = f'{seed}.checksum.txt'
        trans_path = f'build/trans'

        # 1
        shutil.copyfile(c_file, c_file_duplicate)
        self._trace('Transforming generated C program!', verbosity=1)
        sequence = get_transformation_sequence(n=self.trans_seq_len)
        with open(f'{seed}.trans.sequence.txt', 'w') as tseq_file:
            for transformation in sequence:
                tseq_file.write(f'{transformation}\n')
                self._trace(f'Next transformation is "{transformation}".', verbosity=1)
                transform_command = f'./{trans_path} {c_file_duplicate} tmp.c {transformation}'
                os.system(transform_command)
                os.rename('tmp.c', c_file_duplicate)
            os.rename(c_file_duplicate, c_transformed_file)

        # 2
        # Option '-w' disables all warnings
        self._trace('Compiling transformed generated C program!', verbosity=1)
        compile_command = f'{self.compiler} {c_transformed_file} -o {self.compiled_program_name} -w {self.compiler_options}'
        os.system(compile_command)

        # 3
        self._trace('Running transformed generated C program!', verbosity=1)
        output_file = f'{seed}.output.txt'
        run_command = f'./{self.compiled_program_name} > {output_file}'

        finished = True
        with csmith_gen.Timeout(seconds=self.max_run_duration, error_message='Program took too long to run!'):
            try:
                process = subprocess.Popen(run_command, shell=True)
                process.communicate()
            except TimeoutError:
                self._trace('Transformed program timed out...')
                # Making sure he is dead...
                os.kill(process.pid, signal.SIGKILL)
                finished = False

        return finished, sequence

    def cleanup(self) -> None:
        """
        Deletes temporary files.
        """
        self._trace('Taking trash out...', verbosity=1)
        if os.path.exists(self.compiled_program_name):
            os.remove(self.compiled_program_name)

        csmith_gen.kill_remaining_zombies(self.compiled_program_name)

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_value, tb):
        if exc_type is not None:
            traceback.print_exception(exc_type, exc_value, tb)
        self.cleanup()

def trace(content: str, *args, **kwargs) -> None:
    print(f'[verify-global]: {content}', *args, **kwargs)

def verify(seed: int) -> bool:
    """
    Check if two files are equal.
    If two files are equal then test passed else it failed.

    :param seed: Seed
    :return: True if files are equals (transformed and original) and False instead.
    """
    expected_output_filename = f'{seed}.checksum.txt'
    result_output_filename = f'{seed}.output.txt'
    return filecmp.cmp(expected_output_filename, result_output_filename)

def cleanup(seed: int) -> None:
    """
    Deletes temporary files.

    :param seed: Seed
    :param transformator: Transformator that needs to cleanup()
    """
    if seed is not None:
        for path in Path('.').glob(f'{seed}.*'):
            os.remove(path)

def rename_files(seed: int, save_dir: str) -> None:
    """
    Renames saved files in with more intuitive names.

    :param seed: Seed
    :param save_dir: Path where failed test data is stored.
    """
    os.rename(f'{save_dir}/{seed}.c', f'{save_dir}/raw.c')
    os.rename(f'{save_dir}/{seed}.checksum.txt', f'{save_dir}/expected_output.txt')
    os.rename(f'{save_dir}/{seed}.output.txt', f'{save_dir}/result_output.txt')
    os.rename(f'{save_dir}/{seed}.transform.c', f'{save_dir}/transformed.c')
    os.rename(f'{save_dir}/{seed}.trans.sequence.txt', f'{save_dir}/sequence.txt')

def save_test_info(storage_path: str, seed: int) -> None:
    """
    Saves test info to storage.

    :param storage_path: Path where failed test data is stored.
    :param seed: Seed
    """
    save_dir = f'{storage_path}/{seed}'
    if not os.path.exists(save_dir):
        os.mkdir(save_dir)
        for path in Path('.').glob(f'{seed}.*'):
            path_str = str(path)
            shutil.copy(path_str, f'{save_dir}/{path_str}')
        rename_files(seed, save_dir)

def run():
    parser = argparse.ArgumentParser()
    parser.add_argument('--verbosity', help='Increase output verbosity', default=0, type=int)
    parser.add_argument('--compiler', help='gcc or clang', default='gcc', type=str,
                        choices=['gcc', 'clang'])
    parser.add_argument('--compiler-options', help='Compiler options', type=str, default='')
    parser.add_argument('--trans-seq', help='Length of transformation sequence', type=int, default=3)
    parser.add_argument('--tests', help='Number of tests', type=int, default=3)
    parser.add_argument('--max-duration', help='Maximum program time duration', type=int, default=5)
    args = parser.parse_args()

    storage_path = 'storage'
    if not os.path.exists(storage_path):
        os.mkdir(storage_path)

    transformator_params = {
        'verbosity': args.verbosity,
        'compiler': args.compiler,
        'compiler_options': args.compiler_options,
        'trans_seq_len': args.trans_seq,
        'max_run_duration': args.max_duration
    }

    with Transformator(**transformator_params) as transformator:
        seed = None
        iteration = 1
        max_iteration = args.tests

        test_history = {}

        while iteration <= max_iteration:
            trace(f'Iteration {iteration}:')
            try:
                trace('Generating c program...')
                seed = csmith_gen.run(
                    compiler=args.compiler, 
                    compiler_options=args.compiler_options,
                    max_run_duration=args.max_duration)
                if not os.path.exists(f'{storage_path}/{seed}'):
                    trace('Transforming c program...')
                    passed_test, sequence = transformator.transform(seed)
                    test_history[seed] = passed_test, sequence
                    trace('Comparing checksums...')
                    if passed_test:
                        passed_test = verify(seed)
                    if not passed_test:
                        trace('Test failed :(')
                        trace('Saving test info...')
                        save_test_info(storage_path, seed)
                    else:
                        trace('Test Passed :)')

                    iteration += 1
                else:
                    print('Test seed already exists. Skipping...')

                trace('Done!', end='\n\n')
            finally:
                cleanup(seed)

        trace('Test Results...')
        n_passed_test = 0
        for seed, test_info in test_history.items():
            passed_test, sequence = test_info
            if not passed_test:
                trace(f'Test for seed {seed} failed... Sequence: {sequence}')
            else:
                n_passed_test += int(passed_test)

        trace(f'Passed {n_passed_test}/{max_iteration}')


if __name__ == '__main__':
    run()

