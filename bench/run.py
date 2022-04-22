import subprocess
import threading
import os 
print(os.path.dirname(os.path.realpath(__file__)))

N = 12
N_threads = 12

def run_threads(thread_name):
    for i in range(N // N_threads):
        proc = subprocess.Popen("./build/mu " + "build/run.mac " + "out_" + str(thread_name) + "_" + str(i), shell=True, stdout=subprocess.DEVNULL)
        proc.wait()
        print(thread_name, i)

threads = []

for i in range(N_threads):
    th = threading.Thread(target=run_threads, args=(i,))
    th.start()
    threads.append(th)
for th in threads:
    th.join()