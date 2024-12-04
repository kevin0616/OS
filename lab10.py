import sys
import random
import matplotlib.pyplot as plt


def PageTraceGenerator(n, p):
    candidate = [i for i in range(p)]
    page_trace = []
    for i in range(n):
        page_trace.append(random.choice(candidate))
    #print(page_trace)
    return page_trace

pointer = 0

def FindVictom(page_table, f, p):
    global pointer
    tmp_frame = [-1 for i in range(f)]
    for i in range(p):
        if page_table[i][0] != -1:
            tmp_frame[page_table[i][0]] = i

    while page_table[tmp_frame[pointer]][2]:
        page_table[tmp_frame[pointer]][2] = 0
        pointer = (pointer+1) % f
     
    return tmp_frame[pointer]

def Simulate(page_trace, f, p):
    num_frames_avail = f
    frames_list = [i for i in range(num_frames_avail)]
    page_table = [[-1, 0, 0] for i in range(p)]
    num_page_faults = 0

    for pt in page_trace:
        #print(page_table)
        if page_table[pt][1]:
            page_table[pt][2]=1
        else:
            num_page_faults+=1
            if frames_list != []:
                frame = frames_list[0]
                frames_list.pop(0)
                page_table[pt] = [frame, 1, 1]
            else:
                victom = FindVictom(page_table, f, p)
                frame = page_table[victom][0]
                page_table[victom] = [-1, 0, 0]
                page_table[pt] = [frame, 1, 1]

    return num_page_faults

def main():
    n = int(sys.argv[1])
    p = int(sys.argv[2])
    page_trace = PageTraceGenerator(n, p)
    x = []
    y = []
    for f in range(4,p+1):
        global pointer
        pointer = 0
        num_page_faults = Simulate(page_trace, f, p)
        print(f, num_page_faults)
        x.append(f)
        y.append(num_page_faults)
    plt.plot(x, y)
    plt.show()

if __name__ == "__main__":
    main()