import time, subprocess, sys

startTime = round(time.time() * 1000)
print(f'Exe.file is {sys.argv[1]}, ')
subprocess.call(sys.argv[1])
print(f'The time execution is {round(time.time() * 1000) - startTime} ms')