#include <iostream>
#include <vector>
#include <string>

std::vector<int> editBinMap(std::vector<int> binMap, int selectionBarLeft, int selectionBarRight, int destinationBarLeft, int destinationBarRight)
{
    int ss = selectionBarLeft;
    int se = selectionBarRight;
    int ds = destinationBarLeft;
    int de = destinationBarRight;
    int blockSize = se - ss;
    bool movedRight = ds > ss;

    if (ds == ss) // No change
    {
        return binMap;
    }

    std::vector<int> newBinMap(binMap.size(), -1);

    // SET THE BLOCK
    for (int i = ss; i < se; ++i)
    {
        int index = find(binMap.begin(), binMap.end(), i) - binMap.begin();
        newBinMap[index] = ds + (i - ss);
        std::cout << index << " mapped to " << newBinMap[index] << std::endl;
    }

    // SET THE REST
    for (int i = 0; i < binMap.size(); ++i)
    {
        int value = binMap[i];
        if (((value < ss || value >= de) && movedRight) || ((value < ds || value >= se) && !movedRight)) // if outside of action, dont move
        {
            newBinMap[i] = value;
        }
        else if (value >= ss && value < se) // in the block
        {
            ;
        }
        else // outside the block
        {
            if (movedRight)
            {
                newBinMap[i] = value - blockSize;
            }
            else
            {
                newBinMap[i] = value + blockSize;
            }
        }
    }





    return newBinMap;
} 

void printBinMap(std::vector<int> binMap)
{
    for (int i = 0; i < binMap.size(); ++i)
    {
        
        std::cout << binMap[i] << (i < binMap.size() - 1 ? " " : "\n");
    }
}

int main() {
    std::vector<int> binMap;
    for (int i = 0; i < 10; ++i)
    {
        binMap.push_back(i);
    }

    std::cout << "Original: ";
    printBinMap(binMap);
    binMap = editBinMap(binMap, 0, 2, 7, 9);
    std::cout << "Recieved: ";
    printBinMap(binMap);
    std::cout << "Expected: 7 8 0 1 2 3 4 5 6 9" << std::endl << std::endl;

    std::cout << "Original: ";
    printBinMap(binMap);
    binMap = editBinMap(binMap, 6, 8, 4, 6);
    std::cout << "Recieved: ";
    printBinMap(binMap);
    std::cout << "Expected: 5 8 0 1 2 3 6 7 4 9" << std::endl << std::endl;

    std::cout << "Original: ";
    printBinMap(binMap);
    binMap = editBinMap(binMap, 3, 5, 8, 10);
    std::cout << "Recieved: ";
    printBinMap(binMap);
    std::cout << "Expected: 3 6 0 1 2 8 4 5 9 7" << std::endl << std::endl;

    std::cout << "Original: ";
    printBinMap(binMap);
    binMap = editBinMap(binMap, 2, 4, 7, 9);
    std::cout << "Recieved: ";
    printBinMap(binMap);
    std::cout << "Expected: 8 4 0 1 7 6 2 3 9 5" << std::endl << std::endl;

    std::cout << "Original: ";
    printBinMap(binMap);
    binMap = editBinMap(binMap, 5, 10, 3, 8);
    std::cout << "Recieved: ";
    printBinMap(binMap);
    std::cout << "Expected: 6 9 0 1 5 4 2 8 7 3" << std::endl << std::endl;



    
    return 0;
}