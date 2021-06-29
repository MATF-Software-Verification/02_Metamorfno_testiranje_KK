import signal
import subprocess
import os


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


def kill_remaining_zombies(keyword: str) -> None:
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


def saferun(command: str, max_run_duration: int) -> bool:
    """
    Uokviruje proces izvrsavanje komande sa ogranicenim vremenom i unistavanje
    zombi procesa koji mogu potencijalno da ostanu nakon pokretanja `subprocess` komdande

    :param command: Komanda koja se izvrsava
    :param max_run_duration: Dozvoljena duzina za izvrsavanje komance
    :return: Da li je komanda uspesno izvrsena
    """
    with Timeout(seconds=max_run_duration, error_message='Programu treba previse dugo da se izvrsi!'):
        try:
            process = subprocess.Popen(command, shell=True)
            process.communicate()
        except TimeoutError:
            kill_remaining_zombies(command)
            return False
    return True


