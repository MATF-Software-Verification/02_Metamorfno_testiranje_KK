import os
import subprocess
from unittest import TestCase
import time
from timeout import Timeout, saferun


class TestTimeout(TestCase):
    def test_Timeout_no_timeout(self):
        start_time = time.time()
        command = 'sleep 1'

        with Timeout(seconds=2, error_message='Times up!'):
            try:
                subprocess.run(command, shell=True)
            except TimeoutError:
                self.fail()

        self.assertTrue(time.time() - start_time < 2)

    def test_Timeout_timeout(self):
        start_time = time.time()
        command = 'sleep 10'

        with Timeout(seconds=1, error_message='Times up!'):
            try:
                subprocess.run(command, shell=True)
            except TimeoutError:
                self.assertTrue(time.time() - start_time > 1)
                return
        self.fail()

    def test_saferun(self):
        # Arrange
        command = 'sleep 2'

        # Act
        completed1 = saferun(command, 1)
        completed2 = saferun(command, 3)

        # Assert
        self.assertFalse(completed1)
        self.assertTrue(completed2)

        zombie_filename = 'zombies.txt'
        list_zombies_command = f'ps -ef | grep "{command}" > {zombie_filename}'
        subprocess.run(list_zombies_command, shell=True)

        with open(zombie_filename, 'r') as f:
            lines = f.readlines()
            for line in lines:
                self.assertTrue(list_zombies_command in line or 'grep' in line)

        # Cleanup
        os.remove(zombie_filename)
