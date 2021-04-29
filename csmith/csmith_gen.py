#!/usr/bin/env python3

import os
import re
import sys
import random
import signal
import subprocess
from typing import List, Tuple


class Timeout:
    """
    Pomocna funkcija koja zaustavlja rad programa nakon X sekundi ako rad tog programa nije zavrsen.
    Koristi se da se filtriraju programi koji sigurno nemaju beskonacnu petlju.
    Postoji sansa da se program prekine pod pretpostavkom da ima beskonacnu petlju, a samo predugo traje.
    """
    def __init__(self, seconds: int = 1, error_message: str = 'TimeoutError'):
        self.seconds = seconds
        self.error_message = error_message

    def handle_timeout(self, signum: int, frame: object):
        raise TimeoutError(self.error_message)

    def __enter__(self):
        signal.signal(signal.SIGALRM, self.handle_timeout)
        signal.alarm(self.seconds)

    def __exit__(self, exc_type, exc_value, tb):
        signal.alarm(0)


MAX_RUN_DURATION = 5


def trace(content: str, *args, **kwargs):
    print(f'[csmith-gen]: {content}', *args, **kwargs)


def get_csmith_include() -> str:
    """
    Pronalazi relativnu putanju u zavisnosti od vrednosti `CSMITH_PATH` promenljive okruzenja.

    CSmith moze da vrsi kompilaciju komandom `<gcc/clang> <cfile> -I$CSMITH_PATH`,
    ali AST biblioteka ne podrzava `-I` opciju. Ovaj problem se resava tako sto:

    `#include "csmith.h"`

    sa

    `include "[REL_PATH]/csmith.h"`

    :return: `include "[REL_PATH]/csmith.h"`
    """
    assert os.environ['CSMITH_PATH'] is not None, 'Nije podesena "CSMITH_PATH" promenljiva okruzenja!'
    csmith_abs_path = os.environ['CSMITH_PATH']
    script_abs_path = os.path.abspath('.')
    csmith_include_file = 'csmith.h'
    
    csmith_relative_path = os.path.relpath(csmith_abs_path, script_abs_path)
    csmith_include = f'{csmith_relative_path}/{csmith_include_file}'
    return csmith_include


def run_csmith(args: List[str]) -> Tuple[str, int, List[str]]:
    """
    Generise nasumican C program uz pomoc CSmith alata.

    Ovaj skript moze da prihvata sve dodatne opcije koje moze da prima i CSmith alat, jer je predstavlja samo okvir.

    Nakon prvog pokretanja se brise opcija '-s' (u slucaju da to seme izaziva beskonacnu petlju, mora da se izbaci).

    :param args: Argumenti komandne linije bez prvog argumenta (ime programa).
    :return: Ime izlaznog direktorijuma, novo (staro) seme, novi (stari) args
    """
    csmith_args = []

    # Ako seme nije izabrano opcijom `-s`, onda se generise novo nasumicno seme
    try:
        seed_index = args.index('-s')
        assert len(args) > seed_index+1, 'Nedostaje broj nakon `-s` opcije!'
        seed = int(args[seed_index+1])

        # Samo je jednom moguce iskoristiti seme
        del args[seed_index+1]
        del args[seed_index]
    except ValueError:
        seed = random.randrange(sys.maxsize)

    # csmith odbacuje sve argumente sem '-s' i '-o'
    csmith_args.append('-s')
    csmith_args.append(str(seed))

    trace(f'Seme programa je {seed}.')

    # Ako se ne izabare ime izlaza, onda ce to ime da odgovara semenu
    try:
        output_filename_index = args.index('-o')+1
        assert len(args) > output_filename_index, 'Nedostaje ime nakon `-o` opcije!'
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
    Zamena `#include "csmith.h"` sa `include "[csmith_include]"` u `output_filename`.
    Pogledati `get_csmith_include()` funkciju.

    :param output_filename: CSmith generisana datoteka
    :param csmith_include: Relativna putanja CSmith biblioteke
    """
    with open(output_filename, 'r') as f:
        code = f.read()

    # Podesavanja `include` putanje i preslikavanje tipova
    # CSmith generisani tipovi promenljivih nisu kompatabilni sa Clang bibliotekom
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
    CSmith moze da generise C program sa beskonacnom petljom.
    Svi programi koji se izvrsavaju duze od [MAX_RUN_DURATION] se odbacuju.

    Izlaz:
    U `[seed].c` se nalazi program.
    U `[seed].checksum.txt` se nalazi izlaz programa.

    :param compiler: clang/gcc
    :param output_filename: CSmith generisani C program
    :param compiler_options: Opcije kompilatora
    :param max_run_duration: Granica za odbacivanje programa koji mozda imaju beskonacnu petlju.
    :return: True, ako program sigurno nema beskonacnu, a inace False
    """
    compiled_file_name = 'csmith.out'
    # Opcija '-w' iskljucuje upozerenja
    compile_command = f'{compiler} {output_filename} -o {compiled_file_name} -w {compiler_options}'
    os.system(compile_command)

    run_command = f'./{compiled_file_name}'

    filename = output_filename[:-2]
    warn_filename = filename + '.warn.txt'
    checksum_file = filename + '.checksum.txt'
    with Timeout(seconds=max_run_duration, error_message='Programu treba previse dugo da se izvrsi!'):
        try:
            process = subprocess.Popen(f'{run_command} > {checksum_file}', shell=True)
            process.communicate()
        except TimeoutError:
            trace('Programu je istekao mandat...')
            # Ako je mozda zombi proces ostao, pravi se dodatni pokusaj ubistva strasti
            os.kill(process.pid, signal.SIGKILL)
            # Brisanje datoteka programa koji se odbacuje
            for file in [output_filename, checksum_file, warn_filename]:
                if os.path.exists(file):
                    os.remove(file)
            return False

    trace('Generisan je novi program!')
    # Ciscenje
    os.remove(compiled_file_name)
    return True


def kill_remaining_zombies(keyword: str):
    """
    Kako klasa Timeout nije dovoljno pouzdana za ubijanje zombija u nekim situacijama i kako
    dodatan pokusaj ubijanja zombija signalom u procesu ciscenja nije dovoljan, listaju se svi programi
    koji su pokrenuti sa odgovarajucom kljucnom reci i ubijaju (jedan po jedan).

    Ova funkcija se pokazala kao najpouzdanija za otklnjanje zombi procesa.

    :param keyword: Kljucna rec (komanda kojom se poziva program)
    """
    zombie_filename = 'zombies.txt'
    list_zombies_command = f'ps -ef | grep {keyword} > {zombie_filename}'
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
        output_filename, seed, args = run_csmith(args)
        replace_csmith_include(output_filename, csmith_include)
        passed_test = test_generated_c_code(compiler, output_filename, compiler_options, max_run_duration)

    kill_remaining_zombies('csmith.out')
    return seed


if __name__ == '__main__':
    run()
