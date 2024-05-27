from random import *
import os
import re

count_files = 20
repeats = 5
count_tests=100

def parse_profile():
    for i in range(count_files):
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

        print(f"search: {2**i} {avg_cumulative_seconds_search/count_tests}")
        print(f"insert: {2**i} {avg_cumulative_seconds_insert/count_tests}")
        print(f"delete: {2**i} {avg_cumulative_seconds_delete/count_tests}")
        print(f"print_postorder: {2**i} {avg_cumulative_seconds_print_postorder/count_tests}")
        print(f"max: {2**i} {avg_cumulative_seconds_max/count_tests}")


def gen_data_set():
    i=0
    n=1
    for i in range(count_files):
        with open(f"data_set/test_gen_{i}.txt",'w') as file:
            for _ in range(n):  
                rand=randint(0,100000000000)
                file.write(str(rand)+'\n')
                file.write(' \n')
        n*=2

def gen_test_gen():
    for i in range(count_files):
        with open(f"test_gen/test_gen_{i}.txt",'w') as file:
            file.write(f'f data_set/test_gen_{i}.txt\n')
            for _ in range(count_tests):  
                rand=randint(0,100000000000)
                file.write('i\n'+str(rand)+'\n\n')
                file.write('s\n'+str(rand)+'\n\n')
                file.write('r\n'+str(rand)+'\n\n')
                file.write('m\n')
                file.write('p\n')

def test():
    for i in range(count_files):
        for j in range(repeats):
            os.system(f"./bin_gprof < test_gen/test_gen_{i}.txt")
            os.system(f'gprof bin_gprof gmon.out > res_dir/result_{i}_{j}')
            os.system('rm gmon.out')
