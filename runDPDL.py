import subprocess
import sys
import time


def param(type, start, end, heur=999, timeout_value=600):
    for i in range(start, end + 1):

        if heur == 999:
            for h in range(4):
                try:
                    subprocess.run(
                        ['./main', f'{type}{i}', f'{h}'], timeout=timeout_value)
                    time.sleep(1)
                except subprocess.TimeoutExpired:
                    print(
                        f"Timeout of {timeout_value} seconds reached.")
            print("-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------")

        else:
            try:
                subprocess.run(
                    ['./main', f'{type}{i}', f'{heur}'], timeout=timeout_value)
                time.sleep(1)
            except subprocess.TimeoutExpired:
                print(
                    f"Timeout of {timeout_value} seconds reached for subprocess with input {type}{i}.")


if __name__ == "__main__":
    heur_value = 999
    if(len(sys.argv) == 5):
        script_name, script_type, start_value, end_value, heur_value = sys.argv
        try:
            start_value, end_value, heur_value = map(
                int,(start_value, end_value, heur_value))
        except ValueError:
            print("Error: start, end, heur, and timeout must be integers.")
            sys.exit(1)
    else:
        script_name, script_type, start_value, end_value = sys.argv
        try:
            start_value, end_value = map(
                int,(start_value, end_value))
        except ValueError:
            print("Error: start, end, heur, and timeout must be integers.")
            sys.exit(1)

    param(script_type, start_value, end_value, heur_value)
