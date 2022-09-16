import csv
import operator

file = open("dataMPI.csv", "r")

csv_data = csv.reader(file, delimiter = "\t")

sort = sorted(csv_data, key = lambda row: int(row[1]), reverse = False)

for each in sort:
    print(each)

file.close()
file = open("dataMPI.csv", "w")
file.truncate()

csv_data = csv.writer(file, delimiter = "\t")
csv_data.writerows(sort)

file.close()