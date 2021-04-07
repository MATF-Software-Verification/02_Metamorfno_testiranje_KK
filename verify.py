from csmith import csmith_gen
import os
import shutil
import sys
import filecmp
from pathlib import Path

MAX_ITERATION = 10

class Transformator:
    """
    Transformations wrapper.
    """
    def __init__(self, cpp_trans_path: str):
        self.path = cpp_trans_path
        self.compiled_program_name = 'run.out'

        self._initialize()

    def _initialize(self):
        """
        If transformations program is not compiled yet 
        then it compiles before future Transformator usage.
        """
        build_path = f'{self.path}/build'
        if not os.path.exists(build_path):
            print('[verify-transformator]: Compiling transformator library!')
            os.mkdir(build_path)
            owd = os.getcwd()
            os.chdir(build_path)

            build_command = 'cmake -G "Unix Makefiles" ..'
            os.system(build_command)

            os.system('make')

            os.chdir(owd)

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
        transform_command = f'./{trans_path} {c_file} {c_transformed_file} do'
        os.system(transform_command)

        # 2
        # Option '-w' disables all warnings
        compile_command = f'gcc {c_transformed_file} -o {self.compiled_program_name} -w'
        os.system(compile_command)

        # 3
        output_file = f'{seed}.output.txt'
        run_command = f'./{self.compiled_program_name} > {output_file}'
        os.system(run_command)

    def cleanup(self):
        """
        Deletes temporary files.
        """
        if os.path.exists(self.compiled_program_name):
            os.remove(self.compiled_program_name)

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

    transformator = Transformator('trans')

    iteration = 1
    while iteration <= MAX_ITERATION:
        print(f'[verify-global]: Iteration {iteration}:')
        try:
            print('[verify-global]: Generating c program...')
            seed = csmith_gen.run()
            if not os.path.exists(f'{storage_path}/{seed}'):
                print('[verify-global]: Transforming c program...')
                transformator.transform(seed)
                print('[verify-global]: Comparing checksums...')
                passed_test = verify(seed)
                if not passed_test:
                    print('[verify-global]: Test failed :(')
                    print('[verify-global]: Saving test info...')
                    save_test_info(storage_path, seed)
                else:
                    print('[verify-global]: Test Passed :)')

                iteration += 1
            else:
                print('Test seed already exists. Skipping...')

            print('[verify-global]: Done!', end='\n\n')
        finally:
            cleanup(seed, transformator)



if __name__ == '__main__':
    run()

