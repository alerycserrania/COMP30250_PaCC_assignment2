import math
import os
import csv
import subprocess

NB_ITERATION = 25
SIZES = [2, 4, 8, 16, 32, 64, 128, 256, 512, 1024]

FILENAMES_NON_BLOCK = ['./non_blocked_ijk.out', './natural_blas.out']
FILENAMES_BLOCK = ['./blocked_ijk_atlas.out', './blocked_kij_atlas.out']

def factorize(num):
    return (n for n in range(2, num + 1) if num % n == 0)


resultfile = open('data.csv', 'w')
csvwriter = csv.writer(resultfile, delimiter=',', quotechar='|', quoting=csv.QUOTE_MINIMAL)
csvwriter.writerow(['filename', 'matrix_size', 'block_size', 'timing'])


for filename in FILENAMES_NON_BLOCK:
    for size in SIZES:
        total_elapsed = 0.0
        for i in range(NB_ITERATION):
            output = subprocess.run([filename, str(size), 'n', 'y'], stdout=subprocess.PIPE).stdout.decode('utf-8')
            total_elapsed += float(output.split(': ')[-1][:-2])
        csvwriter.writerow([filename, size, 1, total_elapsed / NB_ITERATION])
        print([filename, size, 1, total_elapsed / NB_ITERATION])

for filename in FILENAMES_BLOCK:
    for size in SIZES:
        for block_size in factorize(size):
            total_elapsed = 0.0
            for i in range(NB_ITERATION):
                output = subprocess.run([filename, str(size), str(block_size), 'n', 'y'], stdout=subprocess.PIPE).stdout.decode('utf-8')
                total_elapsed += float(output.split(': ')[-1][:-2])
            csvwriter.writerow([filename, size, block_size, total_elapsed / NB_ITERATION])
            print([filename, size, block_size, total_elapsed / NB_ITERATION])

resultfile.close()