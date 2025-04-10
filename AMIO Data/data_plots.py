import re
import matplotlib.pyplot as plt
import numpy as np

print("Hello World")

# test = np.loadtxt('lab10_data_trial_141.txt')
# print(test.shape())
Matrix = []

# Values to Edit (limited to the number of values inputed)
val_of_interestX = 0 
val_of_interestY = 1

#Date frothe PuTTY Log
year = '2025'
month = '04'
day = '09'
time = '231156'

# Code to parse
f = open('AMIO_'+ year + '_' + month + '_' + day + '_' + time + '.txt', 'r')
lines = f.readlines()
del lines[0] #delete time stamp
lines_split = lines[0].split(' ')
Matrix.append(lines_split)
print(Matrix[0][val_of_interestY])
X, Y,  = [], []
del lines[0] #delete headers 

#Need to delete the last two lines that are empty either with Python or manually
n = 2
del lines[-n:]

#Place data into lists
for row in lines:
    new_row = row.split(' ')
    X.append(float(new_row[val_of_interestX]))
    Y.append(float(new_row[val_of_interestY]))
# print(t_now)

#Create and Display the plot
plt.plot(X, Y)
plt.xlabel(f"{Matrix[0][val_of_interestX]}")
plt.ylabel(f"{Matrix[0][val_of_interestY]}")
plt.title('Plot ' + time)

plt.show()