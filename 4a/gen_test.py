from random import *
import os
import re

def parse_profile():
    for i in range(1,16):
        cumulative_seconds_search = []
        cumulative_seconds_insert = []
        cumulative_seconds__delete = []
        cumulative_seconds_print_postorder=[]
        cumulative_seconds_max=[]

        for j in range(10):
            with open(f'res_dir/result_{i}_{j}') as input_file:
                profile = [next(input_file) for _ in range(28)]
            profile = '\n'.join(profile)
            lines = profile.strip().split('\n')
            results = {}

            for line in lines:
                match = re.match(r'\s*\d+\.\d+\s+(\d+\.\d+)\s+(\d+\.\d+)\s+(\d+)\s+(\d+\.\d+)\s+(\d+\.\d+)\s+(\w+)', line)
                if match:
                    cumulative_seconds, self_seconds, calls, ms_per_call_self, ms_per_call_total, name = match.groups()
                    if name in ('insert', 'search', '_delete','print_postorder','max'):
                        results[name] = {
                            'cumulative_seconds': float(cumulative_seconds),
                        }

            if 'search' in results:
                cumulative_seconds_search.append(results['search']['cumulative_seconds'])
            if 'insert' in results:
                cumulative_seconds_insert.append(results['insert']['cumulative_seconds'])
            if '_delete' in results:
                cumulative_seconds__delete.append(results['_delete']['cumulative_seconds'])
            if 'print_postorder' in results:
                cumulative_seconds_print_postorder.append(results['print_postorder']['cumulative_seconds'])
            if 'max' in results:
                cumulative_seconds_max.append(results['max']['cumulative_seconds'])

        if cumulative_seconds_search:
            avg_cumulative_seconds_search = sum(cumulative_seconds_search) / len(cumulative_seconds_search)
        else:
            avg_cumulative_seconds_search = 0

        if cumulative_seconds_insert:
            avg_cumulative_seconds_insert = sum(cumulative_seconds_insert) / len(cumulative_seconds_insert)
        else:
            avg_cumulative_seconds_insert = 0

        if cumulative_seconds__delete:
            avg_cumulative_seconds__delete = sum(cumulative_seconds__delete) / len(cumulative_seconds__delete)
        else:
            avg_cumulative_seconds__delete = 0
        
        if cumulative_seconds_max:
            avg_cumulative_seconds_max = sum(cumulative_seconds_max) / len(cumulative_seconds_max)
        else:
            avg_cumulative_seconds_max = 0

        if cumulative_seconds_print_postorder:
            avg_cumulative_seconds_print_postorder = sum(cumulative_seconds_print_postorder) / len(cumulative_seconds_print_postorder)
        else:
            avg_cumulative_seconds_print_postorder = 0

        print(f"search: avg_cumulative_seconds= {avg_cumulative_seconds_search} calls = {2**i}")
        print(f"insert: avg_cumulative_seconds= {avg_cumulative_seconds_insert} calls = {2**i}")
        print(f"_delete: avg_cumulative_seconds= {avg_cumulative_seconds__delete} calls = {2**i}")
        print(f"print_postorder: avg_cumulative_seconds= {avg_cumulative_seconds_print_postorder} calls = {2**i}")
        print(f"max: avg_cumulative_seconds= {avg_cumulative_seconds_max} calls = {2**i}")


def gen():
    i=1
    count=2
    while i<=20:
        with open(f"data_set/test_gen_{i}.txt",'w') as file:
            for _ in range(count):  
                rand=randint(0,100000000000)
                file.write(str(rand)+'\n')
                file.write(' \n')
        i+=1
        count*=2
    i-=1
    while i>0:
        with open(f"test_gen/test_gen_{i}.txt",'w') as file:
            file.write(f'f data_set/test_gen_{i}.txt\n')
            for _ in range(1000):  
                rand=randint(0,100000000000)
                file.write('i '+str(rand)+' '+str(rand)+'\n')
                file.write('s '+str(rand)+' a\n')
                file.write('r '+str(rand)+' a\n')
                file.write('m\n')
                file.write('p\n')
        i-=1

def test():
    for i in range(1,21):
        for j in range(10):
            os.system(f"./bin_gprof < test_gen/test_gen_{i}.txt")
            os.system(f'gprof bin_gprof gmon.out > res_dir/result_{i}_{j}')
            os.system('rm gmon.out')

  