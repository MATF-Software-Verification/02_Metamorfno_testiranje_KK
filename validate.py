from csmith import csmith_gen
import os
import shutil
import subprocess
import sys
import filecmp
from pathlib import Path

MAX_ITERATION = 2
OUTPUT_PATH = ''

class Transformator:
    def __init__(self, cpp_trans_path: str):
        self.path = cpp_trans_path
        self.compiled_program_name = 'run.out'

        self._initialize()

    def _initialize(self):
        build_path = f'{self.path}/build'
        if not os.path.exists(build_path):
            print('Compiling transformator library!')
            os.mkdir(build_path)
            owd = os.getcwd()
            os.chdir(build_path)

            build_command = 'cmake -G "Unix Makefiles" ..'
            subprocess.run(build_command, shell=True)

            subprocess.run('make', shell=True)

            os.chdir(owd)

    def transform(self, seed):
        c_file = f'{seed}.c'
        c_transformed_file = f'{seed}.transform.c'
        checksum_file = f'{seed}.checksum.txt'
        trans_path = f'{self.path}/build/trans'

        # Option '-w' disables all warnings
        transform_command = f'./{trans_path} {c_file} {c_transformed_file} do'
        subprocess.run(transform_command, shell=True)

        compile_command = f'gcc {c_transformed_file} -o {self.compiled_program_name} -w'
        subprocess.run(compile_command, shell=True)

        output_file = f'{seed}.output.txt'
        run_command = f'./{self.compiled_program_name} > {output_file}'
        subprocess.run(run_command, shell=True)

    def cleanup(self):
        if os.path.exists(self.compiled_program_name):
            os.remove(self.compiled_program_name)

def verify(seed):
    expected_output_filename = f'{seed}.checksum.txt'
    result_output_filename = f'{seed}.output.txt'
    return filecmp.cmp(expected_output_filename, result_output_filename)

def cleanup(seed, transformator):
    for path in Path('.').glob(f'{seed}.*'):
        os.remove(path)
    transformator.cleanup()

def rename_files(seed, save_dir):
    os.rename(f'{save_dir}/{seed}.c', f'{save_dir}/raw.c')
    os.rename(f'{save_dir}/{seed}.checksum.txt', f'{save_dir}/expected_output.txt')
    os.rename(f'{save_dir}/{seed}.output.txt', f'{save_dir}/result_output.txt')
    os.rename(f'{save_dir}/{seed}.transform.c', f'{save_dir}/transformed.c')

def save_test_info(storage_path, seed):
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

    iteration = 0
    while iteration < MAX_ITERATION:
        print(f'Iteration {iteration}:')
        try:
            print('\tGenerating c program...')
            seed = csmith_gen.run()
            if not os.path.exists(f'{storage_path}/{seed}'):
                print('\tTransforming c program...')
                transformator.transform(seed)
                print('\tComparing checksums...')
                passed_test = verify(seed)
                if not passed_test:
                    print('\tTest failed :(')
                    print('\tSaving test info...')
                    save_test_info(storage_path, seed)
                else:
                    print('\tTest Passed :)')

                iteration += 1
            else:
                print('Test seed already exists. Skipping...')

            print('\tDone!', end='\n\n')
        finally:
            cleanup(seed, transformator)



if __name__ == '__main__':
    run()