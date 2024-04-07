from random import *

arr=[' ','a','0']

with open('test','w')as file:
    file.write('10000;')
    for _ in range(10000):
        tmp1=choice(arr)
        if tmp1=='0':
            tmp1=str(randint(-100,100))
        tmp2=choice(arr)
        if tmp2=='0':
            tmp2=str(randint(-100,100))
        tmp=tmp1+','+tmp2+';'
        file.write(tmp)
