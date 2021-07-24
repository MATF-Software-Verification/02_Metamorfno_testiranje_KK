#!/usr/bin/env python3

from csmith import csmith_gen
from csmith.timeout import saferun
import os
import shutil
import filecmp
from pathlib import Path
import random
import argparse
from typing import List, Tuple
import traceback
from abc import ABC, abstractmethod


class TransformationSequencePolicy(ABC):
    def __init__(self):
        self.transformations = ['do', 'while', 'for', 'o', 'if', 'switch', 'iter', 'u', 'goto']

    @abstractmethod
    def get_transformations_sequence(self) -> List[str]:
        pass

    def get_available_transformations(self) -> List[str]:
        return self.transformations


class RandomTransformationSequence(TransformationSequencePolicy):
    def __init__(self, num_of_transformations):
        super(RandomTransformationSequence, self).__init__()
        self.n = num_of_transformations

    def get_transformations_sequence(self):
        """
        Generise nasumicnu skvenci transformacija

        :return: Sekvenca transformacija
        """
        transformations = self.transformations 
        sequence = []
        hasO = False
        hasU = False

        for _ in range(self.n):
            t = random.choice(transformations)
            if t == 'o':
                r = random.randrange(2)
                t = f'{t}{r}'

                if hasO:
                    continue
                hasO = True
            if t == 'u':
                r = random.randrange(10, 50)
                t = f'{t}{r}'

                if hasU:
                    continue
                hasU = True
            sequence.append(t)
        return sequence


class SelectedTransformationsSequence(TransformationSequencePolicy):
    def __init__(self, transformations):
        super(SelectedTransformationsSequence, self).__init__()
        self.transformations_to_return = [t.strip() for t in transformations]

    def get_transformations_sequence(self) -> List[str]:
        for t in self.transformations_to_return:
            if t.startswith('o'):
                num = int(t[1:])
                assert num == 0 or num == 1
            elif t.startswith('u'):
                num = int(t[1:])
                assert 10 <= num < 50
        return self.transformations_to_return


