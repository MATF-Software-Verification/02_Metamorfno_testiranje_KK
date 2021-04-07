from csmith import csmith_gen
import os
import shutil
import subprocess
import sys
import filecmp
from pathlib import Path

MAX_ITERATION = 10

class Transformator:
    """
    Transformations wrapper.
    """
    def __init__(self, cpp_trans_path: str, verbosity: int = 1):
        self.path = cpp_trans_path
        self.compiled_program_name = 'run.out'
        self.verbosity = verbosity

        self._initialize()

    def _initialize(self):
        """
        If transformations program is not compiled yet 
        then it compiles before future Transformator usage.
        """
        build_path = f'{self.path}/build'
        if not os.path.exists(build_path):
            self._trace('Compiling transformator library!')
            os.mkdir(build_path)
            owd = os.getcwd()
            os.chdir(build_path)

            build_command = 'cmake -G "Unix Makefiles" ..'
            subprocess.run(build_command, shell=True)

            subprocess.run('make', shell=True)

            os.chdir(owd)

    def _trace(self, content: str, verbosity: int = 0, *args, **kwargs):
        if verbosity <= self.verbosity:
            print(f'[verify-transformator]: {content}', *args, **kwargs)

    def transform(self, seed):
        """
        Transforms C file using random transformation.

        Steps:
        1. Transform C file using C++ compiled transformation program;
        2. Compiles transformed C file;
        3. Saves new output.
        """
        c_file = f'{seed}.c'
        c_transformed_file = f'{seed}.transform.c'
        checksum_file = f'{seed}.checksum.txt'
        trans_path = f'{self.path}/build/trans'

        # 1
        self._trace('Transforming generated C program!', verbosity=1)
        transform_command = f'./{trans_path} {c_file} {c_transformed_file} do'
        subprocess.run(transform_command, shell=True)

        # 2
        # Option '-w' disables all warnings
        self._trace('Compiling transformed generated C program!', verbosity=1)
        compile_command = f'gcc {c_transformed_file} -o {self.compiled_program_name} -w'
        subprocess.run(compile_command, shell=True)

        # 3
        self._trace('Running transformed generated C program!', verbosity=1)
        output_file = f'{seed}.output.txt'
        run_command = f'./{self.compiled_program_name} > {output_file}'
        subprocess.run(run_command, shell=True)

    def cleanup(self):
        """
        Deletes temporary files.
        """
        if os.path.exists(self.compiled_program_name):
            os.remove(self.compiled_program_name)

def trace(content: str, *args, **kwargs):
    print(f'[verify-global]: {content}', *args, **kwargs)

def verify(seed):
    """
    Check if two files are equal.
    If two files are equal then test passed else it failed.
    """
    expected_output_filename = f'{seed}.checksum.txt'
    result_output_filename = f'{seed}.output.txt'
    return filecmp.cmp(expected_output_filename, result_output_filename)

def cleanup(seed, transformator):
    """
    Deletes temporary files.
    """
    for path in Path('.').glob(f'{seed}.*'):
        os.remove(path)
    transformator.cleanup()

def rename_files(seed, save_dir):
    """
    Renames saved files in with more intuitive names.
    """
    os.rename(f'{save_dir}/{seed}.c', f'{save_dir}/raw.c')
    os.rename(f'{save_dir}/{seed}.checksum.txt', f'{save_dir}/expected_output.txt')
    os.rename(f'{save_dir}/{seed}.output.txt', f'{save_dir}/result_output.txt')
    os.rename(f'{save_dir}/{seed}.transform.c', f'{save_dir}/transformed.c')

def save_test_info(storage_path, seed):
    """
    Saves test info to storage.
    """
    save_dir = f'{storage_path}/{seed}'
    if not os.path.exists(save_dir):
        os.mkdir(save_dir)
        for path in Path('.').glob(f'{seed}.*'):
            path_str = str(path)
            shutil.copy(path_str, f'{save_dir}/{path_str}')
        rename_files(seed, save_dir)

def run():
    storage_path = 'storage'
    if not os.path.exists(storage_path):
        os.mkdir(storage_path)

    transformator = Transformator('trans', verbosity=1)

    iteration = 1
    while iteration <= MAX_ITERATION:
        print(f'[verify-global]: Iteration {iteration}:')
        try:
            trace('Generating c program...')
            seed = csmith_gen.run()
            if not os.path.exists(f'{storage_path}/{seed}'):
                trace('Transforming c program...')
                transformator.transform(seed)
                trace('Comparing checksums...')
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
            cleanup(seed, transformator)



if __name__ == '__main__':
    run()