import os
import csv

def matchAccuracy(dataDict, filename):
    coarseRight = 0
    fineRight = 0
    with open(filename, 'r') as output:
        csvreader = csv.reader(output, delimiter = " ")
        for row in csvreader:
            if dataDict[row[0]]["coarse"] == row[1].strip("_") or dataDict[row[0]]["coarse"][:-1] == row[1].strip("_"):
                coarseRight += 1
                if dataDict[row[0]]["fine"].strip() == row[2].split("@")[1].strip():
                    fineRight += 1
    return [coarseRight/1213.0, fineRight/1213.0]

def getGroundTruth(filename):
    data = {}
    with open(filename, "r") as csvFile:
        csvreader = csv.reader(csvFile, delimiter = ",")
        for row in csvreader:
            row[0] = row[0].split(",")[0]
            tempDict = {}
            tempDict['coarse'] = row[3].strip()
            tempDict['fine'] = row[2].strip("_test").strip("_")
            data[row[0]] = tempDict
    return data

if __name__ == "__main__":
    testOutputFile = "./output.txt"
    groundTruthFile = "./filename-coarse-fine.csv"
    data = getGroundTruth(groundTruthFile)
    accuracies = matchAccuracy(data, testOutputFile)
    print("Coarse grained classification accuracy: ", accuracies[0])
    print("Fine grained classification accuracy: ", accuracies[1])