class Transformator:
    """
    Okvir za transformacije.
    """
    def __init__(self,
                 verbosity: int,
                 compiler: str,
                 compiler_options: str,
                 trans_seq: str,
                 max_run_duration: int,
                 parallel_cmake_jobs: int):

        self.compiled_program_name = 'run.out'
        self.verbosity = verbosity
        self.compiler = compiler
        self.compiler_options = compiler_options
        self.transformation_policy = RandomTransformationSequence(int(trans_seq)) \
            if trans_seq.isnumeric() \
            else SelectedTransformationsSequence(trans_seq.split(','))
        self.max_run_duration = max_run_duration
        self.parallel_cmake_jobs = parallel_cmake_jobs

        self._initialize()

    def _initialize(self) -> None:
        """
        Da bi transformator mogao da radi, neophodno je da se kompiliraju transformacije.
        Uvek se automatski azuriraju promene u transformacijama.
        """
        build_path = f'build'
        self._trace('Prevodjenje transformator biblioteke!')
        if not os.path.exists(f'{build_path}/trans'):
            Path(build_path).mkdir(parents=True, exist_ok=True)

        owd = os.getcwd()
        os.chdir(build_path)

        build_command = f'cmake -G "Unix Makefiles" --parallel {self.parallel_cmake_jobs} ../trans'
        os.system(build_command)

        os.system('make')

        os.chdir(owd)

    def _trace(self, content: str, verbosity: int = 0, *args, **kwargs) -> None:
        if verbosity <= self.verbosity:
            print(f'[verify-transformator]: {content}', *args, **kwargs)

    def transform(self, seed: int) -> Tuple[bool, List[str]]:
        """
        Transformise C program koristeci nasumican niz transformacija.

        Koraci:
        1. Transformacija C program koristeci C++ kompiliran program za transformacije.
        2. Kompilacija izabrane C datoteke (uglavnom CSmith generisana)
        3. Cuvanje izlaza.

        :param seed: seme
        :return: 
            1. True, ako transformisan program sigurno nema beskonacnu petlju, a False inace
            2. Sekvenca transformacija
        """
        c_file = f'{seed}.c'
        c_file_duplicate = f'{seed}.dup.c'
        c_transformed_file = f'{seed}.transform.c'
        trans_path = f'build/trans'

        # 1
        shutil.copyfile(c_file, c_file_duplicate)
        self._trace('Transformacija generisanog C programa!', verbosity=1)
        sequence = self.transformation_policy.get_transformations_sequence() 
        with open(f'{seed}.trans.sequence.txt', 'w') as tseq_file:
            for transformation in sequence:
                tseq_file.write(f'{transformation}\n')
                self._trace(f'Sledeca transformacija je "{transformation}".', verbosity=1)
                transform_command = f'./{trans_path} {c_file_duplicate} tmp.c {transformation}'
                os.system(transform_command)
                os.rename('tmp.c', c_file_duplicate)
            os.rename(c_file_duplicate, c_transformed_file)

        # 2
        # Opcija '-w' iskljucuje upozorenja
        self._trace('Kompilacija transformisanog, generisanog C programa!', verbosity=1)
        compile_command = f'{self.compiler} {c_transformed_file}' \
                          + f' -o {self.compiled_program_name} -w {self.compiler_options}'
        os.system(compile_command)

        # 3
        self._trace('Pokretanje transformisanog, generisanog C programa!', verbosity=1)
        output_file = f'{seed}.output.txt'
        run_command = f'./{self.compiled_program_name} > {output_file}'

        finished = saferun(run_command, self.max_run_duration)
        return finished, sequence

    def cleanup(self) -> None:
        """
        Brise privremene datoteke i ubija prezivele zombije.
        """
        self._trace('Bacanje djubreta...', verbosity=1)
        if os.path.exists(self.compiled_program_name):
            os.remove(self.compiled_program_name)

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
    Ova funkcija proverava da li su sadrzaji dve datoteke jednaki.
    Ako su sadrzaji dve datoteke jednaki, onda je test prosao (isti je izlaz oba programa)

    :param seed: seme
    :return: True, ako su sadrzaji obe datoteke jednaki, a False inace
    """
    expected_output_filename = f'{seed}.checksum.txt'
    result_output_filename = f'{seed}.output.txt'
    return filecmp.cmp(expected_output_filename, result_output_filename)


def cleanup(seed: int) -> None:
    """
    Brisanje privremenih datoteka (na globalnom nivou)

    :param seed: seme
    """
    if seed is not None:
        for path in Path('.').glob(f'{seed}.*'):
            os.remove(path)


def rename_files(seed: int, save_dir: str) -> None:
    """
    Preimenuje sacuvane datoteke tako da su im imena intuitivnija.

    :param seed: seme
    :param save_dir: Putanja gde su datoteke sacuvane.
    """
    os.rename(f'{save_dir}/{seed}.c', f'{save_dir}/raw.c')
    os.rename(f'{save_dir}/{seed}.checksum.txt', f'{save_dir}/expected_output.txt')
    os.rename(f'{save_dir}/{seed}.output.txt', f'{save_dir}/result_output.txt')
    os.rename(f'{save_dir}/{seed}.transform.c', f'{save_dir}/transformed.c')
    os.rename(f'{save_dir}/{seed}.trans.sequence.txt', f'{save_dir}/sequence.txt')


def save_test_info(storage_path: str, seed: int) -> None:
    """
    Cuva informacije o testovima u skladiste (storage).

    :param storage_path: Putanja gde se cuvaju informacije o neuspelim testovima.
    :param seed: seme
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
    parser.add_argument('--compiler', help='gcc or clang', default='gcc', type=str, choices=['gcc', 'clang'])
    parser.add_argument('--compiler-options', help='Compiler options', type=str, default='')
    parser.add_argument('--trans-seq', help='Length of transformation sequence if value provided is a number.' +
                                            'Or the exact transfomration sequence if --trans=seq="while, goto, ' +
                                            'for" for example', type=str, default="3")
    parser.add_argument('--tests', help='Number of tests', type=int, default=3)
    parser.add_argument('--max-duration', help='Maximum program time duration', type=int, default=5)
    parser.add_argument('--parallel_cmake_jobs', help='Number of parallel cmake jobs to build trans library', type=int,
                        default=1)
    args = parser.parse_args()

    storage_path = 'storage'
    if not os.path.exists(storage_path):
        os.mkdir(storage_path)

    transformator_params = {
        'verbosity': args.verbosity,
        'compiler': args.compiler,
        'compiler_options': args.compiler_options,
        'trans_seq': args.trans_seq,
        'max_run_duration': args.max_duration,
        'parallel_cmake_jobs': args.parallel_cmake_jobs
    }

    with Transformator(**transformator_params) as transformator:
        seed = None
        iteration = 1
        max_iteration = args.tests

        test_history = {}

        while iteration <= max_iteration:
            trace(f'Iteracija {iteration}:')
            try:
                trace('Generisanje C programa...')
                seed = csmith_gen.run(
                    compiler=args.compiler,
                    compiler_options=args.compiler_options,
                    max_run_duration=args.max_duration
                )
                if not os.path.exists(f'{storage_path}/{seed}'):
                    trace('Transformacija c program...')
                    passed_test, sequence = transformator.transform(seed)
                    if passed_test:  # Proslo je izvrsavanje programa
                        trace('Uporedjivanje sumi...')
                        passed_test = verify(seed)
                    if not passed_test:  # Nije proslo izvrsavanje programa
                        trace('Test nije prosao :(')
                        trace('Cuvanje informacija o testu...')
                        save_test_info(storage_path, seed)
                    else:
                        trace('Test je prosao :)')
                    test_history[seed] = passed_test, sequence

                    iteration += 1
                else:
                    print('Test vec postoji u skladistu, preskace se...')

                trace('Kraj!', end='\n\n')
            finally:
                cleanup(seed)

        trace('Rezultati testa...')
        n_passed_test = 0
        for seed, test_info in test_history.items():
            passed_test, sequence = test_info
            if not passed_test:
                trace(f'Test koji ima seme "{seed}" nije prosao... Sekvenca transformacija: {sequence}')
            else:
                n_passed_test += int(passed_test)

        trace(f'Prolaznost: {n_passed_test}/{max_iteration}')


if __name__ == '__main__':
    run()
