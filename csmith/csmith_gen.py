#!/usr/bin/env python3

import os
import re
import sys
import random
from typing import List, Tuple
from timeout import saferun


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

    completed = saferun(f'{run_command} > {checksum_file}', max_run_duration)
    if not completed:
        trace('Programu je istekao mandat!')
        for file in [output_filename, checksum_file, warn_filename]:
            if os.path.exists(file):
                os.remove(file)

    trace('Generisan je novi program!')
    # Ciscenje
    os.remove(compiled_file_name)
    return True


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

    return seed


if __name__ == '__main__':
    run()
