import re
import matplotlib.pyplot as plt
import numpy as np

print("Hello World")

# test = np.loadtxt('lab10_data_trial_141.txt')
# print(test.shape())
Matrix = []

# Values to Edit (limited to the number of values inputed)
val_of_interest_time = 0 
val_of_interest_flex = 1
val_of_interest_FSR = 2
val_of_interest_EMG = 3

#Date frothe PuTTY Log
year = '2025'
month = '04'
day = '10'
time = '092121'

# Code to parse
f = open('AMIO_'+ year + '_' + month + '_' + day + '_' + time + '.txt', 'r')
# f = open('AMIO20250410092121.txt', 'r')
lines = f.readlines()
del lines[0] #delete time stamp
lines_split = lines[0].split(' ')
Matrix.append(lines_split)
print(Matrix[0][val_of_interest_EMG])
index, flex, FSR, EMG  = [], [], [], []
del lines[0] #delete headers 

#Need to delete the last two lines that are empty either with Python or manually
n = 2
del lines[-n:]

#Place data into lists
for row in lines:
    new_row = row.split(' ')
    index.append(float(new_row[val_of_interest_time]))
    flex.append(float(new_row[val_of_interest_flex]))
    FSR.append(float(new_row[val_of_interest_FSR]))
    EMG.append(float(new_row[val_of_interest_EMG]))
# print(t_now)

#Create and Display the plot
plt.figure()
plt.plot(index, flex, label='flex', linestyle='-', color='blue')
plt.plot(index, FSR, label='FSR', linestyle='--', color='red')
plt.plot(index, EMG, label='EMG', linestyle=':', color='green')
# plt.xlabel(f"{Matrix[0][val_of_interestX]}")
# plt.ylabel(f"{Matrix[0][val_of_interestY]}")
plt.xlabel("time")
plt.ylabel("values")
plt.title('Plot ' + time)

plt.show()