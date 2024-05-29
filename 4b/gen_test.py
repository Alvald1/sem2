from random import *
import os
import re
from subprocess import *

count_data_sets = 20
repeats = 10
count_tests=1000
step=10000

def parse_profile():
    for i in range(count_data_sets):
        cumulative_seconds_search = []
        cumulative_seconds_insert = []
        cumulative_seconds_delete = []
        cumulative_seconds_print_postorder = []
        cumulative_seconds_max = []

        for j in range(repeats):
            try:
                with open(f'res_dir/result_{i}_{j}') as input_file:
                    profile = [next(input_file) for _ in range(39)]
                profile = '\n'.join(profile)
            except FileNotFoundError:
                print(f"File res_dir/result_{i}_{j} not found.")
                continue

            lines = profile.strip().split('\n')
            results = {}

            for line in lines:
                match = re.match(r'\s*\d+\.\d+\s+(\d+\.\d+)\s+(\d+\.\d+)\s+(\d+)\s+(\d+\.\d+)\s+(\d+\.\d+)\s+(\w+)', line)
                if match:
                    cumulative_seconds, self_seconds, calls, ms_per_call_self, ms_per_call_total, name = match.groups()
                    if name in ('insert', 'search', 'delete', 'print_postorder', 'max'):
                        results[name] = {
                            'cumulative_seconds': float(cumulative_seconds),
                        }

            if 'search' in results:
                cumulative_seconds_search.append(results['search']['cumulative_seconds'])
            if 'insert' in results:
                cumulative_seconds_insert.append(results['insert']['cumulative_seconds'])
            if 'delete' in results:
                cumulative_seconds_delete.append(results['delete']['cumulative_seconds'])
            if 'print_postorder' in results:
                cumulative_seconds_print_postorder.append(results['print_postorder']['cumulative_seconds'])
            if 'max' in results:
                cumulative_seconds_max.append(results['max']['cumulative_seconds'])

        avg_cumulative_seconds_search = sum(cumulative_seconds_search) / len(cumulative_seconds_search) if cumulative_seconds_search else 0
        avg_cumulative_seconds_insert = sum(cumulative_seconds_insert) / len(cumulative_seconds_insert) if cumulative_seconds_insert else 0
        avg_cumulative_seconds_delete = sum(cumulative_seconds_delete) / len(cumulative_seconds_delete) if cumulative_seconds_delete else 0
        avg_cumulative_seconds_print_postorder = sum(cumulative_seconds_print_postorder) / len(cumulative_seconds_print_postorder) if cumulative_seconds_print_postorder else 0
        avg_cumulative_seconds_max = sum(cumulative_seconds_max) / len(cumulative_seconds_max) if cumulative_seconds_max else 0

        print(f"search: {step * i} {avg_cumulative_seconds_search/count_tests}")
        print(f"insert: {step * i} {avg_cumulative_seconds_insert/count_tests}")
        print(f"delete: {step * i} {avg_cumulative_seconds_delete/count_tests}")
        print(f"print_postorder: {step * i} {avg_cumulative_seconds_print_postorder/count_tests}")
        print(f"max: {step * i} {avg_cumulative_seconds_max/count_tests}")


def data_sets():
    for i in range(count_data_sets):
        with open(f"data_sets/{i}",'w') as file:
            for _ in range(step*i):  
                rand=randint(0,100000000000)
                file.write(f'\n{rand}\n{rand}')

def tests():
    for i in range(count_data_sets):
        with open(f"tests/{i}",'w') as file:
            file.write(f'f\ndata_sets/{i}\n')
            for _ in range(count_tests):  
                rand=randint(0,100000000000)
                file.write(f'i\n{rand}\n{rand}\n')
                file.write(f's\n{rand}\n')
                file.write(f'r\n{rand}\n1\n')
                file.write('S\np\n')
                file.write(f'o\n{randint(0,100000000000)}\n{randint(0,100000000000)}\n')

def run():
    for i in range(count_data_sets):
        for j in range(repeats):
            with open(f"tests/{i}",'r') as stdin, open(f'res_dir/result_{i}_{j}', 'w') as stdout:
                call('./bin_gprof',stdin=stdin,stdout=DEVNULL,stderr=DEVNULL)
                call(['gprof','bin_gprof','gmon.out'],stdout=stdout)
                call(['rm','gmon.out'])
                print(f'i = {i} j = {j}')
