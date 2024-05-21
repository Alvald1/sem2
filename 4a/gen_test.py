from random import *
import os
import re

def parse_profile():
    for i in range(20):
        cumulative_seconds_tree_search = []
        calls_tree_search = []
        ms_per_call_total_tree_search = []
        cumulative_seconds_tree_insert = []
        calls_tree_insert = []
        ms_per_call_total_tree_insert = []
        cumulative_seconds_tree_delete = []
        calls_tree_delete = []
        ms_per_call_total_tree_delete = []

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
                    if name in ('tree_insert', 'tree_search', 'tree_delete'):
                        results[name] = {
                            'cumulative_seconds': float(cumulative_seconds),
                            'calls': int(calls),
                            'ms_per_call_total': float(ms_per_call_total)
                        }

            if 'tree_search' in results:
                cumulative_seconds_tree_search.append(results['tree_search']['cumulative_seconds'])
                calls_tree_search.append(results['tree_search']['calls'])
                ms_per_call_total_tree_search.append(results['tree_search']['ms_per_call_total'])
            if 'tree_insert' in results:
                cumulative_seconds_tree_insert.append(results['tree_insert']['cumulative_seconds'])
                calls_tree_insert.append(results['tree_insert']['calls'])
                ms_per_call_total_tree_insert.append(results['tree_insert']['ms_per_call_total'])
            if 'tree_delete' in results:
                cumulative_seconds_tree_delete.append(results['tree_delete']['cumulative_seconds'])
                calls_tree_delete.append(results['tree_delete']['calls'])
                ms_per_call_total_tree_delete.append(results['tree_delete']['ms_per_call_total'])

        avg_cumulative_seconds_tree_search = sum(cumulative_seconds_tree_search) / len(cumulative_seconds_tree_search)
        avg_calls_tree_search = sum(calls_tree_search) / len(calls_tree_search)
        avg_ms_per_call_total_tree_search = sum(ms_per_call_total_tree_search) / len(ms_per_call_total_tree_search)

        avg_cumulative_seconds_tree_insert = sum(cumulative_seconds_tree_insert) / len(cumulative_seconds_tree_insert)
        avg_calls_tree_insert = sum(calls_tree_insert) / len(calls_tree_insert)
        avg_ms_per_call_total_tree_insert = sum(ms_per_call_total_tree_insert) / len(ms_per_call_total_tree_insert)

        avg_cumulative_seconds_tree_delete = sum(cumulative_seconds_tree_delete) / len(cumulative_seconds_tree_delete)
        avg_calls_tree_delete = sum(calls_tree_delete) / len(calls_tree_delete)
        avg_ms_per_call_total_tree_delete = sum(ms_per_call_total_tree_delete) / len(ms_per_call_total_tree_delete)

        print(f"tree_search: avg_cumulative_seconds= {avg_cumulative_seconds_tree_search} avg_calls= {avg_calls_tree_search} avg_ms_per_call_total= {avg_ms_per_call_total_tree_search}\n")
        print(f"tree_insert: avg_cumulative_seconds= {avg_cumulative_seconds_tree_insert} avg_calls= {avg_calls_tree_insert} avg_ms_per_call_total= {avg_ms_per_call_total_tree_insert}\n")
        print(f"tree_delete: avg_cumulative_seconds= {avg_cumulative_seconds_tree_delete} avg_calls= {avg_calls_tree_delete} avg_ms_per_call_total= {avg_ms_per_call_total_tree_delete}\n")


def gen():
    arr=set()
    count=10000
    for i in range(20):
        with open(f"data_set/test_gen_{i}.txt",'w') as file:
            for _ in range(count*i):  
                file.write('i ')
                rand=randint(0,1000000000)
                arr.add(rand)
                file.write(str(rand)+' ')
                file.write(str(rand)+'\n')
            for j in arr:
               file.write('r ') 
               file.write(str(j)+'\n')

def test():
    for i in range(20):
        for j in range(10):
            os.system(f"./bin_gprof < data_set/test_gen_{i}.txt")
            os.system(f'gprof bin_gprof gmon.out > res_dir/result_{i}_{j}')
            os.system('rm gmon.out')

        
        